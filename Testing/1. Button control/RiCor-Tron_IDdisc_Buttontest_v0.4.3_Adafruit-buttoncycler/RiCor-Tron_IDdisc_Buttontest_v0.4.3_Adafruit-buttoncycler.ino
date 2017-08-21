/********************************************************************************************************************************
Tron Legacy - Sam Flynn's Identity Disk
This sketch was written to test/learn button setup to control functions of the disc. Of course it's usable for other purposes aswell.

The code from "studioTTTguTTT" didn't get me where I wanted so i looked into one of Adafruits example sketches (buttoncycler) in the 
NeoPixel example code folder. This turned out to serve my use case better / I was able to get it to where I wanted it.

I first copied that setup and modified it to my own liking in terms of looks and went from there to get what I wanted.
- The related project page on my website is: https://www.ricor.net/tron-legacy-sam-flynn-identity-disc/
- My GitHub page: https://github.com/RiCor-net/Tron-Legacy-Identity-Disc

I kindly ask that people credit Adafruit and me if they re-use this code.
Thank you and enjoy!

< End of Line >
********************************************************************************************************************************/

// This sketch requires the folowing libraries:


/**************************/
// Definitions

/*Buttos, switches and sensors */
#define BUTTON1_PIN A2
#define BUTTON2_PIN A3
#define BUTTON3_PIN A4


/**************************/
// Global variables

/*Buttos, switches and sensors */
bool oldState1 = HIGH;
bool oldState2 = HIGH;
bool oldState3 = HIGH;
int showType1 = 0;
int showType2 = 2;
int showType3 = 4;


/**************************/
// Functions


////////////////////////////////////////////////////////////////////////////
/* Setup code, runs once */
void setup() 
{
  /* Serial messaging to let the disk talk to the world :) 
     It will also give a status for all the different hardware elements during initialisation lateron */
  Serial.begin(9600);
  delay(1000); /* Delay was added because otheriwse the first lines of serial output where not shown on the screen */ // !! MAYBE REMOVE OR COMMENT IN FINAL DEPOYMENT !!

  Serial.println("Hello World!"); //REMOVE LATER
  Serial.println("================================================");
  Serial.println("RiCor.net");
  Serial.println("Tron Legacy Identity Disc - Button test v0.2");
  Serial.println("System Initializing");
  Serial.println("");
  Serial.println("------------------------------------------------");

/**************************/

  /* Setup the switch pins as digital inputs */
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  Serial.println("> Button Pins setup OK");  

/**************************/

  Serial.println("------------------------------------------------");
  Serial.println("");
  Serial.println("Disc Activated and Synchronized");
  Serial.println("Grid is Live!");
  Serial.println("");
  Serial.println("================================================");
  Serial.println("");
}

////////////////////////////////////////////////////////////////////////////
/* Main code, runs repeatedly */
void loop() 
{ 
/*********************************/
  /* Button used for controlling the blade */
  bool newState1 = digitalRead(BUTTON1_PIN); /* Get current button state (HIGH or LOW) */

  if (newState1 == LOW && oldState1 == HIGH) /* Check if state changed from high to low (button press). */
  {
    delay(20); /* Short delay to debounce button. */
    newState1 = digitalRead(BUTTON1_PIN); /* Check if button is still low after debounce. */
    if (newState1 == LOW) 
    {
      showType1++;       /* increase "showType" by 1 */
      if (showType1 > 1) /* If "showType1" becomes higher then "1" it triggers the next line which resets it to "0"
                            effectively toggling it between 0 and 1. */
        showType1=0;
      ButtonControl(showType1); /* Parses the value of "showType1" to the ButtonControl function. */
    }
  }
  oldState1 = newState1; /* Set the last button state to the old state. */
    
/*********************************/
  /* Button used for turning the music on/off */
  bool newState2 = digitalRead(BUTTON2_PIN);

  if (newState2 == LOW && oldState2 == HIGH)
  {
    delay(20);
    newState2 = digitalRead(BUTTON2_PIN);
    if (newState2 == LOW) 
    {
      showType2++;
      if (showType2 > 3) /* for this button "ShowType2" toggles between 3 and 4 */
        showType2=2;
      ButtonControl(showType2);
    }
  }
  oldState2 = newState2;
    
/*********************************/
  /* Button used for going to the next song */
  bool newState3 = digitalRead(BUTTON3_PIN);

  if (newState3 == LOW && oldState3 == HIGH)
  {
    delay(20);
    newState3 = digitalRead(BUTTON3_PIN);
    if (newState3 == LOW) 
    {
      ButtonControl(showType3); /* For this button only one value is required so it's always
                                   the value given to "showType3" variable. */
    }
  }
  oldState3 = newState3;
    
/*********************************/

}

////////////////////////////////////////////////////////////////////////////
// Functions

/**************************/
/* Button control functions */
void ButtonControl(int i) /* Output from the different showType's is used to control a disc function */
                          // using serial output for now for ease of use/testing
{
  switch(i)
  {
    case 0: Serial.println("Blade deactivated");
            Serial.println("");
            break;
    case 1: Serial.println("Blade activated");
            Serial.println("");
            break;
    case 2: Serial.println("Music paused");
            Serial.println("");
            break;
    case 3: Serial.println("Music playing");
            Serial.println("");
            break;
    case 4: Serial.println("Next song");
            Serial.println("");
            break;
  }
}

/**************************/


////////////////////////////////////////////////////////////////////////////

// END OF LINE //








