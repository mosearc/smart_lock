# SERVER HANDLED SMART LOCK UNIT
![Image](/scheme.png)
## Section links
 - [Description] (##Description)
 - [Video] (#Video)
 - [Presentation] (#Presentation)
 - [Requirments] (#Requirments)
 - [Project Layout] (#Project Layout)
 - [Getting Started] (#Getting Started)
 - [Getting Started] (#Getting Started)
 - [User Guide] (#User Guide)
 - [Team Members] (#Team Members)
## Description
The project consists on a smart lock microcontroller device which authenticate an human user with 2 factors of authentication (NFC card and PIN) with the help of a IP reachable REST API HTTP server. Our model allows to have N smart locks all connected to the same server through IP internet. Thanks to the server, NFC cards and PINs can be created, deleted and handled remotelly.
## Video
[Link video](https://youtu.be/)
## Presentation
[Link presentation]()
## Requirments
### Hardware Requirements
#### 1] MSP432P401R (logic and sensors interconnection):
 - Servo Motor
 - Buzzer
- RFID sensor
- Screen
- Numeral pad
#### 2] ESP8266MOD (server-to-msp interconnection through WiFi internet access):
MCU capable of connecting WiFi to send IP packets and capable of communicate serially through UART with the MSP432. When the MSP432 needs to contact the server, it can send to the ESP8266 a specific command through UART (to check if a NFC card is authorized to open this specific door, for example). The ESP8266 will forward it to the server using WiFi and when receives a response sends it back to the MSP432.
#### 3] SERVER:
Any computer accessable from the lock MCU through IP network capable of running node.
### Software requirements
1] Code Composer Studio (Texas Instrument)
- [MSP Driver Library](https://www.ti.com/tool/MSPDRIVERLIB)
2] Arduino IDE
- ESP8266 Library
3] Visual Studio Code
- nodejs / npm
## Project Layout
## Getting Started
Mount the hardware following the schematic above.
Clone the repo:
``` bash
git clone https://github.com/mosearc/smart_lock.git
```
Use the appropriate IDE for the module you want to compile and upload to the board.
## User Guide

## Next Features
- Remote block/unblock handling
- More local control using numpad for input and screen for output
- More remote control adding server functionalities
- Various apps for handling locks/cardIds/pins (web app / mobile apps / etc.)
## Team members
 - Mosè
   - Developed RFID code
   - ...
 - Stefano
   - Designed main final state machine
   - ...
 - Gabriele
   - Developed
   - ...
 - Luca
   - Developed ESP8622 code
   - Developed Nodejs HTTP REST API server code
