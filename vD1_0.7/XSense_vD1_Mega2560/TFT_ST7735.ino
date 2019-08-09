void displayTime() {
  tft.fillRoundRect(0, 3, 80, 40, 10, ST77XX_BLACK);  
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  
  //-- display TIME  
  tft.setCursor(5,5);
  if(waktu.Hour<10){tft.print(F("0"));}
  tft.print(waktu.Hour);
  tft.print(F(":"));
  if(waktu.Minute<10){tft.print(F("0"));}
  tft.println(waktu.Minute);
  
  //-- display DATE  
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  // tft.setCursor(5,35);
  // tft.print(days[weekday-1]);
  tft.setCursor(5,25);
  if(waktu.Day<10){tft.print(F("0"));}
  tft.print(waktu.Day);
  tft.print(F("/"));
  if(waktu.Month<10){tft.print(F("0"));}
  tft.print(waktu.Month);
  tft.print(F("/20"));
  tft.print(waktu.Year);

  tft.drawLine(0, 35, tft.width(), 35, ST77XX_WHITE);
}

void displayStat(String teks){
  tft.drawLine(0,110, tft.width(),110, ST77XX_WHITE);  
  tft.setTextColor(ST77XX_WHITE); tft.setTextSize(0);
  tft.fillRoundRect(0, 113, tft.width(), 20, 0, ST77XX_BLACK);
  tft.setCursor(5, 115);  tft.println(">>");
  tft.setCursor(20,115);  tft.println(teks);
}

void displaySensorVal(String labelna, float valuena){
  int posX=80, posY1=57, posY2=85;
  String nilaina = String(valuena,3); //float to string
  char clabel[labelna.length()];    //char array for label
  char cnilai[nilaina.length()];    //char array for nilai

  labelna.toCharArray(clabel,labelna.length()); //string label to char array
  nilaina.toCharArray(cnilai,nilaina.length()); //string nilai to char array
  
  tft.fillRoundRect(0, posY1-10, tft.width(), 50, 10, ST77XX_BLACK);
  drawStringCenter(posX, posY1,clabel,1,ST77XX_MAGENTA);
  drawStringCenter(posX, posY2,cnilai,2,ST77XX_CYAN);
}

void displaySensorValUnsignedInt(String labelna, unsigned int valuenaint){
  int posX=80, posY1=57, posY2=85;
  float valuena = float(valuenaint);
  String nilaina = String(valuena,2); //float to string
  char clabel[labelna.length()];    //char array for label
  char cnilai[nilaina.length()];    //char array for nilai

  labelna.toCharArray(clabel,labelna.length()); //string label to char array
  nilaina.toCharArray(cnilai,nilaina.length()); //string nilai to char array
  
  tft.fillRoundRect(0, posY1-10, tft.width(), 50, 10, ST77XX_BLACK);
  drawStringCenter(posX, posY1,clabel,1,ST77XX_MAGENTA);
  drawStringCenter(posX, posY2,cnilai,2,ST77XX_CYAN);
}

void displaySensorValString(String labelna, String nilaina){
  int posX=80, posY1=57, posY2=85;
  char clabel[labelna.length()];    //char array for label
  char cnilai[nilaina.length()];    //char array for nilai

  labelna.toCharArray(clabel,labelna.length()); //string label to char array
  nilaina.toCharArray(cnilai,nilaina.length()); //string nilai to char array
  
  tft.fillRoundRect(0, posY1-10, tft.width(), 50, 10, ST77XX_BLACK);
  drawStringCenter(posX, posY1,clabel,1,ST77XX_MAGENTA);
  drawStringCenter(posX, posY2,cnilai,2,ST77XX_CYAN);
}

void drawStringCenter(int16_t x, int16_t y, const char *string, int8_t textsize, uint16_t Color){
  if(string!=NULL){
    tft.setTextColor(Color);
    tft.setTextSize(textsize);
    tft.setCursor(x - (strlen(string)*3*textsize), y-(4*textsize));
    while(*string){
      tft.write(*string++); }
  }
}

// void displayTempHumPres(){
//   //tft.fillRoundRect(100, 3, tft.width(), 43, 10, ST77XX_BLACK);  
//   tft.setTextSize(2);
//   tft.setTextColor(ST77XX_YELLOW);
//   tft.setCursor(100, 5);    tft.print(currentData.temp,1);
//   tft.println("C");
//   tft.setTextSize(1);
//   tft.setTextColor(ST77XX_BLUE);
//   tft.setCursor(100, 25);   tft.print("H:");
//   tft.setCursor(115, 25);   tft.print(currentData.rhum);
//   tft.println("%");
//   tft.setCursor(100, 35);   tft.print("P:");
//   tft.setCursor(115, 35);   tft.print(currentData.pressure,0);
//   tft.println("hPa");
// }

// void displayGasVal(){
//   int posY1=50, posY2=60;
//   int posX1=15, posX2=50, posX3=85, posX4=120;

//   tft.drawLine(0, 45, tft.width(), 45, ST77XX_WHITE);  
//   tft.setTextSize(1);
//   tft.setTextColor(ST77XX_RED);
//   tft.setCursor(posX1, posY1);    tft.println("CO");
//   tft.setCursor(posX1-10,posY2);  tft.println(currentData.ppbCO);
//   tft.setTextColor(ST77XX_YELLOW);
//   tft.setCursor(posX2, posY1);    tft.println("O3");
//   tft.setCursor(posX2-10,posY2);  tft.println(currentData.ppbO3);
//   tft.setTextColor(ST77XX_MAGENTA);
//   tft.setCursor(posX3, posY1);    tft.println("NO2");
//   tft.setCursor(posX3-10,posY2);  tft.println(currentData.ppbNO2);
//   tft.setTextColor(ST77XX_CYAN);
//   tft.setCursor(posX4, posY1);    tft.println("SO2");
//   tft.setCursor(posX4-10,posY2);  tft.println(currentData.ppbSO2);
// }

// void displayPMmeter(){
//   tft.drawLine(0, 70, tft.width(), 70, ST77XX_WHITE);  
//   tft.setTextColor(ST77XX_GREEN);
//   tft.setCursor(5, 75);
//   tft.println(" PM1.0  PM2.5 PM4.0  PM10");
//   tft.setTextColor(ST77XX_WHITE);
//   tft.setCursor( 5, 85);   tft.print(currentData.pm1);
//   tft.setCursor(45, 85);   tft.print(currentData.pm25);
//   tft.setCursor(85, 85);   tft.print(currentData.pm4);
//   tft.setCursor(120,85);   tft.print(currentData.pm10);
// }

// void displayWind(){  
//   tft.drawLine(0,95, tft.width(),95, ST77XX_WHITE);  
//   tft.setTextColor(ST77XX_WHITE);
//   tft.setCursor(5, 100);    tft.println("Wind:      m/s");
//   tft.setTextColor(ST77XX_CYAN);
//   tft.setCursor(40, 100);   tft.println(currentData.windSpeed);
//   tft.setCursor(95, 100);   //tft.println(wDir);

//   if(currentData.windDirection < 22)
//       tft.print(F("NORTH"));
//     else if (currentData.windDirection < 67)
//       tft.print(F("NORTHEAST"));
//     else if (currentData.windDirection < 112)
//       tft.print(F("EAST"));
//     else if (currentData.windDirection < 157)
//       tft.print(F("SOUTHEAST"));
//     else if (currentData.windDirection < 212)
//       tft.print(F("SOUTH"));
//     else if (currentData.windDirection < 247)
//       tft.print(F("SOUTHWEST"));
//     else if (currentData.windDirection < 292)
//       tft.print(F("WEST"));
//     else if (currentData.windDirection < 337)
//       tft.print(F("NORTHWEST"));
//     else
//       tft.print(F("NORTH"));  
// }