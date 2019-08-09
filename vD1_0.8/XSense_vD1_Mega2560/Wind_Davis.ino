//--------------------------------------------------- Anemometer Config
/*
Black  = pulse from anemometer. Connect to Digital 2 pin, and use a 4k7 resistor as pull up to + 5v.
         use a 10 to 22nF capacitor from pin D2 to ground to debounce the reed switch of anemometer       
Red    = Ground
Green  = Wiper  of Wind direction vane - connect to A0.  Use a 1..10�ｿｽF/16v capacitor between A0 
         and ground (observe C polarity) to avoid jitter
Yellow = + 5v (reference of potentiometer)
*/ 
int16_t adcW;
//--------------------------------------------------- WIND SPEED
void get_windDavis() {
  attachInterrupt(digitalPinToInterrupt(WindSensorPin), isr_rotation, FALLING);
  // Get Wind Direction
  getWindDirection();
  // Only update the display if change greater than 5 degrees. 
  if(abs(CalDirection - LastValue) > 5)
  { LastValue = CalDirection; }
  
  getWindSpeed();
  while (windSpd > 45) { getWindSpeed(); delay(500);}
  Serial.println("WIND:Speed(kmH), Direction");
  // PrintSOD();    
  Serial.print(F("WIND:"));
  Serial.print(windSpd);    //Speed (kmh)
  Serial.print(CalDirection);        
  //Serial.print(getWindKnots(WindSpeed)); PrintComa();  //Knot 
  //Serial.print(F("_"));              
  //getWindHeading(CalDirection); PrintComa();  //Heading Direction 
  //getWindStrength(WindSpeed);                 //Strength


  // // Display to LCD
  // lcd.backlight();
  // lcd.print(F("   WIND SPEED   "));
  // //+Wait(1);
  // lcd.clear();
  // lcd.print(F("SPEED: ")); 
  // lcd.print(windSpd); lcd.print(F("kmH"));
  // // lcd.setCursor(0,1);
  // lcd.print(F("DIR: ")); 
//  detachInterrupt(digitalPinToInterrupt(WindSensorPin), isr_rotation, FALLING);
  detachInterrupt(WindSensorPin);
  //+Wait(3);
}

void getWindSpeed (){             //-- Get Wind Speed
//  unsigned int timeout = 5000;
//  unsigned long timerEnd, timerStart;
//  timerStart = millis();
    if(RotationCount)
  {
     // convert to mp/h using the formula V=P(2.25/T)
     // V = P(2.25/2.5) = P * 0.9
     windSpd = Rotations * 0.9;     // Speed in mph
     windSpd = windSpd * 1.609;   // Speed in kmh
     Rotations = 0;                   // Reset count for next sample
     RotationCount = false; 
  }  
//  timerEnd = millis();
//  if(timerStart - timerEnd > timeout){
//    return -1;
//  }
  currentData.windSpeed = windSpd;
}

void getWindDirection() {     //-- Get Wind Direction
  int maxbit = 1023;
   // adcW = ads3.readADC_SingleEnded(1);
   adcW = analogRead(WindVanePin);
  //   VaneValue = ads0.readADC_SingleEnded(0);
  //   VaneValue = VaneValue - 360;
   windDir = map(adcW, 0, maxbit, 0, 360);
   CalDirection = windDir + VaneOffset;
   
   if(CalDirection > 360)
     CalDirection = CalDirection - 360;
   
   if(CalDirection < 0)
     CalDirection = CalDirection + 360;
   
 currentData.windDirection = CalDirection;

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
//     timerEnd = millis();
//  if(timerStart - timerEnd > timeout){
//    return -1;
//  }
 Serial.println(currentData.windDirection);
 Serial.println(currentData.windDirectionCompass);
}


// void DispDirection(int direction) {   // Display Wind Direction to LCD
//     // converts wind speed to wind strength    
//     if(direction < 22)
//       lcd.print(F("NORTH"));
//     else if (direction < 67)
//       lcd.print(F("NORTHEAST"));
//     else if (direction < 112)
//       lcd.print(F("EAST"));
//     else if (direction < 157)
//       lcd.print(F("SOUTHEAST"));
//     else if (direction < 212)
//       lcd.print(F("SOUTH"));
//     else if (direction < 247)
//       lcd.print(F("SOUTHWEST"));
//     else if (direction < 292)
//       lcd.print(F("WEST"));
//     else if (direction < 337)
//       lcd.print(F("NORTHWEST"));
//     else
//       lcd.print(F("NORTH"));  
// }

// This is the function that the interrupt calls to increment the rotation count
void isr_rotation() {
  // debounce the switch contact.
  if ((millis() - ContactBounceTime) > 15 ) {  
    Rotations++;
    ContactBounceTime = millis();
  }
}

/*
float getWindKnots(float speed) { //-- Convert to Knots
   //return speed * 0.868976; // mph to knot
   return speed * 0.539957;   // kmh to knot
}

void getWindHeading(int WindDir) {  //-- Converts compass direction to heading
    
    if(WindDir < 22)
      Serial.print(F("N"));
    else if (WindDir < 67)
      Serial.print(F("NE"));
    else if (WindDir < 112)
      Serial.print(F("E"));
    else if (WindDir < 157)
      Serial.print(F("SE"));
    else if (WindDir < 212)
      Serial.print(F("S"));
    else if (WindDir < 247)
      Serial.print(F("SW"));
    else if (WindDir < 292)
      Serial.print(F("W"));
    else if (WindDir < 337)
      Serial.print(F("NW"));
    else
      Serial.print(F("N"));  
}

void getWindStrength(float speed) {   //-- get Wind Strength
  if(speed < 2)
    Serial.print(F("Calm"));
  else if(speed >= 2 && speed < 4)
    Serial.print(F("Light Air"));
  else if(speed >= 4 && speed < 8)
    Serial.print(F("Light Breeze"));
  else if(speed >= 8 && speed < 13)
    Serial.print(F("Gentle Breeze"));
  else if(speed >= 13 && speed < 18)
    Serial.print(F("Moderate Breeze"));
  else if(speed >= 18 && speed < 25)
    Serial.print(F("Fresh Breeze"));
  else if(speed >= 25 && speed < 31)
    Serial.print(F("Strong Breeze"));
  else if(speed >= 31 && speed < 39)
    Serial.print(F("Near Gale"));
  else if(speed >= 39 && speed < 46)
    Serial.print(F("Fresh Gale"));
  else
    Serial.print(F("ERROR"));
}
*/


// void get_wh24p() {
//     byte banyakbyte = 21;
//     byte bufferWH24P[banyakbyte];

//     Serial.print(F("\nTest WH24P. Sampling #")); Serial.println(1);

//     for(int z=0;z < banyakbyte;z++){bufferWH24P[z] = 0;}

//     //while((bufferWH24P[0] == 0) && (bufferWH24P[1] == 0) && (bufferWH24P[2] == 0)){
//     unsigned long startTime = millis();
//     while(millis() - startTime < 30000){
//       Serial2.readBytes(bufferWH24P,banyakbyte);
//       Serial.print(".");
//     }

//     Serial.println();
//     for(int z=0;z < banyakbyte;z++){
//         if(bufferWH24P[z] <= 15){Serial.print(0);}
//         Serial.print(bufferWH24P[z],HEX);
//         Serial.print(" ");
//     }

//     //--parsing
//     Serial.println();
//     Serial.print(bufferWH24P[2],HEX); Serial.print(" "); Serial.print(bufferWH24P[3],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[2],BIN); Serial.print(" "); Serial.println(bufferWH24P[3],BIN);
//     currentData.windDirection = (bufferWH24P[3] << 1) & 256; currentData.windDirection = currentData.windDirection | (bufferWH24P[2]); //-- D(256) = B(0001 0000 0000)
//     Serial.println(currentData.windDirection,BIN);
//     Serial.print(F("Wind direction: ")); Serial.print(currentData.windDirection); Serial.println(F(" (degree)"));
//      if(currentData.windDirection < 22)
//       currentData.windDirectionCompass = "NORTH ";
//     else if (currentData.windDirection < 67)
//       currentData.windDirectionCompass = "NORTH-EAST ";
//     else if (currentData.windDirection < 112)
//       currentData.windDirectionCompass = "EAST ";
//     else if (currentData.windDirection < 157)
//       currentData.windDirectionCompass = "SOUTH-EAST ";
//     else if (currentData.windDirection < 212)
//      currentData.windDirectionCompass = "SOUTH ";
//     else if (currentData.windDirection < 247)
//       currentData.windDirectionCompass = "SOUTH-WEST ";
//     else if (currentData.windDirection < 292)
//      currentData.windDirectionCompass = "WEST ";
//     else if (currentData.windDirection < 337)
//       currentData.windDirectionCompass = "NORTH-WEST ";
//     else
//      currentData.windDirectionCompass = "NORTH ";  

//     Serial.println();
//     Serial.print(bufferWH24P[3],HEX); Serial.print(" "); Serial.print(bufferWH24P[4],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[3],BIN); Serial.print(" "); Serial.println(bufferWH24P[4],BIN);
//     unsigned int tempzz = ((bufferWH24P[3] << 8) & 1792) | (bufferWH24P[4]); //-- D(1792) = B(0111 0000 0000)
//     Serial.println(tempzz,BIN);
//     //--rumus temperatur = (tempz-400)/10
//     float tempz = (tempzz-400)/10.0;  //in 'C
//     Serial.print(F("Temperature: ")); Serial.print(tempz); Serial.println(F(" (degree celsius)"));

//     Serial.println();
//     Serial.print(bufferWH24P[5],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.println(bufferWH24P[5],BIN);
//     byte humz = bufferWH24P[5]; //in %
//     Serial.print(F("Humidity: ")); Serial.print(humz); Serial.println(F(" (%)"));

//     Serial.println();
//     Serial.print(bufferWH24P[6],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.println(bufferWH24P[6],BIN);
//     unsigned int windspdz = (bufferWH24P[3] << 4) & 256; windspdz = windspdz | bufferWH24P[6]; //-- D(256) = B(0001 0000 0000)
//     Serial.println(windspdz,BIN);
//     currentData.windSpeed = windspdz/0.14;  //in m/s
//     Serial.print(F("Wind speed: "));  Serial.print(currentData.windSpeed); Serial.println(F(" (m/s)"));

//     Serial.println();
//     Serial.print(bufferWH24P[7],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.println(bufferWH24P[7],BIN);
//     float gustspd = bufferWH24P[7]*1.12;  //in m/s
//     Serial.print(F("Gust speed: "));  Serial.print(gustspd); Serial.println(F(" (m/s)"));

//     Serial.println();
//     Serial.print(bufferWH24P[8],HEX); Serial.print(" "); Serial.print(bufferWH24P[9],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[8],BIN); Serial.print(" "); Serial.println(bufferWH24P[9],BIN);
//     unsigned int accrainfall = (bufferWH24P[8] << 8) | (bufferWH24P[9]); //in mm
//     Serial.println(accrainfall,BIN);
//     Serial.print(F("Accumulated rainfall: ")); Serial.print(accrainfall); Serial.println(F(" (mm)"));

//     Serial.println();
//     Serial.print(bufferWH24P[10],HEX); Serial.print(" "); Serial.print(bufferWH24P[11],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[10],BIN); Serial.print(" "); Serial.println(bufferWH24P[11],BIN);
//     unsigned int ultrav = (bufferWH24P[10] << 8) | (bufferWH24P[11]);  //in uW/cm2
//     Serial.println(ultrav,BIN);
//     Serial.print(ultrav); Serial.println(F(" (uW/cm2)"));

//     Serial.println();
//     Serial.print(bufferWH24P[12],HEX); Serial.print(" "); Serial.print(bufferWH24P[13],HEX); Serial.print(" "); Serial.print(bufferWH24P[14],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[12],BIN); Serial.print(" "); Serial.print(bufferWH24P[13],BIN); Serial.print(" "); Serial.println(bufferWH24P[14],BIN);
//     //unsigned long lightlux = 0; Serial.println(lightlux,BIN);  lightlux = (bufferWH24P[12] << 16) | (bufferWH24P[13] << 8); lightlux = (lightlux) | (bufferWH24P[14]);//in lux
//     unsigned long lightlux = 0; Serial.println(lightlux,BIN); 
//     lightlux = bufferWH24P[12]; lightlux = lightlux << 8; lightlux = lightlux | (bufferWH24P[13]); lightlux = lightlux << 8; lightlux = (lightlux) | (bufferWH24P[14]);//in lux
//     Serial.println(lightlux,BIN); Serial.println(lightlux,HEX);
//     float lightluxx = lightlux / 10.0;
//     Serial.print(lightluxx); Serial.println(F(" (lux)"));

//     Serial.println();
//     Serial.print(bufferWH24P[17],HEX); Serial.print(" "); Serial.print(bufferWH24P[18],HEX); Serial.print(" "); Serial.print(bufferWH24P[19],HEX); Serial.print(F(" <-HEX = BIN-> ")); Serial.print(bufferWH24P[17],BIN); Serial.print(" "); Serial.print(bufferWH24P[18],BIN); Serial.print(" "); Serial.println(bufferWH24P[19],BIN);
//     //unsigned long pressurexx = 0; Serial.println(pressurexx,BIN); pressurexx = (bufferWH24P[17] << 16) | (bufferWH24P[18] << 8); pressurexx = (pressurexx) | (bufferWH24P[19]); //in Pa
//     unsigned long pressurexx = 0; Serial.println(pressurexx,BIN);
//     pressurexx = bufferWH24P[17]; pressurexx = pressurexx << 8; pressurexx = pressurexx | bufferWH24P[18]; pressurexx = pressurexx << 8; pressurexx = (pressurexx) | (bufferWH24P[19]); //in Pa
//     Serial.println(pressurexx,BIN); Serial.println(pressurexx,HEX);
//     float pressurexz = pressurexx / 100.0;
//     Serial.print(pressurexz); Serial.println(F(" (hPa)"));
// }