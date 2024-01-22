#include "MPU6050_light.h"
#include "Wire.h"
#include "Adafruit_VL53L0X.h"
#include "PID_v1.h"
#include <AMS_AS5600.h>

#define IN1 PA0
#define IN2 PA1
#define IN3 PA2
#define IN4 PA3

#define BASESPEED 50
#define WIDTH 175
#define TURN 135

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
#define SHT_LOX1 PB8
#define SHT_LOX2 PB4
#define SHT_LOX3 PA15

double F_sensor, R_sensor, L_sensor;

double vlxKp = 36.2714;
double vlxKi = 1.1757;
double vlxKd = 279.7516;

double vlxSetpoint = 0.0;
double vlxInput = 0.0;
double vlxOutput = 0.0;

PID vlxPID(&vlxInput, &vlxOutput, &vlxSetpoint, vlxKp, vlxKi, vlxKd, DIRECT);

//double mpuKp = 0.0;
//double mpuKi = 0.0;
//double mpuKd = 0.0;
//
//double mpuSetpoint = 0.0;
//double mpuInput = 0.0;
//double mpuOutput = 0.0;
//
//PID mpuPID(&mpuInput, &mpuOutput, &mpuSetpoint, mpuKp, mpuKi, mpuKd, DIRECT);
//
//MPU6050 mpu(Wire);

int mode = 3;
bool isLeft;

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PB12, OUTPUT);
  pinMode(PC14, INPUT_PULLUP);
  pinMode(PC15, INPUT_PULLUP);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  Serial.begin(115200);
  Wire.begin();

//  while (!Serial)
//  {
//    delay(1);
//  }

  setID();
  checkMagnetPresence1(); //check the magnet (blocks until magnet is found)
  checkMagnetPresence2();

//  byte status = mpu.begin();
//  Serial.print(F("MPU6050 status: "));
//  Serial.println(status);
//  while(status!=0){ } // stop everything if could not connect to MPU6050
//  
//  mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
//  Serial.println("Done!\n");

  vlxPID.SetMode(AUTOMATIC);
  vlxPID.SetSampleTime(3);
  vlxPID.SetOutputLimits(-255, 255);

//  mpuPID.SetMode(AUTOMATIC);
//  mpuPID.SetOutputLimits(-255, 255);

  randomSeed(1);
  bool state = HIGH;
  digitalWrite(PB9, HIGH);
  digitalWrite(PB12, HIGH);
  while (digitalRead(PC14))
  {
    updateEncoder();
    if(!digitalRead(PC15) && state)
    {
      mode = (mode % 3) + 1;
      digitalWrite(PB12, mode & 1);
      digitalWrite(PB9, mode & 2);
    }
    state = digitalRead(PC15);
  }
  digitalWrite(PB9, LOW);
  digitalWrite(PB12, LOW);
  delay(1000);
  if(mode == 1)
    isLeft = false;
  else if(mode == 2)
    isLeft = true;
  else
    isLeft = random(2);
}

void loop()
{ 
  read_three_sensors(&F_sensor, &L_sensor, &R_sensor);
  if(!wall(isLeft))
  {
    moveHalf();
    turn(isLeft);
    if(mode == 3)
      isLeft = random(2);
    moveHalf();
  }
  else if(F_sensor > WIDTH * 0.5)
  {
    forward();
  }
  else
  {
    turn(!isLeft);
    if(mode == 3)
      isLeft = random(2);
  }
}
