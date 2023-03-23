void _loop()
{
  while(1)
  {
    if(_wallLeft())
    {
      _turnLeft();
    }
    while (_wallFront())
    {
      _turnRight();
    }
    _moveForward(1);
  }
  
  if (Serial1.available())
  {
    String str = Serial1.readStringUntil('\n');
    if (str == "turnRight")
    {
      _turnRight();
    }
    else if (str == "turnLeft")
    {
      _turnLeft();
    }
    else if (str.startsWith("moveForward "))
    {
      str.remove(0, 12);
      _moveForward(str.toInt());
    }
    else if (str[0] == '$')
    {
      String v = "";
      String u = "";
      int i;
      for (i = 1; str[i] != '$' && i < str.length(); i++)
        v += str[i];
      for (i = i + 1; i < str.length(); i++)
        u += str[i];

    }
  }
  updateEncoder();
  distL = calculate_distanceL() - offsetL;
  distR = calculate_distanceR() - offsetR;
  if (millis() >= lastSample + 1000)
  {
    bluetooth();
  }
}
