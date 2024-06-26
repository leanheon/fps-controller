#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

MPU6050 mpu;
SoftwareSerial bluetooth(2, 3); // RX, TX

const int JOYSTICK_X = A0;
const int JOYSTICK_Y = A1;
const int TRIGGER_BTN = 4;
const int MAGAZINE_BTN = 5;
const int HANDLE_BTN = 6;
const int REAIM_BTN = 7;

// Solenoid pins
const int SOLENOID_PINS[] = {8, 9, 10, 11, 12, 13};
const int SOLENOID_COUNT = 6;

bool reloading = false;
bool triggerHeld = false;
unsigned long lastRecoilTime = 0;
const int RECOIL_INTERVAL = 100; // Time between recoil activations in milliseconds
const int SOLENOID_ACTIVATION_TIME = 20; // Solenoid activation time in milliseconds

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  Wire.begin();
  mpu.initialize();
  
  pinMode(TRIGGER_BTN, INPUT_PULLUP);
  pinMode(MAGAZINE_BTN, INPUT_PULLUP);
  pinMode(HANDLE_BTN, INPUT_PULLUP);
  pinMode(REAIM_BTN, INPUT_PULLUP);
  
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
  bool magazine = !digitalRead(MAGAZINE_BTN);
  bool handle = !digitalRead(HANDLE_BTN);
  bool reaim = !digitalRead(REAIM_BTN);
  
  // Process reload
  if (magazine && handle && !reloading) {
    reloading = true;
  } else if (!magazine && !handle) {
    reloading = false;
  }
  
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
                String(trigger) + "," + String(reloading) + "," +
                String(reaim);
  
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