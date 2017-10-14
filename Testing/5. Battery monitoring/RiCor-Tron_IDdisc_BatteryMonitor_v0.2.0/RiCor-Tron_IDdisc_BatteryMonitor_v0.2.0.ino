/********************************************************************************************************************************
Rik van der Mark, www.RiCor.net 2017
Arduino based Voltmeter to monitor a battery whilst powering that Arduino using that same battery.

Sketch was based on / inspired by sketches from:
- Wawa (https://forum.arduino.cc/index.php?topic=92074.15 - Post #21)
- T.K.Hareendran (http://www.electroschematics.com/9351/arduino-digital-voltmeter/)
Modified based on needs for my Identity Disc project.

* 0 - 30V voltmeter (with 3V Arduino's) or 0 - 50V voltmeter (when used with 5V Arduino's) (BE CAREFUL!)
* Maximum input voltage depends on the resistors used. The 10k and 100k resistors used here "divide" the input voltage by 11
* uses the internal voltage reference since the Arduino runs from the battery that is being measured
* 100k resistor from A1 to the postive (+) battery terminal
* 10k resistor from A1 to ground
* optional 100nF capacitor (ceramic capacitor marked with 104) from Arduino pin connected to the voltage devider to ground for more stable readings 

< End of Line >
********************************************************************************************************************************/
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 98500.0; // resistance of R1 (100K) -see text! (RiCor, measured 98.5 ohms)
float R2 = 10000.0; // resistance of R2 (10K) - see text! (RiCor, measured 10 ohms)
int value = 0;


void setup()
{
  pinMode(analogInput, INPUT);
  lcd.begin(16, 2);
  lcd.print("DC VOLTMETER");
}


void loop()
{
  // read the value at analog input
  value = analogRead(analogInput);
  vout = (value * 3.0) / 1024.0; // see text
  vin = vout / (R2/(R1+R2)); 
  if (vin<0.09) 
  {
    vin=0.0;//statement to quash undesired reading !
  } 
  /* RiCor: Output to serial instead of LCD */
  Serial.print("The battery voltage is: ");
  Serial.println(vin);
    
  delay(500);
}
