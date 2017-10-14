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

int analogInput = A5; //RiCor: edited to port A5 used in my setup.
float vout = 0.0;
float vin = 0.0;
float R1 = 98500.0; /* resistance of R1 (100K) (RiCor: Connected between the battery and the Arduino pin. 
                                                Measured 98.5 ohms. Corrected the R1 value in the sketch) */

float R2 = 10000.0; /* resistance of R2 (10K) (RiCor: Connected between the Arduino pin and ground
                                               Measured 10 ohms on the resistor I used so it's OK) */
int value = 0;
float reference = (INTERNAL); /* RiCor: Reference voltage. Using the INTERNAL reference (3V is the case of the 
                                 Arduino does not work. A value of 2.5(V) was perfect in this case */


void setup()
{
  analogReference(INTERNAL); /* RiCor: set the reference to the internal reference since the Arduino 
                                is powered from the battery that it is measuring. */
  pinMode(analogInput, INPUT);
}


void loop()
{
  // read the value at analog input
  value = analogRead(analogInput);
  vout = (value * reference) / 1024.0; // see text
  vin = vout / (R2/(R1+R2)); 

  Serial.print("The internal reference voltage is: ");   /* RiCor: Output internal reference voltage */ 
  Serial.println(reference);
  
  Serial.print("The battery voltage is: ");   /* RiCor: Output to serial instead of LCD */ 
  Serial.println(vin);
    
  delay(500);
}
