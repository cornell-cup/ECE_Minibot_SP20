/* Modified from Adafruit BNO055 exampls
 * Testing Heading and position
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

double xPos = 0, yPos = 0, headingVel = 0;
uint16_t DELAY_MS = 5; //how often to read data from the board
uint16_t PRINT_DELAY_MS = 500; // how often to print the data
uint16_t printCount = 0; //counter to avoid printing every 10MS sample
double a_x1, a_x2, a_x3, a_y1, a_y2, a_y3, v_x1, v_x2, v_y1, v_y2, s_x, s_y;
uint16_t count = 0;


// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup(void)
{
  Serial.begin(115200);
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1);
  }


  delay(1000);
}

void loop(void)
{
  sensors_event_t orientationData , linearAccelData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

  // use the trepazoid method twice to calculate position
  a_x1 = linearAccelData.acceleration.x;
  a_y1 = linearAccelData.acceleration.y;

  delay(DELAY_MS);

  a_x2 = linearAccelData.acceleration.x;
  a_y2 = linearAccelData.acceleration.y;

  delay(DELAY_MS);

  a_x3 = linearAccelData.acceleration.x;
  a_y3 = linearAccelData.acceleration.y;

  v_x1 += DELAY_MS/2*(a_x1+a_x2)/1000;
  v_x2 += DELAY_MS/2*(a_x2+a_x3)/1000;
  v_y1 += DELAY_MS/2*(a_y1+a_y2)/1000;
  v_y2 += DELAY_MS/2*(a_y2+a_y3)/1000;

  s_x += DELAY_MS/2*(v_x1+v_x2)/1000;
  s_y += DELAY_MS/2*(v_y1+v_y2)/1000;

  
  if (count >= 100){
    //display the data every 100 measurements
    //the units are in mm.
    Serial.print("Heading: ");
    Serial.println(orientationData.orientation.x);
    Serial.print("Position: ");
    Serial.print(1000*s_x); // Convert the unit to mm when printing
    Serial.print(" , ");
    Serial.println(1000*s_y);
    Serial.print("xAcceleration: ");
    Serial.println(linearAccelData.acceleration.x*1000);
    Serial.print("yAcceleration: ");
    Serial.println(linearAccelData.acceleration.y*1000);
    Serial.println("");
    count = 0;
  }
  count ++;
}
