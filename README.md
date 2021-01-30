
The finished code for Door+ module. It's only develop branch from project repository.

Copyright (C) 2021 Door+ <d1n22@onet.pl><br>
This file is part of the Door+ project.<br>
The Door+ project can not be copied and/or distributed without the express<br>
permission of author Dominik Nuszkiewicz <d1n22@onet.pl>.<br>


# Door+

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Additional libraries](#libraries)
* [Components](#components)
* [Photos](#photos)


## General info
Door+ is automatic, contactless door module with IoT elements based on ESP32. It was made for engineering project on Warsaw University of Technology. It was built with MQTT server - https://hivemq.com
Physical module has a few functions. Main of these are:

1. Contactless opening and closing doors with ultrasonic motion sensor.
2. Counting people in the room - based on motion sensor too.
3. Remote communication provided by MQTT server and connected application of smartphone.


## Technologies
I used:
* https://hivemq.com as MQTT server
* MQTT Dash - app to send messages to MQTT
* PlatformIO IDE Ver 2.2.1
* Visual Studio Code Ver 1.52.1
* Tinkercad to create 3D printings

## Additional libraries
* PubSubClient 
* ESPAsyncWiFiManager
* ESPASyncWebServer
* Adafruit GFX Library
* AsyncTCP
* PubSubClient



## Components
Project was created with:
* Wemos D1 UNO R32 ESP32 WIFI Bluetooth
* Motion sensor HC-SR04 x2
* OLED monitor( SSD1306, 128x64px)
* Electric actuator LA10 500N 13mm/s, 12V, length max = 15 cm
* Power supply 12V for actuator
* Relays module with 2 canals 5V 10A/250VAC
* Power supply 9V for Wemos
* 3D Printings
* some typical cables and mini breadboard


## Photos


<p align="center">
  The main part with ESP32<br><br>
  <img width="300" src="https://github.com/ninjavaz/door_plus/blob/main/Photos/1.png?raw=true"><br><br>

  Back part with OLED<br><br>
  <img width="300" src="https://github.com/ninjavaz/door_plus/blob/main/Photos/2.png?raw=true"><br><br>
  
  Contactless open<br><br>
  <img width="300" src="https://github.com/ninjavaz/door_plus/blob/main/Photos/3.png?raw=true"><br><br>
  
  Actuator<br><br>
  <img width="300" src="https://github.com/ninjavaz/door_plus/blob/main/Photos/4.png?raw=true"><br><br>

  tel2<br><br>
  <img width="300" src="https://github.com/ninjavaz/door_plus/blob/main/Photos/telefon2.png?raw=true"><br><br>
  
  tel3<br><br>
  <img width="300" src="https://github.com/ninjavaz/door_plus/blob/main/Photos/telefon_3.png?raw=true"><br><br>
  
  Auto Wifi configuration<br><br>
  <img width="300" src="https://github.com/ninjavaz/door_plus/blob/main/Photos/wifi_config.png?raw=true"><br><br>



</p>

