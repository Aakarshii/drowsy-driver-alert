/**
 * Drowsy Driver Alert System
 * 
 * Hardware: ESP8266 (NodeMCU), MAX30102 sensor, 0.96" OLED display, Buzzer
 * 
 * Logic:
 *   - MAX30102 reads IR value to detect finger presence
 *   - If IR > 5000: finger detected → show BPM, buzzer OFF
 *   - If IR < 5000: no contact → show alert, buzzer ON (until finger returns)
 * 
 * Wiring:
 *   MAX30102 + OLED → I2C: SCL=D1, SDA=D2, VCC=3.3V, GND=GND
 *   Buzzer (+) → D0 (GPIO16), Buzzer (-) → GND
 */

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ──────────────────────────────────────────────
// Pin Definitions
// ──────────────────────────────────────────────
const int BUZZER_PIN = 16;  // D0 on NodeMCU

// ──────────────────────────────────────────────
// OLED Display Setup (128x64, I2C address 0x3C)
// ──────────────────────────────────────────────
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ──────────────────────────────────────────────
// MAX30102 Sensor
// ──────────────────────────────────────────────
MAX30105 particleSensor;

// ──────────────────────────────────────────────
// Heart Rate Variables
// ──────────────────────────────────────────────
const byte RATE_SIZE = 4;       // Number of samples to average
byte rates[RATE_SIZE];          // BPM sample array
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute = 0;
int beatAvg = 0;

// ──────────────────────────────────────────────
// IR threshold: below this = no finger detected
// ──────────────────────────────────────────────
const long IR_THRESHOLD = 5000;

// ──────────────────────────────────────────────
// Setup
// ──────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // I2C: D2 = SDA (GPIO4), D1 = SCL (GPIO5)
  Wire.begin(4, 5);

  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // Start silent

  // OLED initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // MAX30102 initialization
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensor not found!");
    display.println("Check wiring.");
    display.display();
    Serial.println(F("MAX30102 not found. Check wiring."));
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);  // Low red LED for finger detection
  particleSensor.setPulseAmplitudeGreen(0);   // Green LED off

  Serial.println(F("System ready. Place finger on sensor."));
}

// ──────────────────────────────────────────────
// Main Loop
// ──────────────────────────────────────────────
void loop() {
  long irValue = particleSensor.getIR();

  // ── Heart Rate Detection ──
  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60.0 / (delta / 1000.0);

    if (beatsPerMinute > 20 && beatsPerMinute < 255) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) {
        beatAvg += rates[x];
      }
      beatAvg /= RATE_SIZE;
    }
  }

  // ── Update Display ──
  display.clearDisplay();

  // Header
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Driver Alert System");
  display.drawLine(0, 10, 128, 10, WHITE);

  if (irValue < IR_THRESHOLD) {
    // ── No Finger / No Hand Contact ──
    digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON

    display.setTextSize(2);
    display.setCursor(10, 18);
    display.println("ALERT!");

    display.setTextSize(1);
    display.setCursor(5, 42);
    display.println("Place hand on");
    display.setCursor(5, 52);
    display.println("steering wheel!");

    Serial.println("No contact detected — buzzer ON");

  } else {
    // ── Finger Detected ──
    digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF

    display.setTextSize(1);
    display.setCursor(0, 16);
    display.println("Heart Rate:");

    display.setTextSize(3);
    display.setCursor(10, 28);
    display.print(beatAvg);
    display.setTextSize(2);
    display.print(" BPM");

    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print("  BPM=");
    Serial.print(beatsPerMinute);
    Serial.print("  Avg=");
    Serial.println(beatAvg);
  }

  display.display();
}
