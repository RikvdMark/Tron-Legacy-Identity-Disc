/********************************************************************************************************************************
Rik van der Mark, www.RiCor.net 2017

Sketch for enabeling serial communication between two Arduino's.
In this case this concerns a -Adafruit Feather M0 Basic Proto- and a -Adafruit Feather M0 BlueFruit-.

Required connections:
- Tx pin of the Basic Proto is connected to the Rx pin of the BlueFruit board;
- Rx pin of the Basic Proto is connected to the Tx pin of the BlueFruit board;
- Both boards share a commond ground (GND).

This version of the sketch just send a 'H' followed by a 'L' with 1 second delays.
The BlueFruit boards runs the PhysicalPixel sketch from the examples in the Arduino IDE 
(Examples -> Communication -> PhysicalPixel).

Since I'm using M0 boards it turns out there are two available serial interfaces:
- "Serial" = connected to the USB port
- "Serial1"(Serial-ONE) = the serial Tx and Rx pins on the board

********************************************************************************************************************************/


/****************************************************/


/****************************************************/


////////////////////////////////////////////////////////////////////////////
/* Setup code, runs once */
void setup() 
{
  Serial.begin(9600);  /* The USB-serial on the M0 */
  Serial1.begin(9600); /* Serial-pins on the M0 board used to connect to the other board */
}

////////////////////////////////////////////////////////////////////////////
/* Main code, runs repeatedly */
void loop() 
{
  Serial.print('H');  /* Output to the USB-serial to the computer */ 
  Serial1.print('H'); /* Output to the serial connection to the other Arduino M0 */
  delay(1000);
  Serial.print('L');
  Serial1.print('L');
  delay(1000);
}
