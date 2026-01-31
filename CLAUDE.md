# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
pio run                              # Build
pio run --target upload              # Build and flash
pio run --target upload && pio device monitor  # Flash and monitor
pio device monitor                   # Serial monitor (115200 baud)
```

## Code Quality

```bash
./tools/format.sh                    # Format all (clang-format)
./tools/lint.sh                      # Lint all (clangtidy)
pio check --src-filters="+<src/app/app.cpp>"   # Lint single file
```

**Wichtig:** Immer `format.sh` vor `lint.sh` ausfuehren.

## Architecture

**Target**: ESP32-S3 (Seeed XIAO ESP32S3) mit Arduino-Framework

```
src/app/        → Applikationslogik (C++, keine direkten HW-Aufrufe)
src/hw/         → HAL-Implementierungen (C/C++)
include/hw/     → HAL-Schnittstellen (C mit extern "C")
include/common/ → Typen, Konfiguration (error_codes.h, app_config.h)
```

**Sprachsplit**: C fuer Treiber/HAL, C++ fuer Applikation. `serial_hal.cpp` ist C++ wegen Arduino Serial, aber mit `extern "C"` Linkage.

**Entry Point**: `src/main.cpp` mit Arduino `setup()`/`loop()` - `run_forever()` blockiert in `setup()`

**Serial/Logging**: `serial_hal` mit Startup-Delay (`APP_SERIAL_STARTUP_MS`) damit fruehe Ausgaben nach Reset nicht verloren gehen. Logging: `serial_hal_log(LOG_LEVEL_INFO, "Tag", "msg")`

**Wichtigste Regel**: Applikation (`src/app/`) greift **nie direkt** auf Arduino/ESP-IDF zu - nur ueber HAL (`src/hw/`, `include/hw/`). Framework-Aufrufe gehoeren ausschliesslich in die HAL-Schicht.

## Neues HAL-Modul

Siehe `docs/ENTWICKLERDOKU.md` Kapitel 3 fuer vollstaendige Anleitung. Kurzfassung:
1. Header in `include/hw/` mit `extern "C"` Guards
2. Implementierung in `src/hw/` (C fuer reine GPIO, C++ wenn Arduino-Objekte noetig)
3. Konfiguration in `include/common/app_config.h`
4. Integration in `src/app/app.cpp`

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
