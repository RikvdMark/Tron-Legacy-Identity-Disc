/********************************************************************************************************************************
Tron Legacy - Sam Flynn's Identity Disk
This sketch was written to test/learn button setup to control functions of the disc. Of course it's usable for other purposes aswell.

The source of this setup was obtained from the YouTube "studioTTTguTTT". Specifically from part 1, 2 and 3 from the playlist 
"Ultimate Arduino Patch - The last Arduino code you'll ever need" (https://www.youtube.com/playlist?list=PLAXkVXyP6y5Midfk4jZA0lBmJtpodZpAD)
Github page: https://github.com/elosine/ultimatearduino

I first copied that setup and modified it to my own liking in terms of looks and went from there to get what I wanted.
- The related project page on my website is: https://www.ricor.net/tron-legacy-sam-flynn-identity-disc/
- My GitHub page: https://github.com/RiCor-net/Tron-Legacy-Identity-Disc

< End of Line >
********************************************************************************************************************************/

// This sketch requires the folowing libraries:


/**************************/
// Definitions

/*Buttos, switches and sensors */
#define NB 3 /* Number of buttons */


/**************************/
// Variables

/* Momentary button vars */
int bp[NB] = {A1,A2,A3}; /* Arduino input pin numbers */
boolean bg[NB] = {true,true,true}; /* gating system used in the for "void loop" for the momentary press */

/* Toggle button vars */
int btv[NB] = {0,0,0}; /* start value for the button (default toggle value) */
int btamt[NB] = {2,2,2}; /* toggle amount, set to "2" it means that the switches toggle between 0 and 1 */
boolean btg[NB] = {true,true,true}; /* gating system used for the toggle in the "void loop" */

/* value storage */
  // EXPERIMENTAL VARS RELATED TO BUTTONS TO TRIGGER FUNCTIONS - CURRENTLY NOT IN USE
  // Keep as seperate val's or combine? // move to "buttons" section above this?
//int buttonval1 = 0; /* store button values */
//int buttonval2 = 0; /* store button values */
//int buttonval3 = 0; /* store button values */


/**************************/
// Functions


////////////////////////////////////////////////////////////////////////////
/* Setup code, runs once */
void setup() 
{
  Serial.begin(9600); /* Sets baud rate for serial data transmission */
  for(int i=0;i<NB;i++) /* loop that goes through the array of defined buttons based on the number of buttons defined in "NUM_BUTTONS" (NB) */
  {
    pinMode(bp[i], INPUT_PULLUP); /* sets the pins defined in the "button_pins" array to input pins and pulls the pins HIGH 
                                     When switch is triggerd the accompanying pin is pulled low triggering the related code to execute 
                                     (Triggering happens in the "void loop" section of the coded, not here) */
  }
}


////////////////////////////////////////////////////////////////////////////
/* Main code, runs repeatedly */
void loop() 
{ 
  /*********************************/
  //Button loop
  for(int i=0;i<NB;i++) /* for loop to cycle past all the pins configured in "NB" */
  {
    if(digitalRead(bp[i]) == LOW) /* button pushed, pin goes LOW set as a loop for every pin */                          
    {
      //momentary
      if (bg[i]) /* if "bgate" = TRUE */
      {
        bg[i] = false; /* Makes sure this section doesn't run again until the button is pressed again */
        Serial.print("Button");
        Serial.print(String(i));
        Serial.print(":");
        // Serial.print("b" + String(i) + ":");  //Shorter version of the previous 3 lines
        Serial.println(1);         
      }
      //toggle
      if (btg[i]) 
      {
        btg[i] = false;
        btv[i] = (btv[i] + 1) % btamt[i];
        Serial.print("ButtonToggle" + String(i) + ":");
        Serial.println(btv[i]);
      }          
    }    
    else /* button released after pushing, related pin goes HIGH again */
    {
      //momentary
      if (!bg[i]) /* if "bgate" = NOT TRUE */
      {
        bg[i] = true; /* Makes sure this section doesn't run again until the button is released again (after pressing it) */
        Serial.print("Button" + String(i) + ":");
        Serial.println(0);
        Serial.println(""); /* Add a blank line after each button press */        
      }
      //toggle
      if (!btg[i]) /* runs if the toggle "gate" is false, "!" means "NOT" */
      {      
        btg[i] = true; /* reopen the toggle "gate" so it's "open" (true) again for the next loop */
      }       
    }
  }
  /*********************************/
  delay(30);
}


////////////////////////////////////////////////////////////////////////////
/* Functions */


/**************************/


////////////////////////////////////////////////////////////////////////////


// END OF LINE //








