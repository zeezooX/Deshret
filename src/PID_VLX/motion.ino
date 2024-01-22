void forward()
{
  read_three_sensors(&F_sensor, &L_sensor, &R_sensor);
  vlxInput = L_sensor - R_sensor;
  vlxOutput = 0;
  if (L_sensor + R_sensor < WIDTH * 1.5)
  {
    vlxSetpoint = 0.0;
    vlxPID.Compute();
  }
  
  else if (L_sensor + R_sensor < WIDTH * 2.5)
  {
    if(L_sensor > R_sensor)
      vlxSetpoint = (L_sensor + R_sensor) / 2.0;
    else
      vlxSetpoint = -(L_sensor + R_sensor) / 2.0;
    vlxPID.Compute();
  }
  else if (L_sensor + R_sensor < WIDTH * 3.5)
  {
    if(L_sensor > WIDTH && R_sensor > WIDTH)
      vlxSetpoint = 0.0;
    else if(L_sensor > R_sensor)
      vlxSetpoint = (L_sensor + R_sensor) * 2.0/3.0;
    else
      vlxSetpoint = -(L_sensor + R_sensor) * 2.0/3.0;
    vlxPID.Compute();
  }

  int speedR = constrain(BASESPEED + vlxOutput, BASESPEED, 255);
  int speedL = constrain(BASESPEED - vlxOutput, BASESPEED, 255);

  analogWrite(IN1, speedR);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, speedL);
}

void brake()
{
  analogWrite(IN1, 255);
  analogWrite(IN2, 255);
  analogWrite(IN3, 255);
  analogWrite(IN4, 255);

  delay(25);

  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);

  delay(500);
}

void turn(bool isLeft)
{
  brake();
  
  updateEncoder();

  double distStartL = calculate_distanceL();
  double distStartR = calculate_distanceR();

  analogWrite(IN1, BASESPEED * 2 * !isLeft);
  analogWrite(IN2, BASESPEED * 2 * isLeft);
  analogWrite(IN3, BASESPEED * 2 * !isLeft);
  analogWrite(IN4, BASESPEED * 2 * isLeft);

  do
  {
    updateEncoder();
  }
  while ((isLeft ? 1 : -1) * ((distStartL - calculate_distanceL()) + (calculate_distanceR() - distStartR)) < TURN);

  analogWrite(IN1, BASESPEED * isLeft);
  analogWrite(IN2, BASESPEED * !isLeft);
  analogWrite(IN3, BASESPEED * isLeft);
  analogWrite(IN4, BASESPEED * !isLeft);

  delay(25);

  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);

  delay(500);
}

void moveForward(int distance)
{
  updateEncoder();

  double distStart = (calculate_distanceL() + calculate_distanceR()) / 2;

  do
  {
    updateEncoder();

    forward();
  }
  while (distStart + WIDTH * distance > (calculate_distanceL() + calculate_distanceR()) / 2 && F_sensor > WIDTH * 0.5);
}

void moveHalf()
{
  updateEncoder();

  double distStart = (calculate_distanceL() + calculate_distanceR()) / 2;

  do
  {
    updateEncoder();

    forward();
  }
  while (distStart + WIDTH * 0.5 > (calculate_distanceL() + calculate_distanceR()) / 2);
}
