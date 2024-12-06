#ifndef CONTROLMOTOR_H
#define CONTROLMOTOR_H

#include <Arduino.h>

// กำหนดคลาส ControlMotor สำหรับควบคุมมอเตอร์
class ControlMotor {

  private:  
    int _pin1;          // พินสำหรับทิศทางมอเตอร์ 1 (เชื่อมต่อกับตัวขับมอเตอร์)
    int _pin2;          // พินสำหรับทิศทางมอเตอร์ 2 (เชื่อมต่อกับตัวขับมอเตอร์)
    int _ENB;           // พินสำหรับ PWM (พินเปิด/ปิดการทำงานของมอเตอร์)
    int speed = 0;          // ค่าความเร็วที่แมปไว้สำหรับ PWM เริ่มต้นที่ 0
    int _percentSpeed = 0;  // ความเร็วของมอเตอร์ในรูปแบบเปอร์เซ็นต์ (จาก -100 ถึง 100)
    const int resolution = 10; // ความละเอียดของ PWM (จำนวนบิต) ในที่นี้คือ 10 บิต (0 ถึง 1023)
    const int frequency = 900; // ความถี่ของ PWM ในหน่วย Hz (กำหนดความเร็วในการควบคุมมอเตอร์)

  public:
    // ตัวสร้างที่ใช้ในการกำหนดพินของมอเตอร์
    ControlMotor(int pin1, int pin2, int ENB); 

    // ฟังก์ชันสำหรับตั้งค่าความเร็วมอเตอร์ โดยรับค่าเปอร์เซ็นต์ (-100 ถึง 100)
    void setMotorSpeed(int percentSpeed);     

    // ฟังก์ชันที่ใช้คืนค่าความเร็วของมอเตอร์ในรูปแบบเปอร์เซ็นต์
    int showPercentSpeed();                   
};

#endif

