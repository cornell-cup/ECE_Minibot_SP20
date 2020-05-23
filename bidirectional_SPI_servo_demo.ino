#include <SPI.h>
#include <Servo.h>

//Bi-directinal SPI 
//13 SCK  (serial clock)
//12 MISO (master in slave out)
//11 MOSI (master out slave in)
//10 SS (slave select)

Servo myservo;
//initialize the buffer
int bufSize = 4;
char buf [4];
volatile byte pos = 0;
//volatile boolean process_it;
//different sensors pins initialization
int trigPin = 3; //setting the pins for ultrasonic here for testing purpose
int echoPin = A1;
int infra;
int line;
int encoderPin;
int motorpin1;
int motorpin2;
long duration,cm;
int n = 0;
bool valid = false;
bool process_it = false;
String x = "";
int LED = 5;
byte newbyte = 0;

//int interruptPin = 10; //might not be necessary

void setup() {
  Serial.begin(115200);
  pinMode (MISO, OUTPUT);
  SPCR |= bit (SPE); //turn on SPI in slave mode
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  pos = 0;
  //process_it = false;
  myservo.attach(9);
  //turn on the interrupt
  SPI.attachInterrupt();

}

//SPI ISR (Interrupt Service Routine)

ISR (SPI_STC_vect){
  
  byte c = SPDR; //get byte from the SPI data register
  //detect the beginning of the buffer, do not put it in the buffer
  if (c == '\n'){ 
    valid = true;
  }
  //detect the end character
  else if (c == '\r'){
    valid = false;
//    buf[0] = 0;
//    buf[1] = 0;
      pos = 0;
//  process_it = true;
  }
  //put data into the buffer
  if ((valid == true) && (c != '\n') && (c != '\r')){
    if (pos < bufSize ){  ///sizeof buffer
    buf [pos] = c;
    pos ++;
  }
  }
}


/*for servo demo 
 * make the servo to turn the angle specified by pi
*/
void servo_demo(){
  //send 'ss' with the start and end character
  //as indication of setting servo angles
    if (buf[0] == 's' && buf[1] == 's' ){
      int a = buf[2];
      Serial.println(a);
      myservo.write(a);
     
  }
}
  
/*Getting distance data from the ultrasonic sensors  */
void get_distance(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  pinMode(echoPin,INPUT);
  duration = pulseIn(echoPin,HIGH);
  cm = (duration/2)/29.1;

}

/*for testing bidirectional communication
* in the blockly code. Taking in distance 
* data and make the bot do something in blockly
*/
void bi_directional(){

  get_distance();
  
  if (buf[0] == 'd' && buf[1] == 'u'){
      SPDR = cm;
      Serial.println(cm);
  }
}
  


/*custom port switching functi
on */
void set_ports(){
  
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
      else if ((buf[1] == 'R') && (buf[2] == 'M')){
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
        Serial.println("J3 has set up for ultrasonic");
        Serial.println("Distance Data can be read now");
    }
    //infrared sensor
    else if (buf[1] == 'I'){
        infra = 7;
        pinMode(infra, INPUT);
      
  }

    //Line Sensors
    else if (buf[1] == 'L'){
        line = A1;
        pinMode(line, INPUT);
      
    }
  }
  if (buf[0] == '4'){
    //left motor
    if ((buf[1] == 'L') && (buf[2] == 'M') ){
          encoderPin = A2;
          motorpin1 = 8;
          motorpin2 = 5; //pwm pins
          pinMode(motorpin1, OUTPUT);
          pinMode(motorpin2, OUTPUT);
          pinMode(encoderPin, INPUT);       
    }
    //right motor
    else if ((buf[1] == 'R') && (buf[2] == 'M')){
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

    bi_directional();
    servo_demo();

//clear the buffer when a command is executed
  
  if (process_it){
    pos = 0;
    process_it = false;
  }
}
