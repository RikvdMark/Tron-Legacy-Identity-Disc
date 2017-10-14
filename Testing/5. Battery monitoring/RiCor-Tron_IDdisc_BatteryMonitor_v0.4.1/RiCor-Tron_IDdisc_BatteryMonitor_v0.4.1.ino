/********************************************************************************************************************************
Rik van der Mark, www.RiCor.net 2017
Arduino based Voltmeter to monitor a battery whilst powering that Arduino using that same battery.

Sketch was based on / inspired by sketches from:
- Wawa (https://forum.arduino.cc/index.php?topic=92074.15 - Post #21)
- T.K.Hareendran (http://www.electroschematics.com/9351/arduino-digital-voltmeter/)

Modified and expanded based on needs for my Identity Disc project.
Sketch is in the public domain.

* uses the internal voltage reference since the Arduino runs from the battery that is being measured
* 150k resistor from A1 to the postive (+) battery terminal
* 10k resistor from A1 to ground
* optional 100nF capacitor (ceramic capacitor marked with 104) from Arduino pin connected to the voltage devider to ground for more stable readings 

< End of Line >
********************************************************************************************************************************/

#define analogInput A5 /* Pin connected to the voltage divider */
#define LowBattLED A4  /* Connected a red LED to this pin */
//#define PowerCutOff A0 /* At this stage a white LED is connected but in the final setup the pin will connect
//                          to the "kill pin" on the Adafruit Push-button Power Switch Breakout board 
//                          this way the Arduino can power off the entire thing to save the battery */

/*********************************/
unsigned int total; // A/D output
float Vout;         // converted to volt
float Aref = 1.000; // change this to the actual Aref voltage of ---YOUR--- Arduino, or adjust to get accurate voltage reading (1.000- 1.200)

int LowVoltage = 3.20;    /* This value sets the desired "LOW" voltage */ 
//int CritVoltage = 3.10; /* This value sets the desired "CRITICAL" voltage */

//////////////////////////////////////////////////////////////////////////
void setup()
{
  pinMode(analogInput, INPUT); /* Configure the pin connected to the voltage devider as an iput pin */
  pinMode(LowBattLED, OUTPUT); /* Configure the pin connected to the "LOW voltage indicator" LED as an output pin */
  //pinMode(PowerCutOff, OUTPUT); /* Configure the pin connected to the "Critical voltage indicator" LED as an output pin */
  
  //analogReference(INTERNAL); /* USE THIS LINE ON NON-M0 ARDUINO BOARDS THAT DO USE THIS COMMAND
  //                              USE (INTERNAL1V1) IN STEAD OF (INTERNAL) on ARDUINO MEGA boards.
  //                              The reference is set to the internal reference since the Arduino 
  //                              is powered from the same battery that it is measuring. */
  ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INT1V_Val;  /* 1.0V voltage reference. The "analogReference" command 
                                                              does not work on the SAMD21 (M0) and requires this command.
                                                              https://forum.arduino.cc/index.php?topic=409765.0 helped me out. */                                                    
}

//////////////////////////////////////////////////////////////////////////
void loop()
{
  /*********************************/
  //Measure and display voltage
  
  analogRead(analogInput); // one unused reading to clear old sh#t
  for (int x = 0; x < 16; x++) // 16 analogue readings and 1/16 voltage divider = no additional maths
  { 
    total = total + analogRead(analogInput); // add each value
  }
  Vout = total * Aref / 1024; // convert readings to volt
  Serial.println("The battery voltage is: " + String(Vout) + " V");   /* Output the "Vout" value to serial */
  total = 0; // reset value
    
  /*********************************/
  //Turn on the LED if the voltage of the battery is LOW (or CRITICAL)
  
  if (Vout >= LowVoltage)  /* If the battery voltage is -greater than- or -equal to- the "LowVoltage" value */
  {
    digitalWrite(LowBattLED, LOW); /* red LED indicating the battery voltage is LOW stays off */
  }
  else /* If the battery voltage is -less than- the "LowVoltage" value */
  {
    digitalWrite(LowBattLED, HIGH); /* Turn on the red LED indicating the battery voltage is LOW */
    Serial.println("LOW Battery Voltage!"); /* Serial message */
  }

//DISCONTINUED USE OF THE CRITICAL VOLTAGE OPTION BELOW. THE BATTERIES I'M USING ARE PROTECTED AND AUTOMATICALLY CUT OFF AT 3V.
//KEPT THE CODE HERE SHOULD IT BE USEFULL TO ME OR SOMEONE ELSE IN THE FUTURE.
//!!IF USED, DON'T FORGET TO REMOVE THE COMMENT SIGNS IN FRONT OF THE pin definition, "CritVoltage" integer and pin setup AT THE BEGINNING OF THIS SKETCH!!

  //if (Vout >= CritVoltage)  /* If the battery voltage is -greater than- or -equal to- the "CritVoltage" value */
  //{
  //  digitalWrite(PowerCutOff, LOW); /* Turn on the red LED indicating the battery voltage is CRITICAL */
  //}
  //else /* If the battery voltage is -less than- the "CritVoltage" value */
  //{
  //  digitalWrite(PowerCutOff, HIGH); /* Turn on the white LED indicating the battery voltage is CRITICAL 
  //                                      In the final implementation the output will go to the kill pin on the 
  //                                      Adafruit Push-button Power Switch Breakout board which will cutoff the power supply*/
  //  Serial.println("CRITICAL Battery Voltage!"); /* Serial message */
  //}
  
  delay(500); /* wait 500 miliseconds (half a second) */
}  

//////////////////////////////////////////////////////////////////////////

// END OF LINE //



