BME280 bme280;

void get_bme280_vD(){
  Serial.println("-------get_bme280_vD() init");
  Wire.begin();

  if (bme280.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println(" The sensor did not respond. Please check wiring.");
    Serial.println(" Sampling failed.");
  } else {
      Serial.println(" Sampling start.");
      for(int c = 0; c < 5; c++) {
        Serial.print("  Sample #"); Serial.print(c);
        Serial.print(" Humidity: ");
        Serial.print(bme280.readFloatHumidity(), 0);
      
        Serial.print(" Pressure: ");
        Serial.print(bme280.readFloatPressure(), 0);
      
        Serial.print(" Alt: ");
        //Serial.print(bme280.readFloatAltitudeMeters(), 1);
        Serial.print(bme280.readFloatAltitudeFeet(), 1);
      
        Serial.print(" Temp: ");
        Serial.print(bme280.readTempC(), 2);
        //Serial.print(bme280.readTempF(), 2);
      
        Serial.println();
      }
      Serial.println(" Sampling end.");
  }
  currentData.rhum = bme280.readFloatHumidity();
  Serial.print(" currentData.rhum updated with value: "); Serial.println(currentData.rhum);
  
  currentData.pressure = bme280.readFloatPressure();
  Serial.print(" currentData.pressure updated with value: "); Serial.println(currentData.pressure);
  
  currentData.temp = bme280.readTempC();
  Serial.print(" currentData.rhum updated with value: "); Serial.println(currentData.temp);

  Serial.println("-------get_bme280_vD() end");
}
