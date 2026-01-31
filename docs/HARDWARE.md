# Hardware-Dokumentation

Hardwarespezifikation fuer das Seeed Studio XIAO ESP32-S3 Board.

---

## Board-Uebersicht

![Seeed Studio XIAO ESP32-S3](Seeed%20Studio%20XIAO%20ESP32-S3.png)

### Spezifikationen

| Eigenschaft | Wert |
|-------------|------|
| **MCU** | ESP32-S3 (Xtensa LX7 Dual-Core, 240 MHz) |
| **Flash** | 8 MB |
| **PSRAM** | 8 MB (optional) |
| **SRAM** | 512 KB |
| **WiFi** | 802.11 b/g/n (2.4 GHz) |
| **Bluetooth** | BLE 5.0 |
| **USB** | USB-C (Native USB + UART) |
| **Abmessungen** | 21 x 17.5 mm |

---

## Pinbelegung

### Front-Seite (Hauptpins)

| Pin | GPIO | Funktion | Projekt-Verwendung |
|-----|------|----------|-------------------|
| D0 | GPIO1 | ADC1_CH0, Touch | - |
| D1 | GPIO2 | ADC1_CH1, Touch | - |
| D2 | GPIO3 | ADC1_CH2, Touch | - |
| D3 | GPIO4 | ADC1_CH3, Touch | - |
| D4 | GPIO5 | ADC1_CH4, Touch | - |
| D5 | GPIO6 | ADC1_CH5, Touch | - |
| D6 | GPIO43 | TX (UART0) | Serial TX |
| D7 | GPIO44 | RX (UART0) | Serial RX |
| D8 | GPIO7 | ADC1_CH6, SCL | I2C SCL |
| D9 | GPIO8 | ADC1_CH7, SDA | I2C SDA |
| D10 | GPIO9 | ADC1_CH8 | - |
| 5V | - | 5V Eingang/Ausgang | - |
| GND | - | Masse | - |
| 3V3 | - | 3.3V Ausgang | - |

### Rueck-Seite (Debug/Batterie)

| Pad | Funktion | Beschreibung |
|-----|----------|--------------|
| MTDO | GPIO40 | JTAG TDO |
| MTDI | GPIO41 | JTAG TDI |
| MTCK | GPIO39 | JTAG TCK |
| MTMS | GPIO42 | JTAG TMS |
| EN | - | Enable (Reset) |
| D+ | - | USB Data+ |
| D- | - | USB Data- |
| BAT+ | - | Batterie + (3.7V LiPo) |
| BAT- | - | Batterie - |

### Onboard-Komponenten

| Komponente | GPIO | Beschreibung |
|------------|------|--------------|
| **User LED** | GPIO21 | Gelbe LED (Active High) |
| **Charge LED** | - | Ladeanzeige (Orange) |
| **Reset Button** | - | Hardware-Reset |
| **Boot Button** | GPIO0 | Boot-Mode / User Button |

---

## Projekt-GPIO-Mapping

### Aktuelle Belegung

| Funktion | GPIO | Konstante | Datei |
|----------|------|-----------|-------|
| Status-LED | GPIO21 | `APP_BLINK_GPIO` | app_config.h |

### Reservierte GPIOs

| GPIO | Grund | Hinweis |
|------|-------|---------|
| GPIO0 | Boot Button | Low beim Start = Download-Mode |
| GPIO43 | USB Serial TX | Native USB |
| GPIO44 | USB Serial RX | Native USB |
| GPIO19 | USB D- | Nicht verwenden |
| GPIO20 | USB D+ | Nicht verwenden |

### Verfuegbare GPIOs fuer Erweiterung

| GPIO | Empfohlene Verwendung |
|------|----------------------|
| GPIO1-6 | Analog-Eingaenge, Touch |
| GPIO7 | I2C SCL |
| GPIO8 | I2C SDA |
| GPIO9 | Digital I/O |

---

## Schnittstellen

### USB (Native)

```
USB-C Connector
├── CDC Serial (115200 baud default)
├── JTAG Debug (optional)
└── Mass Storage (optional)
```

**Wichtig:** Nach Reset trennt USB kurz. `APP_SERIAL_STARTUP_MS` (2000ms) wartet auf Reconnect.

### I2C

| Signal | GPIO | Pull-Up |
|--------|------|---------|
| SDA | GPIO8 | Extern erforderlich |
| SCL | GPIO7 | Extern erforderlich |

Standard-Adressbereich: 0x08 - 0x77

### SPI (nicht konfiguriert)

| Signal | GPIO |
|--------|------|
| MOSI | GPIO9 |
| MISO | GPIO8 |
| SCK | GPIO7 |
| CS | frei waehlbar |

---

## Stromversorgung

### Eingaenge

| Quelle | Spannung | Max. Strom |
|--------|----------|------------|
| USB-C | 5V | 500mA |
| BAT+/- | 3.7V LiPo | - |
| 5V Pin | 5V | - |

### Ausgaenge

| Pin | Spannung | Max. Strom |
|-----|----------|------------|
| 3V3 | 3.3V | 700mA |
| 5V | 5V (USB durchgeschleift) | begrenzt durch USB |

### Stromverbrauch (typisch)

| Modus | Verbrauch |
|-------|-----------|
| Active (WiFi) | ~100-200 mA |
| Active (CPU only) | ~50 mA |
| Light Sleep | ~1 mA |
| Deep Sleep | ~14 µA |

---

## Anschlussbild

```
                    USB-C
                   ┌─────┐
         ┌─────────┤     ├─────────┐
         │  [RST]  └─────┘  [BOOT] │
         │   ●                 ●   │
    D0 ──┤ ○                     ○ ├── 5V
    D1 ──┤ ○                     ○ ├── GND
    D2 ──┤ ○    XIAO ESP32-S3   ○ ├── 3V3
    D3 ──┤ ○                     ○ ├── D10
    D4 ──┤ ○                     ○ ├── D9 (SDA)
    D5 ──┤ ○      [LED]         ○ ├── D8 (SCL)
    D6 ──┤ ○       GPIO21       ○ ├── D7 (RX)
         └─────────────────────────┘
                   (TX=D6)

    ● = Button
    ○ = Pin
    [LED] = User LED (GPIO21)
```

---

## Hinweise

### Boot-Mode

| GPIO0 beim Start | Modus |
|------------------|-------|
| High (normal) | Normal Boot |
| Low (BOOT gedrueckt) | Download Mode |

### Antenna

- Onboard PCB-Antenne
- Externer U.FL Anschluss (XIAO ESP32-S3 Sense Variante)

### Waermemanagement

- Thermal Pad auf Rueckseite
- Bei hoher Last Kuehlung empfohlen
