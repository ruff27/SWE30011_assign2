// Pin Definitions
const int potPin = A0;               // Potentiometer reading for fan speed
const int tempPin = A1;              // Analog temperature sensor
const int buttonPin = 2;             // Button to start fan
const int greenLedPin = 6;           
const int redLedPin = 5;             
const int fanPin = 9;                

// Thermistor constants
const float resistorValue = 4700.0;      // Fixed resistor value (Ω)
const float thermistorNominal = 10000.0; // Thermistor resistance at 25°C (Ω)
const float beta = 3950.0;               // Beta coefficient of thermistor
const float tempAlertThreshold = 25.0;   // Temperature to trigger alert LED

bool remoteFanControl = false;        // Flask control flag
bool remoteFanState = false;          // Flask fan ON/OFF state

void setup() {
  Serial.begin(9600);
  pinMode(tempPin, INPUT);
  pinMode(potPin, INPUT);
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

  // Read button state
  bool buttonPressed = digitalRead(buttonPin) == LOW;

  // Potentiometer reading
  int potValue = analogRead(potPin);
  delay(300);

  // Temperature reading using Steinhart-Hart equation
  int tempSensorValue = analogRead(tempPin);
  float voltage = tempSensorValue * (5.0 / 1023.0);
  float r = (5.0 - voltage) / voltage * resistorValue;
  float temperature = 1.0 / (log(r / thermistorNominal) / beta + (1.0 / (25.0 + 273.15))) - 273.15;

  // Serial Output
  Serial.print("Temp: ");
  Serial.print(temperature, 2);
  Serial.print(" °C | Pot: ");
  Serial.print(potValue);
  Serial.print(" | Fan button: ");
  Serial.println(buttonPressed ? "PRESSED" : "RELEASED");

  // LED Logic
  if (temperature >= tempAlertThreshold) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    delay(300);
    digitalWrite(redLedPin, LOW);
    delay(300);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    delay(300);
    digitalWrite(greenLedPin, LOW);
    delay(300);
  }

  // Fan logic
  if (remoteFanControl) {
    // Flask control
    analogWrite(fanPin, remoteFanState ? 255 : 0);
  } else {
    // Button + potentiometer control
    if (buttonPressed) {
      int fanSpeed = map(potValue, 0, 1023, 0, 255);
      analogWrite(fanPin, fanSpeed);
    } else {
      analogWrite(fanPin, 0);
    }
  }
}
