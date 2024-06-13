//calibrate_air_mouse
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Mouse.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) { while (1); }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = (gx+300)/150;  // change 300 from 0 to 355
  vy = -(gz-100)/150; // same here about "-100"  from -355 to 0

  Serial.print("gx = ");
  Serial.print(gx);
  Serial.print(" | gz = ");
  Serial.print(gz);
  
  Serial.print("        | X = ");
  Serial.print(vx);
  Serial.print(" | Y = ");
  Serial.println(vy);
  
  Mouse.move(vx, vy);
  
  delay(20);
}