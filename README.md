# esp32s3_ref

Referenzprojekt fuer ESP32-S3 Firmware-Entwicklung mit PlatformIO.

## Hardware

- **Board**: Seeed XIAO ESP32S3
- **LED**: GPIO 21 (konfigurierbar in `app_config.h`)

## Voraussetzungen

- [PlatformIO](https://platformio.org/) (VS Code Extension oder CLI)
- USB-Kabel fuer Programmierung

## Schnellstart

```bash
# Build
pio run

# Build und Flash
pio run --target upload

# Serial Monitor (115200 baud)
pio device monitor
```

## Projektstruktur

```
esp32s3_ref/
├── include/
│   ├── common/          # Gemeinsame Typen und Konfiguration
│   │   ├── app_config.h     # Zentrale Konstanten (Pins, Zeiten, Version)
│   │   └── error_codes.h    # Fehlercodes (APP_ERR_*)
│   └── hw/              # HAL-Schnittstellen
│       ├── gpio_hal.h       # GPIO-Abstraktion
│       └── serial_hal.h     # Serial/Logging-Abstraktion
├── src/
│   ├── app/             # Applikationslogik (C++)
│   │   ├── app.hpp
│   │   └── app.cpp
│   ├── hw/              # HAL-Implementierungen
│   │   ├── gpio_hal.c       # GPIO (C)
│   │   └── serial_hal.cpp   # Serial (C++ mit extern "C")
│   └── main.cpp         # Entry Point (Arduino setup/loop)
├── tools/
│   ├── format.sh        # Code-Formatierung (clang-format)
│   └── lint.sh          # Statische Analyse (clangtidy)
├── docs/
│   ├── ENTWICKLERDOKU.md    # Entwicklerhandbuch
│   ├── HARDWARE.md          # Pinbelegung, Board-Info
│   ├── CODING_STANDARD.md   # Coding-Richtlinien
│   ├── FLOWCHART.md         # Flussdiagramme (Mermaid)
│   └── ALGORITHM.md         # Algorithmus-Spezifikationen
├── platformio.ini       # PlatformIO-Konfiguration
├── .clang-format        # Formatierungsregeln
├── CHANGELOG.md         # Versionshistorie
└── CLAUDE.md            # AI-Assistenten-Anleitung
```

## Architektur

```
┌─────────────────────────────────────┐
│         Application Layer           │
│         (src/app/ - C++)            │
├─────────────────────────────────────┤
│         HAL Layer                   │
│    (src/hw/, include/hw/ - C/C++)   │
├─────────────────────────────────────┤
│      Arduino / ESP-IDF Framework    │
├─────────────────────────────────────┤
│         ESP32-S3 Hardware           │
└─────────────────────────────────────┘
```

**Regel**: Applikation greift nie direkt auf Framework-Treiber zu, sondern nur ueber HAL.

## Code-Qualitaet

```bash
# Formatierung (vor Commit)
./tools/format.sh

# Statische Analyse
./tools/lint.sh
```

**Wichtig**: Immer `format.sh` vor `lint.sh` ausfuehren.

## Konfiguration

Alle Einstellungen in `include/common/app_config.h`:

| Konstante | Beschreibung | Default |
|-----------|--------------|---------|
| `APP_BLINK_GPIO` | LED-Pin | GPIO 21 |
| `APP_BLINK_PERIOD_MS` | Blinkperiode | 500 ms |
| `APP_SERIAL_BAUD` | Baudrate | 115200 |
| `APP_SERIAL_STARTUP_MS` | Startup-Delay | 2000 ms |

## Logging

```cpp
#include "hw/serial_hal.h"

serial_hal_log(LOG_LEVEL_INFO, "Tag", "Nachricht: %d", wert);
```

Log-Levels: `LOG_LEVEL_DEBUG`, `LOG_LEVEL_INFO`, `LOG_LEVEL_WARN`, `LOG_LEVEL_ERROR`

## Dokumentation

- **[ENTWICKLERDOKU.md](docs/ENTWICKLERDOKU.md) - Entwicklerhandbuch**
- [HARDWARE.md](docs/HARDWARE.md) - Pinbelegung, Board-Spezifikation
- [CODING_STANDARD.md](docs/CODING_STANDARD.md) - Namenskonventionen, Regeln
- [FLOWCHART.md](docs/FLOWCHART.md) - Programmablaufplaene
- [ALGORITHM.md](docs/ALGORITHM.md) - Algorithmus-Spezifikationen
- [CHANGELOG.md](CHANGELOG.md) - Versionshistorie
- [CLAUDE.md](CLAUDE.md) - AI-Assistenten-Anleitung

## Lizenz

Proprietaer / Intern
