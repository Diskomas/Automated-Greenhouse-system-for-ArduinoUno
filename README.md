# Automated greenhouse system for ArduinoUno

This template should help set up and start using the system

[Demonstration Video - Part 1](https://www.youtube.com/watch?v=btxUE5TKPLA)

[Demonstration Video - Part 2](https://www.youtube.com/watch?v=Eg5XAr9ddW0)


## Arduino IDE Project Setup

- Install Arduino_JSON library V0.2.0
- Install ArduinoJSON V5.13.2
- Download and install two libraries provided in "Automated-Greenhouse-system-for-ArduinoUno/Libraries"

## ArduinoUno Project Setup

### To edit ArduinoUno

Default set up: Temperature (18 to 35), Humidity (70 to 95), PH (6 to 6.5)

- Download all files from "Automated-Greenhouse-system-for-ArduinoUno/ArduinoUno" forlder.
- Upen SmartGreenhouse.ino using Arduino IDE
- In line 23 select mode (1 - cucumber, 2 - tomatoes) or define custom mode in line (162 - 184)
- Connect ArduinoUno cable to the computer and upload updated code

## NodeMCU Project Setup

### To edit NodeMCU

- Download all files from "Automated-Greenhouse-system-for-ArduinoUno/NodeMCU" forlder.
- Open NodeMCU.ino using Arduino IDE
- Select board: NodeMCU 0.9 (ESP-12 modules)
- Set the upload speed to 115200
- Change the SSID and Password for your Wi-Fi (line 13-14)
- Upload code to NodeMCU 


## General Idea

- Arduino uno reads sensor data
- Data is evaluated
- Based on the readings : water pump, LEDs, and hypotheticaly window controller, is turned on/off.
- Readings are send to ThingSpeak using NodeMCU ESP module via Wi-Fi connection. (Can be set up to host a local server and deliver a website)
