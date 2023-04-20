
//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>


// Replace with your network credentials
const char* ssid     = "Wireless-N"; // <- specify Wi-Fi username
const char* password = ""; // <- specify Wi-Fi password

// ThingSpeak URL
String URL = "http://api.thingspeak.com/update?api_key=GROD703OYN0PZP1I&";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);


void setup() {
  
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);

  WiFiSetup(); // <- connect to Wi-Fi

  while (!Serial) continue;
  Serial.println("System started");
}

void loop() {
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu); // <- try to read some data

  if (data == JsonObject::invalid()) {
    jsonBuffer.clear(); // <- Invalid Json Object
    return;
  }

  Serial.println("JSON Object Recieved");

  // extract all data from JSON object and print to serial
  Serial.print("Recieved Humidity:  ");
  float hum = data["humidity"];
  Serial.println(hum);

  Serial.print("Recieved Temperature:  ");
  float temp = data["temperature"];
  Serial.println(temp);

  Serial.print("Recieved PH:  ");
  float PH = data["PH"];
  Serial.println(PH);

  Serial.print("Recieved movement:  ");
  float mov = data["movement"];
  Serial.println(mov);

    Serial.print("Recieved brighness:  ");
  float bri = data["Birghtness"];
  Serial.println(bri);

  Serial.println("-----------------------------------------");

  uploadData(hum, temp, PH, mov, bri); // <- send received data to ThingSpeak

}

void uploadData(float humdity, float temperature, float PH, float movement, float brighness){
  WiFiClient client;
  HTTPClient http;
  String newUrl= URL + "field1=" + String(temperature) + "&field2=" + String(humdity) + "&field3=" + String(PH) + "&field4=" + String(movement); + "&field5=" + String(brighness); // <- place results in specified fields (depends on field set up)
  
  // send ping
  http.begin(client,newUrl);
  int responsecode=http.GET(); 
  String data=http.getString(); // <- get request to ThingSpeak and record result
  Serial.println(data); // <- result is the n'th number of the ping
  http.end();
}


void WiFiSetup(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // <- print while trying to locate and connect to the Wi-Fi
  }

  // Print local IP address (could be useful for future development where web server is created)
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

