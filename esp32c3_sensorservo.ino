/*
  DM3014 Interactive Devices, Ashley Hi 2026
  Week 8 - Microcontrollers & Sensors
  ESP32-C3 Sensor Servo Example
  When distance is less than 20cm, ultrasonic sensor triggers servo to turn 90º
  and the built-in LED blinks twice.
  Additional Components: Servo Motor, HC-SR04 Sensor
*/

// ====== Reference Libraries ======
#include <ESP32Servo.h>

// ====== Set Servo ======
#define SERVO_PIN 7
#define SERVO_ACTIVE_ANGLE 90   // *** servo angle when triggered (0 – 180)
#define SERVO_REST_ANGLE   0    // *** servo angle when idle (0 – 180)

// ====== Set HC-SR04 Sensor ======
#define TRIG_PIN  3
#define ECHO_PIN  4
#define DISTANCE_THRESHOLD  20 // *** edit the sensor trigger distance here (in cm)

// ====== Set LED ======
#define LED_PIN 8               // *** built-in LED pin (GPIO 8 on many ESP32-C3 boards; check yours)
#define LED_ON  LOW             // *** many C3 boards are active-LOW; swap LOW/HIGH if it's inverted
#define LED_OFF HIGH
#define BLINK_COUNT 2
#define BLINK_MS    150

// ====== Create Servo Objects ======
Servo servo;

// ====== Sensor Variables ======
float duration_us, distance_cm;
bool wasNear = false;           // remembers last state so we only blink on a new detection

void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, LED_ON);
    delay(BLINK_MS);
    digitalWrite(LED_PIN, LED_OFF);
    delay(BLINK_MS);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 500, 2000);
  servo.write(SERVO_REST_ANGLE);
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  bool isNear = (distance_cm < DISTANCE_THRESHOLD);

  if (isNear) {
    servo.write(SERVO_ACTIVE_ANGLE);
    if (!wasNear) {              // just entered range -> blink twice
      blinkLED(BLINK_COUNT);
    }
  } else {
    servo.write(SERVO_REST_ANGLE);
  }
  wasNear = isNear;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}