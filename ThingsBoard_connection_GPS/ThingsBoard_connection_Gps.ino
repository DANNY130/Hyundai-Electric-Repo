//GPS portion
#include <TinyGPS++.h>
float latitude;
float longitude;
float speed;
float satellites;
String direction;

static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

unsigned int move_index = 1;


#define SIM800L_IP5306_VERSION_20190610

#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG          SerialMon
#include<string.h>
#include "utilities.h"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800          // Modem is SIM800
#define TINY_GSM_RX_BUFFER      1024   // Set RX buffer to 1Kb

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
//initiallize GSM modem
TinyGsm modem(SerialAT);
#endif


#include "ThingsBoard.h"


// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "internet";
const char user[] = "";
const char pass[] = "";

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "F34yZw7pwbJgzgYa9Wgb"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"


//Initialize GSM client
TinyGsmClient client(modem);

//Initialize ThingsBoard instance
ThingsBoard tb(client);

//Set to true, if modem is connected
bool modemConnected = false;







void setup() {
  
  // Set console baud rate
  SerialMon.begin(115200);
  //pinMode(relay_pin,OUTPUT);
  delay(10);

  // Start power management
  if (setupPMU() == false) {
    Serial.println("Setting power error");
  }

  // Some start operations
  setupModem();
  
  SerialMon.println("Wait...");

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  delay(6000);

  SerialMon.println("Initializing modem...");
  modem.restart();
  // modem.init();

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);
  delay(1000);

  SerialAT.println("AT"); //Once the handshake test is successful, it will back to OK
}

void loop() {
  delay(1000);
  
  if (!modemConnected) {
    SerialMon.print(F("Waiting for network..."));
    if(!modem.waitForNetwork()) {
      Serial.println("fail");
      delay(1000);
      return;
    }
    Serial.println(" OK");
    
    Serial.print(F("Connecting to "));
    Serial.print(apn);
    if (!modem.gprsConnect(apn, user, pass)) {
      Serial.println("fail");
      delay(10000);
      return;
    }
    modemConnected = true;
    Serial.println(" OK");
  }
  if (!tb.connected()) {
    //Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
    
  }
  Serial.println("Sending data....");
  
   while  (Serial.available() >0) {
    if (gps.encode(Serial.read()))
      displayInfo();
  }
  
   // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details

  tb.sendTelemetryFloat("latitude", latitude);
  tb.sendTelemetryFloat("longitude", longitude);

  tb.loop();
}

void displayInfo() {
  latitude = (gps.location.lat());
  longitude = (gps.location.lng());
  speed = gps.speed.kmph();
  
  direction = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
  satellites = gps.satellites.value(); //get number of satellites
  
  
}