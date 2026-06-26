<<<<<<< HEAD
# 🚗 Touch-Sensitive Drowsy Driver Alert System

A hardware-based, retrofittable safety device that detects when a driver loses hand contact with the steering wheel and immediately triggers an audible alert — no camera, no AI, no vehicle modification required.

> **Invention Disclosure Filed** | TRL 4 — Technology Validated in Lab

---

## 📌 Problem

Drowsy driving causes thousands of accidents every year. Most existing solutions detect drowsiness *after* erratic driving or eye closure — by which point it may already be too late. This device detects the **earliest physical sign**: the driver's hands leaving the steering wheel.

---

## 💡 How It Works

A MAX30102 optical sensor is mounted on the steering wheel. The driver rests their thumb on the sensor while gripping the wheel. The ESP8266 microcontroller continuously reads the IR signal:

- **Finger detected (IR > 5000)** → System is silent, BPM displayed on OLED
- **Finger removed (IR < 5000)** → Buzzer activates immediately and keeps beeping until hand is replaced

Unlike existing systems, **the buzzer does not stop until the driver places their finger back** — preventing the driver from ignoring a one-time beep.

---

## 🔧 Components

| Component | Function |
|-----------|----------|
| ESP8266 (NodeMCU) | Main microcontroller; processes sensor data and controls outputs |
| MAX30102 | Optical sensor for finger/contact detection + heart rate monitoring |
| 0.96" OLED Display | Displays BPM and "Place Finger..." alert message via I²C |
| Buzzer | Audible alert triggered when no hand contact is detected |
| Jumper Wires & Breadboard | Prototyping and circuit connections |
| USB Power | Powered via standard phone charger — no vehicle wiring needed |

---

## 🔌 Circuit Wiring

```
MAX30102  →  ESP8266
  VCC     →  3.3V
  GND     →  GND
  SCL     →  D1
  SDA     →  D2

OLED Display  →  ESP8266
  VCC         →  3.3V
  GND         →  GND
  SCL         →  D1  (shared I²C bus)
  SDA         →  D2  (shared I²C bus)

Buzzer  →  ESP8266
  (+)   →  D0 (GPIO 16)
  (-)   →  GND
```

> Both the OLED and MAX30102 share the same I²C lines (D1/D2). Ensure all components share a common GND.

---

## 💻 Code

### Dependencies (Arduino IDE)

Install these libraries via **Library Manager** (`Sketch > Include Library > Manage Libraries`):

- `MAX3010x` by SparkFun
- `Adafruit SSD1306`
- `Adafruit GFX Library`

### Flash Instructions

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Add ESP8266 board support: `File > Preferences > Additional Boards Manager URLs`:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Select board: `Tools > Board > NodeMCU 1.0 (ESP-12E Module)`
4. Open `src/main.ino`
5. Connect ESP8266 via USB and click **Upload**

---

## ✅ Validation Results

System tested across 15 trials, comparing OLED BPM readings against a reference smartwatch.

| Trial | Finger on Sensor | Smartwatch BPM | OLED BPM | Buzzer | Error % |
|-------|-----------------|---------------|----------|--------|---------|
| 1  | ✅ | 76 | 75 | OFF | 1.32 |
| 2  | ✅ | 89 | 92 | OFF | 3.37 |
| 3  | ✅ | 80 | 80 | OFF | 0.00 |
| 4  | ❌ | — | Place Finger... | ON | — |
| 5  | ✅ | 70 | 65 | OFF | 7.14 |
| 6  | ❌ | — | Place Finger... | ON | — |
| 7  | ✅ | 83 | 83 | OFF | 0.00 |
| 8  | ✅ | 94 | 94 | OFF | 0.00 |
| 9  | ✅ | 66 | 66 | OFF | 0.00 |
| 10 | ✅ | 74 | 74 | OFF | 0.00 |
| 11 | ✅ | 81 | 81 | OFF | 0.00 |
| 12 | ✅ | 72 | 70 | OFF | 2.78 |
| 13 | ✅ | 68 | 65 | OFF | 4.41 |
| 14 | ✅ | 74 | 74 | OFF | 0.00 |
| 15 | ✅ | 72 | 69 | OFF | 4.17 |

**Average BPM Error: ~1.88%** (excluding no-contact trials)  
**Contact detection: 100% accurate** across all trials

---

## 📁 Repository Structure

```
drowsy-driver-alert/
├── README.md
├── src/
│   └── main.ino          ← Arduino source code
├── docs/
│   └── IDF_report.docx   ← Invention Disclosure Format report
├── images/
│   └── (circuit photos, OLED output screenshots)
└── LICENSE
```

---

## 🆚 How This Differs from Prior Art

| Existing Solutions | This Device |
|-------------------|-------------|
| Require OEM integration or full wheel replacement | Clip-on, retrofits any vehicle |
| Detect drowsiness after erratic driving begins | Detects the moment hands leave the wheel |
| Use camera/EEG/complex biosensors | Single MAX30102 optical sensor |
| Expensive, complex systems | Low-cost, USB-powered prototype |
| One-time beep alert | Continuous buzzer until hand contact restored |

---

## 🛡️ IP Status

Formal Invention Disclosure filed (IDF-B, Doc No. 02-IPR-R003). Potential protection under:
- **Indian Patents Act 1970** — core detection mechanism and MAX30102 reuse for contact sensing
- **Designs Act 2000** — physical form and sensor placement
- **Copyright Act 1957** — ESP8266 firmware

---

## 👤 Authors

<!-- Add your name(s) here -->

---

## 📄 License

This project is for academic and research purposes. See `LICENSE` for details.
=======
# drowsy-driver-alert
>>>>>>> c6bfed631d438a2f4f69ef4ed6e91c693da05c3a
