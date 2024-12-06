#ifndef ENCODER_H 
#define ENCODER_H

#include <Arduino.h>

class Encoder {
private:
    int encoderPin;
    volatile int pulseCount;
    unsigned long lastInterruptTime;
    const unsigned long debounceDelay;
    const int pulsesPerRevolution;
    unsigned long prevTime;
    float rpm;

    static Encoder* instances[2]; // เก็บตัวชี้สำหรับ encoder 2 ตัว
    static int instanceCount;    // ตัวนับสำหรับติดตามจำนวน Encoder ที่สร้าง

public:
    Encoder(int pin, int pulsesPerRev, unsigned long debounceDelay = 10);

    static void IRAM_ATTR countPulse0(); // ISR สำหรับ encoder ตัวที่ 1
    static void IRAM_ATTR countPulse1(); // ISR สำหรับ encoder ตัวที่ 2

    void begin();

    float calculateRPM();

    float getRPM();
};

#endif
