/**
 * Automatic Fan Control System — IR Sensor Variant
 * 
 * Alternative implementation using an Infrared (IR) proximity sensor
 * instead of a PIR sensor. Suitable when a PIR sensor is unavailable
 * or when shorter-range object detection is sufficient.
 * 
 * Note on logic inversion:
 *   The IR sensor used here outputs a LOW value when an object is 
 *   present (active-low). The fan logic is therefore inverted relative
 *   to the PIR variant.
 * 
 * Hardware:
 *   - Arduino Uno (ATmega328)
 *   - IR Sensor Module  → Analog Pin A0
 *   - 5V Relay Module   → Digital Pin 6
 *   - 16x2 LCD (I2C)   → SDA: A4, SCL: A5
 * 
 * Dependencies:
 *   - LiquidCrystal_I2C (Frank de Brabander)
 *   - Wire (built-in)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Definitions ---
const int IR_PIN    = A0;  // IR sensor analog output
const int RELAY_PIN = 6;   // Relay module control pin

// --- Configuration ---
const int  OBJECT_THRESHOLD = 600;   // ADC threshold for object detection (0–1023)
const int  LOOP_DELAY_MS    = 100;   // Main loop delay (ms)

// --- LCD Setup ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- State ---
bool fanState = false;  // false = OFF, true = ON


// -----------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  pinMode(IR_PIN,    INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay OFF (active-low logic)

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

  updateDisplay();
}


// -----------------------------------------------------------------------
void loop() {
  int irValue = analogRead(IR_PIN);

  Serial.print("IR Value: ");
  Serial.println(irValue);

  if (irValue > OBJECT_THRESHOLD) {
    // --- No object / person detected (sensor value high = clear path) ---
    lcd.setCursor(0, 0);
    lcd.print("Object: NO      ");

    if (!fanState) {
      fanState = true;
      digitalWrite(RELAY_PIN, LOW);   // Relay ON (active-low)
      updateDisplay();
    }

  } else {
    // --- Object / person detected (sensor value drops below threshold) ---
    lcd.setCursor(0, 0);
    lcd.print("Object: YES     ");

    if (fanState) {
      fanState = false;
      digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
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
  // Note: display label is inverted for this IR sensor variant
  lcd.print(fanState ? "Fan: ON         " : "Fan: OFF        ");
}
