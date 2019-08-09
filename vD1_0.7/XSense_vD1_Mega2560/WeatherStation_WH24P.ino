void get_wh24p() {
    byte banyakbyte = 21;
    byte bufferWH24P[banyakbyte];

    Serial.print(F("\nTest WH24P. Sampling #")); Serial.println(1);

    for(int z=0;z < banyakbyte;z++){bufferWH24P[z] = 0;}

    //while((bufferWH24P[0] == 0) && (bufferWH24P[1] == 0) && (bufferWH24P[2] == 0)){
    unsigned long startTime = millis();
    while(millis() - startTime < 30000){
      Serial2.readBytes(bufferWH24P,banyakbyte);
      Serial.print(".");
    }

    Serial.println();
    for(int z=0;z < banyakbyte;z++){
        if(bufferWH24P[z] <= 15){Serial.print(0);}
        Serial.print(bufferWH24P[z],HEX);
        Serial.print(" ");
    }

    //--parsing
    Serial.println();
    Serial.print(bufferWH24P[2],HEX); Serial.print(" "); Serial.print(bufferWH24P[3],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[2],BIN); Serial.print(" "); Serial.println(bufferWH24P[3],BIN);
    currentData.windDirection = (bufferWH24P[3] << 1) & 256; currentData.windDirection = currentData.windDirection | (bufferWH24P[2]); //-- D(256) = B(0001 0000 0000)
    Serial.println(currentData.windDirection,BIN);
    Serial.print(F("Wind direction: ")); Serial.print(currentData.windDirection); Serial.println(F(" (degree)"));
     if(currentData.windDirection < 22)
      currentData.windDirectionCompass = "NORTH ";
    else if (currentData.windDirection < 67)
      currentData.windDirectionCompass = "NORTH-EAST ";
    else if (currentData.windDirection < 112)
      currentData.windDirectionCompass = "EAST ";
    else if (currentData.windDirection < 157)
      currentData.windDirectionCompass = "SOUTH-EAST ";
    else if (currentData.windDirection < 212)
     currentData.windDirectionCompass = "SOUTH ";
    else if (currentData.windDirection < 247)
      currentData.windDirectionCompass = "SOUTH-WEST ";
    else if (currentData.windDirection < 292)
     currentData.windDirectionCompass = "WEST ";
    else if (currentData.windDirection < 337)
      currentData.windDirectionCompass = "NORTH-WEST ";
    else
     currentData.windDirectionCompass = "NORTH ";  

    Serial.println();
    Serial.print(bufferWH24P[3],HEX); Serial.print(" "); Serial.print(bufferWH24P[4],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[3],BIN); Serial.print(" "); Serial.println(bufferWH24P[4],BIN);
    unsigned int tempzz = ((bufferWH24P[3] << 8) & 1792) | (bufferWH24P[4]); //-- D(1792) = B(0111 0000 0000)
    Serial.println(tempzz,BIN);
    //--rumus temperatur = (tempz-400)/10
    float tempz = (tempzz-400)/10.0;  //in 'C
    Serial.print(F("Temperature: ")); Serial.print(tempz); Serial.println(F(" (degree celsius)"));

    Serial.println();
    Serial.print(bufferWH24P[5],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.println(bufferWH24P[5],BIN);
    byte humz = bufferWH24P[5]; //in %
    Serial.print(F("Humidity: ")); Serial.print(humz); Serial.println(F(" (%)"));

    Serial.println();
    Serial.print(bufferWH24P[6],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.println(bufferWH24P[6],BIN);
    unsigned int windspdz = (bufferWH24P[3] << 4) & 256; windspdz = windspdz | bufferWH24P[6]; //-- D(256) = B(0001 0000 0000)
    Serial.println(windspdz,BIN);
    currentData.windSpeed = windspdz/0.14;  //in m/s
    Serial.print(F("Wind speed: "));  Serial.print(currentData.windSpeed); Serial.println(F(" (m/s)"));

    Serial.println();
    Serial.print(bufferWH24P[7],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.println(bufferWH24P[7],BIN);
    float gustspd = bufferWH24P[7]*1.12;  //in m/s
    Serial.print(F("Gust speed: "));  Serial.print(gustspd); Serial.println(F(" (m/s)"));

    Serial.println();
    Serial.print(bufferWH24P[8],HEX); Serial.print(" "); Serial.print(bufferWH24P[9],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[8],BIN); Serial.print(" "); Serial.println(bufferWH24P[9],BIN);
    unsigned int accrainfall = (bufferWH24P[8] << 8) | (bufferWH24P[9]); //in mm
    Serial.println(accrainfall,BIN);
    Serial.print(F("Accumulated rainfall: ")); Serial.print(accrainfall); Serial.println(F(" (mm)"));

    Serial.println();
    Serial.print(bufferWH24P[10],HEX); Serial.print(" "); Serial.print(bufferWH24P[11],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[10],BIN); Serial.print(" "); Serial.println(bufferWH24P[11],BIN);
    unsigned int ultrav = (bufferWH24P[10] << 8) | (bufferWH24P[11]);  //in uW/cm2
    Serial.println(ultrav,BIN);
    Serial.print(ultrav); Serial.println(F(" (uW/cm2)"));

    Serial.println();
    Serial.print(bufferWH24P[12],HEX); Serial.print(" "); Serial.print(bufferWH24P[13],HEX); Serial.print(" "); Serial.print(bufferWH24P[14],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[12],BIN); Serial.print(" "); Serial.print(bufferWH24P[13],BIN); Serial.print(" "); Serial.println(bufferWH24P[14],BIN);
    //unsigned long lightlux = 0; Serial.println(lightlux,BIN);  lightlux = (bufferWH24P[12] << 16) | (bufferWH24P[13] << 8); lightlux = (lightlux) | (bufferWH24P[14]);//in lux
    unsigned long lightlux = 0; Serial.println(lightlux,BIN); 
    lightlux = bufferWH24P[12]; lightlux = lightlux << 8; lightlux = lightlux | (bufferWH24P[13]); lightlux = lightlux << 8; lightlux = (lightlux) | (bufferWH24P[14]);//in lux
    Serial.println(lightlux,BIN); Serial.println(lightlux,HEX);
    float lightluxx = lightlux / 10.0;
    Serial.print(lightluxx); Serial.println(F(" (lux)"));

    Serial.println();
    Serial.print(bufferWH24P[17],HEX); Serial.print(" "); Serial.print(bufferWH24P[18],HEX); Serial.print(" "); Serial.print(bufferWH24P[19],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[17],BIN); Serial.print(" "); Serial.print(bufferWH24P[18],BIN); Serial.print(" "); Serial.println(bufferWH24P[19],BIN);
    //unsigned long pressurexx = 0; Serial.println(pressurexx,BIN); pressurexx = (bufferWH24P[17] << 16) | (bufferWH24P[18] << 8); pressurexx = (pressurexx) | (bufferWH24P[19]); //in Pa
    unsigned long pressurexx = 0; Serial.println(pressurexx,BIN);
    pressurexx = bufferWH24P[17]; pressurexx = pressurexx << 8; pressurexx = pressurexx | bufferWH24P[18]; pressurexx = pressurexx << 8; pressurexx = (pressurexx) | (bufferWH24P[19]); //in Pa
    Serial.println(pressurexx,BIN); Serial.println(pressurexx,HEX);
    float pressurexz = pressurexx / 100.0;
    Serial.print(pressurexz); Serial.println(F(" (hPa)"));
}