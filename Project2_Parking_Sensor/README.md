# Parking Sensor System 🚗🔊

## Project Overview
This project uses an **Arduino Uno** with an **ultrasonic sensor (HC-SR04)** to create a real-time distance monitoring and alert system.  
The measured distance is displayed on a row of LEDs, and a buzzer generates different alerts depending on how close an object is.

## Components Used
- Arduino Uno
- Ultrasonic sensor (HC-SR04)
- Breadboard
- Jumper wires
- LEDs
- Buzzer
- Shift register (for LED control)

## Features
- Real-time distance measurement (0–150 cm).
- LED bar indicator to visualize distance.
- Buzzer alerts with different tones and durations depending on distance thresholds.
- Educational and practical design for proximity sensing.

## Algorithm
1. Initialize pins for ultrasonic sensor, buzzer, and LED display.
2. Send a trigger pulse to the ultrasonic sensor and measure echo duration.
3. Convert echo duration into distance (cm).
4. Map the distance to LED index values.
5. Trigger buzzer alerts based on thresholds.
6. Update LED display patterns via shift register.
7. Repeat continuously in the loop.

## How to Run
1. Open `src/parking_sensor.ino` in Arduino IDE.  
2. Connect your Arduino Uno to your computer.  
3. Upload the sketch.  
4. Place objects in front of the sensor to test LED and buzzer response.

## Documentation
The full project report is in [`docs/Project2_Parking_Sensor.pdf`](docs/Project2_Parking_Sensor.pdf).

## License
This project is licensed under the MIT License.
