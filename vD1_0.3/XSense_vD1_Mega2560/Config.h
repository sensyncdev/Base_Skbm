#include <Wire.h>
//#include <SerialCommand.h>
//#include <SoftwareSerial.h>
//#include <SimpleTimer.h>
//#include <Scheduler.h>
//#include <avr/wdt.h>

//-- Debug Message
#define ERROR(x)      Serial.print("\nERROR: "); Serial.println(x);
#define lcd_print(x)  Serial.println(x); lcd.print(x);
#define PRINT(x)      Serial.print(x);
#define PRINTLN(x)    Serial.println(x);
#define On            HIGH
#define Off           LOW
#define menit         60000
#define xsampling     10  // sampling 10x
#define Pump_Delay    5000

//--------------------------------------------------- Pin GPIO
#define LED_PIN   13
#define RST_PIN   3
#define PUMP_PIN  9
#define PUMP_RST  14
#define SYS_RST   8
#define BUZZER    41
#define PumpIsOn  LOW
#define PumpIsOff HIGH

//---data yang dikirim ke modem
struct templateData {
  int ppbCO;
  int ppbNO2;
  int ppbO3;
  int ppbSO2;
  float pm1;
  float pm25;
  float pm4;
  float pm10;
  float temp;
  float rhum;
  float pressure;
  int windSpeed;
  int windDirection;
  int irradianceUV;
  byte tanggal;
  byte bulan;
};
templateData currentData;

//start omitting
////--------------------------------------------- variable declaration
//char buffer[40];
//SimpleTimer   schedule;
//int scheduleTask1, scheduleTask2, scheduleTask3, scheduleTask4;
//
//int Counter, countSampling=1; //count for waiting NextLoop
//int errorCount=0, NxtLoop=1800;
//
////const int intrval_hbeat    = 1000;     // interval LED2 blink (mSec)
////const int intrval_chkStat  = 600000;  // interval Heartbeat (mSec)
////const int intrval_sampling = 1200000;  // interval get data from sensor (mSec)
////const int intrval_upload   = 1800000;  // interval upload data to cloud (mSec)
//
//const int ADC_uperLimit   = 4850; //approx 5V 
//const int ADC_lowerLimit  = 0;    //lower 0V
//const int ADC_max_10bit   = 1023; //max ADC 10bit resolution
//const int DefaultTimeout  = 5;    //seconds
//const int samplingDelay   = 50;  //milliSeconds
//
//String dataSensor[15];
//String dataValue ="";
//
//boolean OnProcess, Time2Upload; 
//boolean DebugMode=0;
//
////--------------------------------------------------- BME280
///*
//Connecting the BME280 Sensor:
//Sensor              ->  Board
//-----------------------------
//Vin (Voltage In)    ->  3.3V
//Gnd (Ground)        ->  Gnd
//SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
//SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro
//*/
//#include <Arduino.h>
//#include <Wire.h>
//#include <BMx280MI.h>
//
//#define I2C_ADDRESS 0x76
//#define BME_EN  15
//
////create a BMx280I2C object using the I2C interface with I2C Address 0x76
//BMx280I2C bmx280(I2C_ADDRESS);
//
//float data_temp = 0;
//float data_hum  = 0;
//float data_pres = 0;
//
//int wspd = 0;
//int wdir = 0;
//bool bmeok = false;
////--------------------------------------------------- Sensor DS18S20 Config
//#include <DallasTemperature.h>
//
//#define ONE_WIRE_BUS 5   // pin DS18S20
//OneWire oneWire(ONE_WIRE_BUS);
//// Pass our oneWire reference to Dallas Temperature.
//DallasTemperature ds1820(&oneWire);
//
//float tmpbase;
//
////--------------------------------------------------- ADC ADS1115 Config
//#include <Adafruit_ADS1015.h>
//
//Adafruit_ADS1115 ads1(0x48);    //GND sensor 1-6 CO  NO2
//Adafruit_ADS1115 ads2(0x49);    //SCL sensor 2-5 SO2 H2S
//Adafruit_ADS1115 ads3(0x4A);    //VCC sensor 3-4 O3  NO  
////Adafruit_ADS1115 ads0(0x4B);    //SDA sensor WindSpeed
//
////const int sensornumber = 6;     //Gas Sensor Number
//int16_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8, 
//        adc9, adc10, adc11;
//float o3_ppm, co_ppm, no2_ppm, so2_ppm, no_ppm, h2s_ppm; //=0;
//float o3_val, co_val, no2_val, so2_val, no_val, h2s_val; //=0;
//
////--------------------------------------------------- LCD config
//#include <LiquidCrystal_I2C.h>
//
//LiquidCrystal_I2C lcd(0x27,16,2);  
////LiquidCrystal_I2C lcd(0x3F,16,2);
////LiquidCrystal_I2C lcd(0x20,16,2);
//
//// -- initializing degree characters
//byte Celsius[8] = { B10000,B00110,B01001,B01000,B01000,B01001,B00110,B00000 };
//byte level1[8] = { B10000,B10000,B10000,B10000,B10000,B10000,B10000,B10000 };
//byte level2[8] = { B11000,B11000,B11000,B11000,B11000,B11000,B11000,B11000 };
//byte level3[8] = { B11100,B11100,B11100,B11100,B11100,B11100,B11100,B11100 };
//byte level4[8] = { B11110,B11110,B11110,B11110,B11110,B11110,B11110,B11110 };
//byte level5[8] = { B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111 };
//
////-- Heartbeat Array 
//long heartBeatArray[] = {50, 100, 15, 1200};
//int hbeatIndex = 1;   
//long prevMillis, idleTimer, tickTimer;
//int timer1_counter;
//
////--------------------------------------------------- Tiny RTC 1307 Config 
//String TimeStamp = "";
//byte second,minute,hour,weekday,day,month,year = 0;
//
//const int DS1307 = 0x68; // Address of DS1307 see data sheets
//const char* days[] =
//{"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
////{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//
//int idleTime = 0;
//int Jam, Mnt, Dtk;
//char timestamp[15];
//char filename[11];
//
////--------------------------------------------------- RTC DS3231 Config 
//#include <DS3231.h>
//#include <Wire.h>
//
//DS3231 rtc;
//
//const int addrsDS3231 = 0x68;
//bool Century=false;
//bool h12;
//bool PM, ADy, A12h, Apm;
//byte ADay, AHour, AMinute, ASecond, ABits;
//bool logOut=0;
//
////--------------------------------------------------- Winsen ZH03A
//#define PMDataLength  31   //0x42 + 31 bytes equal to 32 bytes
//#define Winsen_Port   Serial2
//
//unsigned char zh03_buf[PMDataLength];
//
//float PM01_Val = 0;
//float PM25_Val = 0;
//float PM40_Val = 0;
//float PM10_Val = 0;
//String pm01_val, pm25_val, pm04_val, pm10_val;
//
////--------------------------------------------------- ESP8266 Config
////#include "ESP8266AT.h" 
////-- Serial for ESP8266 Wemos D1 mini
////-- Use Software Serial
//#define ESP_TX  10
//#define ESP_RX  11
//#define ESP_RST 6
//#define ESP_INT 12
//SoftwareSerial espSerial(ESP_TX, ESP_RX);
////-- Use Hardware Serial
////#define espSerial Serial1
//
////-- ESP Buffer
//char bufferRX[200];
//int indexBufferRX;
//bool statConnect;
//
////-- WiFi credentials
////const String wifiSSID = "Get Sensync";
//const String wifiSSID = "SENSYNCBASECRBN";
//const String wifiPASS = "makanminggu12";
//
////ESPAT ESP8266(wifiSSID, wifiPASS); 
//
////-- Server link
//const String id         = "xx-bplhd-jabar-crbn";
//const String url_server = "http://server.getsensync.com/proc/bplhd_jabar/base/process.php?id=";
//const String url_login  = "http://server.getsensync.com/proc/bplhd_jabar/base/start.php?id=";
//const String dasboard   = "http://secure.getsensync.com/proc/bplhd_jabar/base/process.php";
//const String url_start  = "http://secure.getsensync.com/proc/bplhd_jabar/base/start.php?id=";
//const String server     = "http://secure.getsensync.com";
//const String url_upload = "/proc/bplhd_jabar/base/process.php?id=";
// 
////--------------------------------------------------- Debug Config 
//#define EnableDebug
//#define SerialMon Serial
//SerialCommand SCmd;           // The SerialCommand object
//
////--------------------------------------------------- Davis Anemotemer 
////#include "TimerOne.h"         // Timer Interrupt set to 2 second for read sensors
//#define WindSensorPin 2         // The pin location of the anemometer sensor
//#define WindVanePin   A0        // The pin the wind vane sensor is connected to
//#define VaneOffset    0         // define the anemometer offset from magnetic north
//
//float windSpd;                  // speed miles per hour
//int VaneValue;                  // raw analog value from wind vane
//int windDir;                    // translated 0 - 360 direction
//int CalDirection;               // converted value with offset applied
//
//volatile bool RotationCount=false;              // this is set true every 2.5s. Get wind speed
//volatile unsigned int LastValue=0;
//volatile unsigned int  TimerCount=0;        // used to determine 2.5sec timer count
//volatile unsigned long Rotations=0;         // cup rotation counter used in interrupt routine
//volatile unsigned long ContactBounceTime; // Timer to avoid contact bounce in interrupt routine

//end omitting

//--------------------------------------------------- Sensirion SPS30 Config 
#include "Wire.h"
#include "sps30.h"
#define SP30_COMMS I2C_COMMS

int Address = 0x69; // device address of SPS30 (fixed).
byte w1, w2,w3;
byte ND[60];
long sps_tmp;
float sps_measure;
String data_pm[6];

//---------------------------------------------------- BME280 Config
#include "Wire.h"
#include "SparkFunBME280.h"

//--------------------------------------------------- RTC DS3231
#include "DS3231_Simple.h"
DS3231_Simple Clock;
DateTime waktu;
// int waktuDay,waktuMonth,waktuYear;

//--------------------------------------------------- TFT ST7735 1.8"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#define TFT_CS        24 // Hallowing display control pins: chip select
#define TFT_RST       25 // Display reset
#define TFT_DC        23 // Display data/command select
#define TFT_SCK       52
#define TFT_SDA       51
#define TFT_BACKLIGHT  22 // Display backlight pin
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
