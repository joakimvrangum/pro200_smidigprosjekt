#include <EEPROM.h>
#include "ESP8266WiFi.h"

String SSIDName;
String SSIDPassword;

int step = 0;

void setup() {
  Serial.begin(9600);
  EEPROM.begin(1024);
  delay(3000);
  Serial.println("Startup");

  Serial.println("SSID length:");
  Serial.print(EEPROM.read(1016));
  Serial.println();

  String esid;
  for (int i = 0; i < (EEPROM.read(1016)); ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(esid);

  Serial.println("Passord length:");
  Serial.print(EEPROM.read(1020));
  Serial.println();

  String esid2;
  for (int i = 200; i < (200 + EEPROM.read(1020)); ++i)
  {
    esid2 += char(EEPROM.read(i));
  }
  Serial.print("Passord: ");
  Serial.println(esid2);


  const char* ssid = esid.c_str();
  const char* pass = esid2.c_str();

  WiFi.begin(ssid, pass);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop

  //print a new line, then print WiFi connected and the IP address
  //Serial.println("");
  //Serial.println("WiFi connected");
  // Print the IP address
  //Serial.println(WiFi.localIP());

}

void loop() {

  if (step == 0) {
    Serial.println("Skriv inn SSID for ønsket nettverk:");
    step = 1;
  } else if (step == 1) {
    while (Serial.available() > 0) {
      SSIDName = Serial.readStringUntil('\n');
      step = 2;
    }

  } else if (step == 2) {
    Serial.println("Skriv inn passord for ønsket nettverk:");
    step = 3;
  } else if (step == 3) {
    while (Serial.available() > 0) {
      SSIDPassword = Serial.readStringUntil('\n');
      step = 4;
    }
  } else if (step == 4) {
    Serial.println(SSIDName);
    Serial.println(SSIDPassword);

    for (int i = 0; i < (SSIDName.length()); ++i)
    {
      EEPROM.write(i, SSIDName[i]);
      //Serial.print("Wrote: ");
      //Serial.println(SSIDName[i]);
    }
    EEPROM.write(1016, SSIDName.length());

    for (int i = 200; i < (200 + SSIDPassword.length()); ++i)
    {
      EEPROM.write(i, SSIDPassword[i - 200]);
      //Serial.print("Wrote: ");
      //Serial.println(SSIDPassword[i - 200]);
    }
    EEPROM.write(1020, SSIDPassword.length());
    EEPROM.commit();

    step = 5;
  }

}

