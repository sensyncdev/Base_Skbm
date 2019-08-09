void displayStat(String teks){
  tft.drawLine(0,110, tft.width(),110, ST77XX_WHITE);  
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 115);  tft.println("Status:");
  tft.fillRoundRect(45, 113, tft.width(), 123, 10, ST77XX_BLACK);
  tft.setCursor(50,115);  tft.println(teks);
}

void displayTime() {
  tft.fillRoundRect(0, 3, 99, 40, 10, ST77XX_BLACK);  
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);
  
  //-- display TIME  
  tft.setCursor(5,5);
  if(waktu.Hour<10){tft.print(F("0"));}
  tft.print(waktu.Hour);
  tft.print(F(":"));
  if(waktu.Minute<10){tft.print(F("0"));}
  tft.println(waktu.Minute);
  
  //-- display DATE  
  tft.setTextSize(0);
  tft.setTextColor(ST77XX_GREEN);
  // tft.setCursor(5,35);
  // tft.print(days[weekday-1]);
  tft.setCursor(30,35);
  if(waktu.Day<10){tft.print(F("0"));}
  tft.print(waktu.Day);
  tft.print(F("/"));
  if(waktu.Month<10){tft.print(F("0"));}
  tft.print(waktu.Month);
  tft.print(F("/20"));
  tft.print(waktu.Year);
}

void displayTempHumPres(){
  //tft.fillRoundRect(100, 3, tft.width(), 43, 10, ST77XX_BLACK);  
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(100, 5);    tft.print(currentData.temp,1);
  tft.println("C");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLUE);
  tft.setCursor(100, 25);   tft.print("H:");
  tft.setCursor(115, 25);   tft.print(currentData.rhum);
  tft.println("%");
  tft.setCursor(100, 35);   tft.print("P:");
  tft.setCursor(115, 35);   tft.print(currentData.pressure,0);
  tft.println("hPa");
}

void displayGasVal(){
  int posY1=50, posY2=60;
  int posX1=15, posX2=50, posX3=85, posX4=120;

  tft.drawLine(0, 45, tft.width(), 45, ST77XX_WHITE);  
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(posX1, posY1);    tft.println("CO");
  tft.setCursor(posX1-10,posY2);  tft.println(currentData.ppbCO);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(posX2, posY1);    tft.println("O3");
  tft.setCursor(posX2-10,posY2);  tft.println(currentData.ppbO3);
  tft.setTextColor(ST77XX_MAGENTA);
  tft.setCursor(posX3, posY1);    tft.println("NO2");
  tft.setCursor(posX3-10,posY2);  tft.println(currentData.ppbNO2);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(posX4, posY1);    tft.println("SO2");
  tft.setCursor(posX4-10,posY2);  tft.println(currentData.ppbSO2);
}

void displayPMmeter(){
  tft.drawLine(0, 70, tft.width(), 70, ST77XX_WHITE);  
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(5, 75);
  tft.println(" PM1.0  PM2.5 PM4.0  PM10");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor( 5, 85);   tft.print(currentData.pm1);
  tft.setCursor(45, 85);   tft.print(currentData.pm25);
  tft.setCursor(85, 85);   tft.print(currentData.pm4);
  tft.setCursor(120,85);   tft.print(currentData.pm10);
}

void displayWind(){  
  tft.drawLine(0,95, tft.width(),95, ST77XX_WHITE);  
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 100);    tft.println("Wind:      m/s");
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(40, 100);   tft.println(currentData.windSpeed);
  tft.setCursor(95, 100);   //tft.println(wDir);

  if(currentData.windDirection < 22)
      tft.print(F("NORTH"));
    else if (currentData.windDirection < 67)
      tft.print(F("NORTHEAST"));
    else if (currentData.windDirection < 112)
      tft.print(F("EAST"));
    else if (currentData.windDirection < 157)
      tft.print(F("SOUTHEAST"));
    else if (currentData.windDirection < 212)
      tft.print(F("SOUTH"));
    else if (currentData.windDirection < 247)
      tft.print(F("SOUTHWEST"));
    else if (currentData.windDirection < 292)
      tft.print(F("WEST"));
    else if (currentData.windDirection < 337)
      tft.print(F("NORTHWEST"));
    else
      tft.print(F("NORTH"));  
}

//---------------------------------------------------batas baru dan lama

// void displaySetup() {
//   tft.setTextWrap(false);
//   tft.fillScreen(ST77XX_BLACK);
//   tft.setTextColor(ST77XX_BLUE);
//   tft.setTextSize(2);
//   tft.setCursor(20, 20);
//   tft.println("XSense");
//   tft.setCursor(20, 50);
//   tft.println("DGS SPEC");
//   tft.setTextColor(ST77XX_YELLOW);
//   tft.setCursor(35, 80);
//   tft.setTextSize(1);
//   tft.println("Sukabumi");
//   delay(500);
// }

// void displayData() {
//   Serial.println(F("Initializing ST7735S TFT..."));

//   //data
//   tft.setCursor(0, 0);
//   tft.fillScreen(ST77XX_BLACK);
//   tft.setCursor(5, 0);
//   tft.setTextSize(0);
//   String bulan;
//   switch (waktuMonth) {
//     case 1: bulan="JAN"; break;
//     case 2: bulan="FEB"; break;
//     case 3: bulan="MAR"; break;
//     case 4: bulan="APR"; break;
//     case 5: bulan="MAY"; break;
//     case 6: bulan="JUN"; break;
//     case 7: bulan="JUL"; break;
//     case 8: bulan="AUG"; break;
//     case 9: bulan="SEP"; break;
//     case 10: bulan="OCT"; break;
//     case 11: bulan="NOV"; break;
//     case 12: bulan="DEC"; break;
//   }
//   String tanggal = waktuDay + ' ' + bulan + ' ' +  waktuYear;
//   tft.println(tanggal); 
//   tft.setCursor(5, 10);
//   drawtextline("Starting...", ST77XX_WHITE, 1000);
//   tft.setCursor(5, 20);
//   drawtextline("Type HELP for command", ST77XX_WHITE, 1000);
//   tft.setCursor(5, 30);
//   drawtextline("Sampling...", ST77XX_WHITE, 1000);
  
//   tft.setCursor(5, 40);
//   String displayTemp = "Temperature: " + String(currentData.temp,2) + " 'C";
//   tft.println(displayTemp);
  
//   String displayRH = "Humidity: " + String(currentData.rhum,2) + " %";
//   tft.println(displayRH);
  
//   String displayPressure = "Pressure: " + String(currentData.pressure,2) + " Pa";
//   tft.println(displayPressure);

//   String displayPM1 = "PM1.0: " + String(currentData.pm1,2) + " ug/mm3";
//   tft.println(displayPM1);

//   String displayPM25 = "PM2.5: " + String(currentData.pm25,2) + " ug/mm3";
//   tft.println(displayPM25);

//   String displayPM4 = "PM4.0: " + String(currentData.pm4,2) + " ug/mm3";
//   tft.println(displayPM4);

//   String displayPM10 = "PM10: " + String(currentData.pm10,2) + " ug/mm3";
//   tft.println(displayPM10);

//   String displayPPBCO = "Gas CO: " + String(currentData.ppbCO) + " ppb";
//   tft.println(displayPPBCO);

//   String displayPPBNO2 = "Gas NO2: " + String(currentData.ppbNO2) + " ppb";
//   tft.println(displayPPBNO2);
// }

// //void TFTprintLogo(){
// //  tft.setTextWrap(false);
// //  tft.fillScreen(ST77XX_BLACK);
// //  tft.setTextColor(ST77XX_BLUE);
// //  tft.setTextSize(3);
// //  tft.setCursor(20, 20);
// //  tft.println("XSense");
// //  tft.setCursor(45, 50);
// //  tft.println("DGS SPEC");
// //  tft.setTextColor(ST77XX_YELLOW);
// //  tft.setCursor(35, 80);
// //  tft.setTextSize(2);
// //  tft.println("Sukabumi");
// //  delay(3000);
// //}
// //
// //void TFTprintBoot() {
// //  tft.setCursor(0, 0);
// //  tft.fillScreen(ST77XX_BLACK);
// //  tft.setCursor(5, 0);
// //  tft.setTextSize(0);
// //  drawtextline("Senin, 10 Juni 2019", ST77XX_GREEN, 500);  
// //  tft.setCursor(5, 10);
// //  drawtextline("Starting...", ST77XX_WHITE, 1000);
// //  tft.setCursor(5, 20);
// //  drawtextline("Type HELP for command", ST77XX_WHITE, 1000);
// //  tft.setCursor(5, 30);
// //  drawtextline("Sampling...", ST77XX_WHITE, 1000);
// //  tft.setCursor(5, 40);
// //  drawtextline("Temp     : 29.64 'C", ST77XX_WHITE, 500);
// //  tft.setCursor(5, 50);
// //  drawtextline("Humidity : 54.67 %RH", ST77XX_WHITE, 500);
// //  tft.setCursor(5, 60);
// //  drawtextline("Pressure : 88120 Pa", ST77XX_WHITE, 500);
// //  tft.setCursor(5, 70);
// //  drawtextline("Int.Temp : 27.62 'C", ST77XX_WHITE, 500);
// //  tft.setCursor(5, 80);
// //  drawtextline("RTC Temp : 30.25 'C", ST77XX_WHITE, 500);
// //  delay(3000);  
// //}

// void drawtextline(char *text, uint16_t color, int txtwait) {
//   tft.setTextColor(color);
//   tft.setTextWrap(false);
//   tft.println(text);
//   delay(txtwait);
// }
