#pragma once

// define default env values
#define MoistureThreshold 100


// soil sensor

int sensorValue = 0;  // variable to store the value coming from the sensor

int sensorPin = A2;    // select the input pin for the potentiometer
int soilPower = 8;      // to Power the soil sensor


void soilSensorSetup(){
  pinMode(8, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(8, LOW);//Set to LOW so no power is flowing through the sensor
}

//This is a function used to get the soil moisture content
int readSoil()
{
    digitalWrite(8, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    int val = analogRead(sensorPin);//Read the SIG value form sensor 
    digitalWrite(8, LOW);//turn D7 "Off"
    float res = val / 10;
    return res;//send current moisture value
}