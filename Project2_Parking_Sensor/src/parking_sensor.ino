// Parking Sensor System (Arduino Uno + HC-SR04 + LEDs + Buzzer)
// Paste your full code implementation here.

const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 11;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Basic distance measurement demo
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 20) {
    tone(buzzerPin, 1000);
  } else {
    noTone(buzzerPin);
  }
  delay(200);
}
