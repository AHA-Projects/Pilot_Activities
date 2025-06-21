// Acknowledgments

// Creator: Anany Sharma at the University of Florida working under NSF grant. 2405373

// This material is based upon work supported by the National Science Foundation under Grant No. 2405373. 
// Any opinions, findings, and conclusions or recommendations expressed in this material are those of the authors and do not necessarily reflect the views of the National Science Foundation.


#include <Wire.h>
#include <VL53L0X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789 display

#define TFT_CS   33  // Chip Select control pin
#define TFT_DC    25  // Data/Command select pin
#define TFT_RST   26  // Reset pin 

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); // Initializing the LED object for its appropriate usage.


VL53L0X sensor;

#define BLACK   0x0000
#define WHITE   0xFFFF


void setup()
{
  Serial.begin(115200);
  Wire.begin();
  display.init(170, 320);  //Intializing and configuring the sensor using the object(tft).
  display.setRotation(3);  

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
  display.fillScreen(BLACK); \
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("Please check your serial ");
  display.setCursor(0, 20);
  display.print("monitor for distance readings.");
}

void loop()
{




  Serial.print("Distance (cm) : ");
  Serial.print((sensor.readRangeContinuousMillimeters() / 10.0));

  Serial.println();
}
