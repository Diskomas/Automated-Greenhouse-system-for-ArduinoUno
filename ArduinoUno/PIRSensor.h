
int inputPin = 7;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

// nightmode detected by the solar panel / allows for smaller thresholds
bool nightMode;

void PIRSetup(){
  pinMode(inputPin, INPUT);
}

 
bool checkForIntruder(){

    val = digitalRead(inputPin);  // read input value

  if (val == HIGH) {            // check if the input is HIGH
    return true;
    if (pirState == LOW) {
      // we have just turned on
      //Serial.println("Motion detected!");
      return true;
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } 
  else {
    return false;
    if (pirState == HIGH){
      // we have just turned of
     // Serial.println("Motion ended!");
      return false;
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}

void securityAlert(){
    if(nightMode){
      
    // read PIRSensor data 
    bool personDetected = checkForIntruder();

    if(personDetected){
      // play alert sound
      tone(13, 262);
      delay(500);
      noTone(13);
      delay(100);
    }
  }
}
