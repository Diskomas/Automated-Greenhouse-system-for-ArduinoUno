

// Arduino to NodeMCU libraries
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial nodemcu(5, 6); // <- indicate RX TX pins for sending data to NodeMCU

// Include sensors headers
#include "TemperatureSensor.h" // <- read from two sensors and provies an average
#include "SoilSensor.h" // <- powers on sensors briefly and reads results
#include "PHSensor.h" // <- returns PH level taking 10 samples and providing an average
#include "WaterPump.h" // <- activates water pump system
#include "PIRSensor.h" // <- looks for movement and if nightMode turns on the alarm

unsigned long sendLastMillis, innerLastMillis; // <- timer counter for sending and reading sensors

// intervals for checking sensors:
int sendDataTimeout = 1; // <- send data to MCU (minutes)
int innerDataTimeout = 1; // <- check data in loop (minutes)

// selecting mode
int Mode = 1; // Cucumbers = 1, Tomatoes = 2

// prep thresholds for mode
float tempLow, tempMax, humidLow, humidMax, PHLow, PHMax;

void setup()
{
  pinMode(4,OUTPUT); // <- set up LED indicator (if sofware started - will turn the light to green)
  pinMode(A4,OUTPUT); // <- 5v cooling fan

  Serial.begin(9600);
  nodemcu.begin(9600);
  
  // sensot setups
  temperatureSetup();
  soilSensorSetup();
  PIRSetup();

  // get current time
  sendLastMillis = millis();
  innerLastMillis = millis();

  nightMode = false; // <- will update in next itteration

  // prepare thresholds
  prepThreshold();

  pinMode(pumpPin,OUTPUT); // water pump activation pin

  // RGB pin LEDs
  pinMode(9,OUTPUT); // green
  pinMode(10,OUTPUT); // blue
  pinMode(11,OUTPUT); // red

  digitalWrite(4, HIGH); // set up complete, turn the green LED on

}

void loop(){

  // Security system integration
  securityAlert(); // <- activates in nightmode

  // Check sensor data
  sensorCheck(); // <- read sensor data

  // NodeMCU send data to ThingSpeak server API
  checkNodeMCUTimer(); // <- send sensor data to NodeMCU

}

void sensorCheck(){
    if (millis() - innerLastMillis >= innerDataTimeout*60*1000UL){ //innerDataTimeout*60*1000UL

    innerLastMillis = millis();  //get ready for the next iteration

    // check current light brightnes and set mode
    float light = analogRead(A1); // 0 to 500 (0V to 5V)

    Serial.println("Light: " + String(light)); // <- print current brigness

    if(light < 50){nightMode = true; digitalWrite(A4, LOW);}  // <- if light received is lower than 50 is very likely that it's night
    else {nightMode = false; digitalWrite(A4, HIGH);} // <- else is day 

    // get current temperature
    float temperature = getTemperature();
    // get current humidity
    int humidity  = readSoil();
    // get PH Level
    float PH = getPH();

    
    Serial.println("Temperature: " + String(temperature));

    // turn LED on colour based on the temperature compared to the set max and min temperatures
    if(temperature <= tempLow){
      clearRGB();
      digitalWrite(10, HIGH);
    }else if(temperature >= tempMax){
      clearRGB();
      digitalWrite(11, HIGH);
    }else{
      clearRGB();
      digitalWrite(9, HIGH);
    }

    Serial.println("Humidity: " + String(humidity));

    if(humidity <= humidLow){ // <- if low on water
      startPumping(); // <- activate wattering system
    }

    Serial.println("PH Level: " + String(PH)); // <- ph can sensor can be connected to a system that drops specific subsance lowering or rising ph level
    /*
    if(PH < PHLow || PH > PHMax){
      // Turn led to red, PH level not suitable
    }
    */

    Serial.println("---------------------------------------");
  }
}

void checkNodeMCUTimer(){

  // send data to NodeMCU
  if (millis() - sendLastMillis >= sendDataTimeout*60*1000UL) // <- check if time has passed
  {
    sendLastMillis = millis();  //get ready for the next iteration

    // read all sensors
    float humidity  = readSoil();
    float temperature = getTemperature();
    float PH = getPH();
    bool movement = checkForIntruder();
    sendToESP(temperature, humidity, PH, movement); // <- send to ESP
  }
}

void sendToESP(float temperature, float humidity, float PH, int movement){
  
  float light = analogRead(A1); // <- read current brightness

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  // insert all data into JSON object
  data["humidity"] = humidity;
  data["temperature"] = temperature;
  data["PH"] = PH;
  data["movement"] = movement;
  data["Birghtness"] = light;

  data.printTo(nodemcu); // Send JSON object to nodeMCU using specified RX TX pins
  jsonBuffer.clear(); // clear JSON
}

void prepThreshold(){
  // set max and min sensor values based on the mode seleted
  switch (Mode) {
   case 1:
      tempLow = 18;
      tempMax = 35;
      humidLow = 70;
      humidMax = 95;
      PHLow = 6;
      PHMax = 6.5;
      break;

   case 2:
      tempLow = 19;
      tempMax = 27;
      humidLow = 75;
      humidMax = 90;
      PHLow = 6.2;
      PHMax = 6.8;
      break;

   default:
      // light red
      while(true){}
      break;
  }
}

void clearRGB(){ // clear RGB LED pins
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}


