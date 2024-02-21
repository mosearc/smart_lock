
# SERVER HANDLED SMART LOCK UNIT
![Image](/scheme.png)
## Section links
 - [Description](#Description)
 - [Video](#Video)
 - [Presentation](#Presentation)
 - [Requirments](#Requirments)
 - [Project Layout](#Project-Layout)
 - [Getting Started](#Getting-Started)
 - [Getting Started](#Getting-Started)
 - [User Guide](#User-Guide)
 - [Team Members](#Team-Members)
## Description
The project consists on a microcontroller device handling the mechanism opening and closing a door. Its job is to take as input authentication factors with sensors (i.e. RFID scanner for NFC card scan; NUMPAD for PIN or PUK), then contact a remote server on an IP network though WiFi and basing on server response decide if opening the door or not. It implements brute force prevention security measures like asking for PUK after 3 wrong PIN insertions. Our model allows to have N smart locks all connected to the same server through IP internet: this allows to add/remove/check identities remotely, but also to monitor when and who access any specific door (i.e. NFC cards and PINs can be created/deleted using a simple web app or mobile application; specific doors can be blocked in specific daily hours; etc.)
## Video
[Link video](https://youtu.be/HvxnWQClJyk)
## Presentation
[Link presentation]()
## Requirments
### Hardware Requirements
#### 1] MSP432P401R (logic and sensors interconnection):
 - Servo Motor
- RFID sensor
- Screen
- Numpad
#### 2] ESP8266MOD (server-to-msp interconnection through WiFi internet access):
MCU capable of connecting WiFi to send IP packets and capable of communicate serially through UART with the MSP432. When the MSP432 needs to contact the server, it can send to the ESP8266 a specific command through UART (to check if a NFC card is authorized to open this specific door, for example). The ESP8266 will forward it to the server using its WiFi module; when receives a response from the server, it informs back the MSP432.
#### 3] SERVER:
Any computer accessible from the lock MCU through IP network capable of running Nodejs.
### Software requirements
1] Code Composer Studio (Texas Instrument)
- [MSP Driver Library](https://www.ti.com/tool/MSPDRIVERLIB)

2] Arduino IDE
- ESP8266 Library / Arduino_JSON

3] Visual Studio Code
- Nodejs / npm
## Project Layout
## Getting Started
- Mount the hardware following the schematic above.
- Clone the repo:
``` bash
git clone https://github.com/mosearc/smart_lock.git
```
- Open the folder corresponding to the module you want to compile/upload/run using the appropriate IDE (as specified in [Software requirements](#Software-requirements) section). Make sure to obtain the libraries needed.
## User Guide

## Next Features
- Remote block/unblock handling
- More local control using numpad for input and screen for output
- More remote control adding server functionalities
- Various apps for handling locks/cardIds/pins (web app / mobile apps / etc.)
- Server side activity monitor logger
- Configurable server side activation/deactivation of specific doors basing on daily time.
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
