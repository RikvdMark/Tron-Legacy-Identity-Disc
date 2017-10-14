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
float Vout = 0.0;
float Vin = 0.0;
float R1 = 98500.0; /* resistance of R1 (100K) (RiCor: Connected between the battery and the Arduino pin. 
                                                Measured 98.5 ohms. Corrected the R1 value in the sketch) */
float R2 = 10000.0; /* resistance of R2 (10K)  (RiCor: Connected between the Arduino pin and ground
                                                Measured 10 ohms on the resistor I used so it's OK) */
int Vdivider = 0; /* RiCor: variable used to store the recorded value from the voltage divider */
//int VdivOut = 0; /* RiCor: messing around with the "Vdivider" value */
float reference = 2.5; /* RiCor: Reference voltage */


void setup()
{
  analogReference(INTERNAL); /* RiCor: set the reference to the internal reference since the Arduino 
                                is powered from the battery that it is measuring. */
  pinMode(analogInput, INPUT); /* RiCor: Configure the pin connected to the voltage devider as an iput pin */
}


void loop()
{
  // read the value at analog input
  Vdivider = analogRead(analogInput); /* RiCor: read the value of the pin connected to the voltage divider and store it 
                                          in the "vdivider" variable */
  // THE 2 LINES BELOW AREN'T USEFULL, THE VALUE GET'S ROUNDED DOWN TO 0 Volts.
  //VdivOut = Vdivider / 1024.0;      /* RiCor: Convert the value to volts */ 
  //Serial.println("The measured value is: " + String(VdivOut) + " V");   /* RiCor: Output the "vdivider" value to serial */               
                                      
  Vin = (Vdivider * reference) / 1024.0; /* RiCor: get the "Vin" (Voltage input) value by multiplying the "measured value" 
                                             with the "reference voltage" and dividing that value by 1024 to convert to volts */
  Vout = Vin / (R2/(R1+R2)); /* RiCor: Use Ohms law to calculate the "Vout" value */
  
  Serial.println("The battery voltage is: " + String(Vout) + " V");   /* RiCor: Output the "Vout" value to serial */
   
  delay(1000); /* RiCor: wait 1 second */
}
