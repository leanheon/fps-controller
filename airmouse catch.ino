#define MOUSE_X_PIN A0
#define MOUSE_Y_PIN A1

SoftwareSerial HC06(HC06_TX, HC06_RX);

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  HC06.begin(9600); // Initialize the HC-06 module at 9600 baud rate
}

void loop() {
  if (HC06.available() > 0) {
    byte xData = HC06.read();
    byte yData = HC06.read();

    // Convert received byte data to float values
    float x = (float)xData;
    float y = (float)yData;

    // Use the received data to control the mouse cursor
    analogWrite(MOUSE_X_PIN, map(x, -1000, 1000, 0, 255));
    analogWrite(MOUSE_Y_PIN, map(y, -1000, 1000, 0, 255));

    delay(10); // Adjust the sampling rate as needed
  }
}