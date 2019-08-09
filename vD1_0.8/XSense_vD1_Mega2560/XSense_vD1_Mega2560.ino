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

struct templateData {
  float ppbCO;
  float ppbNO2;
  float ppbO3;
  float ppbSO2;
  float pm1;
  float pm25;
  float pm4;
  float pm10;
  float temp;
  float rhum;
  float pressure;
  float windSpeed;
  unsigned int windDirection;
  String windDirectionCompass;
  unsigned int irradianceUV;
  byte tanggal;
  byte bulan;
};
templateData currentData;

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
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(3);
  tft.setCursor(45, 30);
  tft.println("BASE");
  tft.setTextColor(ST77XX_BLUE);
  tft.setCursor(35, 60);
  tft.setTextSize(2);
  tft.println("SUKABUMI");
  delay(1000);

  tft.fillRoundRect(0, 85, tft.width(), 20, 0, ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.setCursor(7, 90);
  tft.println("Initializing connection..");
  delay(1000);

//----set pin mode  
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(SYS_RST, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(PUMP_PIN,PumpIsOff);
  digitalWrite(SYS_RST,HIGH);
  digitalWrite(BUZZER,HIGH);
  pinMode(AnalogNoiseSensor_PIN, INPUT);
  pinMode(isWifiConnect_PIN,INPUT); //-- isWifiConnect_PIN
  if(digitalRead(isWifiConnect_PIN) == HIGH) isWifiConnect = true; else isWifiConnect = false;

//--Serial1 for DGS SPEC
  Serial1.begin(9600);
  for (int i=28;i<=31;i++) pinMode(i, OUTPUT);  //set pin mode for all EN_SPEC pin

//--Serial2 for Misol WH24P
  Serial2.begin(9600);

//---Serial3 for modem/ESP8266
  Serial3.begin(9600);
  for(int h=0;h<3;h++) {
    Serial.print(F("Sending url_startup: ")); Serial.println(url_startup);
    Serial3.println(url_startup);
    delay(1000);
  }

//---display status preparing sample
  tft.fillRoundRect(0, 85, tft.width(), 20, 0, ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.setCursor(10, 90);
  tft.println("Preparing first sample..");

//---sampling for first display
  tft.fillRoundRect(0, 103, tft.width(), 20, 0, ST77XX_BLACK); tft.setCursor(50, 103); tft.println("(weather1)");
  get_windDavis();

  tft.fillRoundRect(0, 103, tft.width(), 20, 0, ST77XX_BLACK); tft.setCursor(62, 103); tft.println("(noise)");
  get_noiseVal();
  tft.fillRoundRect(0, 103, tft.width(), 20, 0, ST77XX_BLACK); tft.setCursor(55, 103); tft.println("(chamber)");
  digitalWrite(PUMP_PIN,PumpIsOn); Serial.println(F("Pump turned on. Filling chamber...")); delay(5000);
  tft.fillRoundRect(0, 103, tft.width(), 20, 0, ST77XX_BLACK); tft.setCursor(43, 103); tft.println("(particulate)");
  get_sps30_vD();
  tft.fillRoundRect(0, 103, tft.width(), 20, 0, ST77XX_BLACK); tft.setCursor(70, 103); tft.println("(gas)");
  get_4dgsspec_v2();
  digitalWrite(PUMP_PIN,PumpIsOff); Serial.println(F("Pump turned off."));
  tft.fillRoundRect(0, 103, tft.width(), 20, 0, ST77XX_BLACK); tft.setCursor(50, 103); tft.println("(weather2)");
  get_bme280_vD();
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

  tft.fillRoundRect(0, 47, tft.width(), 110, 10, ST77XX_BLACK);
  drawStringCenter(90, 60, "BASE ",2,ST77XX_BLUE);
  drawStringCenter(90, 85, "SUKABUMI ",2,ST77XX_CYAN);
  displayTime();

//--cek status wifi
  if(digitalRead(isWifiConnect_PIN) == HIGH) {isWifiConnect = true;} else {isWifiConnect = false;}
  tft.fillRoundRect(80, 2, 80, 33, 0, ST77XX_BLACK); tft.setTextColor(ST77XX_WHITE); tft.setTextSize(0); tft.setCursor(120, 8); tft.println("SERVER");
  if (isWifiConnect == true){
    tft.setTextColor(ST77XX_GREEN); tft.setTextSize(0); tft.setCursor(101, 18); tft.println("CONNECTED");
  } else {
    tft.setTextColor(ST77XX_RED); tft.setTextSize(0); tft.setCursor(85, 18); tft.println("DISCONNECTED");
  }

//--sampling
  if(firstSample_flag == true) {
    //--sampling data
    displayStat("Sampling (weather1)..."); get_windDavis();
    displayStat("Sampling (noise)...");get_noiseVal();
    read_time_rtcds3231();
    displayStat("Sampling (chamber)..."); digitalWrite(PUMP_PIN,PumpIsOn); Serial.println(F("Pump turned on. Filling chamber...")); delay(5000);
    displayStat("Sampling (particulate)..."); get_sps30_vD();
    displayStat("Sampling (gas)..."); get_4dgsspec_v2();
    digitalWrite(PUMP_PIN,PumpIsOff); Serial.println(F("Pump turned off."));
    displayStat("Sampling (weather2)..."); get_bme280_vD();
    delay(1000);
  }
  firstSample_flag = true;

//--uploading data
  displayStat("Upload..."); delay(1000);
  sendToModem();
  displayStat("Done upload."); delay(1000);

//--displaying data
  displayTime();
  displayStat("Display value..");
  displaySensorVal("Temperature ('C) ", currentData.temp); delay(2000);
  displaySensorVal("Humidity (%) ", currentData.rhum); delay(2000);
  displaySensorVal("Pressure (hPa) ", currentData.pressure); delay(2000);
  displaySensorVal("PM10 (ug/m3) ", currentData.pm10); delay(2000);
  displaySensorVal("Gas SO2 (ppb) ", currentData.ppbSO2); delay(2000);
  displaySensorVal("Gas NO2 (ppb) ", currentData.ppbNO2); delay(2000);
  displaySensorVal("Gas O3 (ppb) ", currentData.ppbO3); delay(2000);
  displaySensorVal("Gas CO (ppb) ", currentData.ppbCO); delay(2000);
  displaySensorVal("Wind speed (m/s) ", currentData.windSpeed); delay(2000);
  displaySensorValString("Wind compass ", currentData.windDirectionCompass); delay(2000);
  displaySensorValUnsignedInt("Wind direction (degree) ", currentData.windDirection); delay(2000);
  displaySensorValUnsignedInt("Irradiance (W/m2) ", currentData.irradianceUV); delay(2000);

  tft.fillRoundRect(0, 47, tft.width(), 110, 10, ST77XX_BLACK);
  drawStringCenter(90, 60, "BASE ",2,ST77XX_BLUE);
  drawStringCenter(90, 85, "SUKABUMI ",2,ST77XX_CYAN);

  NextLoop();

  Serial.println(F("--------loop end--------\n\n\n"));
  delay(100);
}

void sendToModem() {
  Serial3.flush();
  delay(500);

  String combinedData = url_upload + base_id + "&type=update" +"&co=" + String(currentData.ppbCO) + "&no2=" + String(currentData.ppbNO2) + "&o3=" + String(currentData.ppbO3)
                        + "&so2=" + String(currentData.ppbSO2) + "&pm1=" + String(currentData.pm1) + "&pm25=" + String(currentData.pm25) + "&pm4=" + String(currentData.pm4) + "&pm10=" + String(currentData.pm10) + "&temp="
                        + String(currentData.temp) + "&hum=" + String(currentData.rhum) + "&pres=" + String(currentData.pressure) + "&ws=" + String(currentData.windSpeed) + "&wd=" + String(currentData.windDirection) + "&uv=" + String(currentData.irradianceUV);

  Serial.println(F("Sending (")); Serial.println(combinedData); Serial.println(F(") to modem.")); Serial.flush();
  Serial3.println(combinedData);
  Serial3.flush();
}

void NextLoop(){
  tft.fillRoundRect(0, 113, tft.width(), 123, 10, ST77XX_BLACK);
  tft.drawLine(0,110, tft.width(),110, ST77XX_WHITE);  
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  int nxtlup = 900;
  Serial.print(F("NextLoop in ")); Serial.print(nxtlup); Serial.println(F(" seconds"));
  tft.setCursor(5, 115);  tft.println("Next loop in ");
  for(int nxtloop = nxtlup; nxtloop>0; nxtloop--){
    tft.setTextColor(ST77XX_GREEN);
    tft.fillRoundRect(79, 113, tft.width(), 123, 10, ST77XX_BLACK);
    tft.setCursor(83,115);  tft.print(nxtloop); tft.println(" s");
    if(nxtloop%30==0){
      read_time_rtcds3231(); displayTime();
      Serial.print(F("NextLoop in ")); Serial.print(nxtloop); Serial.println(F(" seconds"));
    }

    String inData = ""; 
    while(Serial.available()){
      inData = Serial.readStringUntil('\n');
    }
    if(inData.indexOf("start")!=-1)   nxtloop=0;
    digitalWrite(LED_PIN, digitalRead(LED_PIN)^1);

    if(digitalRead(isWifiConnect_PIN) == HIGH) {isWifiConnect = true;} else {isWifiConnect = false;}
    tft.fillRoundRect(80, 2, 80, 33, 0, ST77XX_BLACK); tft.setTextColor(ST77XX_WHITE); tft.setTextSize(0); tft.setCursor(120, 8); tft.println("SERVER");
    if (isWifiConnect == true){
      tft.setTextColor(ST77XX_GREEN); tft.setTextSize(0); tft.setCursor(101, 18); tft.println("CONNECTED");
    } else {tft.setTextColor(ST77XX_RED); tft.setTextSize(0); tft.setCursor(85, 18); tft.println("DISCONNECTED");}

    delay(1000);
  }
  Serial.println();
}