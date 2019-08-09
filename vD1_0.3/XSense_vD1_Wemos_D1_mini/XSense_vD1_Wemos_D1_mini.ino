#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#define LED_PIN 2   //D4
#define INT_PIN 5   //D1
#define RST_PIN 16  //D0
#define BAUD_RATE 9600
#define RelayON     LOW
#define RelayOFF    HIGH
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
//const String base_id      = "id=xx-bplhd-jabar-crbn&";
//const String url_upload   = "http://server.getsensync.com/proc/bplhd_jabar/base/process.php?";  
const String url_startup  = "http://128.199.194.1/ganitri/pd/sensor?id=4O5Z56Xu&co=99&no2=99&o3=99&so2=99&pm1=99&pm25=99&pm4=99&pm10=99&temp=99&rhum=99&pres=99&ws=99&wd=99&uv=9999";  
const String url_reset    = "http://128.199.194.1/ganitri/pd/sensor?id=4O5Z56Xu&co=99&no2=99&o3=99&so2=99&pm1=99&pm25=99&pm4=99&pm10=99&temp=99&rhum=99&pres=99&ws=99&wd=99&uv=9999";  
const String url_time     = "http://128.199.194.1/ganitri/pd/sensor?id=4O5Z56Xu&co=99&no2=99&o3=99&so2=99&pm1=99&pm25=99&pm4=99&pm10=99&temp=99&rhum=99&pres=99&ws=99&wd=99&uv=99999";

String datana = "http://128.199.194.1/ganitri/pd/sensor?id=4O5Z56Xu&co=99&no2=99&o3=99&so2=99&pm1=99&pm25=99&pm4=99&pm10=99&temp=99&rhum=99&pres=99&ws=99&wd=99&uv=99";

int ledState = LOW;
int count = 0;
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
  //randomSeed(analogRead(0));
  // getStartUp();
  Serial.println();  
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  inData = ""; 
  // while(Serial.available()){
  //   inData = Serial.readStringUntil('\n');
  // }
  // while(ardSerial.available()){
  //   inData = ardSerial.readStringUntil('\n');
  // }

  //fungsi dari pak beben
  inData = waitResponse2(10000);

  // if(inData.indexOf(".com")!=-1)    UploadData();
  // if(inData.indexOf("chkstat")!=-1) checkStatuz();
  // if(inData.indexOf("startup")!=-1) startUp();
  if(inData.indexOf("getIP")!=-1)   getIPaddrs();
  if(inData.indexOf("ganitri")!=-1)  {
    // ardSerial.flush();
    upDate();
    ardSerial.flush();
  }

  Blinks();
  // if((count%(01*60)==0) && (count != 0)) { Task1(); }
  // if((count%(15*60)==0) && (count != 0)) { Task2(); }
  //if(count>(30*60)){ upDate(); }
}

//=======================================================================
String waitResponse2(long timeout){
  bool quit = false;
  String seriesData;
  unsigned long waktuLama;
  Serial.println(F("Waiting data from Mega"));
  waktuLama = millis();
  //ardSerial.listen();
  while ((!quit) && (millis() - waktuLama <= timeout)){
    if (millis() % 1000 == 0){
      Serial.print(".");
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


void Task1(){
  //requestTimeServer();
  Serial.print("IP Addrs : ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  Serial.print("IdleTimer: ");
  Serial.println(count);  
  //checkStatuz();  
  Serial.println();
}

void Task2(){
  PRINTLN(F("start\n"));
  // yield(); 
}

void requestTimeServer(){
  HTTPClient http;  //Declare object of class HTTPClient

  //-- GET Data
  http.begin(url_time); //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String timeServer = http.getString();    //Get the response payload

  Serial.println(timeServer);    //Print request response payload
  http.end();     //Close connection  
}

void checkStatuz(){
  //"http://secure.getsensync.com/prtl/reset.php?id=cirebon";  
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin(url_reset);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload

  PRINT(F("Status  : "));
  PRINTLN(payload);    //Print request response payload
  yield();
  http.end();     //Close connection  

  if(payload.indexOf("reset")!=-1){ resetSys(); }
  delay(1000);
  digitalWrite(RST_PIN,RelayOFF);  
}

void UploadData(){
  count=0;
  requestTimeServer();
  HTTPClient http;  //Declare object of class HTTPClient

  http.begin( inData ); //Specify request destination
  
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
}

void getStartUp(){
  String resp = startUp();
  if(resp.indexOf("ready")==-1) startUp();
}

String startUp(){
  requestTimeServer();
  HTTPClient http;  //Declare object of class HTTPClient

  http.begin( url_startup ); //Specify request destination
  
  int httpCode    = http.GET(); //Send the request
  Serial.println(httpCode);
  String payload  = http.getString(); //Get the response payload
  
  PRINT(F("Startup... "));
  PRINTLN(payload);    //Print request response payload
  yield();
  http.end();     //Close connection
  return payload;  
}

void upDate(){
  //response for mega2560
  ardSerial.println("ESPReceived");
  Serial.println(F("I gave response (ESPReceived) to Mega2560"));

  // datana = url_upload+base_id+
  //          "&o3a="+String(o3a/100)+"&o3b="+String(o3b/100)+
  //          "&coa="+String(coa/100)+"&cob="+String(cob/100)+
  //          "&no2a="+String(no2a/100)+"&no2b="+String(no2b/100)+
  //          "&so2a="+String(so2a/100)+"&so2b="+String(so2b/100)+
  //          "&pm1="+String(pm01/10)+"&pm25="+String(pm25/10)+"&pm10="+String(pm10/10)+
  //          "&temp="+String(temp/100)+"&hum="+String(hum/10)+"&press="+String(pres)+
  //          "&ws=0&wd=0&ds=26.50&so2a=0&so2b=0&h2sa=0&h2sb=0&voca=0&vocb=0&h2sa=0&h2sb=0";
  
  // count=0;
  // startUp(); 
  HTTPClient http;  //Declare object of class HTTPClient
  inData.trim();
  http.begin( inData ); //Specify request destination
  int httpCode    = http.GET(); //Send the request
  String payload  = http.getString(); //Get the response payload
    
  Serial.println(inData);  
  Serial.print("HTTP return code: ");
  Serial.println(httpCode);   //Print HTTP return code
  // PRINT(F("Response: "));
  // PRINTLN(payload);    //Print request response payload
  // yield();
  http.end();     //Close connection
}

void getIPaddrs(){
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  PRINT(F("IP Address : "));
  PRINTLN(WiFi.localIP());  //IP address assigned to your ESP  
  PRINT(F("Signal RSSI: "));
  PRINTLN(WiFi.RSSI()); 
  Serial.println();  
  yield();
}

void IntCallback(){
  Serial.println("Device is still running..");
  count=0;
  delay(2000);
}

void resetSys(){
  count=0;
  Serial.println("System Reset...!");
  delay(1000);
  digitalWrite(RST_PIN,RelayON);
  delay(5000);  
  digitalWrite(RST_PIN,RelayOFF);  
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
  if(count>(60*60)){ resetSys(); }
}

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

/*
http://server.getsensync.com/proc/bplhd_jabar/base/process.php?
id=xx-bplhd-jabar-crbn
&o3a=0.23&o3a=0.04&coa=0.63&cob=0.18&no2a=0.33&no2b=0.30&so2a=0.55&so2b=0.53
&pm1=100.01&pm25=105.95&pm4=105.95&pm10=105.95
&temp=30.75&hum=47.52&press=92492.00&ws=0&wd=0&ds=28.44
&so2a=0&so2b=0&h2sa=0&h2sb=0&voca=0&vocb=0&h2sa=0&h2sb=0
*/
