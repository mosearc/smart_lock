# SMART LOCK - Project for the UniTn course "Embedded Software for the Internet of Things" (group 5)
![Image](/media/scheme.png)

## Section links
 - [Description](#Description)
 - [Video](#Video)
 - [Presentation](#Presentation)
 - [Requirments](#Requirments)
 - [Project Layout](#Project-Layout)
 - [Software Design](#Software-Design)
 - [Getting Started](#Getting-Started)
 - [Getting Started](#Getting-Started)
 - [Next Features](#Next-Features)
 - [Team Members](#Team-Members)

## Description
The project consists on a microcontroller device handling the mechanism opening and closing a door. Its job is to take as input authentication factors with sensors (i.e. RFID scanner for NFC card scan; NUMPAD for PIN or PUK), then contact a remote server on an IP network though WiFi and basing on server response decide if opening the door or not. It implements brute force prevention security measures like asking for PUK after 3 wrong PIN insertions. Our model allows to have N smart locks all connected to the same server through IP internet: this allows to add/remove/check identities remotely, but also to monitor when and who access any specific door (i.e. NFC cards and PINs can be created/deleted using a simple web app or mobile application; specific doors can be blocked in specific daily hours; etc.)
## Video
[![Video](https://img.youtube.com/vi/HvxnWQClJyk/0.jpg)](https://www.youtube.com/watch?v=HvxnWQClJyk)

[Link video](https://youtu.be/HvxnWQClJyk)

## Presentation
[Link presentation](https://docs.google.com/presentation/d/1c6pP84dEDISyiaMGwwINRryTseTZ8TMKDyQ94MY69VM/edit#slide=id.g2bb3de31d78_3_63)

## Requirments

### Hardware Requirements

#### 1] MSP432P401R Board + Educational booster pack (led, display):
![Image](/media/boosterpack.png)
![Image](/media/msp.jpg)

For our project we used MSP432P401R Board a microcontroller from Texas Instruments based on a 32-bit ARM Cortex-M4 CPU. The Educational Booster Pack with its additional hardware peripherals is also required.
With the led and the lcd in the boosterpack we display the information about the status useful for the user.

####  2] RFID SENSOR
![Image](/media/rfid.png)

This component provide information about the nfc card; needs a power supply of 3.3V to operate. It sends informations to the board throught SPI protocol.

#### 3] PIN PAD
![Image](/media/pinpad.jpg)

This component implement the second authentication factor, it is used to collect the pin code.

#### 4] ESP8266MOD (server-to-msp interconnection through WiFi internet access):
![Image](/media/esp.png)

MCU capable of connecting WiFi to send IP packets and capable of communicate serially through UART with the MSP432. When the MSP432 needs to contact the server, it can send to the ESP8266 a specific command through UART (to check if a NFC card is authorized to open this specific door, for example). The ESP8266 will forward it to the server using its WiFi module; when receives a response from the server, it informs back the MSP432.

#### 5] SERVER:

Any computer accessible from the lock MCU through IP network capable of running Nodejs.

#### 6] DOOR OPEN SYSTEM
![Image](/media/pir.jpg)
![Image](/media/servo.jpg)

This system consist in a servo motor that open the door when the pir sensor detect a movement. To work it needs a power supply of 5V taken from the board.


### Software requirements
1] [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO)
- [MSP Driver Library](https://www.ti.com/tool/MSPDRIVERLIB)
- [MSP Graphics Library](https://www.ti.com/tool/MSP-GRLIB)

2] [Arduino ide](https://www.arduino.cc/en/software)
- ESP8266 Library / Arduino_JSON

3] Visual Studio Code
- Nodejs / npm



  
## Project Layout
``` 
.
├── Esp8266
│   └── main_esp.cpp
├── msp432p401r
│   ├── Debug
│   │   ├── makefile
│   │   └── syscfg
│   │       ├── ti_drivers_config.c
│   │       └── ti_drivers_config.h
│   ├── main_nortos.c
│   ├── Modules
│   │   ├── Buzzer
│   │   │   ├── buzzer.c
│   │   │   └── buzzer.h
│   │   ├── NFC
│   │   │   ├── mfrc522.c
│   │   │   ├── mfrc522.h
│   │   │   ├── nfc.c
│   │   │   └── nfc.h
│   │   ├── PinPad
│   │   │   ├── LcdDriver.c
│   │   │   ├── LcdDriver.h
│   │   │   ├── pin_pad.c
│   │   │   └── pin_pad.h
│   │   ├── PIR
│   │   │   ├── pir.c
│   │   │   └── pir.h
│   │   ├── Servo
│   │   │   ├── servo.c
│   │   │   └── servo.h
│   │   └── UART
│   │       ├── uart.c
│   │       └── uart.h
│   └── servo.syscfg
└── Server
    ├── app.js
    ├── db.json
    ├── package.json
    ├── package-lock.json
    └── protocol.txt
```
## Software Design
![Image](/media/FSM_smart_lock.jpg)

## Getting Started
- Mount the hardware.
- Clone the repo:
``` bash
git clone https://github.com/mosearc/smart_lock.git
```
- Open the folder corresponding to the module you want to compile/upload/run using the appropriate IDE (as specified in [Software requirements](#Software-requirements) section). Make sure to obtain and import the libraries needed.
## Next Features
- Buzzer implementation as an alarm when the system is locked
- More interrupts usage  (i.e. for PIR sensor)
- Additional remote server functionalities
- Side apps development for handling locks/cardIds/pins 
- Activation/deactivation of specific doors basing on daily time
  
## Team members

 We always met each other in person to work at the project

 - Mosè Arcaro 
 - Stefano Camposilvan
 - Gabriel Fumagalli
 - Luca Bortolotti
