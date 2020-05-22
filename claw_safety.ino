#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int n = 180;
int i = 0;
int fsrPin = A0;     // the FSR and 10K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider
 
void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); 
}

void loop() {
  
  fsrReading = analogRead(fsrPin);  //analog reading
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);     // the raw analog reading
  
  for (i = 0; i <= 180; i += 1) { // goes from 0 degrees to 180 degrees
    fsrReading = analogRead(fsrPin);  //analog reading
    Serial.print("Analog reading = ");
    Serial.println(fsrReading);     // the raw analog reading
    // in steps of 1 degree
    if(fsrReading > 309) { //309 is the preset value for too much force
      //once the sensor reads 309, it sets the position to the current value and jumps out of this loop
      n = i;
      i = 181;
    } else {
      pos = i;
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
  for (pos = n; pos >= 0; pos -= 1) { // goes from n (usually 180) degrees to 0 degrees
    fsrReading = analogRead(fsrPin);  //analog reading
    Serial.print("Analog reading = ");
    Serial.println(fsrReading);     // the raw analog reading
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
