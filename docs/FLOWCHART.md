# Flussdiagramme

Programmablaufplaene (PAP) fuer das esp32s3_ref Projekt.

---

## 1) Laufzeit-Ablauf (Runtime)

Vom Reset bis zur Blink-Schleife mit Serial-Initialisierung:

```mermaid
flowchart TD
    A([Start / Reset]) --> B[/Serial init mit Startup-Delay/]
    B --> C{Serial bereit?}
    C -- Nein --> C
    C -- Ja --> D[/Warte APP_SERIAL_STARTUP_MS/]
    D --> E[/Banner ausgeben/]
    E --> F[App::init]
    F --> G[LED-Config aus app_config.h]
    G --> H[gpio_hal_init_output]
    H --> I{Init OK?}
    I -- Nein --> J[/LOG_LEVEL_ERROR ausgeben/]
    J --> K([Halt - Endlosschleife])
    I -- Ja --> L[/LOG_LEVEL_INFO: gestartet/]
    L --> M[App::run_forever]
    M --> N[_toggle_led]
    N --> O[gpio_hal_write]
    O --> P[vTaskDelay]
    P --> M

    subgraph main.cpp
        B
        C
        D
        E
        J
        K
        L
    end

    subgraph app.cpp
        F
        G
        M
        N
    end

    subgraph gpio_hal.c
        H
        O
    end
```

**Legende:**
- Oval `([...])` = Start/Ende
- Rechteck `[...]` = Prozess/Anweisung
- Raute `{...}` = Entscheidung
- Parallelogramm `[/.../]` = Ein-/Ausgabe

---

## 2) Initialisierung Detail

Detaillierter Ablauf der `serial_hal_init()` Funktion:

```mermaid
flowchart TD
    A([Start]) --> B[Serial.begin mit Baudrate]
    B --> C{Serial bereit?}
    C -- Nein --> D[delay 10ms]
    D --> C
    C -- Ja --> E{startup_ms > 0?}
    E -- Ja --> F[delay startup_ms]
    E -- Nein --> G[ANSI Escape: Clear Screen]
    F --> G
    G --> H([Return APP_ERR_SUCCESS])
```

---

## 3) GPIO HAL Init

Ablauf von `gpio_hal_init_output()`:

```mermaid
flowchart TD
    A([Start]) --> B{p_cfg == NULL?}
    B -- Ja --> C([Return APP_ERR_NULL_POINTER])
    B -- Nein --> D[gpio_config_t aufbauen]
    D --> E[gpio_config aufrufen]
    E --> F{err == ESP_OK?}
    F -- Nein --> G([Return APP_ERR_HW_FAILURE])
    F -- Ja --> H[Startzustand setzen: inaktiv]
    H --> I([Return APP_ERR_SUCCESS])
```

---

## 4) Entwicklungs-Workflow

Typischer Dev-Zyklus mit Tools:

```mermaid
flowchart LR
    A[Code aendern] --> B[./tools/format.sh]
    B --> C[pio run]
    C --> D{Build OK?}
    D -- Nein --> A
    D -- Ja --> E[pio run --target upload]
    E --> F[pio device monitor]
    F --> G{Laufzeitfehler?}
    G -- Ja --> A
    G -- Nein --> H[./tools/lint.sh]
    H --> I{Warnungen?}
    I -- Ja --> A
    I -- Nein --> J([Commit])
```

---

## 5) Layer-Architektur

Schichtenmodell des Projekts:

```mermaid
flowchart TB
    subgraph Application Layer
        APP[App Klasse<br/>src/app/]
    end

    subgraph HAL Layer
        GPIO[gpio_hal<br/>src/hw/gpio_hal.c]
        SERIAL[serial_hal<br/>src/hw/serial_hal.cpp]
    end

    subgraph Framework
        ARDUINO[Arduino Framework]
        ESPIDF[ESP-IDF]
        FREERTOS[FreeRTOS]
    end

    subgraph Hardware
        HW[ESP32-S3 Hardware]
    end

    APP --> GPIO
    APP --> SERIAL
    GPIO --> ESPIDF
    SERIAL --> ARDUINO
    ARDUINO --> ESPIDF
    ESPIDF --> FREERTOS
    FREERTOS --> HW
    ESPIDF --> HW
```

**Regel:** Applikation greift nur ueber HAL auf Hardware zu, nie direkt auf Framework/Treiber.

---

## Symbolreferenz

| Symbol | Mermaid | Bedeutung |
|--------|---------|-----------|
| Oval | `([...])` | Start/Ende (Terminator) |
| Rechteck | `[...]` | Prozess/Anweisung |
| Raute | `{...}` | Entscheidung (Ja/Nein) |
| Parallelogramm | `[/...//]` | Ein-/Ausgabe |
| Pfeil | `-->` | Ablauflinie |
