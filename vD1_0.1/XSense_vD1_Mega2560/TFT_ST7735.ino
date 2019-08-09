void displaySetup() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("XSense");
  tft.setCursor(20, 50);
  tft.println("DGS SPEC");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(35, 80);
  tft.setTextSize(1);
  tft.println("Sukabumi");
  delay(500);
}

void displayData() {
  Serial.println(F("Initializing ST7735S TFT..."));

  //data
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 0);
  tft.setTextSize(0);
  String bulan;
  switch (waktuMonth) {
    case 1: bulan="JAN"; break;
    case 2: bulan="FEB"; break;
    case 3: bulan="MAR"; break;
    case 4: bulan="APR"; break;
    case 5: bulan="MAY"; break;
    case 6: bulan="JUN"; break;
    case 7: bulan="JUL"; break;
    case 8: bulan="AUG"; break;
    case 9: bulan="SEP"; break;
    case 10: bulan="OCT"; break;
    case 11: bulan="NOV"; break;
    case 12: bulan="DEC"; break;
  }
  String tanggal = waktuDay + ' ' + bulan + ' ' +  waktuYear;
  tft.println(tanggal); 
  tft.setCursor(5, 10);
  drawtextline("Starting...", ST77XX_WHITE, 1000);
  tft.setCursor(5, 20);
  drawtextline("Type HELP for command", ST77XX_WHITE, 1000);
  tft.setCursor(5, 30);
  drawtextline("Sampling...", ST77XX_WHITE, 1000);
  
  tft.setCursor(5, 40);
  String displayTemp = "Temperature: " + String(currentData.temp,2) + " 'C";
  tft.println(displayTemp);
  
  String displayRH = "Humidity: " + String(currentData.rhum,2) + " %";
  tft.println(displayRH);
  
  String displayPressure = "Pressure: " + String(currentData.pressure,2) + " Pa";
  tft.println(displayPressure);

  String displayPM1 = "PM1.0: " + String(currentData.pm1,2) + " ug/mm3";
  tft.println(displayPM1);

  String displayPM25 = "PM2.5: " + String(currentData.pm25,2) + " ug/mm3";
  tft.println(displayPM25);

  String displayPM4 = "PM4.0: " + String(currentData.pm4,2) + " ug/mm3";
  tft.println(displayPM4);

  String displayPM10 = "PM10: " + String(currentData.pm10,2) + " ug/mm3";
  tft.println(displayPM10);

  String displayPPBCO = "Gas CO: " + String(currentData.ppbCO) + " ppb";
  tft.println(displayPPBCO);

  String displayPPBNO2 = "Gas NO2: " + String(currentData.ppbNO2) + " ppb";
  tft.println(displayPPBNO2);
}

//void TFTprintLogo(){
//  tft.setTextWrap(false);
//  tft.fillScreen(ST77XX_BLACK);
//  tft.setTextColor(ST77XX_BLUE);
//  tft.setTextSize(3);
//  tft.setCursor(20, 20);
//  tft.println("XSense");
//  tft.setCursor(45, 50);
//  tft.println("DGS SPEC");
//  tft.setTextColor(ST77XX_YELLOW);
//  tft.setCursor(35, 80);
//  tft.setTextSize(2);
//  tft.println("Sukabumi");
//  delay(3000);
//}
//
//void TFTprintBoot() {
//  tft.setCursor(0, 0);
//  tft.fillScreen(ST77XX_BLACK);
//  tft.setCursor(5, 0);
//  tft.setTextSize(0);
//  drawtextline("Senin, 10 Juni 2019", ST77XX_GREEN, 500);  
//  tft.setCursor(5, 10);
//  drawtextline("Starting...", ST77XX_WHITE, 1000);
//  tft.setCursor(5, 20);
//  drawtextline("Type HELP for command", ST77XX_WHITE, 1000);
//  tft.setCursor(5, 30);
//  drawtextline("Sampling...", ST77XX_WHITE, 1000);
//  tft.setCursor(5, 40);
//  drawtextline("Temp     : 29.64 'C", ST77XX_WHITE, 500);
//  tft.setCursor(5, 50);
//  drawtextline("Humidity : 54.67 %RH", ST77XX_WHITE, 500);
//  tft.setCursor(5, 60);
//  drawtextline("Pressure : 88120 Pa", ST77XX_WHITE, 500);
//  tft.setCursor(5, 70);
//  drawtextline("Int.Temp : 27.62 'C", ST77XX_WHITE, 500);
//  tft.setCursor(5, 80);
//  drawtextline("RTC Temp : 30.25 'C", ST77XX_WHITE, 500);
//  delay(3000);  
//}

void drawtextline(char *text, uint16_t color, int txtwait) {
  tft.setTextColor(color);
  tft.setTextWrap(false);
  tft.println(text);
  delay(txtwait);
}
