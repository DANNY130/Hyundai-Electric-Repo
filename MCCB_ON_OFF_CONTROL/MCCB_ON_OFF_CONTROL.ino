#define BLYNK_TEMPLATE_ID           "TMPLFTelD9uR"
#define BLYNK_DEVICE_NAME           "Blynk Project"
#define BLYNK_AUTH_TOKEN            "lOW2E9EKoVK8Sw2Wulyz2awdyJVtx3lq"


#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
// #define BLYNK_HEARTBEAT 30

// Please select the corresponding model

#define SIM800L_IP5306_VERSION_20190610
// #define SIM800L_AXP192_VERSION_20200327
// #define SIM800C_AXP192_VERSION_20200609
// #define SIM800L_IP5306_VERSION_20200811

#include "utilities1.h"

#define relay1 13

// Select your modem:
#define TINY_GSM_MODEM_SIM800

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// See all AT commands, if wanted
//#define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[] = "internet";  // search for apn of your network provider on Google
const char gprsUser[] = "";
const char gprsPass[] = "";

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
const char auth[] = BLYNK_AUTH_TOKEN;


/*BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(relay1, pinValue);
  delay(1000);
  // process received value
}*/
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  String mobile_number = "+84983573750";
  String message_for_relay = "";
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

void setup()
{
  // Set console baud rate
  SerialMon.begin(115200);

  pinMode(relay1, OUTPUT);


  digitalWrite(relay1, HIGH);

  delay(10);

  setupModem();

  SerialMon.println("Wait...");

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  delay(6000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // modem.init();

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  Blynk.begin(auth, modem, apn, gprsUser, gprsPass);

}

void loop()
{
  Blynk.run();

}
