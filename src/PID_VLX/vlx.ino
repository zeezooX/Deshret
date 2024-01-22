Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

void setID()
{
  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));

  Serial.println(F("Starting..."));

  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS))
  {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS))
  {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1)
      ;
  }

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if (!lox3.begin(LOX3_ADDRESS))
  {
    Serial.println(F("Failed to boot third VL53L0X"));
    while (1);
  }

  lox1.startRangeContinuous();
  lox2.startRangeContinuous();
  lox3.startRangeContinuous();
}

void read_three_sensors(double *reading1, double *reading2, double *reading3)
{
  *reading1 = lox1.readRangeResult() - 00;
  *reading2 = lox2.readRangeResult() - 00;
  *reading3 = lox3.readRangeResult() - 24;
  if(lox1.readRangeStatus() == 4)
    *reading1 = 8191;
  if(lox2.readRangeStatus() == 4)
    *reading2 = 8191;
  if(lox3.readRangeStatus() == 4)
    *reading3 = 8191;
}

bool wallLeft()
{
  return (L_sensor < WIDTH * 1.0);
}

bool wallRight()
{
  return (R_sensor < WIDTH * 1.0);
}

bool wallFront()
{
  return (F_sensor < WIDTH * 1.0);
}

bool wall(bool isLeft)
{
  if(isLeft)
    return (L_sensor < WIDTH * 1.0);
  else
    return (R_sensor < WIDTH * 1.0);
}
