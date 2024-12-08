// Import libraries
#include <PID_v1_bc.h>
#include "ControlMotor.h"
#include "encoder.h"

// Pin definitions
#define ENA 4
#define pin1 16
#define pin2 17
#define pin3 5
#define pin4 18
#define ENB 19
#define encoderLeftPin 35
#define encoderRightPin 23

// PID variables
double SetpointRight, InputRight, OutputRight;
double SetpointLeft, InputLeft, OutputLeft;
double KpLeft = 2.25, KiLeft = 1.47, KdLeft = 0.07;
double KpRight = 0.94, KiRight = 0.39, KdRight = 0.09;

// Speed and distance variables
float totalDistance = 0;          // Cumulative distance
unsigned long lastCalculationTime = 0;
double rpmRight = 0;              // RPM of right motor
double rpmLeft = 0;               // RPM of left motor

// Timing variables
unsigned long previousTime = 0;
unsigned long previousStartTime = 0;
bool started = false;

// Constants
const float wheelRadius = 0.034;  // Wheel radius in meters

// Create objects
ControlMotor motorLeft(pin1, pin2, ENA);
ControlMotor motorRight(pin3, pin4, ENB);
Encoder encoderLeft(encoderLeftPin, 20);
Encoder encoderRight(encoderRightPin, 20);
PID pidMotorRight(&InputRight, &OutputRight, &SetpointRight, KpRight, KiRight, KdRight, DIRECT);
PID pidMotorLeft(&InputLeft, &OutputLeft, &SetpointLeft, KpLeft, KiLeft, KdLeft, DIRECT);

// Distance calculation function
void calculateDistance(float rpm) {
  unsigned long currentCaldisTime = millis();
  if (currentCaldisTime - lastCalculationTime >= 100) {
    lastCalculationTime = currentCaldisTime;
    float angularVelocity = (rpm * 2 * PI) / 60.0;  // Angular velocity in rad/s
    float linearVelocity = angularVelocity * wheelRadius;  // Linear velocity in m/s
    float distance = linearVelocity * (100 / 1000.0);  // Distance in meters
    totalDistance += distance;  // Update total distance
  }
}

void setup() {
  Serial.begin(115200);  // Start Serial Monitor
  encoderLeft.begin();   // Initialize encoders
  encoderRight.begin();

  // Initialize PID controllers
  pidMotorRight.SetMode(AUTOMATIC);
  pidMotorRight.SetOutputLimits(0, 100);  // PWM range 0-100%
  pidMotorRight.SetSampleTime(100);       // Update every 100 ms

  pidMotorLeft.SetMode(AUTOMATIC);
  pidMotorLeft.SetOutputLimits(0, 100);  // PWM range 0-100%
  pidMotorLeft.SetSampleTime(100);       // Update every 100 ms
}

void loop() {
  // Check if system has started
  if (millis() - previousStartTime >= 10000) {
    rpmRight = encoderRight.calculateRPM();
    rpmLeft = encoderLeft.calculateRPM();
    calculateDistance(rpmRight);

    InputRight = rpmRight;
    InputLeft = rpmLeft;

    // Log RPM and distance every 100 ms
    if (millis() - previousTime >= 100) {
      previousTime = millis();
      Serial.print("Left RPM: ");
      Serial.println(rpmLeft);
      Serial.print("Right RPM: ");
      Serial.println(rpmRight);
      Serial.print("Distance (m): ");
      Serial.println(totalDistance);
      // Serial.print("outputleft : ");
      // Serial.println(OutputLeft);
      // Serial.print("output right");
      // Serial.println(OutputRight);
    }

    // Drive motors until total distance is reached
    if (totalDistance < 5) {  // Distance in meters
      SetpointLeft = 390;  // Target RPM
      SetpointRight = 390;

      pidMotorRight.Compute();
      pidMotorLeft.Compute();

      motorLeft.setMotorSpeed(OutputLeft);
      motorRight.setMotorSpeed(OutputRight);
    } else {
      motorLeft.setMotorSpeed(0);
      motorRight.setMotorSpeed(0);
    }
  }
}
