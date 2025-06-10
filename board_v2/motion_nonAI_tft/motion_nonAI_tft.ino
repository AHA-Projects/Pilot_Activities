#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_ST7789.h> 
#include <SPI.h>             



#define TFT_CS    33  // TFT Chip Select pin: Tells the screen when we're sending it data.
#define TFT_DC    25  // TFT Data/Command pin: Tells the screen if we're sending a command or actual data to display.
#define TFT_RST   26  // TFT Reset pin: Can be used to reset the screen if something goes wrong.

// --- Color Definitions (Standard 16-bit RGB565) ---
#define BLACK   0x0000  
#define WHITE   0xFFFF  
#define GREEN   0x07E0  
#define RED     0xF800  

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); // This is our screen object.
Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); 

  // Initialize TFT
  tft.init(170, 320);     
  Serial.println("TFT initialized"); 
  tft.setRotation(3);     
  tft.fillScreen(BLACK);  
  delay(500);

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize MPU6050!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(5, 5);
    tft.println("MPU6050 Init Failed!");
    // No tft.update() needed for Adafruit_ST7789, it draws directly.
    while (1) { // Halt if MPU fails to initialize
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  tft.fillScreen(BLACK);          
  tft.setTextSize(2);             
  tft.setTextColor(GREEN);        
  tft.setCursor(5, 5);            
  tft.println("MPU6050 Found!");
  delay(1000);

  // Set up motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1); 
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);     
  mpu.setInterruptPinPolarity(true);  
  mpu.setMotionInterrupt(true);       

  Serial.println("MPU6050 motion detection enabled.");
  tft.fillScreen(BLACK);          
  tft.setTextSize(2);             
  tft.setTextColor(WHITE);        
  tft.setCursor(5, 5);            
  tft.println("Ready for motion!");
  delay(1000);

  // Initial display when still
  tft.fillScreen(BLACK); 
  tft.setTextSize(2);    
  tft.setTextColor(WHITE); 
  tft.setCursor(5, 5);   
  tft.print("Still");    
  Serial.println("Initial state: Still"); // Print initial state to Serial
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

 bool motionDetectedThisLoop = mpu.getMotionInterruptStatus();

  static bool wasMoving = false; // Tracks the previous state for display updates

  if (motionDetectedThisLoop) {
    if (!wasMoving) { // Only update if state changed from Still to Moving
      tft.fillScreen(BLACK); 
      tft.setTextSize(2);    
      tft.setTextColor(RED); 
      tft.setCursor(5, 5);   
      tft.print("Moving");   
      Serial.println("\n--- Motion Detected ---"); 
      wasMoving = true;
    }

    Serial.print("AccelX:");
    Serial.print(a.acceleration.x, 2); 
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
    Serial.println(""); 

  } else { // No motion detected by MPU6050
    if (wasMoving) { 
      tft.fillScreen(BLACK); 
      tft.setTextSize(2);    
      tft.setTextColor(GREEN); 
      tft.setCursor(5, 5);   
      tft.print("Still");   
      Serial.println("\n--- Still ---"); 
      wasMoving = false;
    }
  }

  delay(1200); 
}