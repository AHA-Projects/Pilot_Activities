#include <GyverOLED.h>
GyverOLED<SSH1106_128x64> oled; // Ensure this matches your OLED model

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Initialize OLED
  oled.init();
  oled.clear();
  oled.home();
  oled.print("Starting MPU6050...");
  oled.update();
  delay(500);

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize MPU6050!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    oled.clear();
    oled.home();
    oled.print("MPU6050 Init Failed!");
    oled.update();
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  oled.clear();
  oled.home();
  oled.print("MPU6050 Found!");
  oled.update();
  delay(1000);

  // Set up motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1); // lower for more sensitivity
  mpu.setMotionDetectionDuration(20); //  duration in samples
  mpu.setInterruptPinLatch(true);     // Keep it latched until cleared by reading data
  mpu.setInterruptPinPolarity(true);  
  mpu.setMotionInterrupt(true);       // Enable motion interrupt

  Serial.println("MPU6050 motion detection enabled.");
  oled.clear();
  oled.home();
  oled.print("Ready for motion!");
  oled.update();
  delay(1000);

  // Initial display when still
  oled.clear();
  oled.home();
  oled.setScale(2); // Set text size to 2
  oled.print("Still");
  oled.update();
  Serial.println("Initial state: Still"); // Print initial state to Serial
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp); 


  bool motionDetectedThisLoop = mpu.getMotionInterruptStatus(); // This will return true if motion happened recently and was latched

  static bool wasMoving = false; // Tracks the previous state for OLED/Serial updates

  if (motionDetectedThisLoop) {
    if (!wasMoving) { // Only update if state changed from Still to Moving
      oled.clear();
      oled.home();
      oled.setScale(2);
      oled.print("Moving");
      oled.update();
      Serial.println("\n--- Motion Detected ---"); // New line for clarity
      wasMoving = true;
    }

    Serial.print("AccelX:");
    Serial.print(a.acceleration.x, 2); // 2 decimal places
    Serial.print(", AccelY:");
    Serial.print(a.acceleration.y, 2);
    Serial.print(", AccelZ:");
    Serial.print(a.acceleration.z, 2);
    Serial.print(" | GyroX:");
    Serial.print(g.gyro.x, 2);
    Serial.print(", GyroY:");
    Serial.print(g.gyro.y, 2);
    Serial.print(", GyroZ:");
    Serial.print(g.gyro.z, 2);
    Serial.println(""); // New line after each set of readings

  } else { // No motion detected by MPU6050
    if (wasMoving) { // Only update if state changed from Moving to Still
      oled.clear();
      oled.home();
      oled.setScale(2);
      oled.print("Still");
      oled.update();
      Serial.println("\n--- Still ---"); // New line for clarity
      wasMoving = false;
    }
  }

  delay(1000); // Small delay to prevent busy-looping
}