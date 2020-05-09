#include <SPI.h>
#include <Servo.h>

//Bi-directinal SPI 
//13 SCK  (serial clock)
//12 MISO (master in slave out)
//11 MOSI (master out slave in)
//10 SS (slave select)
//send string of Hello to Raspberry Pi

//initializing variables
//J2: S6, S1, S4, ADC0
Servo myservo;
int bufSize = 4;
char buf [4];
volatile byte pos = 0;
volatile boolean process_it;
//ultrasonic setup
int trigPin = 3; //digital pin
int echoPin = A1; //analog pin
int infra;
int line;
int encoderPin;
int motorpin1;
int motorpin2;
long duration,cm;
int n = 0;
bool valid = false;
String x = "";
int LED = 5;

//int interruptPin = 10; //might not be necessary

void setup() {
  Serial.begin(115200);
  pinMode (MISO, OUTPUT); //output because sensor output
  SPCR |= bit (SPE); //turn on SPI in slave mode
  //getting ready for an interrupt
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  pos = 0;
  process_it = false;
  //turn on the interrupt
  SPI.attachInterrupt();

}

//SPI ISR (Interrupt Service Routine)

ISR (SPI_STC_vect){ //pi/arduino communication, this is they key portion of bidirectional spi
  
  Serial.println("Entered ISR");
  if (SPDR != c)
    Serial.println("Value has been changed");
  byte c = SPDR; //get byte from the SPI data register
  Serial.print("ISR Value: ");
  Serial.println(c);
  if (c == '\n'){ //the beginning of the data
    valid = true; //data can start being read, if false, data is useless
  }
  else if (c == '\r'){
    valid = false; //end of character, indicates end of data so stop reading
    process_it = true; //done with this process
  }
  if ((valid == true) && (c != '\n')){
    if (pos < bufSize ){  ///sizeof buf
    buf [pos] = c;
    pos ++;
  }
  }
  //end of room available
}
void LED_test(){
  //will turn on the LED when receive "TL"
  if (buf[0] == 'T' && buf [1] == 'L'){
    digitalWrite(LED,HIGH);
  }
  
}
void get_distance(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  pinMode(echoPin,INPUT);
  duration = pulseIn(echoPin,HIGH);
  cm = (duration/2)/29.1;
  Serial.println(cm);
  //delay(300);
}
// RJ12 = 82 74 49 50
void bi_directional(){
//      get_distance();
//      SPDR = cm;
  if (buf[0] == 'd'){
    if (buf[1] == 'u'){ //array du is a call that information needs to be collected
      get_distance();
      SPDR = cm; //setting data into the register to be processed
    }
  }
  if (buf[0] == 's'){
    SPDR = 0; //stop reading data
  }
}

void set_ports(){ //collects data from different sensors depending on the buffer
    if (buf[0] == '2'){
    //right motor
      if ((buf[1] == 'L') && (buf[2] == 'M') ){
          encoderPin = A0;
          motorpin1 = 4;
          motorpin2 = 6;
          pinMode(motorpin1, OUTPUT);
          pinMode(motorpin2, OUTPUT);
          pinMode(encoderPin, INPUT);        
    }
    //right motor
      else if ((buf[1] == 'L') && (buf[2] == 'M')){
          encoderPin = A0;
          motorpin1 = 4;
          motorpin2 = 6;
          pinMode(motorpin1, OUTPUT);
          pinMode(motorpin2, OUTPUT);
          pinMode(encoderPin, INPUT);      
    }
    //ultrasonic
      else if (buf[1] == 'U'){
        echoPin = A0;
        trigPin = 4;
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }
    //infrared sensor
      else if (buf[1] == 'I'){
        infra = 4;
        pinMode(infra, INPUT);
      
  }
      else if (buf[1] == 'R'){
      
    }
    //Line Sensors
      else if (buf[1] == 'L'){
        line = A0;
        pinMode(line, INPUT);
      
    }
  }
  
 
else if (buf[0] == '3'){
    //left motor
    if ((buf[1] == 'L') && (buf[2] == 'M') ){
      encoderPin = A1;
      motorpin1 = 2;
      motorpin2 = 3;
      pinMode(motorpin1, OUTPUT);
      pinMode(motorpin2, OUTPUT);
      pinMode(encoderPin, INPUT); 
    }
    //right motor
    else if ((buf[1] == 'R') && (buf[2] == 'M')){
      encoderPin = A1;
      motorpin1 = 2;
      motorpin2 = 3;
      pinMode(motorpin1, OUTPUT);
      pinMode(motorpin2, OUTPUT);
      pinMode(encoderPin, INPUT); 
    }
    //ultrasonic
    else if (buf[1] == 'U'){
        echoPin = A1;
        trigPin = 3;
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }
    //infrared sensor
    else if (buf[1] == 'I'){
        infra = 7;
        pinMode(infra, INPUT);
      
  }
    else if (buf[1] == 'R'){
      
    }
    //Line Sensors
    else if (buf[1] == 'L'){
        line = A1;
        pinMode(line, INPUT);
      
    }
  }
  if (buf[0] == '4'){
    //right motor
    if ((buf[1] == 'L') && (buf[2] == 'M') ){
          encoderPin = A2;
          motorpin1 = 8;
          motorpin2 = 5; //pwm pins
          pinMode(motorpin1, OUTPUT);
          pinMode(motorpin2, OUTPUT);
          pinMode(encoderPin, INPUT);       
    }
    //right motor
    else if ((buf[1] == 'L') && (buf[2] == 'M')){
          encoderPin = A2;
          motorpin1 = 8;
          motorpin2 = 5; //pwm pins
          pinMode(motorpin1, OUTPUT);
          pinMode(motorpin2, OUTPUT);
          pinMode(encoderPin, INPUT);       
    }
    //ultrasonic
    else if (buf[1] == 'U'){
        echoPin = A2;
        trigPin = 5;
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }
    //infrared sensor
    else if (buf[1] == 'I'){
        infra = 8;
        pinMode(infra, INPUT);    
  }
    else if (buf[1] == 'R'){

      
    }
    //Line Sensors
    else if (buf[1] == 'L'){
        line = A2;
        pinMode(line, INPUT);
      
    }
  }
  if (buf[0] == '8'){
    //ultrasonic
    if (buf[1] == 'U'){
        echoPin = A3;
        trigPin = 9;
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }
    //infrared sensor
    else if (buf[1] == 'I'){
        infra = 9;
        pinMode(infra, INPUT);
      
  }
    else if (buf[1] == 'R'){
      
    }
    //Line Sensors
    else if (buf[1] == 'L'){
        line = A3;
        pinMode(infra, INPUT);
      
    }
  }
  else if (buf[0] == '9'){
    if (buf[1] == 'L'){
        line = A5;
        pinMode(line, INPUT);
  }
 }
}
void loop() {
  //n++;
  //for (int i=0; i<bufSize;i++){
//  Serial.println(buf);
    
//    for (int a = 0; a < 3; a ++){
//      x = x + buf[a];
//    }
//    if (x == "u12" ){
//      Serial.println("true");
//      echoPin = A3;
//      trigPin = 9;
//      pinMode(trigPin, OUTPUT);
//      pinMode(echoPin, INPUT);
//    }
//    x = "";
//  if (buf[0] == 'u'){
//    Serial.println("true");
//  }
  
    //set_ports();
    bi_directional();
    //get_distance();
//  int i, c;
  //delay(5);
  if (process_it){
    //buf[pos] = 0
//    for (int i = 0; i<bufSize; i++){
//      buf[i] = 0;
//    }
    pos = 0; //resets everything, buffer becomes zero 
    process_it = false;//resets to before data is processed
  }
}
