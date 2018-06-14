#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <WiFiManager.h>

const int pinRX = 5;
const int pinTX = 4;
const int pinBarcodeTrigger = 16;

const int boksID = 1; // Knyttes opp mot kundeID i databasen

SoftwareSerial mySerial(pinRX, pinTX);
WiFiManager wifiManager;

char c;
String barCode;
String postData;
bool blinkGreen = false;
long previousMillis = 0;
long interval = 500;

bool scanningBlink = false;

void setup() {
  pinMode(pinBarcodeTrigger, OUTPUT);
  pinMode(pinRX, INPUT);
  pinMode(pinTX, OUTPUT);

  Serial.begin(9600);
  mySerial.begin(115200);

  wifiManager.setColor(0, 0, 255);
  //wifiManager.resetSettings();
  wifiManager.autoConnect("KOLONIAL.NO SVISJ - OPPSETT");

  if (WiFi.status() != WL_CONNECTED) {
    wifiManager.setColor(255, 0, 0);
    delay(2000);
  }
  if (WiFi.status() == WL_CONNECTED) {
    wifiManager.setColor(0, 255, 0);
    delay(2000);
  }
  wifiManager.setColor(255, 30, 0);
}

void loop() {

  unsigned long currentMillis = millis();

  //Reconnecter til nettverk dersom den ikke er på eller har fallt ut
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mangler nett. Prøver å sette opp nett igjen.");
    reconnect();
  }

  digitalWrite(pinBarcodeTrigger, HIGH);

  if (barCode.length() > 0) {
    if (currentMillis - previousMillis > interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      scanningBlink = !scanningBlink;
    }
  } else {
    wifiManager.setColor(255, 30, 0);
  }

  //Tar imot data fra skanneren
  if (mySerial.available()) {
    delay(400);
    c = mySerial.read();
    if ((!blinkGreen) && (barCode.length() > 0) && (barCode.length() < 2)) {
      wifiManager.setColor(0, 255, 0);
      delay(1000);
      blinkGreen = true;
    }

    if (scanningBlink) {
      wifiManager.setColor(255, 30, 0);
    } else {
      wifiManager.setColor(0, 0, 0);
    }

    //Bruker dataen fra skanneren og sender en post request til serveren
    if ((int)c == 13) {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Sender strekkode til server >> " + barCode + " >> ");
        HTTPClient http;
        postData = String("boks_id=") + boksID + "&upc=" + barCode;
        http.begin("http://www.vrangum.com/kolonial/handlekurv");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpCode = http.POST(postData);
        String response = http.getString();
        Serial.println("Server >> " + (String) httpCode + " >> " + response);
        http.end();
        c = '\0';
        barCode = "";

        //Setter farge på LED'ene utifra responsen fra server
        if (response == "OK") {
          blinkGreen = false;
          //wifiManager.setColor(0, 255, 0); Lyser grønt om varen blir lagt til i handlekurven
          delay(500);
        } else if (response == "FAIL") {
          blinkGreen = false;
          wifiManager.setColor(255, 0, 0);
          delay(3000);
        } else if (response == "LEVERING BESTILT") {
          wifiManager.setColor(255, 255, 255);
          delay(2000);
        }

      }
    } else {

      if ((' ' <= c) && (c <= '~')) barCode += c;

    }
  }
  delay(400);
}

//Metode for å reconnecte til nettverk
void reconnect() {
  wifiManager.setColor(0, 0, 255);
  WiFiManager wifiManager;
  wifiManager.autoConnect("KOLONIAL.NO SVISJ - OPPSETT");
}
