#include "ESP8266.h"
#include <MsTimer2.h>

const char *SSID     = "Xiaomi_A077";
const char *PASSWORD = "xiaomiwifi";

SoftwareSerial mySerial(7, 6);
ESP8266 wifi(mySerial); 
unsigned long task1time = 0;
unsigned long task2time = 0;
const int OpenPin = 16;

void(* resetFunc) (void) = 0;

void wake() {
  digitalWrite(OpenPin, LOW);
  delay(500);
  digitalWrite(OpenPin, HIGH);
}

void task1(){
  String IPStatus = wifi.getIPStatus();
  if( IPStatus.indexOf("119.28.180.170") == -1) {
    resetFunc();
  } else {
    wifi.sendSingle("Alive!");
  }
}

void task2(){
  uint8_t RecvBuffer[200] = {0};
  if (wifi.recvSingle(RecvBuffer, 200) > 0) {
    String text = (char*)RecvBuffer;
    text.trim();
    Serial.println(text);
    if ( text.indexOf("wake") != -1 )
    {
      wake();  
    }
  }
}

void setup(void)
{
  //Start Serial Monitor at any BaudRate
  Serial.begin(57600);
  Serial.println("System Begin");
  pinMode(OpenPin, OUTPUT);
  digitalWrite(OpenPin, HIGH);
  wifi.autoSetBaud();
  wifi.setOprToStation();
  while (!wifi.joinAP(SSID, PASSWORD))
  {
    Serial.println("Wifi Join failed. Check configuration.");
  }
  wifi.disableMUX();
  while (!wifi.createTCP("119.28.180.170", 5880))
  {
    Serial.println("TCP Connect failed. Check configuration.");
  }
}


void loop(void)
{  
  unsigned long nowtime = millis();
  if(nowtime - task1time > 60000)
  {
    task1();
    task1time = nowtime;
  }
  if(nowtime - task2time > 1000)
  {
    task2();
    task2time = nowtime;
  }
}
