#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

String SSIDName;
String SSIDPassword;

const char *APID = "koloPUBGPROELITE";
const char *APPASS = "";

const byte DNS_PORT = 53;

bool wificonnected;

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

  //TODO READ FROM EEPROM TO CHECK IF THERE IS A WIFINAME AND PASSWORD
  wificonnected = false;



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

  
  String ssid = "test";
  String password = "smaltansikt";

  int address = 0;
  char arrayToStore[64];
  char arrayThatWasStored[64];
  ssid.toCharArray(arrayToStore, ssid.length()+1);
  Serial.print("Writing this to EEPROM : ");

  Serial.println(arrayToStore);
  
  EEPROM.put(address, arrayToStore);
  EEPROM.get(address, arrayThatWasStored);
  Serial.print("Got this from EEPROM : ");

  Serial.println(arrayThatWasStored);


  address = 32;
  password.toCharArray(arrayToStore, password.length()+1);
  Serial.print("Writing this to EEPROM : ");
  Serial.println(arrayToStore);

  
  EEPROM.put(address, arrayToStore);
  EEPROM.get(address, arrayThatWasStored);
  Serial.print("Got this from EEPROM : ");

    Serial.println(arrayThatWasStored);


  
  //writeEEPROM(0, ssid);
  Serial.println();
  //writeEEPROM(32, password);
  Serial.println();
  Serial.println("Wrote to EEPROM, let's check if it actually did anything");

  //SSIDName = readEEPROM(0,32);
  //SSIDPassword = readEEPROM(32, 64);

  Serial.println();
  //Serial.println(SSIDName);
  //Serial.println(SSIDPassword);

  
/*
  String winame = readEEPROM(0, 32);
  Serial.println("We got this from EEPROM as name:");
  Serial.println(winame);


  String wipassword = readEEPROM(32, 64);
  Serial.println("We got this from EEPROM as password:");
  Serial.println(wipassword);
*/
}

void writeEEPROM(int startOfMemoryToAppend, String stringToAppend) {


  int i = 0;
  i += startOfMemoryToAppend;
  for (i; i < (stringToAppend.length() + startOfMemoryToAppend) ; i++) {
    EEPROM.write(i + startOfMemoryToAppend, stringToAppend[i]);
    Serial.print(stringToAppend[i]);
  }
  EEPROM.commit();
  delay(1000);

}


String readEEPROM(int startOfMemoryToRead, int lengthToRead) {
  //char *stringyasfuck = (char*)malloc (sizeof(char) * lengthToRead);
  //char buf[lengthToRead];
  String string;
  int i = 0;
  for (i; i < lengthToRead ; i++) {

    string[i] += char(EEPROM.read(i + startOfMemoryToRead));
    Serial.print(string[i]);
    Serial.print("<");

  }


  return string;
}


void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  delay(1);
}

void getInputs() {
  //Hvis denne siden blir lastet betyr det at noen enten har gått inn på 192.168.4.1/input på egenhånd
  //Eller så har data blitt submittet
  //Må i så fall hente data fra siden
  Serial.println("We are outside of if statement");
  /*
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
      ssid.toCharArray(bufid, sizeof(ssid));
      password.toCharArray(passid, sizeof(password));

      writeEEPROMchar(0, bufid);
      writeEEPROMchar(32, passid);
      Serial.println("Wrote to EEPROM, let's check if it actually did anything");
      //char *winame = readEEPROM(0, 32);
      Serial.println("We got this from EEPROM as name:");
      Serial.println(winame);

      free(winame);

      //char *wipassword = readEEPROM(32, 64);

      Serial.println("We got this from EEPROM as password:");
      Serial.println(wipassword);
      free(wipassword);


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
              char bufid[32];
              char passid[64];
              ssid.toCharArray(bufid, sizeof(ssid));
              password.toCharArray(passid, sizeof(password));
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
                //Dette funker ikke, hvorfor fungerer det ikke?
                //readEEPROM returnerer en char pointer som peker til adressen til char *string deklarert inni readEEPROM
                //når jeg kjører først én readEEPROM og så bruker dataen til å skrive ut via serial print går det fint
                //Men hvis man kjører readEEPROM to ganger på rad, uten å skrive ut vil den første variablen som returneres
                //overskrives av den neste readEEPROM fordi de begge peker til samme adresser.
                //Med andre ord kan den ikke returnere en char* ELLER så må man bare bruke dataene før man leser neste.
                writeEEPROMchar(0, bufid);
                writeEEPROMchar(32, passid);
                Serial.println("Wrote to EEPROM, let's check if it actually did anything");
                char *winame = readEEPROM(0, 32);

                Serial.println("We got this from EEPROM as name:");
                Serial.println(winame);
                char *wipassword = readEEPROM(32, 64);

                Serial.println("We got this from EEPROM as password:");
                Serial.println(wipassword);

                Serial.println("BREAKING");
              }

            } else {
              Serial.println("You need to provide a password");
            }
          }

        } else {
          Serial.println("Not the right network");
        }

        Serial.println();
        Serial.println();
        }
        if (!wificonnected) Serial.println("Could not find the network");


    }
  */

  Serial.println("Inputs updated");
}

void handleRoot() {
  //String html = "<!DOCTYPE HTML> <html> <head> </head> <body> <h1>Welcome to KOLOSVISJ</h1> </body> </html>";
  String html = "<!DOCTYPE HTML><html><head> <title>KOLONIAL SVISJ SETUP</title> <meta charset=\"UTF-8\"> <meta name=\"author\" content=\"Gruppe 10\"> </head><body> <h1>WIFI AND PASSWORD</h1> <input type=\"text\" name=\"WIFI\" id=\"WIFI\"> <input type=\"text\" name=\"PASSWORD\" id=\"PASSWORD\"> <button onclick=\"submit()\">Submit</button> <p id=\"demo\"></p> <script>function submit() { var wifiname = document.getElementById(\"WIFI\").value; var wifipassword = document.getElementById(\"PASSWORD\").value; var url = \"/input\"; var params = \"wifiname=\"+wifiname+\"&wifipass=\"+wifipassword; var xmlHTTP = new XMLHttpRequest(); xmlHTTP.open(\"GET\", url+\"?\"+params, true); xmlHTTP.send(null);}</script> </body></html>";
  server.send(200, "text/html", html);

  Serial.println("Root page shown");
}

/*
void writeEEPROMchar(int startOfMemoryToAppend, char * stringToAppend, int lengde) {
  Serial.println("We are inside write function and this is what we are writing:");

  Serial.println(stringToAppend);
  int len = strlen(stringToAppend);
  Serial.println(len);
  int i = 0;
  for (i; i < len ; i++) {
    EEPROM.write(i + startOfMemoryToAppend, stringToAppend[i]);
    Serial.print(stringToAppend[i]);
    Serial.print(" ");

  }
  Serial.println(i);
  //int y = (len > 0) ? 1 : 0;

  EEPROM.write(i + 1, '\0');

  EEPROM.commit();
  delay(2000);
  Serial.println();
}
*/


/*
  char* readEEPROM(int startOfMemoryToRead, int lengthToRead) {
  char * string = "Go fack yerself u whore";
  char string2[24];
  char string6[24];
  char string3[] = "Go fack yerself u whore";
  char *string4 = strdup(string);
  Serial.println("String size:");
  Serial.println(sizeof(string    ));
  Serial.println("String2 size:");
  Serial.println(sizeof(string2    ));
  Serial.println("String3 size:");
  Serial.println(sizeof(string3    ));
  Serial.println("String4 size:");
  Serial.println(sizeof(string4    ));

  string2[0] = 'f';
  string2[1] = 'u';
  string2[2] = 'c';
  string2[3] = 'k';
  string2[4] = 'y';
  string2[5] = '\0';
  string2[6] = 'y';

  char * string5 = strdup(string2);
  Serial.println("String2 size after fuck0");
  Serial.println(sizeof(string2));
  Serial.println("String 5 size:");
  Serial.println(sizeof(string5));

  Serial.println("Length of string2 that has fixed size, but has fuck with zero termination");
  Serial.println(strlen(string2));

  Serial.println("Length of string6 that has fixed size");
  Serial.println(strlen(string6));

  Serial.println("Length of string3 that has a string");
  Serial.println(strlen(string3));

  Serial.println();

  free(string4);
  free(string5);
  //char buf[64];
  int i = 0;
  string[0] = 'F';
  string[1] = 'U';
  string[2] = 'C';
  string[3] = 'K';
  string[4] = '\0';
  while(string[i] != '\0'){
    Serial.println(string[i]);
    i++;
  }
  /*
    for (i; i < lengthToRead ; i++) {
    //string[i] = char(EEPROM.read(i + startOfMemoryToRead));
    //if (string[i] == '\0') break;

      if (buf[i] == '\0') {
      Serial.println("We found a null terminating zero babyyy");
      break;
      } else {
      buf[i] = char(EEPROM.read(i + startOfMemoryToRead));
      Serial.print(buf[i]);
      Serial.print("_");

      }

    }
*/
/*
  if (string != NULL) {
  buf[++i] = '\0';
  int index = 0;
  while (buf[index] != '\0') {
    string[index] = buf[index];
    Serial.print(string[index]);
    Serial.print(":");
    index++;
  }
  }

  Serial.println();
  return string;
  }
*/

