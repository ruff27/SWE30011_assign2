// Pin Definitions
const int tempPin = A1;
const int potPin = A0;
const int buttonPin = 2;
const int greenLedPin = 6;
const int redLedPin = 5;
const int fanPin = 9;

// Threshold
const float tempAlertThreshold = 20.0;

void setup() {
  Serial.begin(9600);

  pinMode(tempPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
}

void loop() {
  // Read button state
  bool buttonPressed = digitalRead(buttonPin) == LOW;

  // Smoothed potentiometer value
  int stablePotValue = 0;
  for (int i = 0; i < 10; i++) {
    stablePotValue += analogRead(potPin);
    delay(2);
  }
  stablePotValue /= 10;

  // Temperature reading
  int tempRaw = analogRead(tempPin);
  float voltage = tempRaw * (5.0 / 1023.0);
  float temperature = voltage * 7.44;

  // Serial Output with Pot Value
  Serial.print("Temp: ");
  Serial.print(temperature, 2);
  Serial.print(" °C | Pot: ");
  Serial.print(stablePotValue);
  Serial.print(" | Fan button: ");
  Serial.println(buttonPressed ? "PRESSED" : "RELEASED");

  // LED Logic (based on temperature only)
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

  // Fan logic (based on button only)
  if (buttonPressed) {
    int fanSpeed = map(stablePotValue, 0, 1023, 0, 255);
    analogWrite(fanPin, fanSpeed);
  } else {
    analogWrite(fanPin, 0);
  }


}
