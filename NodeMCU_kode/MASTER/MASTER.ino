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

const char *APID = "Svisj er delish";
const char *APPASS = "";

const byte DNS_PORT = 53;
bool wificonnected = false;
bool runOnceAP = false;
bool runOnceSTAT = false;
bool APStart = false;
bool changePass = false;
bool blinkGreen = false;

DNSServer dnsServer;
ESP8266WebServer server(80);

const int pinRX = 5;    //5;  // 3
const int pinTX = 4;    //4;  // 1
const int pinBarcodeTrigger = 16;

const int boksID = 1;
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

  Serial.begin(9600);
  EEPROM.begin(1024);
  mySerial.begin(115200);
  pixels.begin();

  int address = 0;
  char winame[32];
  char wipass[64];
  EEPROM.get(address, winame);
  Serial.println(winame);
  address = 100;
  EEPROM.get(address, wipass);
  Serial.println(wipass);

  Serial.println("Starter nettverk prosess");
  WiFi.begin(winame, wipass);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED && count < 20) {
    setColor(0, 0, 255);
    delay(700);
    setColor(0, 0, 0);
    count++;
    Serial.println("Prøver å koble til nettverk");
    delay(700);
  }

  if ((WiFi.status() == WL_CONNECTED) && (!changePass)) {
    setColor(0, 255, 0);
    delay(2500);
    wificonnected = true;
    Serial.println();
    Serial.print("The wifi we are connected to is called : ");
    Serial.println(winame);
    Serial.print("The wifi's password is saved in plain text as : ");
    Serial.println(wipass);
  } else {
    setColor(255, 0, 0);
    delay(2500);
    wificonnected = false;
    WiFi.softAP(APID, APPASS);
    IPAddress APIP = WiFi.softAPIP();
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    Serial.println("APIP er");
    Serial.println(APIP);
    server.on("/", handleRoot);
    server.on("/input", getInputs);
    server.begin();
    APStart = true;
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    if (!runOnceAP && !APStart) {
      runOnceAP = true;
      WiFi.mode(WIFI_AP_STA);
      Serial.println("Satt nettverksmodus til station access point");
      runOnceSTAT = false;
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
    setColor(0, 0, 255);
    dnsServer.processNextRequest();
    server.handleClient();
    delay(1);
  } else {
    if (!runOnceSTAT) {
      runOnceSTAT = true;
      APStart = false;
      WiFi.mode(WIFI_STA);
      Serial.println("Satt nettverksmodus til station");
      runOnceAP = false;

      dnsServer.stop();
      server.close();

    }
    setColor(255, 30, 0);
    digitalWrite(pinBarcodeTrigger, HIGH);
    if (mySerial.available()) {
      Serial.println("Serial lest");
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

}

void getInputs() {
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
    WiFi.begin(bufid, passid);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED && count < 60) {
      delay(500);
      Serial.println("connecting");
      count++;
    }
    if (WiFi.status() == WL_CONNECTED) {

      Serial.println("Connection successful, saving password to EEPROM after a wipe");
      for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
      }

      int address = 0;
      EEPROM.put(address, bufid);
      address = 100;
      EEPROM.put(address, passid);
      EEPROM.end();
      Serial.println("Restarting ESP");
      ESP.restart();
    } else {
      Serial.println("Could not connect to network");
      //server.send(200, "text/plain", "Did not world");
    }
  } // IF NOT THEN NO WIFINAME
}

void handleRoot() {
  //String html = "<!DOCTYPE HTML> <html> <head> </head> <body> <h1>Welcome to KOLOSVISJ</h1> </body> </html>";
String html ="<!DOCTYPE HTML> <html> <head> <title>Kolonial.no Svisj oppsett</title> <meta charset=\"UTF-8\"> <meta name=\"author\" content=\"Gruppe 10\"> <style> #overskrift{ max-width: 600px; margin: auto; } .content{ max-width: 420px; margin: auto; } p{ margin-bottom: 0px; } </style> </head> <body> <h1 id=\"overskrift\">Vennligst legg til internett navn og passord</h1> <div class=\"content\"> <table> <tr> <th><p>WIFI NAVN</p></th> <th><p>WIFI PASSORD</p></th> </tr> <tr> <th><input type=\"text\" name=\"WIFI\" id=\"WIFI\" autofocus></th> <th><input type=\"text\" name=\"PASSWORD\" id=\"PASSWORD\" ></th> <th><button id=\"THEBUTTON\">Send oppdatering</button></th> </tr> </table> </div> <p id=\"demo\"></p> <script> function sendData() { var wifiname = document.getElementById(\"WIFI\").value; var wifipassword = document.getElementById(\"PASSWORD\").value; if((wifiname.length < 32 && wifiname.length > 0) && (wifipassword.length < 64)){ var url = \"/input\"; var params = \"wifiname=\"+wifiname+\"&wifipass=\"+wifipassword; var xmlHTTP = new XMLHttpRequest(); xmlHTTP.open(\"GET\", url+\"?\"+params, true); /* xmlHTTP.onreadystatechange = function(){ if(xmlHTTP.readyState === 4 && xmlHTTP.status === 200){ document.getElementById(\"demo\").innerHTML = \"Changed the text\"; } } */ xmlHTTP.send(null); alert(\"Vi oppdaterer nå wifi på din Kolonial.no Svisj\"); }else{ alert(\"Navn kan ikke være lenger enn 31 symboler og passord kan ikke være lenger enn 63 symboler\"); } } document.getElementById(\"THEBUTTON\").onclick = sendData; </script> </body> </html>";


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
