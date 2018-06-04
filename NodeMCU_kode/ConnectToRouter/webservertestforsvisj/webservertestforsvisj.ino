#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>


String SSIDName;
String SSIDPassword;

const char *APID = "kolonial-svisj-setup";
const char *APPASS = "";

const byte DNS_PORT = 53;

//IPAddress ip(192,168,4,1);
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);
//IPAddress dns(192,168,1,1);


DNSServer dnsServer;
ESP8266WebServer server(80);
//TODO REMOVE EVERY STRING OBJECT AND REPLACE WITH C STRING 
void setup() {
  Serial.begin(9600);
  //WRITE TO OR READ FROM EEPROM
  EEPROM.begin(1024);
  
  //WiFi.mode(WIFI_AP);
  //WiFi.config(ip, gateway, subnet, dns);
  WiFi.softAP(APID, APPASS);
  

  IPAddress APIP = WiFi.softAPIP();
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  
  Serial.println("APIP er");
  Serial.println(APIP);
  server.on("/", handleRoot);
  server.on("/input", getInputs);
  server.begin();


}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  
}

void getInputs(){
  //Hvis denne siden blir lastet betyr det at noen enten har gått inn på 192.168.4.1/input på egenhånd
  //Eller så har data blitt submittet
  //Må i så fall hente data fra siden
  if(server.arg("wifiname")!= ""){
    String ssid = server.arg("wifiname");
    String password = "";
    Serial.print("Wifi is called ");
    Serial.println(ssid);
    Serial.println("Checking if there was a password input");
    if(server.arg("wifipass") != ""){
    password = server.arg("wifipass");
    Serial.print("There is a password and it is: ");
    Serial.println(password);
  }
    //Lager en liste over nettverk
    int networks = WiFi.scanNetworks();
    bool wififound = false;
    for(int i = 0; i< networks; i++){
      Serial.print("Name of network : ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Strength : ");
      Serial.println(WiFi.RSSI(i));
      //Passord eller ikke
      Serial.print("Encrypted with : ");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      bool encrypted = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? false : true;
      if(ssid == WiFi.SSID(i)){
        wififound = true;
        Serial.println("NETWORK FOUND");
        if(encrypted){
          Serial.println("Password needed");
          if(password != ""){
            char bufid[32];
            char passid[64];
            ssid.toCharArray(bufid, sizeof(ssid)); 
            password.toCharArray(passid, sizeof(password)); 
            WiFi.begin(bufid, passid);
            int count = 0;
            while(WiFi.status() != WL_CONNECTED && count < 20){
              delay(500);
              Serial.println("connecting");
              count++;
            }
            if(WiFi.status() == WL_CONNECTED){
              Serial.println("Connection successful, saving password to EEPROM");
              //Dette funker ikke, hvorfor fungerer det ikke? 
              //readEEPROM returnerer en char pointer som peker til adressen til char *string deklarert inni readEEPROM
              //når jeg kjører først én readEEPROM og så bruker dataen til å skrive ut via serial print går det fint
              //Men hvis man kjører readEEPROM to ganger på rad, uten å skrive ut vil den første variablen som returneres 
              //overskrives av den neste readEEPROM fordi de begge peker til samme adresser. 
              //Med andre ord kan den ikke returnere en char* ELLER så må man bare bruke dataene før man leser neste.
              writeEEPROMchar(0, bufid);
              writeEEPROMchar(32, passid);
              Serial.println("Wrote to EEPROM, let's check if it actually did anything");
              char *winame = readEEPROM(0, strlen(bufid));
              char *wipassword = readEEPROM(32, strlen(passid));
              Serial.println("We got this from EEPROM as name:");
              Serial.println(winame);
              
              
              Serial.println("We got this from EEPROM as password:");
              Serial.println(wipassword);
            }
            
          }else{
            Serial.println("You need to provide a password");
          }
        }
        break;
      }else{
        Serial.println("Not the right network");
      }
      Serial.println();
      Serial.println();
    }
    if(!wififound) Serial.println("Could not find the network");

    
    
  }


  Serial.println("Inputs updated");
}

void handleRoot(){
  //String html = "<!DOCTYPE HTML> <html> <head> </head> <body> <h1>Welcome to KOLOSVISJ</h1> </body> </html>";
  String html = "<!DOCTYPE HTML><html><head> <title>KOLONIAL SVISJ SETUP</title> <meta charset=\"UTF-8\"> <meta name=\"author\" content=\"Gruppe 10\"> </head><body> <h1>WIFI AND PASSWORD</h1> <input type=\"text\" name=\"WIFI\" id=\"WIFI\"> <input type=\"text\" name=\"PASSWORD\" id=\"PASSWORD\"> <button onclick=\"submit()\">Submit</button> <p id=\"demo\"></p> <script>function submit() { var wifiname = document.getElementById(\"WIFI\").value; var wifipassword = document.getElementById(\"PASSWORD\").value; var url = \"/input\"; var params = \"wifiname=\"+wifiname+\"&wifipass=\"+wifipassword; var xmlHTTP = new XMLHttpRequest(); xmlHTTP.open(\"GET\", url+\"?\"+params, true); xmlHTTP.send(null);}</script> </body></html>";
  server.send(200, "text/html", html);

  Serial.println("Root page shown");
}

bool writeEEPROMchar(int startOfMemoryToAppend, const char *stringToAppend){
  int len = strlen(stringToAppend);
  for(int i = 0; i < len ;i++){
    EEPROM.write(i + startOfMemoryToAppend, stringToAppend[i]);
    Serial.print(stringToAppend[i]);
    Serial.print(" ");
  }
  Serial.println();
}

bool writeEEPROM(int startOfMemoryToAppend, String stringToAppend){
  //int lengthOfString = stringToAppend.length();
  char buf[sizeof(stringToAppend)];
  stringToAppend.toCharArray(buf, sizeof(stringToAppend));
  
  if(stringToAppend.length() <= 0) return false;
  //int beginning = startOfMemoryToAppend
  for(int i = 0; i < sizeof(stringToAppend) ;i++){
    EEPROM.write(i + startOfMemoryToAppend, buf[i]);
    Serial.print(buf[i]);
  }
  Serial.println();
}

char* readEEPROM(int startOfMemoryToRead, int lengthToRead){
    char *string = "";
    char buf[64];
    for(int i = 0; i < lengthToRead ;i++){
      buf[i] = char(EEPROM.read(i + startOfMemoryToRead));
      Serial.print(buf[i]);
      Serial.print("_");
  }
  buf[lengthToRead] = '\0';
  int index = 0;
  while(index <= strlen(buf)){
    string[index] = buf[index];
    Serial.print(string[index]);
    Serial.print(":");
    index++;
  }
  Serial.println();
  return string;
}


