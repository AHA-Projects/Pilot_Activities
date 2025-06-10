#include <GyverOLED.h>
GyverOLED<SSH1106_128x64> oled; // Assuming SSH1106 and 128x64, adjust if needed

#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);

  // Initialize OLED
  oled.init();          // Initialize OLED
  oled.clear();         // Clear the buffer
  oled.home();          // Set cursor to 0,0
  oled.print("Starting APDS9960...");
  oled.update();        // Display on OLED

  if (!apds.begin()) {
    Serial.println("Failed to initialize device! Please check your wiring.");
    oled.clear();
    oled.home();
    oled.print("APDS Init Failed!");
    oled.update();
    while (1); // Halt if APDS fails to initialize
  } else {
    Serial.println("Device initialized!");
    oled.clear();
    oled.home();
    oled.print("APDS Initialized!");
    oled.update();
  }

  //gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);
  apds.enableGesture(true);

  delay(1000); // Give some time to read the initial message
  oled.clear(); // Clear the "Initialized" message
  oled.update();
}

// the loop function runs over and over again forever
void loop() {
  //read a gesture from the device
  uint8_t gesture = apds.readGesture();

  oled.clear(); // Clear the previous display
  oled.home();  // Set cursor to top-left
  
  if (gesture == 0) {
    oled.print("Waiting for gesture...");
    oled.update();
    return; // Nothing new to show, so skip the rest.
  }

  if (gesture == APDS9960_DOWN) {
    Serial.println("v");
    oled.print("Gesture: DOWN");
    
  } else if (gesture == APDS9960_UP) {
    Serial.println("^");
    oled.print("Gesture: UP");
  } else if (gesture == APDS9960_LEFT) {
    Serial.println("<");
    oled.print("Gesture: LEFT");
  } else if (gesture == APDS9960_RIGHT) {
    Serial.println(">");
    oled.print("Gesture: RIGHT");
  }

  oled.update(); // Display the updated text on the OLED
  delay(500); // Small delay to prevent rapid updates and make it readable
}