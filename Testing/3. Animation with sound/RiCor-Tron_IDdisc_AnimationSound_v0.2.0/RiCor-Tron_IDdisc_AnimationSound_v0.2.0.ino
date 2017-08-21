/********************************************************************************************************************************
Tron Legacy - Sam Flynn's Identity Disk

This sketch was written to continue testing with buttontest- and NeoPixel test sketches that I created before.
Now I'm adding sound to it using the Adafruit Music Maker FeatherWing.

The code used in this sketch was based upon Adafruit example sketches. The rest is me tinkering around.
The sketch isn't perfect yet, still tinkering around with getting the actual animations going and getting
more control during operation.

- The related project page on my website is: https://www.ricor.net/tron-legacy-sam-flynn-identity-disc/
- My GitHub page: https://github.com/RiCor-net/Tron-Legacy-Identity-Disc

I kindly ask that people credit Adafruit and me if they re-use this code.
Thank you and enjoy!

< End of Line >
********************************************************************************************************************************/

// This sketch requires the folowing libraries:
/* include SPI, MP3 and SD libraries for the Music Maker FeatherWing; */
#include <Adafruit_VS1053.h>
#include <SPI.h>
#include <SD.h>

/* NeoPixel related libraries; */
#include <Adafruit_NeoPixel.h>


/****************************************************/
// Definitions

/*Buttos, switches and sensors */
#define BUTTON1_PIN A2
#define BUTTON2_PIN A3
#define BUTTON3_PIN A4

/**************************/
/* Pins used for the Music Maker Featherwing */
#define VS1053_RESET   -1     /* VS1053 reset pin (not used!) */
#define VS1053_CS       6     /* VS1053 chip select pin (output) */
#define VS1053_DCS     10     /* VS1053 Data/command select pin (output) */
#define CARDCS          5     /* Card chip select pin */
#define VS1053_DREQ     9     /* VS1053 Data request, ideally an Interrupt pin, *if possible* (not possible on 32u4) */

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

/**************************/
/* NeoPixel strip definitions for the outer ring */
#define PIXEL_PIN 11    /* Digital IO pin connected to the NeoPixels strip. */
#define PIXEL_COUNT 88  /* Number of NeoPixels on the strip */
#define BRIGHTNESS 60   /* Set the brightness of the NeoPixels. 0=off, 255=max brightness (See Adafruit NeoPixel Überguide) */ //!! FINETUNE BEFORE FINAL DEPLOYMENT !!

/* NeoPixel strip definitions for the inner c-ring 
   Actually consists of 118 LEDs but you only drive 59 "unique" LEDs */ 
#define PIXEL_PIN2 12    /* Digital IO pin connected to the NeoPixels strip. */ 
#define PIXEL_COUNT2 59  /* Number of NeoPixels on the strip */ 
#define BRIGHTNESS2 30    /* Set the brightness of the NeoPixels. 0=off, 255=max brightness (See Adafruit NeoPixel Überguide) */ //!! FINETUNE BEFORE FINAL DEPLOYMENT !!

/* This section delares the NeoPixel objects. Used later to control the strips. (Refer to Adafruit NeoPixel überguide for more information)*/
// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick

/*   First line declares the NeoPixel strip used for the outer ring, */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRBW + NEO_KHZ800);

/* Second line declares the NeoPixel strip used for the inner c-ring. */
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(PIXEL_COUNT2, PIXEL_PIN2, NEO_GRBW + NEO_KHZ800);

/**************************/


/****************************************************/
// Global variables

/*Buttos, switches and sensors */
bool oldState1 = HIGH;
bool oldState2 = HIGH;
bool oldState3 = HIGH;
int showType1 = 0;
int showType2 = 2;
int showType3 = 4;

/**************************/
/* Colors */ 

// Adafruit NeoPixel Überguide;
/*The next three arguments are the pixel color, expressed as red, green and blue brightness levels, 
  where 0 is dimmest (off) and 255 is maximum brightness. The last optional argument is for white, 
  which will only be used if the strip was defined during creation as an RGBW type and the strip actually is RGBW type. */

//Set 4th digit to 0 for now

uint32_t Black = strip.Color(0,0,0,0);
uint32_t White = strip.Color(0,0,0,255);
uint32_t BlueishWhite = strip.Color(60,127,200,0);    // Blueish white
uint32_t WhiteHalf = strip.Color(15,30,40,0);
uint32_t Red = strip.Color(255,60,0,0);
uint32_t Redhalf = strip.Color(60,30,30,0);
uint32_t Green = strip.Color(0,255,0,0);
uint32_t Blue = strip.Color(0,0,255,0);

uint32_t BlueFlash = strip.Color(100,200,255,0);
uint32_t RedFlash = strip.Color(255,200,0,0);

uint32_t WhiteInnerRing = strip.Color(30,64,100,0);
uint32_t RedInnerRing = strip.Color(150,30,0,0);

uint32_t BlueBlade = strip.Color(0,45,255,0);
uint32_t RedBlade = strip.Color(255,90,0,0);

/**************************/


/****************************************************/
// Functions


////////////////////////////////////////////////////////////////////////////
/* Setup code, runs once */
void setup() 
{
  Serial.begin(9600); /* set data rate in bits per second (baud) for serial data communication  */
  delay(1000); /* 1 second delay to make sure all the serial output is displayed during setup. */
               /* Not active when the disc is used stand-alone */

  /* Serial messaging to let the disk talk to the world :) 
     It will also give a status for all the different hardware elements during initialisation lateron */
  Serial.println("Hello World!"); //REMOVE LATER
  Serial.println("================================================");
  Serial.println("RiCor.net");
  Serial.println("Tron Legacy Identity Disc v0.1");
  Serial.println("System Initializing");
  Serial.println("");
  Serial.println("------------------------------------------------");


/*********************************/

  /* Setup the switch pins as digital inputs */
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  Serial.println("> Button Pins setup OK");  


/*********************************/
              
  /* Initialize the Music Maker FeatherWing */
  if (! musicPlayer.begin()) 
  { 
    Serial.println(F(">>> Couldn't find VS1053, do you have the right pins defined?"));
    while (1); /* don't do anything more */
  }
  Serial.println(F("> VS1053 found and initialized"));
  
  musicPlayer.sineTest(0x44, 500); /* Make a tone to indicate VS1053 is working */ //!!REMOVE IN FINAL VERSION!!
 
  if (!SD.begin(CARDCS)) 
  {
    Serial.println(F(">>> SD failed, or not present"));
    while (1); /* don't do anything more */
  }
  Serial.println("> SD OK!");
  
  musicPlayer.setVolume(10,255);   /* Set volume for the left and right channels. 
                                      The Adafruit forum noted that values range from 0 (loudest) to 255 (off)
                                      I set the right channel to 255 (off) because it is not used */

  /* Setup timer interrupts */ 
  // If DREQ is on an interrupt pin we can do background audio playing. NOT possible on a "32u4" feather!
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int


/*********************************/
              
  /* Initializing the two NeoPixel strips */
  Serial.println("> Initialising the NeoPixel strips");
  
  strip.setBrightness(BRIGHTNESS); /* Set (limit) brightness of the NeoPixel strip for the outer ring */
  strip.begin(); /* Prepare data pin for NeoPixel output */
  strip.show();  /* Initialize all pixels to 'off' */
  
  strip2.setBrightness(BRIGHTNESS2); /* Set (limit) brightness of the NeoPixel strip for the inner c-ring */
  strip2.begin(); /* Prepare data pin for NeoPixel output */
  strip2.show();  /* Initialize all pixels to 'off' */


/*********************************/

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
      ButtonControl(showType1); /* Parses the value of "showType*" to the ButtonControl function. */
    }
  }
  oldState1 = newState1;  /* Set the last button state to the old state. */
    
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
      ButtonControl(showType3); /* For this button only one value is required so it always parses
                                   the value given to "showType3" variable to the "ButtonControl" function. */
    }
  }
  oldState3 = newState3;
    
/*********************************/

} 

////////////////////////////////////////////////////////////////////////////
// Functions


/*********************************/
/* Button control functions */
void ButtonControl(int i) /* Output from the different showType's is used to control a disc functions */
                          // using serial output for now for ease of use/testing
{
  switch(i)
  {
    case 0: Serial.println("C-ring deactivated"); /* Display a message via the serial connection */
            powerOff2(); /* Trigger the function mentioned */
            Serial.println("");
            break;
    case 1: Serial.println("C-ring activated");
            colorWipe2();
            Serial.println("");
            break;
    case 2: Serial.println("Blade deactivated");
            powerOff();
            Serial.println("");
            break;
    case 3: Serial.println("Blade activated");
            colorWipe();
            Serial.println("");
            break;
    case 4: Serial.println("Button 3");
            Serial.println("");
            break;
  }
}


/*********************************/
/* Animation functions */

/* Fill the NeoPixels for the blade one after the other with a color */
void colorWipe(void) 
{
  //musicPlayer.startPlayingFile("track060.wav"); // DON'T TRY THIS (TWO FILES AFTER EACH OTHER)!
                                                  // I STUPIDLY THOUGH THIS WOULD WORK BUT IT 
                                                  // FREAKED OUT THE MUSIC MAKER ;)
  musicPlayer.startPlayingFile("track061.wav"); /* continue with humming sound */
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, BlueishWhite);
    strip.show();
  }
}

/* Fill the NeoPixels for the c-ring one after the other with a color */
void colorWipe2(void) 
{
  for(uint16_t i=0; i<strip2.numPixels(); i++) 
  {
    strip2.setPixelColor(i, BlueishWhite);
    strip2.show();
  }
}

/* Turn the blade off */
void powerOff(void)
{
  musicPlayer.pausePlaying(true); //PAUSE (STOP) THE MUSIC
  for(uint16_t i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, Black);
  }
  strip.show();
}

 /* Turn the c-ring off */
void powerOff2(void)
{
  for(uint16_t i=0; i<strip.numPixels(); i++)
  {
    strip2.setPixelColor(i, Black);
  }
  strip2.show();  
}


/*********************************/
/* Music Maker functions */

/**************************/
/* <Music Maker> file listing helper */
void printDirectory(File dir, int numTabs) 
{ 
   while(true) 
   {
     File entry =  dir.openNextFile();
     if (! entry) 
     {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) 
     {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) 
     {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } 
     else 
     {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}  

/*********************************/


////////////////////////////////////////////////////////////////////////////

// END OF LINE //








