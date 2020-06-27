void setup()
{
  pinMode(22, OUTPUT);
}

void loop()
{
  digitalWrite(22, HIGH);
  delay(1000);
  digitalWrite(22, LOW);
  delay(1000);
}
