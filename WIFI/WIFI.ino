#include <SoftwareSerial.h>
#include <string.h>
SoftwareSerial Wifi(7, 6);

String OpenCmd = "wake";
const int OpenPin = 16;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wifi.begin(115200);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(OpenPin, OUTPUT);
  digitalWrite(OpenPin, HIGH);
  Serial.println("System is OK！");
  delay(5000);
  Wifi.println("AT+RST");
  delay(5000);
  Wifi.println("AT+CWMODE=1");
  delay(3000);
  Wifi.println("AT+CIPMUX=0");
  delay(3000);
  Wifi.println("AT+CIPSTART=\"TCP\",\"119.28.180.170\",8888");
  delay(3000);
  Serial.println("Wifi is OK！");
  Wifi.println("AT+CIPSEND=15");
  delay(500);
  Wifi.println("WakeServerStart");
}

void wake() {
  digitalWrite(OpenPin, LOW);
  delay(500);
  digitalWrite(OpenPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
    if ( Serial.available() ) {
      String cmd;
      while( Serial.available() ) {
        cmd += (char)Serial.read();
        delay(5);
      }
      cmd.trim();
      Serial.println(cmd);
      delay(100);
      Wifi.println(cmd);
    }
    if ( Wifi.available() ) {
      String cmd;
      while( Wifi.available() ) {
          cmd += (char)Wifi.read();
          delay(5);  
      }
      cmd.trim();
      Serial.println(cmd);
      delay(100);
      if( strstr(cmd.c_str(), OpenCmd.c_str()) != NULL ) {
        Serial.println("yes");
        wake();
      }
    }
}
