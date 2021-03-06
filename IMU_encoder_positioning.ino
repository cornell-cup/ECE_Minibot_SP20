#include <elapsedMillis.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

elapsedMillis timeElapsed;

int val;
//Regulates two motors to spin at same speed for encoder of motor one
int encoder0PinA = A1; //J3 motor on board
//int encoder0PinB = 4;
int encoder0Pos = 0; //Motor's angular position read by the encoder
int encoder0PinALast = LOW;

//for encoder for motor two
int encoder1PinA = A2; //J4 motor on board
int encoder1Pos = 0;
int encoder1PinALast = LOW;

//int encoderCountpRev = 360;
//setpoint is turn rate to compare to/reach
int setpoint = 120; //(degrees/sec)
double Integral0 = 0; //accumulated error with motors from desired number of turns
double Integral1 = 0; //accumulated error with motors from desired number of turns
int n = LOW;
int m = LOW;

//for driver for IN1 and IN2 for motor one
int motor0pin1 = 2; // J3 on Board
int motor0pin2 = 3; //pwm (controls voltage signals) pin
int pwm0 = 80; //123
int digital0 = 1; //0?

//for driver for IN1 and IN2 for motor two
int motor1pin1 = 8; // J4 on Board
int motor1pin2 = 5; //pwm pin
int pwm1 = 80; //123
int digital1 = 1; //0?

int encoder0PrevCount = 0;
int lastSpeed0 = 0;
int encoder1PrevCount = 0;
int lastSpeed1 = 0;

double timeSec = 1.0;
double timeMSec = 1000;

double distance = 0;
double linearSpeed0;
double linearSpeed1;

double k = 0.0019; // Constant for converting angular velocity to linear velocity

//PID constants
//P (proportional) is how much to adjust when turn rate is not equal to set rate. Matters most.
double kP = 0.25;//0.20 or .15
//I (integral) is how much to adjust based on accumulated error
double kI = 0.2;//0.01 or .05
//D (derivative) how quickly it deviates from set rate. Adjusts quicker for greater rates
double kD = 0.211;//0.01 or .01

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup() {
  pinMode (encoder0PinA, INPUT);
  //  pinMode (encoder0PinB, INPUT);
  pinMode (motor0pin1, OUTPUT);
  pinMode (motor0pin2, OUTPUT);

  pinMode (encoder1PinA, INPUT);
  //pinMode (encoder1PinB, INPUT);
  pinMode (motor1pin1, OUTPUT);
  pinMode (motor1pin2, OUTPUT);

  Serial.begin (115200);
  
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1);
  }
  delay(1000);
}


void PID() {
   //for motor one, determines if moving forward (equals 1)
   //Equals 0 if moving backwards
   if (digital0 == 1)
    digitalWrite( motor0pin1, HIGH);
  else digitalWrite( motor0pin1, LOW);
  analogWrite( motor0pin2, pwm0); //Sets analog value of pin from 0 to 255 based on PID algorithm

  //for motor two, determines if moving forward (equals 1)
  //Equals 0 if moving backwards
  if (digital1 == 1)
    digitalWrite( motor1pin1, HIGH);
  else digitalWrite( motor1pin1, LOW);
  analogWrite( motor1pin2, pwm1);


  timeElapsed = 0; //variable internal to the board
  while ( timeElapsed < 500 ) {

    //reading encoder pin data and see whether voltqage data compares to previous read
    n = digitalRead(encoder0PinA);
    if ((encoder0PinALast == LOW) && (n == HIGH)) {
      encoder0Pos++;
    }

    //Set this read as a new encoder reading to compare to
    encoder0PinALast = n;

    //Same thing but for motor two
    m = digitalRead(encoder1PinA);
    if ((encoder1PinALast == LOW) && (m == HIGH)) {
      encoder1Pos++;
    }

    encoder1PinALast = m;

  }

  adjustPWM();
  Serial.println(" ");

}

  //Set PWM rate for motor based on PID algorithm
void adjustPWM() {
  //Current speed calculated
  int speedNow0 = calculateSpeed0();
  //Deviation from set point
  int error0 = setpoint - speedNow0;
  //Change in error
  double dError0 = ((double)speedNow0 - (double)lastSpeed0) / timeSec;
  //Accumulated error
  Integral0 += (double) error0;

  int speedNow1 = calculateSpeed1();
  int error1 = setpoint - speedNow1;
  double dError1 = ((double)speedNow1 - (double)lastSpeed1) / timeSec;
  Integral1 += (double) error1;

  //Cap error of the rate at 255 because it is the highest it can reach
  if (Integral0 > 255) Integral0 = 255;
  else if (Integral0 < 0) Integral0 = 0;

  if (Integral1 > 255) Integral1 = 255;
  else if (Integral1 < 0) Integral1 = 0;

  //Determine correction value based on PID constants and error
  int adjust0 = (kP * (double)error0) + kI * Integral0 + kD * dError0;
  int adjust1 = (kP * (double)error1) + kI * Integral1 + kD * dError1;

  //Adjusts according to above correction
  //Add to pwm if moving backwards and subtract if moving forwards
  if (digital0 == 0) pwm0 += adjust0;
  else pwm0 -= adjust0;

  if (digital1 == 0) pwm1 += adjust1;    
  else pwm1 -= adjust1;

  //Caps rate at max 255 and min 0
  if (pwm0 > 255) pwm0 = 255;
  else if (pwm0 < 0) pwm0 = 0;

  if (pwm1 > 255) pwm1 = 255;
  else if (pwm1 < 0) pwm1 = 0;

  //Store speeds as global variable for future comparison
  lastSpeed0 = speedNow0;
  lastSpeed1 = speedNow1;

  Serial.print("adjustment0: ");
  Serial.println( adjust0);
  Serial.print("PWM0: ");
  Serial.println( pwm0 );

  Serial.print("adjustment1: ");
  Serial.println( adjust1);
  Serial.print("PWM1: ");
  Serial.println( pwm1 );
}


int calculateSpeed0() {
  //Calculates difference in encoder positions over time
  int speedDetect = (encoder0Pos - encoder0PrevCount) / timeSec;
  Serial.print("Encoder0pos: ");
  Serial.print( encoder0Pos );
  Serial.print("  ");
  Serial.println( encoder0PrevCount);
  encoder0PrevCount = encoder0Pos;
  Serial.print( "Speed0: ");
  Serial.println( speedDetect);
  return speedDetect;
}

int calculateSpeed1() {
  int speedDetect = (encoder1Pos - encoder1PrevCount) / timeSec;
  Serial.print("Encoder0pos: ");
  Serial.print( encoder1Pos);
  Serial.print("  ");
  Serial.println( encoder1PrevCount);
  encoder1PrevCount = encoder1Pos;
  Serial.print( "Speed1: ");
  Serial.println( speedDetect);
  return speedDetect;
}

void calculateDistance(){
  linearSpeed0 = lastSpeed0*k;
  linearSpeed1 = lastSpeed1*k;

  // distance = average speed of two motors * time / 2
  distance += (linearSpeed0+linearSpeed1)*0.5/2;
  Serial.print("distance");
  Serial.println(distance);
}

void turn(double setAngle){
  double currentAngle = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();
  while (abs(currentAngle-setAngle) > 10){
    currentAngle = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();
    Serial.print("currentAnglea: ");
    Serial.println(currentAngle);
    if (fmod(currentAngle+360-setAngle,360) < 180){
      //turn left
       digitalWrite(motor0pin2, HIGH);
       analogWrite(motor0pin1, 120);
       analogWrite(motor1pin2, 120);
       digitalWrite(motor1pin1, HIGH);
       delay(100);
       digitalWrite(motor0pin2, LOW);
       digitalWrite(motor0pin1, LOW);
       digitalWrite(motor1pin2, LOW);
       digitalWrite(motor1pin1, LOW);
       delay(200);
    } else {
      //turn right
      analogWrite(motor0pin2, 120);
      digitalWrite(motor0pin1, HIGH);
      digitalWrite(motor1pin2, HIGH);
      analogWrite(motor1pin1, 120);
      delay(100);
      digitalWrite(motor0pin2, LOW);
      digitalWrite(motor0pin1, LOW);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor1pin1, LOW);
      delay(200);
    }
  }
  //stop
   digitalWrite(motor0pin2, LOW);
   digitalWrite(motor0pin1, LOW);
   digitalWrite(motor1pin2, LOW);
   digitalWrite(motor1pin1, LOW);
   delay(500);
}

void move(double setDistance, double angle){
  turn(angle);
  distance = 0;
  while (distance < setDistance) {
    PID();
    calculateDistance();
  }
  // stop
    digitalWrite( motor0pin1, LOW);
    digitalWrite( motor0pin2, LOW);
    digitalWrite( motor1pin1, LOW);
    digitalWrite( motor1pin2, LOW);
    delay(500);
  }

 void loop(){
  // draw CUP
  // C
  move(0.45, 270);
  move(0.50, 180);
  move(0.45, 90);
  
  move(0.6,28.7);

  // U
  move(0.50, 180);
  move(0.38, 90);
  move(0.50, 0);
  
  move(0.6,159.3);

  // P
  move(0.50, 0);
  move(0.38, 90);
  move(0.28, 180);
  move(0.38, 270);
  delay(10000);

}
