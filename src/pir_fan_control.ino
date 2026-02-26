/**
 * Automatic Fan Control System — PIR Sensor Variant
 * 
 * Automatically controls an axial fan based on human presence
 * detected by a Passive Infrared (PIR) sensor.
 * 
 * Fan status (ON/OFF) and motion detection state are displayed
 * in real time on a 16x2 I2C LCD module.
 * 
 * Hardware:
 *   - Arduino Uno (ATmega328)
 *   - PIR Motion Sensor → Analog Pin A0
 *   - 5V Relay Module   → Digital Pin 9
 *   - 16x2 LCD (I2C)   → SDA: A4, SCL: A5
 * 
 * Dependencies:
 *   - LiquidCrystal_I2C (Frank de Brabander)
 *   - Wire (built-in)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Definitions ---
const int PIR_PIN   = A0;  // PIR sensor analog output
const int RELAY_PIN = 9;   // Relay module control pin

// --- Configuration ---
const int  MOTION_THRESHOLD = 900;   // ADC threshold for motion detection (0–1023)
const int  LOOP_DELAY_MS    = 100;   // Main loop delay (ms) to prevent rapid toggling

// --- LCD Setup ---
// I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- State ---
bool fanState = false;  // false = OFF, true = ON


// -----------------------------------------------------------------------
void setup() {
  pinMode(PIR_PIN,   INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Ensure fan is OFF on startup

  // Initialise LCD
  lcd.begin(16, 2);
  lcd.backlight();

  // Splash screen
  lcd.setCursor(0, 0);
  lcd.print("Automatic Fan");
  lcd.setCursor(0, 1);
  lcd.print("Control System");
  delay(2000);
  lcd.clear();

  // Show initial state
  updateDisplay();
}


// -----------------------------------------------------------------------
void loop() {
  int pirValue = analogRead(PIR_PIN);

  if (pirValue > MOTION_THRESHOLD) {
    // --- Motion detected ---
    lcd.setCursor(0, 0);
    lcd.print("Motion: YES     ");

    if (!fanState) {
      fanState = true;
      digitalWrite(RELAY_PIN, HIGH);  // Activate relay → fan ON
      updateDisplay();
    }

  } else {
    // --- No motion ---
    lcd.setCursor(0, 0);
    lcd.print("Motion: NO      ");

    if (fanState) {
      fanState = false;
      digitalWrite(RELAY_PIN, LOW);   // Deactivate relay → fan OFF
      updateDisplay();
    }
  }

  delay(LOOP_DELAY_MS);
}


// -----------------------------------------------------------------------
/**
 * Updates the second LCD row with the current fan state.
 */
void updateDisplay() {
  lcd.setCursor(0, 1);
  lcd.print(fanState ? "Fan: ON         " : "Fan: OFF        ");
}
