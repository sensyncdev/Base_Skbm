void get_4dgsspec_v2() {
  Serial.println("-------get_4dgsspec() init");
  Serial.println(" Sampling start..");
  for (int c=28;c<=31;c++) {  //read data from 4 sensor by multiplexing the uart through enable pin. pin 28-31 are EN_SPEC pin
   for (int i=28;i<=31;i++) { //set all enable pin low
    digitalWrite(i, LOW); 
   }
   digitalWrite(c, HIGH); //set enable pin for specified sensor
   Serial1.write("\r"); //send wakeup-from-low-power-sleep command

   Serial.print("  Data from sensor #");Serial.print(c-27);
   switch (c) {
      case 28: Serial.print(" (CO)");break;
      case 29: Serial.print(" (NO2)");break;
      case 30: Serial.print(" (O3)");break;
      case 31: Serial.print(" (SO2)");break;
   }
   Serial.print(": ");

   Serial1.flush();
   bool isDataReceived = false;
   // long int whenDataReceived = millis();
   // bool isDataTimeout = false;
   String dariSensor;
   
  
  // SerialFlashln("Start Measuring NO2");
  // TFTFlashln("Start Measuring NO2");
  Serial1.write('\r');
  unsigned long waktuLama = millis();
  while ((!isDataReceived) && (millis() - waktuLama <= 5000)){
    if (millis() % 1000 == 0){
      Serial1.write('\r');
    }
    if (Serial1.available() > 0){
      char x = Serial1.read();
      if (x == '\n'){
        isDataReceived = true;
      } else {
        if (x != ' '){
          dariSensor += x;
        }
      }
    }
  }

   if(!isDataReceived) {
    Serial.println("(Tidak terbaca. Timeout setelah 5 detik)");
    switch (c) {
      case 28: currentData.ppbCO = 0; Serial.print("    currentData.ppbCO updated with value: "); Serial.println(currentData.ppbCO);break;
      case 29: currentData.ppbNO2 = 0; Serial.print("    currentData.ppbNO2 updated with value: "); Serial.println(currentData.ppbNO2);break;
      case 30: currentData.ppbO3 = 0; Serial.print("    currentData.ppbO3 updated with value: "); Serial.println(currentData.ppbO3);break;
      case 31: currentData.ppbSO2 = 0; Serial.print("    currentData.ppbSO2 updated with value: "); Serial.println(currentData.ppbSO2);break;
    }
   }
   else {
    Serial.println(dariSensor);

//--parsing
    String arrayData[11];
    for(int b=0;b<11;b++){
      arrayData[b]="";
    }
    int dataKe = 0;
    for(int i = 0; i < dariSensor.length();i++){
      if(dariSensor[i] != ','){
        arrayData[dataKe] += dariSensor[i];
      } else {
        dataKe++;
      }
    }
//--end parsing

//--display hasil parsing
//    Serial.println("   Parsed data: ");
//    for(int n=0;n<11;n++){
//      switch (n) {
//        case 0: Serial.print("    SN:");break;
//        case 1: Serial.print("    PPB:");break;
//        case 2: Serial.print("    Temp ('C):");break;
//        case 3: Serial.print("    RH (%):");break;
//        case 4: Serial.print("    ADC Raw:");break;
//        case 5: Serial.print("    Temp Raw:");break;
//        case 6: Serial.print("    RH Raw:");break;
//        case 7: Serial.print("    Day:");break;
//        case 8: Serial.print("    Hour:");break;
//        case 9: Serial.print("    Minute:");break;
//        case 10: Serial.print("    Second:");break;
//      }
//      Serial.println(arrayData[n]);
//    }
//--end display hasil parsing

//--input hasil parsing ppb
    int datappb = arrayData[1].toInt();
    //Serial.println(datappb);
    switch (c) {
      case 28: currentData.ppbCO = datappb; Serial.print("    currentData.ppbCO updated with value: "); Serial.println(currentData.ppbCO);break;
      case 29: currentData.ppbNO2 = datappb; Serial.print("    currentData.ppbNO2 updated with value: "); Serial.println(currentData.ppbNO2);break;
      case 30: currentData.ppbO3 = datappb; Serial.print("    currentData.ppbO3 updated with value: "); Serial.println(currentData.ppbO3);break;
      case 31: currentData.ppbSO2 = datappb; Serial.print("    currentData.ppbSO2 updated with value: "); Serial.println(currentData.ppbSO2);break;
    }
   }

//--set all enable pin low
   for (int i=28;i<=31;i++) {
    digitalWrite(i, LOW); 
   }
   delay(100);
  }
  Serial.println("-------get_4dgsspec() end");
}


//--------------------yg bawah yg lama

// void get_4dgsspec() {
//   Serial.println("-------get_4dgsspec() init");
//   Serial.println(" Sampling start..");
//   for (int c=28;c<=31;c++) {  //read data from 4 sensor by multiplexing the uart through enable pin. pin 28-31 are EN_SPEC pin
//    for (int i=28;i<=31;i++) { //set all enable pin low
//     digitalWrite(i, LOW); 
//    }
//    digitalWrite(c, HIGH); //set enable pin for specified sensor
//    Serial1.write("\r"); //send wakeup-from-low-power-sleep command

//    Serial.print("  Data from sensor #");Serial.print(c-27);
//    switch (c) {
//       case 28: Serial.print(" (CO)");break;
//       case 29: Serial.print(" (NO2)");break;
//       case 30: Serial.print(" (O3)");break;
//       case 31: Serial.print(" (SO2)");break;
//    }
//    Serial.print(": ");

//    Serial1.flush();
//    bool isDataReceived = false;
//    long int whenDataReceived = millis();
//    bool isDataTimeout = false;
//    String dariSensor;
   
//    while (!isDataReceived && !isDataTimeout) {
//       Serial1.write("\r"); //request-measurement command
//       if(Serial1.available()) {
//         dariSensor = Serial1.readStringUntil("\n");
//       }
//       // if(dariSensor.length() > 10) {
//       //   int commaCount = 0;
//       //   for(int cc = 0; cc < dariSensor.length(); cc++) {
//       //     if (dariSensor[cc] == ',') {commaCount++;}
//       //   }
//       //   if(commaCount == 10) {
//       //     isDataReceived = true;
//       //   }
//       // }
//       if(dariSensor.length() > 10) {
//         isDataReceived = true;
//       }
//       if((millis() - whenDataReceived) > 5000) {
//         isDataTimeout = true;
//       }
//       delay(50);
//    }

//    if(isDataTimeout) {
//     Serial.println("(Tidak terbaca. Timeout setelah 5 detik)");
//     switch (c) {
//       case 28: currentData.ppbCO = 0; Serial.print("    currentData.ppbCO updated with value: "); Serial.println(currentData.ppbCO);break;
//       case 29: currentData.ppbNO2 = 0; Serial.print("    currentData.ppbNO2 updated with value: "); Serial.println(currentData.ppbNO2);break;
//       case 30: currentData.ppbO3 = 0; Serial.print("    currentData.ppbO3 updated with value: "); Serial.println(currentData.ppbO3);break;
//       case 31: currentData.ppbSO2 = 0; Serial.print("    currentData.ppbSO2 updated with value: "); Serial.println(currentData.ppbSO2);break;
//     }
//    }
//    else {
//     Serial.print(dariSensor);

// //--parsing
//     String arrayData[11];
//     for(int b=0;b<11;b++){
//       arrayData[b]="";
//     }
//     int dataKe = 0;
//     for(int i = 0; i < dariSensor.length();i++){
//       if(dariSensor[i] != ','){
//         arrayData[dataKe] += dariSensor[i];
//       } else {
//         dataKe++;
//       }
//     }
// //--end parsing

// //--display hasil parsing
// //    Serial.println("   Parsed data: ");
// //    for(int n=0;n<11;n++){
// //      switch (n) {
// //        case 0: Serial.print("    SN:");break;
// //        case 1: Serial.print("    PPB:");break;
// //        case 2: Serial.print("    Temp ('C):");break;
// //        case 3: Serial.print("    RH (%):");break;
// //        case 4: Serial.print("    ADC Raw:");break;
// //        case 5: Serial.print("    Temp Raw:");break;
// //        case 6: Serial.print("    RH Raw:");break;
// //        case 7: Serial.print("    Day:");break;
// //        case 8: Serial.print("    Hour:");break;
// //        case 9: Serial.print("    Minute:");break;
// //        case 10: Serial.print("    Second:");break;
// //      }
// //      Serial.println(arrayData[n]);
// //    }
// //--end display hasil parsing

// //--input hasil parsing ppb
//     int datappb = arrayData[1].toInt();
//     //Serial.println(datappb);
//     switch (c) {
//       case 28: currentData.ppbCO = datappb; Serial.print("    currentData.ppbCO updated with value: "); Serial.println(currentData.ppbCO);break;
//       case 29: currentData.ppbNO2 = datappb; Serial.print("    currentData.ppbNO2 updated with value: "); Serial.println(currentData.ppbNO2);break;
//       case 30: currentData.ppbO3 = datappb; Serial.print("    currentData.ppbO3 updated with value: "); Serial.println(currentData.ppbO3);break;
//       case 31: currentData.ppbSO2 = datappb; Serial.print("    currentData.ppbSO2 updated with value: "); Serial.println(currentData.ppbSO2);break;
//     }
//    }

// //--set all enable pin low
//    for (int i=28;i<=31;i++) {
//     digitalWrite(i, LOW); 
//    }
//    delay(100);
//   }
//   Serial.println("-------get_4dgsspec() end");
// }
