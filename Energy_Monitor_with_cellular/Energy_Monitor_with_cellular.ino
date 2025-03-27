#define BLYNK_PRINT Serial
#define BLYNK_HEARTBEAT 30
#define TINY_GSM_MODEM_SIM800

#define SIM800L_IP5306_VERSION_20190610
#define BLYNK_TEMPLATE_ID "TMPLFTelD9uR"
#define BLYNK_DEVICE_NAME "Blynk Project"
#define BLYNK_AUTH_TOKEN "lOW2E9EKoVK8Sw2Wulyz2awdyJVtx3lq"

#include "utilities.h"


#include <EmonLib.h>
EnergyMonitor emon1;
//#define DHTPIN 2
//#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);

//const int soil = 4;
//int soilMoistureValue = 0;
//int soilmoisturepercent  = 0;
//float t,h;

#define relay1 13

float realPower;
float apparentPower;
float powerFactor;
float supplyVoltage;
float Irms;
float previousVrms;

int countOFF = 0;


String mobile_number = "+19206779818";
//String mobile_number = "+84983573750";
//Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

//  Set serial for AT commands (to the module)
// Use Hardware Serial on Mega,  Leonardo, Micro
#define SerialAT Serial1



// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

//Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG  SerialMon

#define TINY_GSM_RX_BUFFER    1024 // Set RX buffer to 1kb




//Set GSM PIN, if any
#define GSM_PIN   ""

//Your GPRS credentials, if any
const char apn[] = "internet"; // search for apn of your network provider on Google
const char gprsUser[] = "";
const char gprsPass[] = "";

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <string.h>



#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);



// Reading message portion
char* lower = "";
char* response = "";
String res = "";
bool new_data = 0;
int relay = 0;

#define SMS_TARGET  "+19206779818"
//#define SMS_TARGET "+84983573750"
//char* msg = "+cmt: \"+19206779818\"";
//char* msg = "+cmt: \"+84983573750\"";
char* value1 = "relay on";
char* value0 = "relay off";


//
BlynkTimer timer;
BlynkTimer time_sms;

const char auth[] = BLYNK_AUTH_TOKEN; // Change AUTH

/*BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  if(pinValue == 1)
  {
  digitalWrite(relay1, HIGH);
  }
  else
  {
  digitalWrite(relay1, LOW);

}  delay(1000);
}*/
String message_for_relay;
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  if (pinValue == 1) {
    digitalWrite(relay1, HIGH);
    message_for_relay = "The relay is activated";
    modem.sendSMS(mobile_number, message_for_relay);
    message_for_relay = "";
    Serial.println("The relay is on");
  }
  else
  {
    digitalWrite(relay1, LOW);
    message_for_relay = "The relay is deactivated";
    modem.sendSMS(mobile_number, message_for_relay);
    message_for_relay = "";
    Serial.println("The relay is off");
  }
}

void sendSensor() {
  emon1.calcVI(20, 2000);
  
  supplyVoltage = emon1.Vrms;
  Irms = emon1.Irms;
  powerFactor = -emon1.powerFactor;
  apparentPower = emon1.apparentPower;
  realPower = -emon1.realPower;
  
  Blynk.virtualWrite(V2, supplyVoltage );
  Blynk.virtualWrite(V3, Irms);
  previousVrms = supplyVoltage;
  

}

void setup() {
  //Set console baud rate
  SerialMon.begin(115200);
  
  pinMode(relay1, OUTPUT);
  //pinMode(soil, INPUT);
  pinMode(14, INPUT);
  pinMode(12,INPUT);
  
  emon1.voltage(14, 70, 0);
  emon1.current(12, 0.13);
  
  digitalWrite(relay1, HIGH);
  
  delay(10);
  
  //Start power management
  if(setupPMU() == false) {
    Serial.println("Setting power error");
    
  }
  // Some start operations
  setupModem();
  
  SerialMon.println("Wait...");
  
  //Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  
  delay(6000);
  
  //Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // modem.init();
  
  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);
  delay(1000);
  
  
  SerialAT.println("AT"); //Once the handshake test is successful, it will back to OK
  //updateSerial();
  
  SerialAT.println("AT+CMGF=1"); //Configuring TEXT Mode
  //updateSerial();
  SerialAT.println("AT+CNMI=1,2,0,0,0"); //Decides how newly arrived SMS messages should be handled 
  //updateSerial();
  
  
  //dht.begin();
  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, modem, apn, gprsUser, gprsPass);
  //time_sms.setInterval(500L, updateSerial);
}

void loop()
{
  Blynk.run();
  timer.run();
  //time_sms.run();
  
}
void updateSerial() {
  while (Serial.available()) {
    SerialAT.write(Serial.read()); //Forward what Serial received to Software Serial Port
  }
  while (SerialAT.available()) {
    
    
    char add = SerialAT.read();
    res = res + add;
    delay(1);
    new_data = 1;
    
  }
  if (new_data) {
    response = &res[0];
    //----------------Converting every character of the String in lower form
    const int length = strlen(response); // get the length of the text
    lower = (char*)malloc(length + 1); // allocate 'length' bytes +1 (for null terminator) and cast to char*
    lower[length] = 0; //Set the last byte to a null terminator
    //----------------------------------------------copy all character bytes to the new buffer using tolower
    for( int i = 0; i < length; i++) {
      lower[i] = tolower( response[i]);
      
    }
    Serial.println(lower);// printin the String in lower character form
    Serial.println("\n");
    relay_control(lower);
    response = "";
    res = "";
    lower = "";
    new_data = 0;
  }
}

void relay_control(char* lower) {
  if (strstr(lower,msg)) {
    String source  = (String)lower;
    
    
    char* desti = &source[0];
    int i = 0;
    while (desti[i+2] != '\n') {
      i++;
    }
    String destin = (String)desti;
    source = source.substring(51);
    
    Serial.print("Your message is = "); Serial.print(source);
    char* relay_command = &source[0];
    if(strstr(relay_command, value1)) {
      Blynk.virtualWrite(V1, HIGH);
      relay = 1;
      return;
    }
    if(strstr(relay_command, value0)) {
      Blynk.virtualWrite(V1, LOW);
      relay = 0;
      return;
    }
  }
}








