# Safe-Home: Invisible Doorframe NFC Security Camera

A DIY smart-home security camera that uses a custom-built, hidden antenna to trigger photo captures and smartphone notifications.

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform: ESP32-S3](https://img.shields.io/badge/Platform-ESP32--S3%20%7C%20OV2640-orange.svg)](#)
[![Frequency: 13.56 MHz](https://img.shields.io/badge/Antenna-13.56%20MHz%20NFC-brightgreen.svg)](#)

---

## How It Works

Traditional doorbells and security keypads are conspicuous, ugly, and vulnerable to weather and vandalism. **Safe-Home** replaces traditional hardware with an invisible physical computing checkpoint:

- **The Setup:** You hide an **ESP32-S3 camera module** and a **custom-coiled copper wire antenna** (built narrow to fit the slim space) inside a standard plastic doorframe.
- **The Action:** Instead of typing a PIN on a keypad or pressing a doorbell button, you simply **tap your smartphone or an NFC keychain** against the unmarked doorframe.
- **The Result:** The custom antenna catches the tap, telling the ESP32-S3 camera to immediately snap a picture and send a push notification with the photo directly to your phone over Wi-Fi.

In short, it is a **seamless, invisible security checkpoint** that replaces traditional keypads and buttons with a simple NFC tap to automate a smartphone notification.

```
 [ Phone / NFC Keychain Tap ] ──► [ Custom Hidden Doorframe Coil (25x75mm) ]
                                                    │
                                                    ▼ (Harvested RF Pulse)
   [ Smartphone Push Notification ] ◄── [ ESP32-S3 Snaps Photo & Sends via Wi-Fi ]
```

---

## System Schematic

Detailed vector schematic: [hardware/schematic.svg](hardware/schematic.svg)

```
       Narrow Doorframe Coil (L1) ────┬────────►|─────────┬────────► ESP32-S3 GPIO 1
       25x75 mm (4 turns, 28 AWG)     │        BAT54      │          (Trigger Interrupt)
                                     [C1]                [C2]   [R1]
                                    82 pF                10nF  100kΩ
                                      │                   │      │
       Ground ────────────────────────┴───────────────────┴──────┴──► ESP32-S3 GND
```

---

## Bill of Materials

| Component | Specification | Purpose |
| :--- | :--- | :--- |
| **Microcontroller & Camera** | ESP32-S3 Camera Board (OV2640 sensor) | Low-power controller with camera interface and 2.4 GHz Wi-Fi |
| **Custom Antenna Coil** | 28 AWG enamelled magnet wire | 4-turn rectangular coil ($25 \times 75\text{ mm}$, $L \approx 1.75\ \mu\text{H}$) |
| **Tuning Capacitor** | 82 pF ceramic capacitor (C0G/NP0) | Resonates with custom coil at 13.56 MHz |
| **Detector Diode** | BAT54 Schottky diode (or 1N4148) | Rectifies high-frequency RF into DC |
| **Smoothing Filter** | 10 nF ceramic capacitor + 100 kΩ resistor | Smoothes ripple and discharges line after tap ($RC \approx 1\text{ ms}$) |
| **Enclosure** | Standard hollow plastic/PVC doorframe molding | Completely hides the antenna, camera pinhole, and controller |

---

## Repository Structure

```
safe-home/
├── README.md                      # Project overview, workflow & quickstart
├── LICENSE                        # MIT License
├── hardware/
│   ├── doorframe_antenna.py       # Python calculator for rectangular coil geometry & LC tuning
│   └── schematic.svg              # Complete system and circuit schematic
├── firmware/
│   ├── safe_home.ino              # Main ESP32-S3 camera & notification firmware
│   ├── camera_pins.h              # Pin mappings for ESP32-S3 camera modules
│   └── config_template.h          # Wi-Fi and Telegram push notification credentials
└── docs/
    ├── doorframe_installation.md  # Step-by-step physical installation & coil winding guide
    └── writeup.md                 # Technical report on antenna design and interaction goals
```

---

## Quickstart Guide

### 1. Verify Antenna Inductance & LC Tuning
Run the doorframe coil calculation script:
```bash
python hardware/doorframe_antenna.py
```

### 2. Configure Firmware
1. Open [firmware/config_template.h](firmware/config_template.h) in your code editor.
2. Enter your Wi-Fi credentials (`WIFI_SSID` and `WIFI_PASSWORD`).
3. Set your Telegram Bot credentials (`TELEGRAM_BOT_TOKEN` and `TELEGRAM_CHAT_ID`). *(Free to set up via `@BotFather` on Telegram).*
4. Rename or include in `firmware/safe_home.ino`.

### 3. Flash to ESP32-S3
- Open [firmware/safe_home.ino](firmware/safe_home.ino) in Arduino IDE.
- Select your board (e.g. *ESP32S3 Dev Module*), ensure **PSRAM** is enabled (*OPI PSRAM*), and upload.
- Open Serial Monitor at 115200 baud to confirm Wi-Fi connection and camera initialization.

### 4. Install & Test
- Follow [docs/doorframe_installation.md](docs/doorframe_installation.md) to place the coil and lens behind the doorframe trim.
- Tap your phone or NFC keychain against the doorframe. Within 2 seconds, you will receive a push notification on your smartphone with a clear photo snapshot.

---

## Documentation
- **Physical Installation Guide:** [docs/doorframe_installation.md](docs/doorframe_installation.md)
- **Technical Write-Up & Evaluation:** [docs/writeup.md](docs/writeup.md)

*Author: Harsha Kalavagunta (<sreeharshakalavagunta@gmail.com>)*
