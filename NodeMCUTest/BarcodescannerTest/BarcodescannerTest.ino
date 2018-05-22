#include <SoftwareSerial.h>

const int pinRX = 0;
const int pinTX = 1;
const int pinBarcodeTrigger = 8;

String barcodeValue;
String readString;
int x = 1;

SoftwareSerial mySerial(0, 1); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(pinBarcodeTrigger, OUTPUT);
//  byte message[] = {0x0B, 0x04, 0x31, 0x00, 0x00, 0x41, 0x30, 0x30, 0x30, 0x30, 0xFF, 0xFD, 0xC0};
//  mySerial.write(message, sizeof(message));
}

void loop() {
  Serial.println("Ferdig med setup");

  digitalWrite(pinBarcodeTrigger, HIGH);
  delay(20);
  digitalWrite(pinBarcodeTrigger, LOW);
  if (Serial.available()) {
    Serial.println("Serial er available");
    Serial.println((char)mySerial.read());
    //byte x = mySerial.read();
    //Serial.println(x, DEC);
  }
  delay(1000);
//  
//  if (x == 1) { //run the loop once
//
//    byte message[] = {0x0A, 0x04, 0x31, 0x00, 0x00, 0x59, 0x4c, 0x54, 0x4b, 0xFF, 0xFD, 0x7E};
//    mySerial.write(message, sizeof(message));
//    Serial.println("sendt message");
//    for (int i = 0; i < 40; i++) {
//      Serial.print(i);
//      Serial.println(" venter på serial");
//      while (!mySerial.available()); // wait for a character
//      
//      char incomingByte = mySerial.read();
//      Serial.print(incomingByte);
//      Serial.print(' ');
//    }
//    
//    Serial.println("ferdig");
//    x = 0;
//  }
}
