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
  // WiFi
  const char* ssid = "wifibeater";
  const char* pass = "atlassound314";
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(pinBarcodeTrigger, OUTPUT);
  pinMode(pinRX, INPUT);
  pinMode(pinTX, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
//  byte message[] = {0x0B, 0x04, 0x31, 0x00, 0x00, 0x41, 0x30, 0x30, 0x30, 0x30, 0xFF, 0xFD, 0xC0};
//  if (mySerial.write(message, sizeof(message))) Serial.println("Melding skrevet");
}

void loop() {
  digitalWrite(pinBarcodeTrigger, HIGH);
  delay(30);
  digitalWrite(pinBarcodeTrigger, LOW);
  if (mySerial.available()) {
    if (mySerial.available()) {
    c = mySerial.read();
    Serial.print(c);
    if ((int)c == 13) {
      Serial.print("Sender strekkode til server >> " + barCode);

      if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin("http://www.oasisarchive.com/kolonial/handlekurv/1/" + barCode);
      int httpCode = http.GET();
      if (httpCode > 0) {
        String response = http.getString();
        Serial.println(response);
      }
      http.end();
      }
      c = '\0';
      barCode = "";
      Serial.println();
    } else {
      barCode += c;
      
    }
  }
  }
  delay(400);
}
