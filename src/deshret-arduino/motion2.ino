void _moveForward(int distance)
{
  updateEncoder();

  float distStart = ((calculate_distanceL() - offsetL) + (calculate_distanceR() - offsetR)) / 2;

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, SRIGHT); //Right wheels forward
  analogWrite(PA2, SLEFT); //Left wheels forward
  analogWrite(PA3, 0); //Left wheels backward

  do
  {
    updateEncoder();
  
    distL = calculate_distanceL() - offsetL;
    distR = calculate_distanceR() - offsetR;
  }
  while (distStart + CELL * distance > (distL + distR) / 2 + BRAKEF);

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, 0); //Right wheels forward
  analogWrite(PA2, 255); //Left wheels forward
  analogWrite(PA3, 255); //Left wheels backward

  delay(10);

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, 0); //Right wheels forward
  analogWrite(PA2, 0); //Left wheels forward
  analogWrite(PA3, 0); //Left wheels backward

  delay(500);

  bluetooth_e();
}

void _turnRight()
{
  updateEncoder();

  float distStartL = calculate_distanceL() - offsetL;
  float distStartR = calculate_distanceR() - offsetR;

  analogWrite(PA0, SRIGHT); //Right wheels backward
  analogWrite(PA1, 0); //Right wheels forward
  analogWrite(PA2, SLEFT); //Left wheels forward
  analogWrite(PA3, 0); //Left wheels backward

  do
  {
    updateEncoder();
  
    distL = calculate_distanceL() - offsetL;
    distR = calculate_distanceR() - offsetR;
  }
  while ((distL - distStartL) + (distStartR - distR) < TURN * 2 - BRAKET);

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, 0); //Right wheels forward
  analogWrite(PA2, 255); //Left wheels forward
  analogWrite(PA3, 255); //Left wheels backward

  delay(10);

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, 0); //Right wheels forward
  analogWrite(PA2, 0); //Left wheels forward
  analogWrite(PA3, 0); //Left wheels backward

  delay(500);

  bluetooth_e();
}

void _turnLeft()
{
  updateEncoder();

  float distStartL = calculate_distanceL() - offsetL;
  float distStartR = calculate_distanceR() - offsetR;

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, SRIGHT); //Right wheels forward
  analogWrite(PA2, 0); //Left wheels forward
  analogWrite(PA3, SLEFT); //Left wheels backward

  do
  {
    updateEncoder();
  
    distL = calculate_distanceL() - offsetL;
    distR = calculate_distanceR() - offsetR;
  }
  while ((distStartL - distL) + (distR - distStartR) < TURN * 2 - BRAKET);

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, 0); //Right wheels forward
  analogWrite(PA2, 255); //Left wheels forward
  analogWrite(PA3, 255); //Left wheels backward

  delay(10);

  analogWrite(PA0, 0); //Right wheels backward
  analogWrite(PA1, 0); //Right wheels forward
  analogWrite(PA2, 0); //Left wheels forward
  analogWrite(PA3, 0); //Left wheels backward
  
  delay(500);

  bluetooth_e();
}
