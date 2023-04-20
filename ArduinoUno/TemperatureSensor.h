#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wires are plugged into pin 2 and 3 on the Arduino
#define ONE_WIRE_BUS_IN 2
#define ONE_WIRE_BUS_OUT 3
 
// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWireFirst(ONE_WIRE_BUS_IN);
OneWire oneWireSecond(ONE_WIRE_BUS_OUT);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature firstSensor(&oneWireFirst);  
DallasTemperature secondSensor(&oneWireSecond); 
 
void temperatureSetup()
{
  // Start up the library
  firstSensor.begin();
  secondSensor.begin();
}
 
float getTemperature()
{

  // Send the command to get temperatures
  firstSensor.requestTemperatures(); 
  secondSensor.requestTemperatures();

  float req_1 = firstSensor.getTempCByIndex(0);
  float req_2 = secondSensor.getTempCByIndex(0);

  return ((req_1 + req_2) /2); // return average of the two sensors (more accuracy)

}