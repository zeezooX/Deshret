void bluetooth()
{
  lastSample = millis();

  Serial1.print("$distL$" + String(distL) + "#");
  Serial1.print("$distR$" + String(distR) + "#");
  Serial1.print("$offsetL$" + String(offsetL) + "#");
  Serial1.print("$offsetR$" + String(offsetR) + "#");
  Serial1.print("$lastSample$" + String(lastSample) + "#");
  Serial1.print("$wallLeft$" + String(_wallLeft()) + "#");
  Serial1.print("$wallRight$" + String(_wallRight()) + "#");
  Serial1.print("$wallFront$" + String(_wallFront()) + "#");
  Serial1.println("$batteryLevel$" + String(batteryLevel));
}

void bluetooth_e()
{
  Serial1.print("$E$");
  Serial1.print("Distance 1: ");
  Serial1.print(distL);
  Serial1.print(", Distance 2: ");
  Serial1.print(distR);
  Serial1.print(", Left Wall: ");
  Serial1.print(_wallLeft());
  Serial1.print(", Right Wall: ");
  Serial1.print(_wallRight());
  Serial1.print(", Front Wall: ");
  Serial1.print(_wallFront());
}

void serial()
{
  Serial.print("Distance 1: ");
  Serial.print(distL);
  Serial.print(", Distance 2: ");
  Serial.print(distR);
  Serial.print(", Offset 1: ");
  Serial.print(offsetL);
  Serial.print(", Offset 2: ");
  Serial.println(offsetR);
}
