# Entwicklerdokumentation

Technische Dokumentation fuer Entwickler des esp32s3_ref Projekts.

---

## 1. Entwicklungsumgebung

### 1.1 Installation

```bash
# PlatformIO CLI (alternativ: VS Code Extension)
pip install platformio

# Projekt klonen und bauen
git clone <repo-url>
cd esp32s3_ref
pio run
```

### 1.2 VS Code Setup

Empfohlene Extensions (`.vscode/extensions.json`):
- PlatformIO IDE
- C/C++ (Microsoft)
- Clang-Format

### 1.3 Erster Build

```bash
pio run                      # Kompilieren
pio run --target upload      # Flashen
pio device monitor           # Serial Monitor oeffnen
```

**Erwartete Ausgabe nach Reset:**
```
----------------------------------------
  esp32s3_ref v1.0.0
  Board: Seeed XIAO ESP32S3
----------------------------------------

[INFO ] [Main] Initialisiere...
[DEBUG] [App] GPIO 21 als LED konfigurieren
[INFO ] [App] LED initialisiert (GPIO 21)
[INFO ] [Main] Applikation gestartet

[INFO ] [App] Hauptschleife gestartet (Periode: 500 ms)
```

---

## 2. Architektur

### 2.1 Layer-Modell

```
┌─────────────────────────────────────────────────────────┐
│                  APPLICATION LAYER                       │
│  src/app/app.cpp, app.hpp                               │
│  - Applikationslogik (C++)                              │
│  - Zustandsverwaltung                                   │
│  - Keine direkten Treiber-Aufrufe                       │
├─────────────────────────────────────────────────────────┤
│                     HAL LAYER                            │
│  src/hw/*.c, src/hw/*.cpp                               │
│  include/hw/*.h                                         │
│  - Hardware-Abstraktion                                 │
│  - Kapselt ESP-IDF/Arduino                              │
│  - C mit extern "C" fuer C++ Kompatibilitaet            │
├─────────────────────────────────────────────────────────┤
│                  COMMON LAYER                            │
│  include/common/*.h                                     │
│  - Typen (error_codes.h)                                │
│  - Konfiguration (app_config.h)                         │
│  - Keine Implementierung                                │
├─────────────────────────────────────────────────────────┤
│              FRAMEWORK (Arduino/ESP-IDF)                 │
├─────────────────────────────────────────────────────────┤
│                 HARDWARE (ESP32-S3)                      │
└─────────────────────────────────────────────────────────┘
```

### 2.2 Abhaengigkeitsregeln

| Von | Nach | Erlaubt |
|-----|------|---------|
| app/ | hw/ | Ja |
| app/ | common/ | Ja |
| app/ | Arduino/ESP-IDF | **Nein** |
| hw/ | common/ | Ja |
| hw/ | Arduino/ESP-IDF | Ja |
| common/ | * | Nein (nur Typen/Defines) |

### 2.3 Dateikonventionen

| Typ | Sprache | Dateiendung | Beispiel |
|-----|---------|-------------|----------|
| HAL-Treiber | C | `.c` | `gpio_hal.c` |
| HAL mit Arduino-Objekten | C++ | `.cpp` | `serial_hal.cpp` |
| HAL-Header | C | `.h` | `gpio_hal.h` |
| Applikation | C++ | `.cpp/.hpp` | `app.cpp` |
| Konfiguration | C | `.h` | `app_config.h` |

---

## 3. Neues Modul erstellen

### 3.1 HAL-Modul (Beispiel: Button)

**Schritt 1: Header erstellen** (`include/hw/button_hal.h`)

```c
/**
 * @file    button_hal.h
 * @brief   Hardware-Abstraktionsschicht fuer Taster.
 */

#ifndef BUTTON_HAL_H_
#define BUTTON_HAL_H_

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "common/error_codes.h"
#include "driver/gpio.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * TYPES
 * ============================================================================ */

typedef struct {
    gpio_num_t pin;
    bool is_active_low;
} button_hal_cfg_t;

/* ============================================================================
 * OEFFENTLICHE FUNKTIONEN
 * ============================================================================ */

error_code_e button_hal_init(const button_hal_cfg_t *p_cfg);
error_code_e button_hal_read(gpio_num_t pin, bool *p_pressed);

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_HAL_H_ */
```

**Schritt 2: Implementierung** (`src/hw/button_hal.c`)

```c
/**
 * @file    button_hal.c
 * @brief   Implementierung der Button Hardware-Abstraktionsschicht.
 */

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include "hw/button_hal.h"

/* ============================================================================
 * OEFFENTLICHE FUNKTIONEN
 * ============================================================================ */

error_code_e button_hal_init(const button_hal_cfg_t *p_cfg)
{
    if (p_cfg == NULL) {
        return APP_ERR_NULL_POINTER;
    }

    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << (uint32_t)p_cfg->pin);
    io_conf.pull_up_en = p_cfg->is_active_low ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = p_cfg->is_active_low ? GPIO_PULLDOWN_DISABLE : GPIO_PULLDOWN_ENABLE;

    const esp_err_t err = gpio_config(&io_conf);
    return (err == ESP_OK) ? APP_ERR_SUCCESS : APP_ERR_HW_FAILURE;
}

error_code_e button_hal_read(gpio_num_t pin, bool *p_pressed)
{
    if (p_pressed == NULL) {
        return APP_ERR_NULL_POINTER;
    }

    *p_pressed = (gpio_get_level(pin) == 0);  /* Active-low */
    return APP_ERR_SUCCESS;
}
```

**Schritt 3: Konfiguration** (`include/common/app_config.h`)

```c
/* Button-Konfiguration */
#define APP_BUTTON_GPIO      (GPIO_NUM_0)
#define APP_BUTTON_ACTIVE_LOW (true)
```

**Schritt 4: In App integrieren** (`src/app/app.cpp`)

```cpp
#include "hw/button_hal.h"

// In App::init()
button_hal_cfg_t btn_cfg = {
    .pin = APP_BUTTON_GPIO,
    .is_active_low = APP_BUTTON_ACTIVE_LOW
};
button_hal_init(&btn_cfg);
```

### 3.2 Checkliste neues Modul

- [ ] Header mit `extern "C"` Guards
- [ ] Doxygen-Kommentare (`@file`, `@brief`, `@param`, `@return`)
- [ ] Sektionskommentare (INCLUDES, TYPES, FUNKTIONEN)
- [ ] Fehlerbehandlung mit `error_code_e`
- [ ] Nullzeiger-Pruefungen
- [ ] Konstanten in `app_config.h`
- [ ] `./tools/format.sh` ausgefuehrt
- [ ] `./tools/lint.sh` ohne Fehler

---

## 4. Fehlerbehandlung

### 4.1 Error Codes

```c
typedef enum {
    APP_ERR_SUCCESS      =  0,  /* Kein Fehler */
    APP_ERR_NULL_POINTER = -1,  /* NULL-Zeiger */
    APP_ERR_INVALID_ARG  = -2,  /* Ungueltiges Argument */
    APP_ERR_INVALID_STATE= -3,  /* Ungueltiger Zustand */
    APP_ERR_HW_FAILURE   = -4,  /* Hardware-Fehler */
    APP_ERR_TIMEOUT      = -5   /* Timeout */
} error_code_e;
```

### 4.2 Fehlerbehandlungs-Pattern

```c
error_code_e funktion(const config_t *p_cfg)
{
    /* 1. Nullzeiger pruefen */
    if (p_cfg == NULL) {
        return APP_ERR_NULL_POINTER;
    }

    /* 2. Argumente validieren */
    if (p_cfg->value > MAX_VALUE) {
        return APP_ERR_INVALID_ARG;
    }

    /* 3. Hardware-Operation */
    const esp_err_t err = hw_operation();
    if (err != ESP_OK) {
        return APP_ERR_HW_FAILURE;
    }

    return APP_ERR_SUCCESS;
}
```

### 4.3 Fehler loggen

```c
const error_code_e err = some_function();
if (err != APP_ERR_SUCCESS) {
    serial_hal_log(LOG_LEVEL_ERROR, TAG, "Fehler: %d", (int)err);
    return err;
}
```

---

## 5. Logging

### 5.1 Log-Levels

| Level | Verwendung |
|-------|------------|
| `LOG_LEVEL_DEBUG` | Entwickler-Details, Pin-Nummern |
| `LOG_LEVEL_INFO` | Normale Statusmeldungen |
| `LOG_LEVEL_WARN` | Warnungen, nicht-kritische Fehler |
| `LOG_LEVEL_ERROR` | Kritische Fehler |

### 5.2 Modul-Tag definieren

```c
/* Am Anfang jeder .c/.cpp Datei */
static const char *const TAG = "ModulName";
```

### 5.3 Log-Aufrufe

```c
serial_hal_log(LOG_LEVEL_DEBUG, TAG, "GPIO %d konfiguriert", (int)pin);
serial_hal_log(LOG_LEVEL_INFO, TAG, "Initialisierung abgeschlossen");
serial_hal_log(LOG_LEVEL_WARN, TAG, "Buffer fast voll: %u%%", usage);
serial_hal_log(LOG_LEVEL_ERROR, TAG, "Init fehlgeschlagen: %d", (int)err);
```

---

## 6. Code-Qualitaet

### 6.1 Workflow

```bash
# 1. Code schreiben
# 2. Formatieren
./tools/format.sh

# 3. Lint pruefen
./tools/lint.sh

# 4. Build testen
pio run

# 5. Auf Hardware testen
pio run --target upload && pio device monitor
```

### 6.2 Compiler-Flags

Definiert in `platformio.ini`:

| Flag | Bedeutung |
|------|-----------|
| `-Wall` | Alle Warnungen aktivieren |
| `-Wextra` | Zusaetzliche Warnungen |
| `-Werror` | Warnungen als Fehler behandeln |
| `-std=gnu++17` | C++17 Standard |

### 6.3 Deaktivierte Lint-Checks

Fuer Embedded unpassende Checks (siehe `platformio.ini`):

- `pro-type-vararg` - printf-style benoetigt
- `avoid-c-arrays` - C-Arrays sind Standard
- `avoid-non-const-global-variables` - Singleton-Pattern
- `easily-swappable-parameters` - zu strikt
- `avoid-magic-numbers` - Defines werden genutzt
- `init-variables` - False Positives bei const-Init

---

## 7. Debugging

### 7.1 Serial Monitor

```bash
pio device monitor
```

**Tastenkuerzel:**
- `Ctrl+C` - Beenden
- `Ctrl+T` - Menuee (Baudrate, etc.)

### 7.2 Reset-Verhalten

Nach Reset wartet das System 2 Sekunden (`APP_SERIAL_STARTUP_MS`) damit der Monitor verbinden kann. Fruehe Ausgaben gehen nicht verloren.

### 7.3 Fehlersuche

1. **Build-Fehler**: `pio run -v` fuer verbose Output
2. **Lint-Fehler**: Einzelne Datei pruefen: `pio check --src-filters="+<src/app/app.cpp>"`
3. **Laufzeitfehler**: Log-Level auf DEBUG setzen, Ausgaben pruefen
4. **Hardware-Fehler**: GPIO-Nummern in `app_config.h` pruefen

---

## 8. Versionierung

### 8.1 Versionsnummern

In `include/common/app_config.h`:

```c
#define APP_VERSION_MAJOR (1U)
#define APP_VERSION_MINOR (0U)
#define APP_VERSION_PATCH (0U)
```

### 8.2 Changelog fuehren

Bei Aenderungen:
1. Version in `app_config.h` erhoehen
2. Changelog-Eintrag schreiben
3. Git-Tag setzen: `git tag -a v1.0.1 -m "Beschreibung"`

---

## 9. Git-Workflow

### 9.1 Repository-URL

```
https://github.com/unger-robotics/esp32s3_ref
SSH: git@github.com:unger-robotics/esp32s3_ref.git
```

### 9.2 Ersteinrichtung (neues Projekt)

```bash
# 1. Git initialisieren
git init

# 2. Alle Dateien stagen
git add .

# 3. Initialer Commit
git commit -m "Initial commit: ESP32-S3 Referenzprojekt v1.0.0"

# 4. Remote hinzufuegen (SSH)
git remote add origin git@github.com:unger-robotics/esp32s3_ref.git

# 5. Branch umbenennen zu main
git branch -M main

# 6. Push mit Upstream-Tracking
git push -u origin main
```

### 9.3 Taeglicher Workflow

```bash
# Aenderungen anzeigen
git status
git diff

# Dateien stagen
git add <dateien>           # Einzelne Dateien
git add .                   # Alle Aenderungen

# Commit erstellen
git commit -m "Kurze Beschreibung"

# Push zu GitHub
git push

# Aenderungen von GitHub holen
git pull
```

### 9.4 Feature-Branch Workflow

```bash
# Neuen Branch erstellen und wechseln
git checkout -b feature/button-hal

# Arbeiten, committen...
git add .
git commit -m "Add button HAL module"

# Zurueck zu main und mergen
git checkout main
git pull
git merge feature/button-hal

# Push und Branch loeschen
git push
git branch -d feature/button-hal
```

### 9.5 Release erstellen

```bash
# Version in app_config.h erhoehen
# CHANGELOG.md aktualisieren

git add .
git commit -m "Release v1.1.0"
git tag -a v1.1.0 -m "Version 1.1.0: Button-HAL hinzugefuegt"
git push
git push --tags
```

### 9.6 Nuetzliche Befehle

| Befehl | Beschreibung |
|--------|--------------|
| `git status` | Aktueller Status |
| `git log --oneline -10` | Letzte 10 Commits |
| `git diff` | Unstaged Aenderungen |
| `git diff --staged` | Staged Aenderungen |
| `git stash` | Aenderungen zwischenspeichern |
| `git stash pop` | Zwischengespeichertes zurueckholen |
| `git reset HEAD <datei>` | Datei unstagen |
| `git checkout -- <datei>` | Aenderungen verwerfen |

---

## 10. Referenz

### 10.1 Projektdateien

| Datei | Beschreibung |
|-------|--------------|
| `platformio.ini` | Build-Konfiguration, Lint-Einstellungen |
| `.clang-format` | Code-Formatierungsregeln |
| `.gitignore` | Git-Ausschluesse |
| `CLAUDE.md` | AI-Assistenten-Anleitung |

### 10.2 Dokumentation

| Datei | Inhalt |
|-------|--------|
| `README.md` | Projektueberblick |
| `docs/CODING_STANDARD.md` | Namenskonventionen, Regeln |
| `docs/FLOWCHART.md` | Programmablaufplaene |
| `docs/ALGORITHM.md` | Algorithmus-Spezifikationen |
| `docs/ENTWICKLERDOKU.md` | Diese Datei |

### 10.3 Externe Ressourcen

- [PlatformIO Docs](https://docs.platformio.org/)
- [ESP-IDF GPIO API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/gpio.html)
- [Arduino-ESP32](https://docs.espressif.com/projects/arduino-esp32/)
- [BARR-C Coding Standard](https://barrgroup.com/embedded-systems/books/embedded-c-coding-standard)
