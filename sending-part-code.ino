#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

MPU6050 mpu;
SoftwareSerial bluetooth(2, 3); // RX, TX

const int JOYSTICK_X = A0;
const int JOYSTICK_Y = A1;
const int TRIGGER_BTN = 4;
const int MAGAZINE_BTN = 5;
const int REAIM_BTN = 6;
const int SKILL_1 = 7;
const int SKILL_2 = 8;
const int SKILL_3 = 9;
const int SKILL_4 = 10;
const int SKILL_5 =11;
const int ZOOM = A2;
const int JUMP = A3;

// Solenoid pins
const int SOLENOID_PINS[] = {12, 13};
const int SOLENOID_COUNT = 2;

bool reloading = false;
bool triggerHeld = false;
unsigned long lastRecoilTime = 0;
const int RECOIL_INTERVAL = 100; // Time between recoil activations in milliseconds
const int SOLENOID_ACTIVATION_TIME = 20; // Solenoid activation time in milliseconds

int lastMagazineState = HIGH;
int currentMagazineState;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  Wire.begin();
  mpu.initialize();
  
  pinMode(TRIGGER_BTN, INPUT_PULLUP);
  pinMode(MAGAZINE_BTN, INPUT_PULLUP);
  pinMode(REAIM_BTN, INPUT_PULLUP);
  pinMode(SKILL_1, INPUT_PULLUP);
  pinMode(SKILL_2, INPUT_PULLUP);
  pinMode(SKILL_3, INPUT_PULLUP);
  pinMode(SKILL_4, INPUT_PULLUP);
  pinMode(SKILL_5, INPUT_PULLUP);
  pinMode(ZOOM, INPUT_PULLUP);
  pinMode(JUMP, INPUT_PULLUP);
  
  for (int i = 0; i < SOLENOID_COUNT; i++) {
    pinMode(SOLENOID_PINS[i], OUTPUT);
    digitalWrite(SOLENOID_PINS[i], LOW);
  }
}

void loop() {
  // Read gyroscope data
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Read joystick data
  int joyX = analogRead(JOYSTICK_X);
  int joyY = analogRead(JOYSTICK_Y);
  
  // Read button states
  bool trigger = !digitalRead(TRIGGER_BTN);
  bool reaim = !digitalRead(REAIM_BTN);
  bool skill1 = !digitalRead(SKILL_1);
  bool skill2 = !digitalRead(SKILL_2);
  bool skill3 = !digitalRead(SKILL_3);
  bool skill4 = !digitalRead(SKILL_4);
  bool skill5 = !digitalRead(SKILL_5);
  bool magazine = false;
  bool zoom = !digitalRead(ZOOM);
  bool jump = !digitalRead(JUMP);
  
  currentMagazineState = digitalRead(magazineDetectPin);

  if (currentMagazineState == LOW && lastMagazineState == HIGH) {
    bool magazine = true;
    delay(50);  // 키 입력 시간
    bool magazine = false;
    
    // 디바운싱을 위한 짧은 지연
    delay(50);

    magazine = !digitalRead(MAGAZINE_BTN);
  }
  // 탄창이 빠졌을 때 (rising edge: LOW에서 HIGH로 변경)
  else if (currentMagazineState == HIGH && lastMagazineState == LOW) {
    // 탄창 제거 시 추가 동작이 필요하다면 여기에 코드를 작성하세요
  }

  // 현재 상태를 저장
  lastMagazineState = currentMagazineState;
  
  // Process shooting and recoil
  if (trigger && !reloading) {
    triggerHeld = true;
    if (millis() - lastRecoilTime >= RECOIL_INTERVAL) {
      simulateRecoil();
      lastRecoilTime = millis();
    }
  } else {
    triggerHeld = false;
  }
  
  // Prepare data to send
  String data = String(gx) + "," + String(gy) + "," + 
                String(joyX) + "," + String(joyY) + "," +
                String(trigger) + "," + String(magazine) + "," +
                String(reaim) + "," + String(skill1) + "," + 
                String(skill2) + "," + String(skill3) + "," + 
                String(skill4) + "," + String(skill5) + "," +
                String(zoom) + "," + String(jump);
  
  // Send data via Bluetooth
  bluetooth.println(data);
  
  delay(10); // Adjust delay as needed for performance
}

void simulateRecoil() {
  // Activate all solenoids simultaneously
  for (int i = 0; i < SOLENOID_COUNT; i++) {
    digitalWrite(SOLENOID_PINS[i], HIGH);
  }
  
  delay(SOLENOID_ACTIVATION_TIME); // Keep solenoids activated for the specified time
  
  // Deactivate all solenoids
  for (int i = 0; i < SOLENOID_COUNT; i++) {
    digitalWrite(SOLENOID_PINS[i], LOW);
  }
}