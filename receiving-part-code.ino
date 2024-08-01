#include <SoftwareSerial.h>
#include <Keyboard.h>
#include <Mouse.h>

SoftwareSerial bluetooth(2, 3); // RX, TX

const int SENSITIVITY = 50; // Adjust as needed
const int CENTER_BUTTON = 7; // Button to center the mouse

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  Keyboard.begin();
  Mouse.begin();
  pinMode(CENTER_BUTTON, INPUT_PULLUP);
}

void loop() {
  if (bluetooth.available()) {
    String data = bluetooth.readStringUntil('\n');
    processData(data);
  }
  
  // Check for center button press
  if (digitalRead(CENTER_BUTTON) == LOW) {
    centerMouse();
  }
}

void processData(String data) {
  int gx, gy, joyX, joyY;
  bool trigger, megazine, reaim, skill1, skill2, skill3, skill4, skill5, zoom, jump;
  
  sscanf(data.c_str(), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &gx, &gy, &joyX, &joyY, &trigger, &megazine, &reaim, &skill1, &skill2, &skill3, &skill4, &skill5, &zoom, &jump);
  
  // Process mouse movement
  int mouseX = map(gx, -32768, 32767, -SENSITIVITY, SENSITIVITY);
  int mouseY = map(gy, -32768, 32767, -SENSITIVITY, SENSITIVITY);
  Mouse.move(mouseX, mouseY, 0);
  
  // Process WASD movement
  if (joyY < 400) Keyboard.press('W');
  else if (joyY > 600) Keyboard.press('S');
  else {
    Keyboard.release('W');
    Keyboard.release('S');
  }
  
  if (joyX < 400) Keyboard.press('A');
  else if (joyX > 600) Keyboard.press('D');
  else {
    Keyboard.release('A');
    Keyboard.release('D');
  }
  
  // Process trigger
  if (trigger) Mouse.press(MOUSE_LEFT);
  else Mouse.release(MOUSE_LEFT);
  
  // Process reloading
  if (megazine) Keyboard.press('R');
  else Keyboard.release('R');
  
  // Process reaim
  if (reaim) {
    centerMouse();
  }

  if (megazine) Keyboard.press('R');
  else Keyboard.release('R');

  if (skill1) Keyboard.press('1');
  else Keyboard.release('1');

  if (megazine) Keyboard.press('2');
  else Keyboard.release('2');

  if (megazine) Keyboard.press('2');
  else Keyboard.release('2');

  if (megazine) Keyboard.press('3');
  else Keyboard.release('3');

  if (megazine) Keyboard.press('4');
  else Keyboard.release('4');

  if (megazine) Keyboard.press('5');
  else Keyboard.release('5');

  if (zoom) Mouse.press(MOUSE_RIGHT);
  else Mouse.release(MOUSE_RIGHT);

  if (megazine) Keyboard.press(32);
  else Keyboard.release(32);

}

void centerMouse() {
  // Get the current screen resolution
  // Note: This is a placeholder. You'll need to set these values manually
  // based on the target computer's screen resolution.
  int screenWidth = 1920;
  int screenHeight = 1080;
  
  // Move the mouse to the center of the screen
  Mouse.move((screenWidth / 2) - Mouse.getScreenWidth(), 
             (screenHeight / 2) - Mouse.getScreenHeight());
}