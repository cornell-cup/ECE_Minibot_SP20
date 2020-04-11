#include <SPI.h>

//Bi-directinal SPI 
//13 SCK  (serial clock)
//12 MISO (master in slave out)
//11 MOSI (master out slave in)
//10 SS (slave select)
//send string of Hello to Raspberry Pi
int bufSize = 4;
char buf [4];
volatile byte pos;
volatile boolean process_it;
//ultrasonic setup
int trigPin = 9;
int echoPin = A3;
long duration,cm;
int n = 0;

//int interruptPin = 10; //might not be necessary

void setup() {
  Serial.begin(115200);
  pinMode (MISO, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  SPCR |= bit (SPE); //turn on SPI in slave mode
  //getting ready for an interrupt
  pos = 0;
  process_it = false;
  //turn on the interrupt
  SPI.attachInterrupt();
}

//SPI ISR (Interrupt Service Routine)

ISR (SPI_STC_vect){
  byte c = SPDR; //get byte from the SPI data register
  Serial.println(c);
  //get_distance();
  //SPDR = n;
  if (pos < sizeof buf ){
    buf [pos ++] = c;
  }
  //end of room available
  if (c == '\n'){
    process_it = true;
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
void loop() {
  //get_distance();
  //n++;
  //for (int i=0; i<bufSize;i++){
    Serial.println(buf[0]);
  //}
  
//  int i, c;
  //delay(5);
  if (process_it){
    buf[pos] = 0;
    Serial.println(buf);
    pos = 0;
    process_it = false;
  }
}
