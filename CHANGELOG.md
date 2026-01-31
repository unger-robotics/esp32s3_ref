# Changelog

Alle relevanten Aenderungen am Projekt werden in dieser Datei dokumentiert.

Format basiert auf [Keep a Changelog](https://keepachangelog.com/de/1.0.0/).
Versionierung folgt [Semantic Versioning](https://semver.org/lang/de/).

---

## [Unreleased]

### Geplant
- Button-HAL mit Interrupt und Debounce
- I2C-HAL fuer Sensoren
- Deep-Sleep Unterstuetzung

---

## [1.0.0] - 2025-01-31

### Hinzugefuegt

#### Projektstruktur
- PlatformIO-Projekt fuer Seeed XIAO ESP32S3
- Layer-Architektur: Application → HAL → Framework
- Sprachsplit: C fuer Treiber/HAL, C++ fuer Applikation

#### HAL-Module
- `gpio_hal` - GPIO-Abstraktion (Init, Write)
- `serial_hal` - Serial/Logging mit Startup-Delay

#### Applikation
- `App`-Klasse mit Init und Run-Forever Pattern
- LED-Blink Beispiel (GPIO21, 500ms Periode)

#### Konfiguration
- `app_config.h` - Zentrale Konstanten (keine Magic Numbers)
- `error_codes.h` - Einheitliche Fehlercodes (APP_ERR_*)

#### Tools
- `format.sh` - clang-format Wrapper
- `lint.sh` - clangtidy Wrapper

#### Dokumentation
- README.md - Projektueberblick
- CLAUDE.md - AI-Assistenten-Anleitung
- CODING_STANDARD.md - Namenskonventionen, Regeln
- ENTWICKLERDOKU.md - Entwicklerhandbuch
- FLOWCHART.md - Programmablaufplaene (Mermaid)
- ALGORITHM.md - Algorithmus-Spezifikationen
- HARDWARE.md - Pinbelegung, Board-Info

#### Code-Qualitaet
- BARR-C + MISRA-orientierter Coding-Standard
- Deutsche Kommentare mit Doxygen-Tags
- Compiler: `-Wall -Wextra -Werror`
- Static Analysis: clangtidy (cppcoreguidelines, bugprone)

---

## Versionsschema

```
MAJOR.MINOR.PATCH

MAJOR - Inkompatible API-Aenderungen
MINOR - Neue Features (rueckwaertskompatibel)
PATCH - Bugfixes (rueckwaertskompatibel)
```

## Aenderungstypen

- **Hinzugefuegt** - Neue Features
- **Geaendert** - Aenderungen an bestehenden Features
- **Veraltet** - Features die bald entfernt werden
- **Entfernt** - Entfernte Features
- **Behoben** - Bugfixes
- **Sicherheit** - Sicherheitsrelevante Aenderungen
