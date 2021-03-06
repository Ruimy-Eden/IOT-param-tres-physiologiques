#include <SoftwareSerial.h> // va creer un serial virtuel
#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
 
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
SoftwareSerial BTSerial(2, 3); // RX | TX  = > BT-TX=2 BT-RX=3
 
PulseOximeter pox;
//byte beat=pox.getHeartRate();
//byte spodeux=pox.getSpO2();
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    //Serial.println("Beat!");
}
 
void setup()
{
  
    Serial.begin(9600);
    BTSerial.begin(9600); 

    Serial.print("Initializing pulse oximeter..");
    lcd.begin(16,2);
    lcd.print("Initializing...");
    delay(3000);
    lcd.clear();
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{    
  
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > 1000) {
        //Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        BTSerial.print(pox.getHeartRate());
        Serial.print("&");
        BTSerial.print("&");
        //Serial.print("bpm / SpO2:");
        Serial.println(pox.getSpO2());
        BTSerial.println(pox.getSpO2());
        //Serial.println("%");
 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BPM : ");
        lcd.print(pox.getHeartRate());
        
        lcd.setCursor(0,1);
        lcd.print("SpO2: ");
        lcd.print(pox.getSpO2());
        lcd.print("%");
        
        tsLastReport = millis();
    }
}
