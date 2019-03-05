#include <SoftwareSerial.h>
#include <string.h>
#include <Metro.h>
SoftwareSerial Wifi(7, 6);

String OpenCmd = "wake";
const int OpenPin = 16;

Metro CheckLinkStatus = Metro(60000); // 周期 1分钟
Metro GetWifiCmd = Metro(0); // 周期 0

void initEsp8266();
void wake();
void connectToWifi();
void loginToServer();
bool tcpStatus();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wifi.begin(9600);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(OpenPin, OUTPUT);
  digitalWrite(OpenPin, HIGH);
  Serial.println("System is OK！");
  initEsp8266();
}

void wake() {
  digitalWrite(OpenPin, LOW);
  delay(500);
  digitalWrite(OpenPin, HIGH);
}

void initEsp8266() {
  Serial.println("Send AT+RST！");
  Wifi.println("AT+RST");
  Serial.println("Send AT+UART_DEF=9600,8,1,0,0！");
  Wifi.println("AT+UART_DEF=9600,8,1,0,0");
  delay(5000);
  Serial.println("Send AT+CWMODE=1！");
  Wifi.println("AT+CWMODE=1");
  delay(1000);
  Serial.println("Send AT+CIPMUX=0！");
  Wifi.println("AT+CIPMUX=0");
  delay(1000);
  connectToWifi();
  Serial.println("Wifi is OK！");
}

void connectToWifi() {
  Serial.println("Disconnect From Wifi! ");
  Wifi.println("AT+CWQAP");
  delay(5000);
  Serial.println("Connect To Wifi Xiaomi_A077 ! ");
  Wifi.println("AT+CWJAP=\"Xiaomi_A077\",\"xiaomiwifi\"");
  delay(5000);
}

void loginToServer() {
  Serial.println("Disconnected From Server ! ");
  Wifi.println("AT+CIPCLOSE");
  delay(1000);
  Serial.println("Connected To Server ! ");
  Wifi.println("AT+CIPSTART=\"TCP\",\"119.28.180.170\",5880");
  delay(5000);
}

bool tcpStatus() {
  Serial.println("Send AT+CIPSTATUS ! ");
  Wifi.println("AT+CIPSTATUS");
  String result;
  while(!Wifi.available());
  while(Wifi.available()) {
    result += (char)Wifi.read();
    delay(5); 
  }
  result.trim();
  if (strstr(result.c_str(), "STATUS:3") != NULL) {
    return true;
  }
  return false;
}

void CheckStatus() {
  Serial.println("Check Tcp Link Status ! ");
  if (!tcpStatus()) {
      loginToServer();
    }
}

void GetCmd() {
  if ( Wifi.available() ) {
      String cmd;
      while( Wifi.available() ) {
          cmd += (char)Wifi.read();
          delay(5);  
      }
      cmd.trim();
      delay(100);
      if( strstr(cmd.c_str(), OpenCmd.c_str()) != NULL ) {
        wake();
      }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( CheckLinkStatus.check() )
       CheckStatus();
  if ( GetWifiCmd.check() )
       GetCmd();
}
