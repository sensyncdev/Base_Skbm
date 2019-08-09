/*--- lama
void get_noiseVal() {
  int noiseVal;
  for(int t=0;t<20;t++){
    noiseVal = analogRead(13);
    Serial.print(F("Analog value of noise read is: ")); Serial.println(noiseVal);
    delay(500);
  }
}
*/

// #define AnalogNoiseSensor_PIN A15

int acquire_noiseVal() {
	const byte noiseBufferSize = 10;

	int noiseBuffer[noiseBufferSize];
	int noiseAveraging = 0;
	Serial.print(F("Sampling noiseVal for ")); Serial.print(noiseBufferSize); Serial.println(F(" times"));
	for(int i=0;i<noiseBufferSize;i++){
		noiseBuffer[i] = analogRead(AnalogNoiseSensor_PIN);
		noiseAveraging = ((noiseAveraging * i) + noiseBuffer[i])/(i+1);

		Serial.print(F("Sample #")); Serial.print(i);
		Serial.print(F(" ADC count=")); Serial.print(noiseBuffer[i]);
		Serial.print(F(" Average value= ")); Serial.println(noiseAveraging);
		delay(500);
	}
	return noiseAveraging;
}

void get_noiseVal() {
	int noiseVal = acquire_noiseVal();
	Serial.print(F("Noise value is: ")); Serial.print(noiseVal); Serial.println(F(" (ADC count)"));
}

// /*---example sketch
// void setup() {
// 	Serial.begin(9600);
// 	pinMode(AnalogNoiseSensor_PIN,OUTPUT);
// }

// void loop() {
// 	int noiseVal = acquire_noiseVal();
// 	Serial.print(F("Noise value is: ")); Serial.print(noiseVal); Serial.println(F(" (ADC count)"));
// 	delay(500);
// }
// */