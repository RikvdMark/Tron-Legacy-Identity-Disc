/********************************************************************************************************************************
Rik van der Mark, www.RiCor.net 2017
Arduino based Voltmeter to monitor a battery whilst powering that Arduino using that same battery.

Sketch was based on / inspired by sketches from:
- Wawa (https://forum.arduino.cc/index.php?topic=92074.15 - Post #21)
- T.K.Hareendran (http://www.electroschematics.com/9351/arduino-digital-voltmeter/)
Modified and expanded based on needs for my Identity Disc project.

* 0 - 30V voltmeter (with 3V Arduino's) or 0 - 50V voltmeter (when used with 5V Arduino's) (BE CAREFUL!)
* Maximum input voltage depends on the resistors used. The 10k and 100k resistors used here "divide" the input voltage by 11
* uses the internal voltage reference since the Arduino runs from the battery that is being measured
* 100k resistor from A1 to the postive (+) battery terminal
* 10k resistor from A1 to ground
* optional 100nF capacitor (ceramic capacitor marked with 104) from Arduino pin connected to the voltage devider to ground for more stable readings 

< End of Line >
********************************************************************************************************************************/

#define analogInput A5 /* Pin connected to the voltage divider */
#define LowBattLED A4 /* Connected a red LED to this pin */
#define PowerCutOff A0 /* At this stage a white LED is connected but in the final setup the pin will connect
                          to the "kill pin" on the Adafruit Push-button Power Switch Breakout board 
                          this way the Arduino can power off the entire thing to save the battery */

/*********************************/
float Vout = 0.0;
float Vin = 0.0;
float R1 = 98500.0; /* resistance of R1 (100K) (RiCor: Connected between the battery and the Arduino pin. 
                                                Measured 98.5 ohms. Corrected the R1 value in the sketch) */
float R2 = 10000.0; /* resistance of R2 (10K)  (RiCor: Connected between the Arduino pin and ground
                                                Measured 10 ohms on the resistor I used so it's OK) */
int Vdivider = 0; /* RiCor: variable used to store the recorded value from the voltage divider */
//int VdivOut = 0; /* RiCor: messing around with the "Vdivider" value */
float reference = 2.5; /* RiCor: Reference voltage */

int LowVoltage = 3.5; /* This value determins at which voltage the LED for Low Battery Voltage is activated */ 
int CritVoltage = 3.0; /* This value determins when the project is shut down to prevent too low batter voltage */

//////////////////////////////////////////////////////////////////////////
void setup()
{
  analogReference(INTERNAL); /* The reference is set to the internal reference since the Arduino 
                                is powered from the same battery that it is measuring. */
  pinMode(analogInput, INPUT); /* Configure the pin connected to the voltage devider as an iput pin */
  pinMode(LowBattLED, OUTPUT); /* Configure the pin connected as an output pin */
  pinMode(PowerCutOff, OUTPUT); 
}

//////////////////////////////////////////////////////////////////////////
void loop()
{
  /*********************************/
  Vdivider = analogRead(analogInput); /* Read the value of the pin connected to the voltage divider and store it 
                                          in the "vdivider" variable */           
                                      
  Vin = (Vdivider * reference) / 1024.0; /* Get the "Vin" (Voltage input) value by multiplying the "measured value" 
                                             with the "reference voltage" and dividing that value by 1024 to convert to volts */
  Vout = Vin / (R2/(R1+R2)); /* RiCor: Use Ohms law to calculate the "Vout" value */
  
  Serial.println("The battery voltage is: " + String(Vout) + " V");   /* Output the "Vout" value to serial */
  
  /*********************************/
  if (Vout > LowVoltage)  /* If the battery voltage is -greater than- the "LowVoltage" value */
  {
    digitalWrite(LowBattLED, LOW); /* Turn on the red LED indicating the battery voltage is LOW */
  }
  else //(Vout <= LowVoltage)  /* If the battery voltage is -less than- or -equal to- the "LowVoltage" value */
  {
    digitalWrite(LowBattLED, HIGH); /* Turn on the red LED indicating the battery voltage is LOW */
    Serial.println("LOW Battery Voltage!"); /* Serial message */
  }

  if (Vout > CritVoltage)  /* If the battery voltage is -greater than- the "CritVoltage" value */
  {
    digitalWrite(PowerCutOff, LOW); /* Turn on the red LED indicating the battery voltage is LOW */
  }
  else //(Vout <= CritVoltage) /* If the battery voltage is -less than- or -equal to- the "CritVoltage" value */
  {
    digitalWrite(PowerCutOff, HIGH); /* Turn on the white LED indicating the battery voltage is LOW 
                                        In the final implementation the output will go to the kill pin on the 
                                        Adafruit Push-button Power Switch Breakout board which will cutoff the power supply*/
    Serial.println("CRITICAL Battery Voltage!"); /* Serial message */
  }
  delay(1000); /* wait 1 second */
}  

//////////////////////////////////////////////////////////////////////////

// END OF LINE //



