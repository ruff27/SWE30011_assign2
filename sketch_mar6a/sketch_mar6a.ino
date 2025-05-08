// Pin Definitions
const int potPin = A0;
const int motionPin = 2;
const int fanRelayPin = 3;
const int buzzerPin = 4;
const int redLedPin = 5;
const int greenLedPin = 6;

// Threshold values
int fanThreshold = 800;
int alertThreshold = 600;

void setup() {
  pinMode(potPin, INPUT);
  pinMode(motionPin, INPUT);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  Serial.begin(9600);
  delay(2000);
}

void loop() {
  int potValue = analogRead(potPin);
  int motionDetected = digitalRead(motionPin);
  int temperature = map(potValue, 0, 1023, 15, 45);  // Simulate °C

  // Send to Python script
  Serial.print(potValue);
  Serial.print(",");
  Serial.print(motionDetected);
  Serial.print(",");
  Serial.println(temperature);

  // LED/Fan logic
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  digitalWrite(buzzerPin, LOW);

  if (potValue > fanThreshold) {
    digitalWrite(fanRelayPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    digitalWrite(fanRelayPin, LOW);
  }

  if (motionDetected == HIGH && potValue > alertThreshold) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  }

  delay(1000);
}
