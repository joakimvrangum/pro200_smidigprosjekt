#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int pinRX = 5;    //5;  // 3
const int pinTX = 4;    //4;  // 1
const int pinBarcodeTrigger = 16;

const int kundeId = 1;
char c;
String barCode;
String postData;


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
}

void loop() {
  digitalWrite(pinBarcodeTrigger, HIGH);
  if (mySerial.available()) {
    c = mySerial.read();
    if ((int)c == 13) {
      if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Sender strekkode til server >> " + barCode + " >> ");
      HTTPClient http;
      postData = String("kunde=1") + "&upc=" + barCode;
      http.begin("http://www.vrangum.com/kolonial/handlekurv");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpCode = http.POST(postData);
      String response = http.getString();
      Serial.println("Server >> " + (String) httpCode + " >> " + response);
//      Serial.println("http://www.vrangum.com/kolonial/handlekurv/1/" + barCode);
//      http.begin("http://www.vrangum.com/kolonial/handlekurv/1/" + barCode);
//      int httpCode = http.GET();
//      if (httpCode > 0) {
//        String response = http.getString();
//        Serial.println(response);
//      }
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
