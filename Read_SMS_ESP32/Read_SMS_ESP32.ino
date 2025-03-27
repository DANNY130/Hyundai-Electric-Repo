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









// Please select the corresponding model

#define SIM800L_IP5306_VERSION_20190610

// Define the serial console for debug prints, if needed
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
TinyGsm modem(SerialAT);
#endif

char* lower = "";
char* response = " ";
String res = "";
bool new_data = 0;
int relay = 0;

#define SMS_TARGET  "+19206779818"
String mob_num = "+19206779818";
char* msg = "+cmt: \"+19206779818\"";
char* ring = "+clip: \"+19206779818\"";

char* value1 = "relay on";
char* value0 = "relay off";

#define relay_pin 13


void setup()
{

  // Set console baud rate
  SerialMon.begin(9600);
  pinMode(relay_pin,OUTPUT);
  delay(10);
  
  

  // Start power management
  if (setupPMU() == false) {
    Serial.println("Setting power error");
  }

  // Some start operations
  setupModem();
  
  //Set-up modem reset

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
  updateSerial();

  SerialAT.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  SerialAT.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();
  
  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }
  
}

void loop()
{
  updateSerial();
  
  while  (Serial.available() >0) {
    if (gps.encode(Serial.read()))
      displayInfo();
  }
}

void displayInfo() {
  latitude = (gps.location.lat());
  longitude = (gps.location.lng());
  speed = gps.speed.kmph();
  
  direction = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
  satellites = gps.satellites.value(); //get number of satellites
  
  
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    SerialAT.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (SerialAT.available())
  {
    //Serial.write(SerialAT.read());//Forward what Software Serial received to Serial Port

    char add = SerialAT.read();
    res = res + add;
    delay(1);
    new_data = 1;
  }

  if (new_data) {
    response = &res[0];
    //------------------------------------------------- Converting every character of the String in lower form
    const int length = strlen( response ); // get the length of the text
    lower = ( char* )malloc( length + 1 ); // allocate 'length' bytes + 1 (for null terminator) and cast to char*
    lower[ length ] = 0; // set the last byte to a null terminator
    //------------------------------------------------- copy all character bytes to the new buffer using tolower
    for ( int i = 0; i < length; i++ )
    {
      lower[ i ] = tolower( response[ i ] );
    }
    Serial.println(lower);// printin the String in lower character form
    Serial.println("\n");
    relay_control( lower);
    response = "";
    res = "";
    lower = "";
    new_data = 0;

  }
}

void relay_control(char* lower)
{

  if (strstr(lower, msg))
  {
  

    String source = (String)lower;
    

    char* desti = &source[0];
    int i = 0;
    while (desti[i+2] != '\n' )
      i++;
    String destin = (String)desti;
    source = source.substring(i+3);
    
    Serial.print("Your message is = ");Serial.print(source);
    char* relay1 = &source[0];
    if(strstr(relay1, value1))
    {
      digitalWrite(relay_pin, HIGH);
      res = modem.sendSMS(SMS_TARGET, String("Relay is turned on"));
      DBG("SMS:", res ? "OK" : "fail");
      relay = 1;
      return; 
      
    }
    if(strstr(relay1, value0))
  {
    digitalWrite(relay_pin, LOW);
    res = modem.sendSMS(SMS_TARGET, String("Relay is turned off"));
    DBG("SMS:", res ? "OK" : "fail");
    relay = 0;
    return; 
  }
  }

  if (strstr(lower, ring))
  {
    
    bool res = modem.callHangup();
    DBG("Hang up:", res ? "OK" : "fail"); 
    if(relay == 1)
    {
      digitalWrite(relay_pin, LOW);
      relay = 0;
      res = modem.sendSMS(SMS_TARGET, String("Relay is turned off"));
      DBG("SMS:", res ? "OK" : "fail");
      return;    
    }
    if(relay == 0)
    {
      digitalWrite(relay_pin, HIGH);
      relay = 1;
      res = modem.sendSMS(SMS_TARGET, String("Relay is turned on"));
      DBG("SMS:", res ? "OK" : "fail");
      return;
    }
  }
  
  if (strstr(lower, "location")) {
    String message = "";
    message = "Your Vehicle Current Location: " + (String)latitude + "," + (String)longitude + "\n" +"Check Map: \n" +" https:www.google.com/maps/@" +(String)latitude + "," + (String)longitude;
    modem.sendSMS(mob_num, message);
    message = "";
    return;
  }

}