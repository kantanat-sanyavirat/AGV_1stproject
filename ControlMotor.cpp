#include "ControlMotor.h"

// Constructor: ใช้กำหนดค่าเริ่มต้นให้กับมอเตอร์
ControlMotor::ControlMotor(int pin1, int pin2, int ENB) {
  _pin1 = pin1; // ขา GPIO ที่เชื่อมต่อกับ input1 ของมอเตอร์
  _pin2 = pin2; // ขา GPIO ที่เชื่อมต่อกับ input2 ของมอเตอร์
  _ENB = ENB;   // ขา GPIO ที่เชื่อมต่อกับ ENB (PWM)

  // ตั้งค่าขา GPIO ให้เป็น output
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  pinMode(_ENB, OUTPUT);

  // ตั้งค่าการเชื่อมต่อ PWM สำหรับขา ENB
  ledcAttach(_ENB, frequency, resolution); // เชื่อม ENB กับช่อง PWM (Channel 0)
}

// เมธอดสำหรับตั้งค่าความเร็วของมอเตอร์
void ControlMotor::setMotorSpeed(int percentSpeed) {
  _percentSpeed = percentSpeed; // บันทึกค่าความเร็วในตัวแปร `_percentSpeed`

  // แปลงค่าความเร็วจากเปอร์เซ็นต์ (-100 ถึง 100) ให้เป็นค่า duty cycle (0 ถึง 1023 สำหรับ 10-bit resolution)
  speed = map(abs(_percentSpeed), 0, 100, 0, 1023);

  if (_percentSpeed > 0) {
    // หมุนไปข้างหน้า
    digitalWrite(_pin1, HIGH); // เปิด input1
    digitalWrite(_pin2, LOW);  // ปิด input2
  } else if (_percentSpeed < 0) {
    // หมุนถอยหลัง
    digitalWrite(_pin1, LOW);  // ปิด input1
    digitalWrite(_pin2, HIGH); // เปิด input2
  } else {
    // หยุดมอเตอร์
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
  }

  // ส่งสัญญาณ PWM ไปที่ขา ENB เพื่อควบคุมความเร็ว
  ledcWrite(_ENB, speed);
}

// เมธอดสำหรับดึงค่าความเร็วปัจจุบันเป็นเปอร์เซ็นต์
int ControlMotor::showPercentSpeed() {
  return _percentSpeed; // คืนค่าความเร็วปัจจุบันในหน่วยเปอร์เซ็นต์
}
