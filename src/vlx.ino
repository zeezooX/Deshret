// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

// set the pins to shutdown
#define SHT_LOX1 PB9
#define SHT_LOX2 PB15
#define SHT_LOX3 PB8

/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
*/

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;

void setID() {
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

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if (!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while (1);
  }
}

void read_triple_sensors() {

  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print(F("Left: "));
  if (measure1.RangeStatus != 4) {    // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print(F("8191"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("Right: "));
  if (measure2.RangeStatus != 4) {    // if not out of range
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print(F("8191"));
  }

  Serial.print(F(" "));

  // print sensor three reading
  Serial.print(F("Front: "));
  if (measure3.RangeStatus != 4) {    // if not out of range
    Serial.print(measure3.RangeMilliMeter);
  } else {
    Serial.print(F("8191"));
  }

  Serial.println();
}

void vlx_setup() {
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  setID();
}

bool _wallLeft()
{
  int readings = 0;
  for(int i = 1; i--;)
  {
    lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
    if (measure1.RangeStatus != 4)
      readings += measure1.RangeMilliMeter < CELL;
  }
  return readings > 0;
}

bool _wallRight()
{
  int readings = 0;
  for(int i = 1; i--;)
  {
    lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
    if (measure2.RangeStatus != 4)
      readings += measure2.RangeMilliMeter < CELL;
  }
  return readings > 0;
}

bool _wallFront()
{
  int readings = 0;
  for(int i = 1; i--;)
  {
    lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
    if (measure3.RangeStatus != 4)
      readings += measure3.RangeMilliMeter < CELL;
  }
  return readings > 0;
}
