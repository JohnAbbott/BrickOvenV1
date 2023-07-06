/*
 * Read all the thermocouples in the brick oven.
 *  Display them on my phone using Virtuino
 * Copyright Joseph Grey and the person who invented him
 * 
 * Version 1.0 -- May 2023
 * 
 */

#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_MAX31855.h"

#define MAXDO   8
#define MAXCS   10
#define MAXCLK  9

LiquidCrystal_I2C lcd(0x3F,20,4);
LiquidCrystal_I2C lcd2(0x27,20,4);
// A5 (SCL) and A4 (SDA).

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);
  //DO connects to digital 8, CS connects to digital 10, and CLK connects to pin 9
//Mux control pins
int s0 = 4;
int s1 = 5;
int s2 = 6;
int s3 = 7;

int currentTemp;

void setup(){
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  Serial.begin(9600);
// initialize and clear display
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
// initialize and clear display
  lcd2.init();                      // initialize the lcd 
  lcd2.init();
  lcd2.backlight();

delay(500);
  Serial.print("Initializing sensor...");
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }


}

void loop(){

  //Loop through and read all 16 values
  for(int i = 0; i < 4; i ++){
    Serial.print("Channel ");
    Serial.println(i);
    readMux(i);
    delay(1000);  // Allows the multiplexer to settle for a millisecond before reading.
     Serial.print("Fahrenheit = ");
     //Serial.println(thermocouple.readInternal());
     
     currentTemp = round(thermocouple.readFahrenheit());
     Serial.println(currentTemp);
    double c = thermocouple.readCelsius();
    if (isnan(c)) {
     Serial.println("Thermocouple fault(s) detected!");
     uint8_t e = thermocouple.readError();
     if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
     if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
     if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
    } else {
     Serial.print("C = ");
     Serial.println(c);
     
     if (i == 0){
       Serial.println("inside i equals 0");
     lcd.setCursor(3,0);
     lcd.print("Dome Top");
     lcd.setCursor(12,0);
     lcd.print("     ");
     lcd.setCursor(12,0);
     lcd.print(currentTemp);
    } else if (i == 1){
      Serial.println("inside i equals 1");
     lcd.setCursor(1,1);
     lcd.print("Dome Middle");
     lcd.setCursor(13,1);
     lcd.print("     ");
    lcd.setCursor(13,1);
     lcd.print(currentTemp);
    } else if (i == 2){
     Serial.println("inside i equals 2");
     lcd.setCursor(1,2);
     lcd.print("Dome Front");
     lcd.setCursor(13,2);
     lcd.print("     ");
     lcd.setCursor(13,2);
     lcd.print(currentTemp);
    } else if (i == 3){
      Serial.println("inside display to second LCD");
      lcd2.setCursor(1, 1);
      lcd2.print("hello");
    }
   
   }

    //delay(1000);
  }

}


float readMux(int channel){
 // Serial.println("Inside Read MUX");
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
    //Serial.println("inside the channel change");
    //delay(1000);
  }

}
