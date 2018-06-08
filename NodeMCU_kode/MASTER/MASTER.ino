#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>

bool changePass = false;
bool blinkGreen = false;

const int pinRX = 5;    //5;  // 3
const int pinTX = 4;    //4;  // 1
const int pinBarcodeTrigger = 16;
const int pinLED = 12;

const int boksID = 1;
char c;
String barCode;
String postData;

#define NUMPIXELS 9

SoftwareSerial mySerial(pinRX, pinTX); // RX,  TX
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, pinLED, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(pinBarcodeTrigger, OUTPUT);
  pinMode(pinRX, INPUT);
  pinMode(pinTX, OUTPUT);

  Serial.begin(9600);
  mySerial.begin(115200);
  pixels.begin();
  int count = 0;
  setColor(0, 0, 255);
  WiFiManager wifiManager;
  wifiManager.autoConnect("KOLONIAL.NO SVISJ - OPPSETT");
  setColor(255, 30, 0);
  /**
   BLINKING VED OPPKOBLING - *IKKE* implementert enda
  while (WiFi.status() != WL_CONNECTED) {
    setColor(0, 0, 255);
    delay(700);
    setColor(0, 0, 0);
    count++;
    Serial.println("Prøver å koble til nettverk");
    delay(700);
  }
  **/
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mangler nett. Prøver å sette opp nett igjen.");
    reset();
  }
    setColor(255, 30, 0);
    digitalWrite(pinBarcodeTrigger, HIGH);
    if (mySerial.available()) {
      delay(400);
      c = mySerial.read();
      if ((!blinkGreen) && (barCode.length() > 0) && (barCode.length() < 2)) {
        setColor(0, 255, 0);
        delay(500);
        blinkGreen = true;
      }
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

          if (response == "OK") {
            blinkGreen = false;
            setColor(0, 255, 0);
            delay(500);
          } else if (response == "FAIL") {
            blinkGreen = false;
            setColor(255, 0, 0);
            delay(2000);
          } else if (response == "LEVERING BESTILT") {
            setColor(255, 255, 255);
            delay(2000);
          }

        }
      } else {
        if ((' ' <= c) && (c <= '~')) barCode += c;
      }
    }
    delay(400);
}

void setColor(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
    delay(10);
  }
}

void reset() {
  setColor(0, 0, 255);
  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.autoConnect("KOLONIAL.NO SVISJ - OPPSETT");
}

