#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

String SSIDName;
String SSIDPassword;

const char *APID = "Svisj - Oppsett";
const char *APPASS = "";

const byte DNS_PORT = 53;
bool wificonnected = false;

DNSServer dnsServer;
ESP8266WebServer server(80);

const int pinRX = 5;    //5;  // 3
const int pinTX = 4;    //4;  // 1
const int pinBarcodeTrigger = 16;

const int kundeId = 1;
char c;
String barCode;
String postData;

SoftwareSerial mySerial(pinRX, pinTX); // RX,  TX

#define LEDPIN            12
#define NUMPIXELS      9

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
int delayval = 10;

void setup() {
  pinMode(pinBarcodeTrigger, OUTPUT);
  pinMode(pinRX, INPUT);
  pinMode(pinTX, OUTPUT);

  Serial.begin(115200);
  EEPROM.begin(1024);
  mySerial.begin(115200);
  pixels.begin();

  int address = 0;
  char winame[32];
  char wipass[64];
  EEPROM.get(address, winame);
  address = 100;
  EEPROM.get(address, wipass);

  WiFi.begin(winame, wipass);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED && count < 60) {
    delay(500);
    count++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wificonnected = true;
    Serial.println();
    Serial.print("The wifi we are connected to is called : ");
    Serial.println(winame);
    Serial.print("The wifi's password is saved in plain text as : ");
    Serial.println(wipass);
  } else {
    wificonnected = false;
    WiFi.softAP(APID, APPASS);
    IPAddress APIP = WiFi.softAPIP();
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    Serial.println("APIP er");
    Serial.println(APIP);
    server.on("/", handleRoot);
    server.on("/input", getInputs);
    server.begin();
  }
}

void loop() {
  if (!wificonnected) {
    setColor(255, 0, 0);
    dnsServer.processNextRequest();
    server.handleClient();
    delay(1);
  } else {
    setColor(0, 255, 0);
    //digitalWrite(pinBarcodeTrigger, HIGH);
    if (mySerial.available()) {
      Serial.println("Serial lest");
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

}

void getInputs() {
  //Hvis denne siden blir lastet betyr det at noen enten har gått inn på 192.168.4.1/input på egenhånd
  //Eller så har data blitt submittet
  //Må i så fall hente data fra siden
  Serial.println("We are outside of if statement");

  if (!wificonnected && server.arg("wifiname") != "") {
    String ssid = server.arg("wifiname");
    String password = "";
    Serial.print("Wifi is called ");
    Serial.println(ssid);
    Serial.println("Checking if there was a password input");
    if (server.arg("wifipass") != "") {
      password = server.arg("wifipass");
      Serial.print("There is a password and it is: ");
      Serial.println(password);
    }

    char bufid[32];
    char passid[64];
    ssid.toCharArray(bufid, ssid.length() + 1);
    password.toCharArray(passid, password.length() + 1);

    //Lager en liste over nettverk
    int networks = WiFi.scanNetworks();

    for (int i = 0; i < networks; i++) {
      Serial.print("Name of network : ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Strength : ");
      Serial.println(WiFi.RSSI(i));
      //Passord eller ikke
      Serial.print("Encrypted with : ");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      bool encrypted = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? false : true;
      if (ssid == WiFi.SSID(i)) {

        Serial.println("NETWORK FOUND");
        if (encrypted) {
          Serial.println("Password needed");
          if (password != "") {
            WiFi.begin(bufid, passid);
            int count = 0;
            while (WiFi.status() != WL_CONNECTED && count < 60) {
              delay(500);
              Serial.println("connecting");
              count++;
            }
            if (WiFi.status() == WL_CONNECTED) {
              wificonnected = true;
              Serial.println("Connection successful, saving password to EEPROM");
              int address = 0;
              EEPROM.put(address, bufid);
              address = 100;
              EEPROM.put(address, passid);
            }
          } else {
            Serial.println("You need to provide a password");
          }
        } else {
          Serial.println("You dont need a password here, but you do need to add some code to connect to wifi");
        }
      } else {
        Serial.println("Not the right network");
      }
      Serial.println();
      Serial.println();
    }
    if (!wificonnected) {
      Serial.println("Could not find the network");
    }
  }
  Serial.println("Inputs updated");
}

void handleRoot() {
  //String html = "<!DOCTYPE HTML> <html> <head> </head> <body> <h1>Welcome to KOLOSVISJ</h1> </body> </html>";
  String html = "<!DOCTYPE HTML><html><head> <title>KOLONIAL SVISJ SETUP</title> <meta charset=\"UTF-8\"> <meta name=\"author\" content=\"Gruppe 10\"> </head><body> <h1>WIFI AND PASSWORD</h1> <input type=\"text\" name=\"WIFI\" id=\"WIFI\"> <input type=\"text\" name=\"PASSWORD\" id=\"PASSWORD\"> <button onclick=\"submit()\">Submit</button> <p id=\"demo\"></p> <script>function submit() { var wifiname = document.getElementById(\"WIFI\").value; var wifipassword = document.getElementById(\"PASSWORD\").value; var url = \"/input\"; var params = \"wifiname=\"+wifiname+\"&wifipass=\"+wifipassword; var xmlHTTP = new XMLHttpRequest(); xmlHTTP.open(\"GET\", url+\"?\"+params, true); xmlHTTP.send(null);}</script> </body></html>";
  server.send(200, "text/html", html);

  Serial.println("Root page shown");
}

void setColor(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
    delay(delayval);
  }
}
