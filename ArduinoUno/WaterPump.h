
int pumpingTimer = 10; // seconds

const int pumpPin = 12; // power trigger pin

void startPumping(){

  Serial.println("Activating water pump!");

  digitalWrite(pumpPin, HIGH); // <- activate pump

  delay(pumpingTimer*1000UL); // <- wait for water to pump

  digitalWrite(pumpPin, LOW); // <- de-activate pump
}