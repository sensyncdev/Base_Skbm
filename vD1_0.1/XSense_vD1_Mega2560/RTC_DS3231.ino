void get_rtcds3231() {
 Serial.println("-------get_rtcds3231() start");
 Clock.begin();
 DateTime waktu;

 Serial.println(" Sampling start..");
 for(int iterasi = 0; iterasi<1; iterasi++){
   waktu = Clock.read();
   Serial.print("  Sample #"); Serial.print(iterasi+1); Serial.print(": ");
   Serial.print(waktu.Day);
   waktuDay = (waktu.Day - 32);
   Serial.print("/");
   Serial.print(waktu.Month);
   Serial.print("/");
   Serial.print(waktu.Year);
   Serial.print(" ");
   Serial.print(waktu.Hour);
   Serial.print(":");
   Serial.print(waktu.Minute);
   Serial.print(":");
   Serial.println(waktu.Second);
 }

 waktuMonth = waktu.Month;
 waktuYear = waktu.Year;

 Serial.println(" Sampling end..");
 Serial.println("-------get_rtcds3231() end");
}
