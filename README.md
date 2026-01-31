# esp32s3_ref

## Zweck des Projekts

`esp32s3_ref` ist ein **Referenz-/Template-Projekt** fuer **ESP32-S3 Firmware-Entwicklung mit PlatformIO**. Es bietet eine **saubere, wiederholbare Grundstruktur** (Architektur, Konfiguration, Logging, Code-Qualitaet, Dokumentation), damit neue Firmware-Projekte nicht jedes Mal bei Null starten.

## Ziel-Hardware

| Eigenschaft | Wert |
|-------------|------|
| **Board** | Seeed XIAO ESP32S3 |
| **Beispiel-I/O** | LED an GPIO 21 (ueber Konfiguration umstellbar) |

## Schnellstart (Build/Flash/Monitor)

```bash
pio run                      # Build
pio run --target upload      # Build + Flash
pio device monitor           # Serial Monitor (115200 baud)
```

## Projektstruktur

Kernidee: **Applikation (Logik) getrennt von Hardwarezugriff (HAL)**.

```
esp32s3_ref/
├── include/
│   ├── common/                  # Gemeinsame Typen und Konfiguration
│   │   ├── app_config.h             # Zentrale Konstanten (Pins, Zeiten, Version)
│   │   └── error_codes.h            # Fehlercodes (APP_ERR_*)
│   └── hw/                      # HAL-Schnittstellen
│       ├── gpio_hal.h               # GPIO-Abstraktion
│       └── serial_hal.h             # Serial/Logging-Abstraktion
├── src/
│   ├── app/                     # Applikationslogik (C++)
│   │   ├── app.hpp
│   │   └── app.cpp
│   ├── hw/                      # HAL-Implementierungen
│   │   ├── gpio_hal.c               # GPIO (C)
│   │   └── serial_hal.cpp           # Serial (C++ mit extern "C")
│   └── main.cpp                 # Arduino setup()/loop() Entry-Point
├── tools/
│   ├── format.sh                # Code-Formatierung (clang-format)
│   └── lint.sh                  # Statische Analyse (clang-tidy)
├── docs/                        # Dokumentation
│   ├── ENTWICKLERDOKU.md            # Entwicklerhandbuch
│   ├── HARDWARE.md                  # Pinbelegung, Board-Info
│   ├── CODING_STANDARD.md           # Namenskonventionen, Regeln
│   ├── FLOWCHART.md                 # Programmablaufplaene (Mermaid)
│   └── ALGORITHM.md                 # Algorithmus-Spezifikationen
├── platformio.ini               # PlatformIO-Konfiguration
├── .clang-format                # Formatierungsregeln
├── CHANGELOG.md                 # Versionshistorie
├── CLAUDE.md                    # AI-Assistenten-Anleitung
└── LICENSE                      # MIT-Lizenz
```

## Architekturregel (wichtigster Punkt)

**Regel:** Die Applikation greift **nie direkt** auf Framework-/Treiber-APIs zu, sondern **nur ueber die HAL-Schicht**.

```
┌─────────────────────────────────────┐
│         Application Layer           │
│         (src/app/ - C++)            │
│    ✗ Kein Arduino/ESP-IDF hier      │
├─────────────────────────────────────┤
│         HAL Layer                   │
│    (src/hw/, include/hw/ - C/C++)   │
│    ✓ Framework-Aufrufe nur hier     │
├─────────────────────────────────────┤
│      Arduino / ESP-IDF Framework    │
├─────────────────────────────────────┤
│         ESP32-S3 Hardware           │
└─────────────────────────────────────┘
```

**Warum das hilft (praktisch):**

- Du kannst Hardware austauschen (Pins/Board/Framework) ohne die Applogik umzuschreiben.
- Du kannst HAL mocken/stubben (Tests/Simulation), ohne dass deine Logik an Arduino/ESP-IDF "klebt".

## Konfiguration & Logging

### Konfiguration

Alle zentralen Einstellungen liegen in `include/common/app_config.h`:

| Konstante | Beschreibung | Default |
|-----------|--------------|---------|
| `APP_BLINK_GPIO` | LED-Pin | GPIO 21 |
| `APP_BLINK_PERIOD_MS` | Blinkperiode | 500 ms |
| `APP_SERIAL_BAUD` | Baudrate | 115200 |
| `APP_SERIAL_STARTUP_MS` | Startup-Delay | 2000 ms |

### Logging

Nutzung ueber die Serial-HAL:

```c
serial_hal_log(LOG_LEVEL_INFO, "Tag", "Nachricht: %d", wert);
```

Log-Levels: `LOG_LEVEL_DEBUG`, `LOG_LEVEL_INFO`, `LOG_LEVEL_WARN`, `LOG_LEVEL_ERROR`

## Code-Qualitaet (Workflow)

**Regel:** Vor Analyse/Review zuerst formatieren, dann linten:

```bash
./tools/format.sh    # 1. Formatieren
./tools/lint.sh      # 2. Statische Analyse
```

## Wie du es typischerweise weiterverwendest

1. **Neue Funktionalitaet** baust du **oben** in `src/app/` ein (Zustandsautomat, Scheduler, Protokolle, etc.)

2. **Hardwarezugriffe** kapselst du **unten** in `src/hw/` + `include/hw/` (z.B. SPI/I2C/CAN, Sensoren, Aktoren)

3. **Projektweite Parameter** bleiben **zentral** in `app_config.h`

### Geplante Erweiterungen

- Button-HAL mit Interrupt und Debounce
- I2C-HAL fuer Sensoren
- Deep-Sleep Unterstuetzung

## Dokumentation

- **[ENTWICKLERDOKU.md](docs/ENTWICKLERDOKU.md)** - Entwicklerhandbuch (empfohlen)
- [HARDWARE.md](docs/HARDWARE.md) - Pinbelegung, Board-Spezifikation
- [CODING_STANDARD.md](docs/CODING_STANDARD.md) - Namenskonventionen, Regeln
- [FLOWCHART.md](docs/FLOWCHART.md) - Programmablaufplaene
- [ALGORITHM.md](docs/ALGORITHM.md) - Algorithmus-Spezifikationen
- [CHANGELOG.md](CHANGELOG.md) - Versionshistorie
- [CLAUDE.md](CLAUDE.md) - AI-Assistenten-Anleitung

## Lizenz

MIT License - siehe [LICENSE](LICENSE)
