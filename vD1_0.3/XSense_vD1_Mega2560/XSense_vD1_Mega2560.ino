/////////////////////////////////////////////////////////////////////////////
/*
 * ----- XSense vD1 -----
 * BOARD  : ARDUINO MEGA 2560 Pro
 * COMM   : ESP8266 as Modem
 * SERVER : Sensync
 * Particulate Sensor : Sensirion SPS30
 * Gas Sensor : DGS SPEC
 */
/////////////////////////////////////////////////////////////////////////////

#include "Config.h"

void setup() {
  Serial.begin(9600);
  Serial.println(F("--------setup begin--------"));

  //---TFT setup
  Serial.println(F("TFT initialization"));
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on
  tft.initR(INITR_HALLOWING);   
  tft.initR(INITR_BLACKTAB);    
  tft.setRotation(1);

  //---TFT first display
  Serial.println(F("TFT first display start.."));
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(3);
  tft.setCursor(29, 30);
  tft.println("XSENSE");
  // tft.setCursor(45, 50);
  // tft.println("BASE");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(35, 60);
  tft.setTextSize(2);
  tft.println("SUKABUMI");
  delay(3000);

//----set pin mode  
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(PUMP_RST, OUTPUT);
//  pinMode(ESP_RST, OUTPUT);
//  pinMode(ESP_INT, OUTPUT);
  pinMode(SYS_RST, OUTPUT);
  pinMode(BUZZER, OUTPUT);
//  pinMode(BME_EN, OUTPUT);

//--DGS SPEC
  Serial1.begin(9600);
  for (int i=28;i<=31;i++) { //set pin mode for all EN_SPEC pin
    pinMode(i, OUTPUT); 
  }

  digitalWrite(PUMP_PIN,PumpIsOff);
  digitalWrite(PUMP_RST,LOW);
//  digitalWrite(ESP_RST,HIGH);
//  digitalWrite(ESP_INT,HIGH);
  digitalWrite(SYS_RST,HIGH);
  digitalWrite(BUZZER,HIGH);
//  digitalWrite(BME_EN,LOW);

//---Serial3 for modem/ESP8266
  Serial3.begin(9600);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.setCursor(10, 90);
  tft.println("Preparing first sample..");

//---sampling for first display
  get_sps30_vD();
  get_bme280_vD();
  get_4dgsspec_v2();
  read_time_rtcds3231();

  tft.fillScreen(ST77XX_BLACK);
  Serial.println(F("TFT first display finished."));

  Serial.println(F("--------setup end--------\n"));
}

void loop() {
  Serial.println(F("--------loop begin--------"));

//--TFT main display
  Serial.println(F("TFT main display start.."));
  tft.fillScreen(ST77XX_BLACK);
  displayTime();
  displayStat("Sampling...");
  displayTempHumPres();
  displayGasVal();
  displayPMmeter();
  displayWind();

//--sampling data
  read_time_rtcds3231();
  get_sps30_vD();
  get_bme280_vD();
  get_4dgsspec_v2();

//--displaying data
  tft.fillScreen(ST77XX_BLACK);
  displayTime();
  displayTempHumPres();
  displayGasVal();
  displayPMmeter();
  displayWind();

//--uploading data
  displayStat("Upload...");
  sendToModem();
 // delay(1000);
  // sendToModem();
  // displayData();
  displayStat("Done");

  delay(1000);
  NextLoop();

  Serial.println(F("--------loop end--------\n\n\n"));
  delay(1000);
}

void sendToModem() {
  Serial3.flush();
  delay(500);
  String combinedData = "http://128.199.194.1/ganitri/pd/sensor?id=4O5Z56Xu&co=" + String(currentData.ppbCO) + "&no2=" + String(currentData.ppbNO2) + "&o3=" + String(currentData.ppbO3)
                        + "&so2=" + String(currentData.ppbSO2) + "&pm1=" + String(currentData.pm1) + "&pm25=" + String(currentData.pm25) + "&pm4=" + String(currentData.pm4) + "&pm10=" + String(currentData.pm10) + "&temp="
                        + String(currentData.temp) + "&rhum=" + String(currentData.rhum) + "&pres=" + String(currentData.pressure) + "&ws=" + String(currentData.windSpeed) + "&wd=" + String(currentData.windDirection) + "&uv=" + String(currentData.irradianceUV);

  Serial.println(F("Sending (")); Serial.println(combinedData); Serial.println(F(") to modem.")); Serial.flush();
  Serial3.println(combinedData);
  Serial3.flush();

  /*
  //---cek respon serial esp

  bool isSerialESPResponse = false;
  String responseSerialESP = "";
  bool isESPReceive = false;
  unsigned long timeSendingToESP = millis();
  unsigned long trialTime = millis();

  while ((!isESPReceive) && (millis() - timeSendingToESP <= 10000)) {
    while ((!isSerialESPResponse) && (millis() - trialTime <= 3000)){
      if ((millis() % 1000 > 5) && (millis() % 1000 < 100)){
        Serial.println("p");
        Serial3.println(combinedData);
        Serial3.flush();
      }
      if (Serial3.available() > 0){
        char x = Serial3.read();
        if (x == '\n'){
          isSerialESPResponse = true;
        } else {
          if (x != ' '){
            responseSerialESP += x;
          }
        }
      }
    }

    if(!isSerialESPResponse) {
      Serial.println(F("No response from ESP."));
      //clear the buffer
      responseSerialESP = "";
    } else {
      responseSerialESP.trim();
      Serial.print(F("Response from ESP :")); Serial.println(responseSerialESP);
    }

    if(responseSerialESP == "ESPReceived") {
      isESPReceive = true;
    } else {
      Serial.print(F("Response from ESP invalid."));
      //clear the buffer
      responseSerialESP = "";
    }
    trialTime = millis();
  }

  if(!isESPReceive) {
    Serial.print(F("\nTransmit failed :(\nThis string ("));Serial.print(combinedData);
    Serial.println(F("sent by Mega2560 but ESP failed to receive the data after 10s."));
  } else {
    Serial.print(F("\nTransmit success!\nThis string ("));Serial.print(combinedData);
    Serial.println(F(") have transmitted by Mega2560 and ESP have received it."));
  }
  */

}

void NextLoop(){
  tft.fillRoundRect(0, 113, tft.width(), 123, 10, ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 115);  tft.println("Next loop in ");
  for(int nxtloop=10; nxtloop>0; nxtloop--){
    tft.fillRoundRect(79, 113, tft.width(), 123, 10, ST77XX_BLACK);
    tft.setCursor(83,115);  tft.print(nxtloop); tft.println(" s");
    Serial.print(F("Next loop in "));  Serial.println(nxtloop);
    if(nxtloop%20==0){read_time_rtcds3231(); displayTime();}
    delay(1000);
  }
}