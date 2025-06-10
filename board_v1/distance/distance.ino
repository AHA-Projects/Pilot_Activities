// Acknowledgments

// Creator: Anany Sharma at the University of Florida working under NSF grant. 2405373

// This material is based upon work supported by the National Science Foundation under Grant No. 2405373. 
// Any opinions, findings, and conclusions or recommendations expressed in this material are those of the authors and do not necessarily reflect the views of the National Science Foundation.


#include <GyverOLED.h>
GyverOLED<SSH1106_128x64> oled;



const int trigPin = 33 ;
const int echoPin = 32 ;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  oled.init();  
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  oled.clear();
  oled.setScale(1);
  oled.setCursor(0, 0); 
  oled.print("Check Serial Monitor!"); // Top-left
  oled.update();
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
     
     
  Serial.println("--------------------------");

  // Prints the distance in the Serial Monitor
  Serial.println("Distance(cm) : ");
  Serial.println(distanceCm);
  Serial.println("Distance(Inch) : ");
  Serial.println(distanceInch);
  

      

  
  delay(1000);
}