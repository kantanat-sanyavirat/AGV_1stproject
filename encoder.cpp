#include "encoder.h"

Encoder* Encoder::instances[2] = {nullptr, nullptr}; // เก็บตัวชี้ของ encoder
int Encoder::instanceCount = 0; // เริ่มต้นจำนวน instance เป็น 0

Encoder::Encoder(int pin, int pulsesPerRev, unsigned long debounceDelay)
    : encoderPin(pin),
      pulseCount(0),
      lastInterruptTime(0), 
      debounceDelay(debounceDelay),
      pulsesPerRevolution(pulsesPerRev),
      prevTime(0),
      rpm(0) {
    if (instanceCount < 2) {
        instances[instanceCount++] = this; // เพิ่ม encoder instance ในอาร์เรย์
    }
}

void IRAM_ATTR Encoder::countPulse0() {
    if (instances[0]) {
        unsigned long interruptTime = micros();
        if (interruptTime - instances[0]->lastInterruptTime > instances[0]->debounceDelay) {
            instances[0]->pulseCount++;
            instances[0]->lastInterruptTime = interruptTime;
        }
    }
}

void IRAM_ATTR Encoder::countPulse1() {
    if (instances[1]) {
        unsigned long interruptTime = micros();
        if (interruptTime - instances[1]->lastInterruptTime > instances[1]->debounceDelay) {
            instances[1]->pulseCount++;
            instances[1]->lastInterruptTime = interruptTime;
        }
    }
}

void Encoder::begin() {
    pinMode(encoderPin, INPUT_PULLUP);
    if (this == instances[0]) {
        attachInterrupt(digitalPinToInterrupt(encoderPin), Encoder::countPulse0, RISING);
    } else if (this == instances[1]) {
        attachInterrupt(digitalPinToInterrupt(encoderPin), Encoder::countPulse1, RISING);
    }
}

float Encoder::calculateRPM() {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - prevTime;

    if (elapsedTime >= 100) {
        prevTime = currentTime;
        noInterrupts();
        int pulses = pulseCount;
        pulseCount = 0;
        interrupts();
        rpm = (float)pulses / pulsesPerRevolution * (60000.0 / elapsedTime);
    }

    return rpm;
}

float Encoder::getRPM() {
    return rpm;
}
