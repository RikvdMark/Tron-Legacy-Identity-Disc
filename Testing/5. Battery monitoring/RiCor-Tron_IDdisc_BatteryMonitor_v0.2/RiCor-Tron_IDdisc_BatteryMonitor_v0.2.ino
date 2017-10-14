/*
Source of the sketch used here is: 
//Source:   https://forum.arduino.cc/index.php?topic=92074.15
//Post:     #21

0 - ~17volt voltmeter
works with 3.3volt and 5volt Arduinos
uses the internal 1.1volt reference
150k resistor from A1 to +batt
10k resistor from A1 to ground
optional 100n capacitor from A1 to ground for stable readings
*/


float Aref = 1.910; // change this to the actual Aref voltage of ---YOUR--- Arduino, or adjust to get accurate voltage reading (1.000- 1.200)
unsigned int total; // A/D output
float voltage; // converted to volt
//float reference; //RiCor: added to display refernce voltage

//
void setup() 
{
  Serial.begin(9600); // ---set serial monitor to this value---
  
  //analogReference(INTERNAL); // use the internal ~1.1volt reference, change (INTERNAL) to (INTERNAL1V1) for a Mega
  ADC->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INT1V_Val;  /* 1.0V voltage reference. The "analogReference" command 
                                                              does not work on the SAMD21 (M0) and requires this command.
                                                              https://forum.arduino.cc/index.php?topic=409765.0 helped me out. */
}


//
void loop() 
{
  analogRead(5); // one unused reading to clear old sh#t
  for (int x = 0; x < 16; x++) // 16 analogue readings and 1/16 voltage divider = no additional maths
  { 
    total = total + analogRead(5); // add each value
  }
  voltage = total * Aref / 1024; // convert readings to volt
  // print to serial monitor
  Serial.print("The battery is ");
  Serial.print(voltage);
  Serial.println(" volt");
  Serial.println(""); /* RiCor: Empty line for readability */
  total = 0; // reset value
  delay(1000); // readout delay
}
