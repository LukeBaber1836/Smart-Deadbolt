#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "fauxmoESP.h"
#include <Servo.h>

#define WIFI_SSID "(ENTER-NETWORK-NAME)"
#define WIFI_PASS "(ENTER-PASSWORD)"
#define SERIAL_BAUDRATE 115200

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------
// Servo
// -----------------------------------------------------------------------------

// Create a new servo object:
Servo myservo;

// Define the servo pin:
const int servoPin = 2; 
const int buttonPin1 = 12;
const int buttonPin2 = 13;
int serv_pos = 0;

// Create a variable to store the servo position:
int angle = 0;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("FauxMo demo sketch");
  Serial.println("After connection, ask Alexa/Echo to 'turn <devicename> on' or 'off'");

  // Wifi
  wifiSetup();

  // Fauxmo
  fauxmo.addDevice("Door_Lock");
  // Gen3 Devices or above
  fauxmo.setPort(80);

  // Allow the FauxMo to be discovered
  fauxmo.enable(true);

  // Attach the Servo variable to a pin:
  myservo.attach(servoPin);
 
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    Serial.print("Device: ");Serial.print(device_name);
    Serial.print(" state");
  
    if(state) {
      Serial.println("ON!");
      myservo.write(0);
    }
    else {
      Serial.println("off");
      myservo.write(180);
    }
  });

}

void loop() {
  fauxmo.handle();
}
