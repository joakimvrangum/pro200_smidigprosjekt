#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int pinRX = 5;    //5;  // 3
const int pinTX = 4;    //4;  // 1
const int pinBarcodeTrigger = 16;

char c;
String barCode;


SoftwareSerial mySerial(pinRX, pinTX); // RX,  TX

void setup() {
  // Pins and serial settings
  pinMode(pinBarcodeTrigger, OUTPUT);
  pinMode(pinRX, INPUT);
  pinMode(pinTX, OUTPUT);
  mySerial.begin(115200);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // WiFi
  const char* ssid = "kolonialnett";
  const char* pass = "18626ff2d2c5";
  WiFi.begin(ssid, pass);
  Serial.print("Kobler til Wifi.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Set scanner to TTL-mode
//  byte mode_ttl[] = {0x0B, 0x04, 0x31, 0x00, 0x00, 0x41, 0x30, 0x30, 0x30, 0x30, 0xFF, 0xFD, 0xC0};
//  if (mySerial.write(mode_ttl, sizeof(mode_ttl))) Serial.println("TTL-modus satt.");
}

void loop() {
//  digitalWrite(pinBarcodeTrigger, HIGH);
//  delay(30);
  digitalWrite(pinBarcodeTrigger, HIGH);
  if (mySerial.available()) {
    c = mySerial.read();
    //Serial.print(c);
    if ((int)c == 13) {
      if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Sender strekkode til server >> " + barCode + " >> ");
      HTTPClient http;
      Serial.println("http://www.oasisarchive.com/kolonial/handlekurv/1/" + barCode);
      http.begin("http://www.oasisarchive.com/kolonial/handlekurv/1/" + barCode);
      int httpCode = http.GET();
      if (httpCode > 0) {
        String response = http.getString();
        Serial.println(response);
      }
      http.end();
      c = '\0';
      barCode = "";
      }
    } else {
      if ((' ' <= c) && (c <= '~')) barCode += c;
    }
  }
  delay(400);
}
