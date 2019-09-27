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

const int SS1 = 10; //MCP3008 slave select (must be low for conversation)
const int SS2 = 9;  //SD reader slave select

unsigned long T;    //Wall clock time
int RTDR;           //A1
int RTD1;           //A7
int RTD2;           //A6
int RTD3;           //A5
int SGR;            //A0
int SG1;            //A4
int SG2;            //A3
int X;              //Channel 1 ADC
int Y;              //Channel 2 ADC
int Z;              //Channel 3 ADC

char data_buffer[200]; //Buffer for writing data to before saving or transmitting via Serial port.
File file;

void setup() {
  Serial.begin(9600); // Serial connection for debugging
  analogReference(EXTERNAL); // Set voltage reference to external 
  
  pinMode(3, OUTPUT);
  pinMode(SS2, OUTPUT);
  
  digitalWrite(3, HIGH); // Turn on status LED
  
  adc.begin(13, 11, 12, SS1);
  SD.begin(SS2);

  file = SD.open("flightlog.csv", FILE_WRITE); // Create the flightlog file and write a reference string to it
  if (file) {
    file.println("T,RTD1,RTD2,RTD3,RTDR,SG1,SG2,SGR,X,Y,Z");
    file.close();
  }
}

void loop() {
  //Record a time stamp and read all sensors
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

  // Use sprintf to fill the buffer with the data in a csv format
  sprintf(data_buffer, "%ul,%i,%i,%i,%i,%i,%i,%i,%i,%i", T, RTD1, RTD2, RTD3, RTDR, SG1, SG2, SGR, X, Y, Z);
  Serial.println(data_buffer);
  Serial.flush();

  //save data to flightlog
  if (SD.exists("flightlog.csv")) {
    file = SD.open("flightlog.csv", FILE_WRITE);
    if (file) {
      file.println(data_buffer);
      file.close();
    }
  }
}
