SPS30 sps30;

void get_sps30_vD() {
  Serial.println("-------get_sps30_vD() init");
  //--disable BME280
//  pinMode(15,OUTPUT);
//  digitalWrite(15,LOW);

//  // set driver debug level
  sps30.EnableDebugging(0);
//  /////////////////////////////////////////////////////////////
///* define driver debug
// * 0 : no messages
// * 1 : request sending and receiving
// * 2 : request sending and receiving + show protocol errors */
// //////////////////////////////////////////////////////////////
//
  // Begin communication channel;
  if (sps30.begin(SP30_COMMS) == false) {
    Serial.println(" Could not initialize communication channel.");
  }
//
  // check for SPS30 connection
  if (sps30.probe() == false) {
    Serial.println(" Could not probe / connect with sensor (SPS30).");
  }
  else {
    Serial.println(F(" Sensor (SPS30) detected."));
    // reset SPS30 connection
    if (sps30.reset() == false) {
      Serial.println(" Could not reset.");
    }
  
    if (sps30.start() == true)
      Serial.println(F(" Sampling start.."));
    else
      Serial.println(" Could not start measurement");
  
    if (SP30_COMMS == I2C_COMMS) {
      if (sps30.I2C_expect() == 4)
        Serial.print(F(""));
    }
    
    uint8_t ret = 0;
    struct sps_values val;
  
    for(int c = 0; c < 5; c++){
      ret = sps30.GetValues(&val);
      while(ret != ERR_OK){
        if (ret == ERR_DATALENGTH){
          Serial.println("Error during reading values: ");
          return(false);
          delay(1000);
      }
  
        // if other error
        else if(ret != ERR_OK) {
          Serial.println("Error during reading values: ");
          return(false);
        }
        ret = sps30.GetValues(&val);
      }

      Serial.print(F("  Sample #")); Serial.print(c+1);
      Serial.print(F(" PM1.0: "));
      Serial.print(val.MassPM1);
      Serial.print(F(" PM2.5: "));
      Serial.print(val.MassPM2);
      Serial.print(F(" PM4.0: "));
      Serial.print(val.MassPM4);
      Serial.print(F(" PM10: "));
      Serial.print(val.MassPM10);
      Serial.print(F("\n"));
    }
    Serial.println(" Sampling end");
    
    currentData.pm1 = val.MassPM1;
    Serial.print(" currentData.pm1 updated with value: "); Serial.println(currentData.pm1);

    currentData.pm25 = val.MassPM2;
    Serial.print(" currentData.pm25 updated with value: "); Serial.println(currentData.pm25);

    currentData.pm4 = val.MassPM4;
    Serial.print(" currentData.pm4 updated with value: "); Serial.println(currentData.pm4);

    currentData.pm10 = val.MassPM10;
    Serial.print(" currentData.pm10 updated with value: "); Serial.println(currentData.pm10);
  }
  
  Serial.println("get_sps30_vD() end");
}
