/********************************************************************************************************************************
Rik van der Mark, www.RiCor.net 2017

Sketch for enabeling serial communication between two Arduino's.
In this case this concerns a -Adafruit Feather M0 Basic Proto- and a -Adafruit Feather M0 BlueFruit-.

Required connections:
- Tx pin of the Basic Proto is connected to the Rx pin of the BlueFruit board;
- Rx pin of the Basic Proto is connected to the Tx pin of the BlueFruit board;
- Both boards share a commond ground (GND).

This version uses the PhysicalPixel sketch which can be found in the Arduino IDE by going to:
Examples -> Communication -> PhysicalPixel.
I did this to get a feel for serial communication in a sketch format I feel comfortable with.

Since I'm using M0 boards it turns out there are two available serial interfaces:
- "Serial" = connected to the USB port
- "Serial1"(Serial-ONE) = the serial Tx and Rx pins on the board

********************************************************************************************************************************/


/****************************************************/
const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into

/****************************************************/


////////////////////////////////////////////////////////////////////////////
/* Setup code, runs once */
void setup() 
{
  // initialize serial communication:
  Serial.begin(9600);  /* The USB-serial on the M0 */
  Serial1.begin(9600); /* Serial-pins on the M0 board used to connect to the other board */
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

////////////////////////////////////////////////////////////////////////////
/* Main code, runs repeatedly */
void loop() 
{
  // see if there's incoming serial data:
  if (Serial1.available() > 0) 
  {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial1.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    Serial.println("Incoming byte is: " + String(incomingByte)); /* Print the incomingByte for debugging purposes
                                                                    This led me to discover I had to use Serial-ONE- (Serial1)
                                                                    for the Arduino-Arduino serial link after referencing the
                                                                    Adafruit guides on the M0 (Bluefruit) */
    if (incomingByte == 'H') 
    {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED on"); /* Output to the USB-serial link to the connected computer */
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') 
    {
      digitalWrite(ledPin, LOW);
      Serial.println("LED off"); /* Output to the USB-serial link to the connected computer */
    }
  }
  delay(10);
}
