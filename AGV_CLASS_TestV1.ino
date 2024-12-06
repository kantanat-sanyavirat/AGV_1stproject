// import library ที่ใช้ในงานนี้
#include "ControlMotor.h"
#include "encoder.h"

//ประกาศPin ที่ใช้ในการเชื่อมต่อกับบอร์ด esp32
#define ENA 4
#define pin1 16
#define pin2 17
#define pin3 5
#define pin4 18
#define ENB 19
#define encoderLeftPin 35
#define encoderRightPin 23

// สร้าง object ให้กับ class
ControlMotor motorLeft(pin1, pin2, ENA);
ControlMotor motorRight(pin3, pin4, ENB);
Encoder encoderLeft(encoderLeftPin, 20);
Encoder encoderRight(encoderRightPin, 20);

//ตัวแปรสำหรับเก็บค่าระยะทางและความเร็ว
float distance = 0;
double rpmRight = 0;
double rpmLeft = 0;

// ตัวแปรสำหรับการแสดงผล
unsigned long previousTime = 0;

// ตัวแปรสำหรับการหน่วงเวลาก่อนการทำงาน
unsigned long previousStartTime = 0;
bool started = false;  // ตัวแปรเช็คว่าเริ่มทำงานหรือยัง

// กำหนดค่าคงที่ที่ใช้ในการคำนวณระยะทาง
const float wheelRadius = 0.034;        // รัศมีล้อในหน่วยเมตร
float totalDistance = 0;                // ระยะทางสะสม
unsigned long lastCalculationTime = 0;  // เวลาสุดท้ายที่คำนวณระยะทาง


// ฟังก์ชันสำหรับคำนวณระยะทางสะสม
void calculateDistance(float rpm) {

  unsigned long currentCaldisTime = millis();
  float distance = 0;  // ตัวแปรสำหรับเก็บระยะทางในช่วงเวลานั้น

  // ตรวจสอบว่าถึงเวลาในการคำนวณใหม่หรือยัง
  if (currentCaldisTime - lastCalculationTime >= 100) {
    lastCalculationTime = currentCaldisTime;                     // อัปเดตเวลา
    float angularVelocity = (rpm * 2 * PI) / 60.0;         // คำนวณความเร็วเชิงมุม (rad/s)
    float linearVelocity = angularVelocity * wheelRadius;  // คำนวณความเร็วเชิงเส้น (m/s)
    distance = linearVelocity * (100 / 1000.0);            // คำนวณระยะทางในช่วงเวลานั้น (m)
    totalDistance += distance;                             // เพิ่มระยะทางที่คำนวณได้ในระยะทางสะสม
  }
}

void setup() {
  Serial.begin(115200);  // เริ่มต้น Serial Monitor ด้วย baud rate 115200 เพื่อใช้สำหรับการแสดงผลข้อมูล
  encoderLeft.begin();   // เริ่มต้นตัวอ่านค่าการเข้ารหัส (encoder) สำหรับการตรวจจับการหมุน
  encoderRight.begin();
}


void loop() {

  if (millis() - previousStartTime >= 10000) {

    rpmRight = encoderRight.calculateRPM();  // คำนวณรอบต่อนาที (RPM) จากตัวอ่านค่า encoder
    rpmLeft = encoderLeft.calculateRPM();    // คำนวณรอบต่อนาที (RPM) จากตัวอ่านค่า encoder

    motorRight.setMotorSpeed(100);
    motorLeft.setMotorSpeed(100);
    calculateDistance(rpmRight);

    if (millis() - previousTime >= 100) {
      previousTime = millis();
      Serial.print("Left RPM: ");
      Serial.println(rpmLeft);  // แสดงค่าของ RPM ใน Serial Monitor
      Serial.print("Right RPM: ");
      Serial.println(rpmRight);  // แสดงค่าของ RPM ใน Serial Monitor
      Serial.print("distance");
      Serial.println(totalDistance);
    }
  }
}
