#include <string>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <SoftwareSerial.h>

using namespace std;

#define SSID "SSID_NAME"
#define PASSWORD "PASSWORD"
#define LOCKID "LOCK0"
#define API_KEY "1234"
#define SERVER_URL "IP_OR_DOMAIN"


//INPUT
#define D1 5
#define D2 4
//OUTPUT
#define D5 14
#define D6 12
// SoftwareSerial uartTX(D1, D5); //RX, TX //SELF COMMUNICATION (ONLY for testing without MSP available)
SoftwareSerial uart(D2, D6); //RX, TX
#define MAX_MESSAGE_SIZE 512

#define DEBUG_SERIAL_BAUD 9600
#define UART_BAUD 115200


//GENERAL
void halt() {
  Serial.println("Program halted!");
  while(1) {
    delay(10000);
  }
}

//LED
void ledSetup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void ledOn() {
  digitalWrite(LED_BUILTIN, LOW);
}

void ledOff() {
  digitalWrite(LED_BUILTIN, HIGH);
}

//WIFI
void checkWiFi() {
  if(WiFi.status() == WL_CONNECTED) {
    return;
  }

  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    Serial.print("Status: ");
    Serial.println(WL_CONNECTED);
    //Make led blink until connection is established
    ledOff();
    delay(500);
    ledOn();
    delay(500);
  }

  Serial.println("Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void WiFiSetup() {
  WiFi.begin(SSID, PASSWORD);
  checkWiFi();
}

//HTTP
int sendApiRequest(HTTPClient& http, const string& uri) {
  WiFiClient client;

  checkWiFi();

  http.begin(client, (SERVER_URL + uri).c_str());
  http.addHeader("Api-Key", API_KEY);
  int code = http.GET();
  Serial.print("HTTP response status code: ");
  Serial.println(code);
  return code;
}

char parseCommand(const string& cmd) {
  HTTPClient http;

  //Append LOCKID to all request so the server can authenticate the lock (using ApiKey too)
  int code = sendApiRequest(http, cmd + (cmd.find("?") != string::npos ? "&" : "?") + "lockId=" + LOCKID);

  //Convert json response and errors to simple protocol understandable by msp
  if(code == 200) {
    JSONVar obj = JSON.parse(http.getString());
    if(JSON.typeof(obj) == "undefined" || !obj.hasOwnProperty("result")) {
      return '2';
    } else {
      return ((String) obj["result"]) == "true" ? '1' : '0';
    }
  } else if(code == 401) {
    return '3';
  } else if(code > 0) {
    return '4';   
  } else {
    return '5';
  }

  http.end();
}


//SETUP
void setup() {
  Serial.begin(DEBUG_SERIAL_BAUD);
  Serial.print("\n");
  Serial.println("SETUP");

  ledSetup();
  ledOn(); //Led always on when WiFi connection is established
  WiFiSetup();

  // uartTX.begin(UART_BAUD); //SELF COMMUNICATION (ONLY for testing without MSP available)
  //Be sure to agree the UART configuration with the msp
  uart.begin(UART_BAUD, SWSERIAL_8N1, D2, D6, false); //RX, TX

  Serial.println("DONE");
}

//LOOP
//Listen to msp incoming messages and send response
string msg;
void loop() {
  //SELF COMMUNICATION (ONLY for testing without MSP available)
  // string sendMessage = "/card/isauthorized?cardIdPhysical=dadaiwnaevnsievniv";
  // Serial.print("uartTX sends: "); Serial.println(sendMessage.c_str());
  // for(char c : sendMessage) {
  //   uartTX.write(c);
  // }
  // uartTX.write('\n');
  // Serial.println("uartTX sent");
  //------------------------------------------------------------------

  if(uart.available() <= 0) {
    return;
  }

  char c = uart.read();
  Serial.print(c);

  //MESSAGE FORMAT:
  //ESP.RX<---MSP.TX   << \rBYTES WHICH ARE A COMMAND FOR THE ESP\n <<   //Variable length request (START_BYTE = '\r'; STOP_BYTE = '\n')
  //ESP.TX--->MSP.RX   >>                   B                       >>   //Single byte response
  if(c == '\r' || msg.size() > MAX_MESSAGE_SIZE) { //'\r' ==> Start of command (Discard any partial command)
    msg = "";                                      //         NOTE: If MAX_MESSAGE_SIZE is reached discard all the bytes and restart (for memory overflow prevention if too many bytes are sent)
  } else if(c == '\n') { //'\n' ==> End of command (Parse it and send response)
    Serial.print("Received message: "); Serial.println(msg.c_str());
    char res = parseCommand(msg.c_str());
    
    msg = "";

    Serial.print("Sending response: "); Serial.println(res);
    uart.write(res); //Send single byte response
    Serial.println("Sent");
  } else { //Accumulate characters into msg
    msg += c;
  }

  //SELF COMMUNICATION (ONLY for testing without MSP available)
  // Serial.print("uartTX receives: ");
  // n = 0;
  // while(uartTX.available() > 0 && n < MAX_MESSAGE_SIZE) {
  //   char c = uartTX.read();
  //   if(c == '\n')
  //     break;
  //   Serial.print(c);
  //   n++;
  // }
  // Serial.print("\n");
  // Serial.println("Received");
  //--------------------------------------------------
}
