/* Parking Sensor System – Arduino Uno + HC-SR04 + 74HC595 + Buzzer
   - Measures distance (cm) every ~200 ms
   - Displays a bar on 8 LEDs via 74HC595
   - Buzzer beeps with different tones based on distance buckets

   Adjust the pin numbers below to match your wiring.
*/

// -------- Pins --------
// Ultrasonic sensor
const int TRIG_PIN   = 9;
const int ECHO_PIN   = 10;

// Shift register 74HC595 (LED bar)
const int LATCH_PIN  = 4;   // ST_CP
const int CLOCK_PIN  = 5;   // SH_CP
const int DATA_PIN   = 6;   // DS

// Buzzer
const int BUZZER_PIN = 11;

// -------- Config --------
const int MAX_CM = 150;           // valid reading range upper bound
const int MIN_CM = 0;             // valid reading range lower bound
const unsigned long REFRESH_MS = 200;

// LED bar patterns (MSB..LSB). 8 levels from 1 LED to all LEDs.
const byte VALUES_LEDS[8] = {
  B00000001,  // level 0  (farthest)
  B00000011,  // level 1
  B00000111,  // level 2
  B00001111,  // level 3
  B00011111,  // level 4
  B00111111,  // level 5
  B01111111,  // level 6
  B11111111   // level 7  (closest)
};

// Tone settings
const unsigned int TONE_NEAR_FREQ   = 5000;  // for closer range (5–6)
const unsigned int TONE_MID_FREQ    = 200;   // for mid range (3–4)
const unsigned int TONE_DURATION_MS = 200;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  Serial.begin(9600);
  // Clear LEDs at start
  shiftOutByte(0);
}

void loop() {
  int distance = readDistanceCm();

  // validate / log
  if (distance < MIN_CM || distance > MAX_CM || distance == 0) {
    Serial.println(F("Out of range"));
    shiftOutByte(0);      // turn LEDs off
    noTone(BUZZER_PIN);   // mute
    delay(REFRESH_MS);
    return;
  }

  Serial.print(F("Distance (cm): "));
  Serial.println(distance);

  // Map distance to a level 0..7 (nearer object -> higher level)
  int lookUp = map(distance, 0, MAX_CM, 7, 0);
  lookUp = constrain(lookUp, 0, 7);

  // Update LED bar
  shiftOutByte(VALUES_LEDS[lookUp]);

  // Buzzer logic:
  // 0–2  -> mute
  // 3–4  -> short 200 Hz beep
  // 5–6  -> short 5 kHz beep
  // 7    -> mute
  if (lookUp >= 3 && lookUp <= 4) {
    tone(BUZZER_PIN, TONE_MID_FREQ, TONE_DURATION_MS);
  } else if (lookUp >= 5 && lookUp <= 6) {
    tone(BUZZER_PIN, TONE_NEAR_FREQ, TONE_DURATION_MS);
  } else {
    noTone(BUZZER_PIN);
  }

  delay(REFRESH_MS);
}

// -------- Helpers --------
int readDistanceCm() {
  // trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // echo time with a timeout to avoid blocking too long (~25 ms for ~4m)
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return 0; // timeout

  // Convert to centimeters: speed of sound ~ 0.034 cm/us, divide by 2 (go & back)
  int distance = (int)(duration * 0.034 / 2.0);
  return distance;
}

void shiftOutByte(byte value) {
  digitalWrite(LATCH_PIN, LOW);
  // send MSB first to match VALUES_LEDS bit order
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, value);
  digitalWrite(LATCH_PIN, HIGH);
}
