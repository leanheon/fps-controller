#include <Arduino.h>
#include <SoftwareSerial.h>

// Define the HC-05 module pins
#define HC05_RX 2
#define HC05_TX 3

// Define the gyroscope sensor pin connections (e.g., MPU6050)
#define GYRO_X_PIN A0
#define GYRO_Y_PIN A1
#define GYRO_Z_PIN A2

SoftwareSerial HC05(HC05_TX, HC05_RX);

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  HC05.begin(9600); // Initialize the HC-05 module at 9600 baud rate
}

void loop() {
  int16_t gx = analogRead(GYRO_X_PIN);
  int16_t gy = analogRead(GYRO_Y_PIN);
  int16_t gz = analogRead(GYRO_Z_PIN);

  // Convert gyro data to a usable format (e.g., float values)
  float x = map(gx, 0, 1023, -1000, 1000);
  float y = map(gy, 0, 1023, -1000, 1000);

  // Send the gyro data to the HC-05 module
  HC05.write((byte)x); // Send x-axis value as a byte (2 bytes for float)
  HC05.write((byte)y); // Send y-axis value as a byte (2 bytes for float)

  delay(10); // Adjust the sampling rate as needed
}