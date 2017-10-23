/********************************************************************************************************************************
Rik van der Mark, www.RiCor.net 2017

Arduino Feather M0 based Voltmeter to monitor a battery whilst powering that Arduino using that same battery.
In my case I know that the Feather M0 Basic Proto and Feather M0 Bluefruit have a voltage divider on board to do this.
See: https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le?view=all#power-management
Sketch was based on a example sketch from Adafruit oin that same page.

Modified and expanded based on needs for my Identity Disc project.

Adafruit refenreces pin A7 in the skecth and website. On the board this is marked as pin 9.
https://learn.adafruit.com/assets/46245 shows that digital pin 9 (D9) is also analog pin 7 (A7).

* uses the internal voltage reference since the Arduino runs from the battery that is being measured
* 100k resistor from the postive (+) battery terminal of the JST connector on the Feather pin 9 (A7)
* 100k resistor from ping 9 (A7) to ground
* optional 100nF capacitor (ceramic capacitor marked with 104) from Arduino pin connected to the voltage devider to ground for more stable readings 

Starting from v0.5.0 serial communication was added

********************************************************************************************************************************/

#define VBATPIN A7
#define LowBattLED A4
   
/*********************************/
int LowVoltage = 3.2;    /* This value sets the desired "LOW" voltage */ 

//////////////////////////////////////////////////////////////////////////
void setup()
{
  pinMode(VBATPIN, INPUT); /* Configure the pin connected to the voltage devider as an iput pin */
  pinMode(LowBattLED, OUTPUT); /* Configure the pin connected to the "LOW voltage indicator" LED as an output pin */
  
  //ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INT1V_Val;  /* 1.0V voltage reference. The "analogReference" command 
  //                                                            does not work on the SAMD21 (M0) and requires this command.
  //                                                            https://forum.arduino.cc/index.php?topic=409765.0 helped me out. */                                                    
}

//////////////////////////////////////////////////////////////////////////
void loop()
{
  /*********************************/
  //Measure and display voltage
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  Serial.print("VBat: " ); 
  Serial.println(measuredvbat);
    
  /*********************************/
  //Turn on the LED if the voltage of the battery is LOW (or CRITICAL)
  
  if (measuredvbat >= LowVoltage)  /* If the battery voltage is -greater than- or -equal to- the "LowVoltage" value */
  {
    digitalWrite(LowBattLED, LOW); /* red LED indicating the battery voltage is LOW stays off */
  }
  else /* If the battery voltage is -less than- the "LowVoltage" value */
  {
    digitalWrite(LowBattLED, HIGH); /* Turn on the red LED indicating the battery voltage is LOW */
    Serial.println("LOW Battery Voltage!"); /* Serial message */
  }

  delay(500); /* wait 500 miliseconds (half a second) */
}  

//////////////////////////////////////////////////////////////////////////

// END OF LINE //



