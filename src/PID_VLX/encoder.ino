TwoWire Wire2 (2, I2C_FAST_MODE);

int magnetStatus1 = 0; //value of the status register (MD, ML, MH)
int magnetStatus2 = 0;
int lowbyte1; //raw angle 7:0
float pi = 3.14285714;
word highbyte1; //raw angle 7:0 and 11:8
int rawAngle1; //final raw angle
float degAngle1; //raw angle in degrees (360/4096 * [value between 0-4095])

int lowbyte2; //raw angle 7:0
word highbyte2; //raw angle 7:0 and 11:8
int rawAngle2; //final raw angle
float degAngle2; //raw angle in degrees (360/4096 * [value between 0-4095])

int quadrantNumber1, previousquadrantNumber1; //quadrant IDs
float numberofTurns1 = 0; //number of turns
float correctedAngle1 = 0; //tared angle - based on the startup value
float startAngle1 = 0; //starting angle
float totalAngle1 = 0; //total absolute angular displacement
float previoustotalAngle1 = 0; //for the display printing

int quadrantNumber2, previousquadrantNumber2; //quadrant IDs
float numberofTurns2 = 0; //number of turns
float correctedAngle2 = 0; //tared angle - based on the startup value
float startAngle2 = 0; //starting angle
float totalAngle2 = 0; //total absolute angular displacement
float previoustotalAngle2 = 0; //for the display printing

AS5600 Sensor1(&Wire); // the Wire object
AS5600 Sensor2(&Wire2); // the Wire2 object

void ReadRawAngle1()
{
  if (!Sensor1.isMagnetDetected() || !Sensor1.isConnected())
    return;
  rawAngle1 = Sensor1.getRawAngle();
  degAngle1 = rawAngle1 * 0.087890625;
}
void ReadRawAngle2()
{
  if (!Sensor2.isMagnetDetected() || !Sensor2.isConnected())
    return;
  rawAngle2 = Sensor2.getRawAngle();
  degAngle2 = rawAngle2 * 0.087890625;
}
void correctAngle1()
{
  //recalculate angle
  correctedAngle1 = degAngle1 - startAngle1; //this tares the position

  if (correctedAngle1 < 0) //if the calculated angle is negative, we need to "normalize" it
  {
    correctedAngle1 = correctedAngle1 + 360; //correction for negative numbers (i.e. -15 becomes +345)
  }
  else
  {
    //do nothing
  }
  //Serial.print("Corrected angle: ");
  //Serial.println(correctedAngle1, 2); //print the corrected/tared angle
}
void correctAngle2()
{
  //recalculate angle
  correctedAngle2 = degAngle2 - startAngle2; //this tares the position

  if (correctedAngle2 < 0) //if the calculated angle is negative, we need to "normalize" it
  {
    correctedAngle2 = correctedAngle2 + 360; //correction for negative numbers (i.e. -15 becomes +345)
  }
  else
  {
    //do nothing
  }
  //Serial.print("Corrected angle: ");
  //Serial.println(correctedAngle2, 2); //print the corrected/tared angle
}
void read_total()
{
  /*
    //Quadrants:
    4  |  1
    ---|---
    3  |  2
  */

  //Quadrant 1
  if (correctedAngle1 >= 0 && correctedAngle1 <= 90)
  {
    quadrantNumber1 = 1;
  }

  //Quadrant 2
  if (correctedAngle1 > 90 && correctedAngle1 <= 180)
  {
    quadrantNumber1 = 2;
  }

  //Quadrant 3
  if (correctedAngle1 > 180 && correctedAngle1 <= 270)
  {
    quadrantNumber1 = 3;
  }

  //Quadrant 4
  if (correctedAngle1 > 270 && correctedAngle1 < 360)
  {
    quadrantNumber1 = 4;
  }
  //Serial.print("Quadrant: ");
  //Serial.println(quadrantNumber); //print our position "quadrant-wise"

  if (quadrantNumber1 != previousquadrantNumber1) //if we changed quadrant
  {
    if (quadrantNumber1 == 1 && previousquadrantNumber1 == 4)
    {
      numberofTurns1++; // 4 --> 1 transition: CW rotation
    }

    if (quadrantNumber1 == 4 && previousquadrantNumber1 == 1)
    {
      numberofTurns1--; // 1 --> 4 transition: CCW rotation
    }
    //this could be done between every quadrants so one can count every 1/4th of transition

    previousquadrantNumber1 = quadrantNumber1;  //update to the current quadrant

  }
  if (correctedAngle2 >= 0 && correctedAngle2 <= 90)
  {
    quadrantNumber2 = 1;
  }

  //Quadrant 2
  if (correctedAngle2 > 90 && correctedAngle2 <= 180)
  {
    quadrantNumber2 = 2;
  }

  //Quadrant 3
  if (correctedAngle2 > 180 && correctedAngle2 <= 270)
  {
    quadrantNumber2 = 3;
  }

  //Quadrant 4
  if (correctedAngle2 > 270 && correctedAngle2 < 360)
  {
    quadrantNumber2 = 4;
  }
  //Serial.print("Quadrant: ");
  //Serial.println(quadrantNumber); //print our position "quadrant-wise"

  if (quadrantNumber2 != previousquadrantNumber2) //if we changed quadrant
  {
    if (quadrantNumber2 == 1 && previousquadrantNumber2 == 4)
    {
      numberofTurns2++; // 4 --> 1 transition: CW rotation
    }

    if (quadrantNumber2 == 4 && previousquadrantNumber2 == 1)
    {
      numberofTurns2--; // 1 --> 4 transition: CCW rotation
    }
    //this could be done between every quadrants so one can count every 1/4th of transition

    previousquadrantNumber2 = quadrantNumber2;  //update to the current quadrant

  }
  totalAngle1 = (numberofTurns1 * 360) + correctedAngle1; //number of turns (+/-) plus the actual angle within the 0-360 range
  //Serial.print("Total angle1: ");
  //Serial.println(totalAngle1, 2);

  totalAngle2 = (numberofTurns2 * 360) + correctedAngle2; //number of turns (+/-) plus the actual angle within the 0-360 range
  // Serial.print("Total angle2: ");
  // Serial.println(totalAngle2, 2);
  //error =(totalAngle1*(-1))-totalAngle2;
  // Serial.println("Error inside main fun");
  //Serial.println(error);
}

void checkMagnetPresence1()
{
  Sensor1.begin();
  Sensor1.setClock();

  while (!Sensor1.isConnected())
    //delay(1);
    Serial.println("1 connecting...");
  while (!Sensor1.isMagnetDetected())
    //delay(1);
    Serial.println("1 detecting...");

  ReadRawAngle1(); //make a reading so the degAngle gets updated
  startAngle1 = degAngle1; //update startAngle with degAngle - for taring
}
void checkMagnetPresence2()
{
  Sensor2.begin();
  Sensor2.setClock();

  while (!Sensor2.isConnected())
    //delay(1);
    Serial.println("2 connecting...");
  while (!Sensor2.isMagnetDetected())
    //delay(1);
    Serial.println("2 detecting...");

  ReadRawAngle2();
  startAngle2 = degAngle2;
}
float calculate_distanceR()
{
  float distanceR = (totalAngle2 * 29 * pi) / 360;
  return distanceR;
}
float calculate_distanceL()
{
  float distanceL = (totalAngle1 * 29 * pi) / 360;
  return ((distanceL) * -1);
}
void updateEncoder()
{
  ReadRawAngle1();
  ReadRawAngle2();
  correctAngle1();
  correctAngle2();
  read_total();
}
