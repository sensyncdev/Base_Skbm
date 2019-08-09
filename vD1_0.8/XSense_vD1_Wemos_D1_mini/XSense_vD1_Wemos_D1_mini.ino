#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#define LED_PIN 2   //D4
#define INT_PIN 5   //D1
#define RST_PIN 4  //D2
#define BUZZER_PIN 5 //D1
#define WIFI_STAT_PIN 16 //D0
#define BAUD_RATE 9600
#define RelayON     HIGH
#define RelayOFF    LOW
#define PRINT(x)    Serial.print(x);    ardSerial.print(x);
#define PRINTLN(x)  Serial.println(x);  ardSerial.println(x);

//-- ESP_TX pin 14/D5 -  15/D8
//-- ESP_RX pin 12/D6 -  13/D7
SoftwareSerial ardSerial(D3, D4);

/* Set these to your desired credentials. */
const char *ssid = "Get Sensync";  
const char *pswd = "makanminggu12";

//Web/Server address to read/write from 
const char *host = "http://sever.getsensync.com";   
const long interval = 1000;

//-- Server link
const String base_id      = "id=xx-klh-skbm&";
const String url_upload   = "http://server.getsensync.com/proc/klh_skbm/base/process.php?";
const String url_startup  = "http://server.getsensync.com/prtl/startup.php?id=sukabumi";  
const String url_reset    = "http://server.getsensync.com/prtl/reset.php?id=sukabumi";  
const String url_time     = "http://server.getsensync.com/publicsource/datetime/";

//String datana = "http://server.getsensync.com/proc/bplhd_jabar/base/process.php?id=xx-bplhd-jabar-crbn&o3a=0.23&o3a=0.05&coa=0.53&cob=0.21&no2a=0.34&no2b=0.32&so2a=0.49&so2b=0.49&pm1=25.60&pm25=35.20&pm10=66.30&temp=28.08&hum=64.57&press=88402.00&ws=0&wd=0&ds=26.50&so2a=0&so2b=0&h2sa=0&h2sb=0&voca=0&vocb=0&h2sa=0&h2sb=0";

byte ledState = LOW;
unsigned int count = 0;
unsigned long previousMillis = 0;

String server, url, inData;

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(INT_PIN, INPUT_PULLUP);
  digitalWrite(RST_PIN,RelayOFF);  
  pinMode(WIFI_STAT_PIN, OUTPUT); 

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN,LOW);
  delay(10);
  digitalWrite(BUZZER_PIN,HIGH);
  delay(100);  
  digitalWrite(BUZZER_PIN,LOW);
  delay(50);
  digitalWrite(BUZZER_PIN,HIGH);
  delay(100);  
  digitalWrite(BUZZER_PIN,LOW);
  delay(50);
  digitalWrite(BUZZER_PIN,HIGH);
  delay(100);  
  digitalWrite(BUZZER_PIN,LOW);
  delay(10);

  Serial.begin(9600);
  ardSerial.begin(9600);
  delay(1000);
  
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, pswd);     //Connect to your WiFi router
  Serial.println("\nWiFi Connecting");
  //-- Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(WIFI_STAT_PIN, LOW);
    Serial.print(".");
    //digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
    digitalWrite(LED_PIN, HIGH);
    delay(150);
    digitalWrite(LED_PIN, LOW);
    delay(150);
    digitalWrite(LED_PIN, HIGH);
    delay(150);
    digitalWrite(LED_PIN, LOW);
    delay(150);
  }
  //attachInterrupt(digitalPinToInterrupt(INT_PIN), IntCallback, RISING);
  Serial.println("\nConnected!");
  digitalWrite(WIFI_STAT_PIN, HIGH);
  //randomSeed(analogRead(0));
  // getStartUp();
  Serial.println();  
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  inData = ""; 
/*--fungsi lama
  while(Serial.available()){
    inData = Serial.readStringUntil('\n');
  }
  while(ardSerial.available()){
    inData = ardSerial.readStringUntil('\n');
  }
*/
  //fungsi dari pak beben
  inData = waitResponse2(5000);

  if(inData.indexOf(".com")!=-1)    uploadData();
  if(inData.indexOf("chkstat")!=-1) checkStatus();
  if(inData.indexOf("getIP")!=-1)   getIPaddrs();
  if(inData.indexOf("startup")!=-1) startUp();
  if(inData.indexOf("update")!=-1)  upDate();
  if(inData.indexOf("reset")!=-1)  resetSys();

  if(WiFi.status() == WL_CONNECTED) digitalWrite(WIFI_STAT_PIN, HIGH); else digitalWrite(WIFI_STAT_PIN, LOW);

  Blinks();
  if((count%(1*12)==0) && (count != 0)) { task1(); }
  // if((count%(15*60)==0) && (count != 0)) { task2(); }
  if(count>(60*60)) resetSys();
}

//=======================================================================

String waitResponse2(unsigned long timeout){
  bool quit = false;
  String seriesData;
  unsigned long waktuLama;
  Serial.print(F("\nWaiting data from Mega")); Serial.flush();
  waktuLama = millis();
  while ((!quit) && (millis() - waktuLama <= timeout)){
    if (millis() % 1000 == 0){
      Serial.print("."); Serial.flush();
      digitalWrite(LED_PIN,!(digitalRead(LED_PIN)));
      delay(1);
    }
    if (Serial.available() > 0){
      char x = Serial.read();
      if (x == '\n'){
        quit = true;
      } else {
        if (x != ' '){
          seriesData += x;
        }
      }
    }
    if (ardSerial.available() > 0){
      char x = ardSerial.read();
      if (x == '\n'){
        quit = true;
      } else {
        if (x != ' '){
          seriesData += x;
        }
      }
    }
  }
  return seriesData;
}


void task1(){
  Serial.println(F("\n-------- task1() begin"));
  requestTimeServer();
  // Serial.print("IP Addrs : ");
  // Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  getIPaddrs();
  Serial.print("IdleTimer: ");
  Serial.println(count);  
  checkStatus();  
  Serial.println(F("\n-------- task1() end"));
}

/*
void task2(){
  PRINTLN(F("start\n"));
  yield(); 
}
*/

void requestTimeServer(){
  Serial.println(F("\n-------- requestTimeServer() begin"));
  HTTPClient http;  //Declare object of class HTTPClient

  //-- GET Data
  http.begin(url_time); //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String timeServer = http.getString();    //Get the response payload

  Serial.println(timeServer);    //Print request response payload
  http.end();     //Close connection  
  Serial.println(F("\n-------- requestTimeServer() end"));
}

void checkStatus(){
  Serial.println(F("\n-------- checkStatus() begin"));
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin(url_reset);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
  payload.trim();

  PRINT(F("\nStatus  : "));
  PRINTLN(payload);    //Print request response payload
  yield();
  http.end();     //Close connection  

  if(payload.indexOf("reset")!=-1){ resetSys(); }
  delay(1000);
  digitalWrite(RST_PIN,RelayOFF);  
  Serial.println(F("\n-------- checkStatus() end"));
}

void uploadData(){
  Serial.println(F("\n-------- uploadData() begin"));
  count=0;
  requestTimeServer();
  HTTPClient http;  //Declare object of class HTTPClient

  http.begin(inData); //Specify request destination
  
  int httpCode    = http.GET(); //Send the request
  String payload  = http.getString(); //Get the response payload
  payload.trim(); //-- remove space
  
  //Serial.println(inData);
  Serial.print("HTTP return code: ");
  Serial.println(httpCode);   //Print HTTP return code
  PRINT("Response: ");
  PRINTLN(payload);    //Print request response payload
  yield();
  http.end();     //Close connection  
  //delay(2000);
  //startUp();   
  Serial.println(F("\n-------- uploadData() end"));
}

/*
void getStartUp(){
  String resp = startUp();
  if(resp.indexOf("ready")==-1) startUp();
}
*/

void startUp(){
  Serial.println(F("\n-------- startUp() begin"));
  count = 0;
  requestTimeServer();
  HTTPClient http;  //Declare object of class HTTPClient

  http.begin(url_startup); //Specify request destination
  Serial.print(F("Sending: ")); Serial.println(url_startup);
  
  int httpCode    = http.GET(); //Send the request
  Serial.print(F("HTTP return code: ")); Serial.println(httpCode);
  String payload  = http.getString(); //Get the response payload
  payload.trim();
  
  PRINT(F("Response: "));
  PRINTLN(payload);    //Print request response payload
  yield();
  http.end();     //Close connection
  Serial.println(F("\n-------- startUp() end"));
}

void upDate(){
  digitalWrite(BUZZER_PIN,LOW);
  delay(10);
  digitalWrite(BUZZER_PIN,HIGH);
  delay(700);  
  digitalWrite(BUZZER_PIN,LOW);
  delay(10);

  Serial.println(F("--- upDate() begin.")); Serial.flush();
/*---old method. giving respond when serial communication succeed
  //response for mega2560
  // ardSerial.println("ESPReceived");
  // Serial.println(F("I gave response (ESPReceived) to Mega2560"));
*/

/*---old
  datana = url_upload+base_id+
           "&o3a="+String(o3a/100)+"&o3b="+String(o3b/100)+
           "&coa="+String(coa/100)+"&cob="+String(cob/100)+
           "&no2a="+String(no2a/100)+"&no2b="+String(no2b/100)+
           "&so2a="+String(so2a/100)+"&so2b="+String(so2b/100)+
           "&pm1="+String(pm01/10)+"&pm25="+String(pm25/10)+"&pm10="+String(pm10/10)+
           "&temp="+String(temp/100)+"&hum="+String(hum/10)+"&press="+String(pres)+
           "&ws=0&wd=0&ds=26.50&so2a=0&so2b=0&h2sa=0&h2sb=0&voca=0&vocb=0&h2sa=0&h2sb=0";
*/

  count=0;
  HTTPClient http;  //Declare object of class HTTPClient
  inData.trim();
  http.begin(inData); //Specify request destination
  int httpCode    = http.GET(); //Send the request
  String payload  = http.getString(); //Get the response payload
    
  Serial.println(inData);  
  Serial.print("HTTP return code: ");
  Serial.println(httpCode);   //Print HTTP return code
  if (httpCode == 200) {
    digitalWrite(BUZZER_PIN,LOW);
    delay(10);
    digitalWrite(BUZZER_PIN,HIGH);
    delay(200);  
    digitalWrite(BUZZER_PIN,LOW);
    delay(50);
    digitalWrite(BUZZER_PIN,HIGH);
    delay(200);  
    digitalWrite(BUZZER_PIN,LOW);
    delay(10);
  }


  PRINT(F("Response: "));
  PRINTLN(payload);    //Print request response payload
  yield();
  http.end();     //Close connection
  Serial.println(F("--- upDate() end.")); Serial.flush();
}

void getIPaddrs(){
  Serial.print("Connected to ");
  Serial.println(ssid);
  PRINT(F("IP Address : "));
  PRINTLN(WiFi.localIP());  //IP address assigned to your ESP  
  PRINT(F("Signal RSSI: "));
  PRINTLN(WiFi.RSSI()); 
  Serial.println();  
  yield();
}

/*
void IntCallback(){
  Serial.println("Device is still running..");
  count=0;
  delay(2000);
}
*/

void resetSys(){
  Serial.println(F("-------- resetSys() begin"));
  count=0;
  delay(1000);
  digitalWrite(RST_PIN,RelayON);
  delay(2000);  
  digitalWrite(RST_PIN,RelayOFF);  
  Serial.println(F("-------- resetSys() end"));
  delay(1000);  
}

void Blinks(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;  // Note that this switches the LED *off*
    } else {
      ledState = LOW;  // Note that this switches the LED *on*
    }
    //digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
    digitalWrite(LED_PIN, ledState);
    count++;
  }  
}

/*
void Flipper(){
    digitalWrite(LED_PIN,LOW);
    delay(100);
    digitalWrite(LED_PIN,HIGH);
    delay(200);  
    digitalWrite(LED_PIN,LOW);
    delay(300);
    digitalWrite(LED_PIN,HIGH);
    delay(100);  
}
*/

/*
http://server.getsensync.com/proc/bplhd_jabar/base/process.php?
id=xx-bplhd-jabar-crbn
&o3a=0.23&o3a=0.04&coa=0.63&cob=0.18&no2a=0.33&no2b=0.30&so2a=0.55&so2b=0.53
&pm1=100.01&pm25=105.95&pm4=105.95&pm10=105.95
&temp=30.75&hum=47.52&press=92492.00&ws=0&wd=0&ds=28.44
&so2a=0&so2b=0&h2sa=0&h2sb=0&voca=0&vocb=0&h2sa=0&h2sb=0
*/
