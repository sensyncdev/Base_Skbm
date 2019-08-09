void get_noiseVal() {
  int noiseVal;
  for(int t=0;t<20;t++){
    noiseVal = analogRead(13);
    Serial.print(F("Analog value of noise read is: ")); Serial.println(noiseVal);
    delay(500);
  }
}