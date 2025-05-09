// Pin Definitions
const int tempPin = A0;              // Analog temperature sensor
const int buttonPin = 2;             // Button to start fan
const int greenLedPin = 6;           // Green LED for safe temperature
const int redLedPin = 5;             // Red LED for high temperature
const int fanPin = 9;                // Fan control pin

// Thermistor constants
const float resistorValue = 4700.0;      // Fixed resistor value (Ω)
const float thermistorNominal = 10000.0; // Thermistor resistance at 25°C (Ω)
const float beta = 3950.0;               // Beta coefficient of thermistor
const float tempAlertThreshold = 24.0;   // Temperature to trigger alert LED

bool remoteFanControl = false;        // Flask control flag
bool remoteFanState = false;          // Flask fan ON/OFF state
bool manualOverride = false;          // Flag for manual override
bool prevButtonState = HIGH;          // To detect button toggle
bool fanState = false;                // Current fan ON/OFF state

void setup() {
  Serial.begin(9600);
  pinMode(tempPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);   // Button with internal pullup
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
}

void loop() {
  // Check for remote control command from Flask
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "FAN_ON") {
      remoteFanControl = true;
      remoteFanState = true;
    } else if (command == "FAN_OFF") {
      remoteFanControl = true;
      remoteFanState = false;
    }
  }

  // Read button state and toggle manual override
  bool buttonPressed = digitalRead(buttonPin) == LOW;
  if (buttonPressed && prevButtonState == HIGH) {
    manualOverride = !manualOverride;   // Toggle override mode
    remoteFanControl = false;           // Cancel remote control if manual override is used
  }
  prevButtonState = !buttonPressed ? HIGH : LOW;

  delay(2000);

  // Temperature reading using Steinhart-Hart equation
  int tempSensorValue = analogRead(tempPin);
  float voltage = tempSensorValue * (5.0 / 1023.0);
  float r = (5.0 - voltage) / voltage * resistorValue;
  float temperature = 1.0 / (log(r / thermistorNominal) / beta + (1.0 / (25.0 + 273.15))) - 273.15;

  // LED Logic
  String ledStatus = "";
  if (temperature >= tempAlertThreshold) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    ledStatus = "RED";
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    ledStatus = "GREEN";
  }

  // Fan Logic
  if (remoteFanControl) {
    analogWrite(fanPin, remoteFanState ? 255 : 0);
    fanState = remoteFanState;
  } else if (manualOverride) {
    analogWrite(fanPin, 255); // Toggle on each override
    fanState = true;
  } else {
    // Auto control by temperature
    if (temperature >= tempAlertThreshold) {
      analogWrite(fanPin, 255);
      fanState = true;
    } else {
      analogWrite(fanPin, 0);
      fanState = false;
    }
  }

  // Serial Output
  Serial.print("Temp: ");
  Serial.print(temperature, 2);
  Serial.print(" °C | LED: ");
  Serial.print(ledStatus);
  Serial.print(" | Fan: ");
  Serial.println(fanState ? "ON" : "OFF");
}
