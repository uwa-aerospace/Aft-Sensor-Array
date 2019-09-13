/*
   ===================================
       Aft Sensor Array Datalogger

             by Toby Digney
   ===================================
*/

#include <SPI.h>
#include <SD.h>
#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc;

//define all variables
const int SS1 = 10; //MCP3008 slave select (must be low for conversation)
const int SS2 = 9; //SD reader slave select
unsigned long T;
int RTDR; //A1
int RTD1; //A7
int RTD2; //A6
int RTD3; //A5
int SGR; //A0
int SG1; //A4
int SG2; //A3
int X; //Channel 1 ADC
int Y; //Channel 2 ADC
int Z; //Channel 3 ADC
String data;
File file;

void setup() { 
  analogReference(EXTERNAL); //set voltage reference to external 
  pinMode(3,OUTPUT);
  pinMode(SS2,OUTPUT);
  digitalWrite(3, HIGH); //turn on status LED
  adc.begin(13, 11, 12, SS1);
  SD.begin(SS2);
  file=SD.open("flightlog.csv",FILE_WRITE); //create the flightlog file and write a reference string to it
  if(file) {
    file.println("T,RTD1,RTD2,RTD3,RTDR,SG1,SG2,SGR,X,Y,Z");
    file.close();
  }
}

void loop() {
  
  //record a time stamp and read all sensors
  T = millis();
  RTDR = analogRead(1);
  RTD1 = analogRead(7);
  RTD2 = analogRead(6);
  RTD3 = analogRead(5);
  SGR = analogRead(0);
  SG1 = analogRead(4);
  SG2 = analogRead(3);
  X = adc.readADC(1);
  Y = adc.readADC(2);
  Z = adc.readADC(3);

  //create an empty string and concatenate all data to it seperating values with commas to facilitate csv format
  data = "";
  data.concat(T);
  data.concat(",");
  data.concat(RTD1);
  data.concat(",");
  data.concat(RTD2);
  data.concat(",");
  data.concat(RTD3);
  data.concat(",");
  data.concat(RTDR);
  data.concat(",");
  data.concat(SG1);
  data.concat(",");
  data.concat(SG2);
  data.concat(",");
  data.concat(SGR);
  data.concat(",");
  data.concat(X);
  data.concat(",");
  data.concat(Y);
  data.concat(",");
  data.concat(Z);

  //save date to flightlog
  if(SD.exists("flightlog.csv")) {
    file=SD.open("flightlog.csv",FILE_WRITE);
    if(file) {
      file.println(data);
      file.close();
    }
  }
}