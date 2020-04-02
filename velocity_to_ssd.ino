#include <Event.h>
#include <Timer.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

int digit1 = 11; //PWM Display leftmost 
int digit2 = 10; //PWM Display one from left
int digit3 = 9; //PWM Display one from right
int digit4 = 6; //PWM Display rightmost 
Timer t;


float mag = 0.0;

float x1 = 0.0; 
float x2 = 0.0;
float x3 = 0.0;

float v.x = 0.0;
float v.y = 0.0; 
float v.z = 0.0; 


#define DIGIT_ON  LOW
#define DIGIT_OFF  LOW
#define DISPLAY_BRIGHTNESS  500

int segA = 2; 
int segB = 3; 
int segC = 4; 
int segD = 5; 
int segE = 12; //seg 6 is used bij display 1 for its pwm function
int segF = 7; 
int segG = 8; 
int segPD = 13; 

String string1 = "";
String string2 = "";
String string3 = "";
String string4 = "";



/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (2)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/

void setup(void)
{

  
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segPD, OUTPUT);
  
 pinMode(digit1, OUTPUT);
 pinMode(digit2, OUTPUT);
 pinMode(digit3, OUTPUT);
 pinMode(digit4, OUTPUT);

 t.every(200, takeReading);
 //t.every(201, takeReading2);

  Serial.begin(9600);
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  bno.setExtCrystalUse(true);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");

//make code so that if the first digit is not 0, then make the first digit 0 and shift it down 
//also have to account for the decimal place being a part of the string (detect decimal point, delete it, then splice it back together)

}

void display0(){
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, HIGH);   
  digitalWrite(segE, HIGH);   
  digitalWrite(segF, HIGH);   
  digitalWrite(segG, LOW);  
}
void display1(){
  digitalWrite(segA, LOW);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, LOW);   
  digitalWrite(segE, LOW);   
  digitalWrite(segF, LOW);   
  digitalWrite(segG, LOW);

}

void display2() {
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, LOW);   
  digitalWrite(segD, HIGH);   
  digitalWrite(segE, HIGH);   
  digitalWrite(segF, LOW);   
  digitalWrite(segG, HIGH);

}

void display3() {
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, HIGH);   
  digitalWrite(segE, LOW);   
  digitalWrite(segF, LOW);   
  digitalWrite(segG, HIGH);  
}

void display4() {
  digitalWrite(segA, LOW);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, LOW);   
  digitalWrite(segE, LOW);   
  digitalWrite(segF, HIGH);   
  digitalWrite(segG, HIGH);  
}

void display5(){
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, LOW);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, HIGH);   
  digitalWrite(segE, LOW);   
  digitalWrite(segF, HIGH);   
  digitalWrite(segG, HIGH);   
}

void display6(){
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, LOW);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, HIGH);   
  digitalWrite(segE, HIGH);   
  digitalWrite(segF, HIGH);   
  digitalWrite(segG, HIGH);  
}

void display7(){
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, LOW);   
  digitalWrite(segE, LOW);   
  digitalWrite(segF, LOW);   
  digitalWrite(segG, LOW);    
}

void display8(){
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, HIGH);   
  digitalWrite(segE, HIGH);   
  digitalWrite(segF, HIGH);   
  digitalWrite(segG, HIGH);   
}

void display9(){
  digitalWrite(segA, HIGH);   
  digitalWrite(segB, HIGH);   
  digitalWrite(segC, HIGH);   
  digitalWrite(segD, LOW);   
  digitalWrite(segE, LOW);   
  digitalWrite(segF, HIGH);   
  digitalWrite(segG, HIGH);  
  
}

void displaying(String give){
  int var = give.toInt();
  switch(var){
    case 0:
      display0(); 
      break;
    case 1:
      display1();
      break; 
    case 2:
      display2();
      break; 
    case 3:
      display3();
      break; 
    case 4:
      display4();
      break; 
    case 5:
      display5();
      break; 
    case 6:
      display6();
      break; 
    case 7:
      display7();
      break;
    case 8:
      display8();
      break;
    case 9:
      display9();
      break;      
  }
}
/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void ssd(String mags){

  String magstring = mags;


  digitalWrite(digit1, LOW);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);

  if(magstring.length() == 5){
    string1 = magstring.substring(0,1);    
  }
  else {
    string1 = '0';
  }
   
  displaying(string1);
  delay(5);               // wait for a second  
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);  
  delay(1);
 
  digitalWrite(segPD, HIGH);    
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, LOW);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);  

  if(magstring.length() == 5){
    string2 = magstring.substring(1,2);
  }
  else {
    string2 = magstring.substring(0,1);
  }
  
  displaying(string2);  
  delay(5);               // wait for a second

  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);
  delay(1);
  
  digitalWrite(segPD, LOW);

  
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, LOW);
  digitalWrite(digit4, HIGH);  

  if(magstring.length() == 5) {
    string3 = magstring.substring(3,4);
  }
  else {
    string3 = magstring.substring(2,3);
  }
  
    
  displaying(string3);
  delay(5);               // wait for a second

  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);
  delay(1);
  
  
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, LOW); 

  if(magstring.length() == 5){
    string4 = magstring.substring(4,5);
  }
  else {
    string4 = magstring.substring(3,4);
  }
  
  displaying(string4); 
  delay(5);               // wait for a second

  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);
  delay(1);


}


void loop(void){
  /* Display the floating point data */

  String magstring = String(abs(mag));
  t.update();
  
  ssd(magstring);

}

void takeReading()
{
  imu::Vector<3> lineacc = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  
  //take 1/2*(x1 + two1) * 5 = velocity value
  //take magnitude of all components (pythagorean thm) 
  //serial.print velocity
  
  x1 = lineacc.x;
  x2 = lineacc.y;
  x3 = lineacc.z;

  delay(5);

  float two1 = lineacc.x;
  float two2 = lineacc.y;
  float two3 = lineacc.z;  

  v.x = 2.5*(x1 + two1);
  v.y = 2.5*(x2 + two2);
  v.z = 2.5*(x3 + two3);

  mag = sqrt(pow((v.x),2) + (pow((v.y,2))) + (pow((v.z,2)))) 
  
  Serial.println(abs(mag));  
}

//void takeReading2()
//{
  //sensors_event_t event1; 
  //bno.getEvent(&event1);

  //float two1 = event1.orientation.x;
  //float two2 = event1.orientation.y;
  //float two3 = event1.orientation.z;   
  //Serial.println(x1);

  //Serial.println(two1);
  
  //mag = abs((two1-x1))/.1;

  //Serial.println(abs(mag));
  
//}
