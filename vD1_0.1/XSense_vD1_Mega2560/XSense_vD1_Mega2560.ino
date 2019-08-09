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

  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on
  tft.initR(INITR_HALLOWING);   
  tft.initR(INITR_BLACKTAB);    
  tft.setRotation(1);

  displaySetup();

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

  Serial3.begin(9600);

  Serial.println(F("--------setup end--------\n"));
}

void loop() {
  Serial.println(F("--------loop begin--------"));
  get_sps30_vD();
  get_bme280_vD();
  get_rtcds3231();
  get_4dgsspec();
  sendToModem();
  // delay(1000);
  // sendToModem();
  displayData();
  Serial.println(F("--------loop end--------\n\n\n"));
  delay(1000);
}

void sendToModem() {
  Serial3.println("ping");
  Serial3.flush();
  delay(500);
  Serial3.println("ping");
  Serial3.flush();
  delay(500);
  String combinedData = "http://128.199.194.1/ganitri/pd/sensor?id=4O5Z56Xu&co=" + String(currentData.ppbCO) + "&no2=" + String(currentData.ppbNO2) + "&o3=" + String(currentData.ppbO3)
                        + "&so2=" + String(currentData.ppbSO2) + "&pm1=" + String(currentData.pm1) + "&pm25=" + String(currentData.pm25) + "&pm4=" + String(currentData.pm4) + "&pm10=" + String(currentData.pm10) + "&temp="
                        + String(currentData.temp) + "&rhum=" + String(currentData.rhum) + "&pres=" + String(currentData.pressure) + "&ws=" + String(currentData.windSpeed) + "&wd=" + String(currentData.windDirection) + "&uv=" + String(currentData.irradianceUV);
  Serial3.flush();
  Serial3.println(combinedData);
  Serial3.flush();
  Serial.print(F(" String(\n   "));
  Serial.println(combinedData);
  Serial.println(F(" )sent to modem."));
}
