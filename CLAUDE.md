# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
pio run                      # Build
pio run --target upload      # Build and flash
pio run --target clean       # Clean
pio device monitor           # Serial monitor (115200 baud)
```

## Code Quality

```bash
./tools/format.sh            # Format (clang-format)
./tools/lint.sh              # Static analysis (clangtidy)
```

**Wichtig:** Immer `format.sh` vor `lint.sh` ausfuehren - Format und Lint arbeiten zusammen.

## Architecture

**Target**: ESP32-S3 (Seeed XIAO ESP32S3) mit Arduino-Framework

**Sprachsplit**: C fuer Treiber/HAL, C++ fuer Applikation (Ausnahme: `serial_hal.cpp` ist C++ wegen Arduino Serial, aber mit `extern "C"` Linkage)

**Layer-Trennung**: Applikationscode (`src/app/`) greift nie direkt auf ESP-IDF/Arduino-Treiber zu. Hardware-Zugriff nur ueber HAL (`src/hw/`, `include/hw/`).

**Entry Point**: `src/main.cpp` mit Arduino `setup()`/`loop()` - `run_forever()` blockiert in `setup()`

**Serial/Logging**: `serial_hal` mit Startup-Delay (`APP_SERIAL_STARTUP_MS`) damit fruehe Ausgaben nach Reset nicht verloren gehen. Logging mit Level und Modul-Tag: `serial_hal_log(LOG_LEVEL_INFO, "Tag", "msg")`

## Coding Rules (BARR-C + MISRA-orientiert)

- **Fehlerbehandlung**: `error_code_e` zurueckgeben, `APP_ERR_SUCCESS` (0) bei Erfolg, negative Werte bei Fehler (APP_ERR_ Prefix wegen lwIP-Kollision)
- **Keine Magic Numbers**: Konfiguration in `app_config.h`
- **Typen**: `stdint.h`-Typen, keine impliziten Casts
- **C-Header**: `extern "C"` Guards fuer C++ Kompatibilitaet
- **ISR**: Kurz halten, nur Queue/Notification setzen
- **C++**: RAII ja, Exceptions nein, Fehler ueber Return-Code
- **Kein `strcpy/strcat`**: Laengen immer pruefen (Buffer-Safety)

## Naming

| Element | Style | Beispiel |
|---------|-------|----------|
| Dateien | snake_case | `gpio_hal.c` |
| Typen (C) | snake_case + `_e`/`_t` | `error_code_e`, `gpio_hal_cfg_t` |
| Funktionen | snake_case | `gpio_hal_write` |
| Klassen | PascalCase | `App` |
| Private Member | `_` prefix | `_led_on` |
| Konstanten | UPPER_SNAKE_CASE | `APP_BLINK_GPIO` |

## Kommentare (Deutsch, BARR-C)

- **Datei-Header**: `@file`, `@brief` mit Modulbeschreibung
- **Sektionen**: `INCLUDES`, `TYPES`, `OEFFENTLICHE FUNKTIONEN`, `PRIVATE METHODEN`
- **Doxygen**: `@param`, `@return`, `@note` fuer API-Dokumentation
- **Inline**: Erklaerungen bei nicht-trivialem Code
