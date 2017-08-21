/********************************************************************************************************************************
Tron Legacy - Sam Flynn's Identity Disk
This sketch has been written for use with the Adafruit Feather M0 Basic Proto combined with a Adafruit Music Maker FeatherWing.

Copyright Rik van der Mark, www.RiCor.net, March 2017

I kindly ask that people credit Adafruit and me if they re-use this code.
Thank you and enjoy!

< End of Line >
********************************************************************************************************************************/

// This sketch requires the folowing libraries:
/* Libraries related to the Feather M0; */
#include <Arduino.h> 

/* include SPI, MP3 and SD libraries for the Music Maker FeatherWing; */
#include <Adafruit_VS1053.h>
#include <SPI.h>
#include <SD.h>

/* NeoPixel related libraries; */
#include <Adafruit_NeoPixel.h>


/**************************/
// Definitions

/* Pins used on the Feather M0 or 32u4 */
#define VS1053_RESET   -1     /* VS1053 reset pin (not used!) */
#define VS1053_CS       6     /* VS1053 chip select pin (output) */
#define VS1053_DCS     10     /* VS1053 Data/command select pin (output) */
#define CARDCS          5     /* Card chip select pin */
#define VS1053_DREQ     9     /* VS1053 Data request, ideally an Interrupt pin, *if possible* (not possible on 32u4) */

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

/* NeoPixel strip definitions for the outer ring */
#define PIXEL_PIN 11    /* Digital IO pin connected to the NeoPixels strip. */
#define PIXEL_COUNT 88  /* Number of NeoPixels on the strip */
#define BRIGHTNESS 50   /* Number of NeoPixels on the strip */ !! FINETUNE BEFORE FINAL DEPLOYMENT !!

/* NeoPixel strip definitions for the inner c-ring 
   Actually consists of 118 LEDs but you only drive 59 "unique" LEDs */ 
#define PIXEL_PIN2 12    /* Digital IO pin connected to the NeoPixels strip. */ 
#define PIXEL_COUNT2 59  /* Number of NeoPixels on the strip */ 
#define BRIGHTNESS 50    /* Number of NeoPixels on the strip */ !! FINETUNE BEFORE FINAL DEPLOYMENT !!

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
// Global variables

// Neopixel color
/* Integer for Adafruit striptest, rainbow effect.
   Had to rename it to "NEO_gamma" because the original "gamma" caused errors. Adafruit has since fixed that issue */
int NEO_gamma[] = 
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
};


/**************************/
// Functions


////////////////////////////////////////////////////////////////////////////
/* Setup code, runs once */

void setup() 
{
  /* Serial messaging to let the disk talk to the world :) 
     It will also give a status for all the different hardware elements during initialisation */
  Serial.begin(9600);
  Serial.println("Hello World!");
  Serial.println("------------------------------------------------");
  Serial.println("Tron Legacy Identity Disc v1.0");
  Serial.println("System Initializing");
  Serial.println("------------------------------------------------");

/**************************/
              
  /* Initializing the two NeoPixel strips */
  Serial.println("> Initialising the NeoPixel strips");
  strip.setBrightness(BRIGHTNESS); /* Set (limit) brightness of the NeoPixel strip for the outer ring */
  strip.begin(); /* Prepare data pin for NeoPixel output */
  strip.show();  /* Initialize all pixels to 'off' */
  
  strip.setBrightness(BRIGHTNESS); /* Set (limit) brightness of the NeoPixel strip for the inner c-ring */
  strip2.begin(); /* Prepare data pin for NeoPixel output */
  strip2.show();  /* Initialize all pixels to 'off' */

/**************************/
              
  /* Initialize the Music Maker FeatherWing */
  if (! musicPlayer.begin()) 
  { 
    Serial.println(F(">>> Couldn't find VS1053, do you have the right pins defined?"));
    while (1); /* don't do anything more */
  }
  Serial.println(F("> VS1053 found"));
  
  musicPlayer.sineTest(0x44, 500); /* Make a tone to indicate VS1053 is working */ //!!REMOVE IN FINAL VERSION!!
 
  if (!SD.begin(CARDCS)) 
  {
    Serial.println(F(">>> SD failed, or not present"));
    while (1); /* don't do anything more */
  }
  Serial.println("> SD OK!");
  
  printDirectory(SD.open("/"), 0); /* List files on SD-card */
  
  musicPlayer.setVolume(10,255);   /* Set volume for the left and right channels. 
                                      The Adafruit forum noted that values range from 0 (loudest) to 255 (off)
                                      I set the right channel to 255 (off) because it is not used */

  /* Setup timer interrupts */ //Everything but the "else" setion will be removed later since only the "DREQ int" is required/works.
  #if defined(__AVR_ATmega32U4__) 
    // Timer interrupts are not suggested, better to use DREQ interrupt!
    // but we don't have them on the 32u4 feather...
    /* RiCor: (and on the M0, Adafruit's  "player_simple" sketch does not work on the M0 where the 
     "feather_player" sketch, which uses DREQ interrupts, does) */
    // musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int 
  #else
    // If DREQ is on an interrupt pin we can do background audio playing. NOT possible on a "32u4" feather!
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  #endif

  Serial.println("------------------------------------------------");
  Serial.println("Disc Activated and Synchronized");
  Serial.println("Grid is Live!");
  Serial.println("------------------------------------------------");

// !!! THIS SEGMENT NEEDS TO MOVE TO THE APPROPRIATE FUNCTION LATER !!! //
  // "PlayFullFile" doesn't run in the background, sketch won't continue until it finishes.
  //Serial.println(F("Playing full track 002"));
  //musicPlayer.playFullFile("track002.mp3");

  // Play a file in the background, REQUIRES interrupts!
  Serial.println(F("Playing track 012"));
  musicPlayer.startPlayingFile("track012.mp3");
// !!! END -TO BE MOVED- SECTION !!! //

}

////////////////////////////////////////////////////////////////////////////
/* Main code, runs repeatedly */
void loop() 
{ 
  /* Loop for Music Maker control */
  Serial.print("."); /* print dots while a file is playing in the background */
  if (musicPlayer.stopped()) 
  {
    Serial.println("Done playing music");
    while (1);
  }

  /* Control the player via Serial console */
  if (Serial.available()) 
  {
    char c = Serial.read(); /* Check serial console for input */
    if (c == 's') /* if we get an 's' on the serial console, stop! */
    {
      musicPlayer.stopPlaying();
    }
    
    if (c == 'p') /* if we get an 'p' on the serial console, pause/unpause! */
    {
      if (! musicPlayer.paused()) /* If the MusicMaker is not pause then pause it */
      {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } 
      else 
      { 
      Serial.println("Resumed"); /* If the MusicMaker is paused, start playing again */
      musicPlayer.pausePlaying(false);
      }
    }
  }
  delay(100); 

/**************************/

/* Loop for NeoPixel control */ //temporary, part of de Adafruit Test-code
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  colorWipe(strip.Color(0, 0, 0, 255), 50); // White
  colorWipe2(strip2.Color(255, 0, 0), 50); // Red
  colorWipe2(strip2.Color(0, 255, 0), 50); // Green
  colorWipe2(strip2.Color(0, 0, 255), 50); // Blue
  colorWipe2(strip2.Color(0, 0, 0, 255), 50); // White

  whiteOverRainbow(20,75,5);  
  whiteOverRainbow2(20,75,5);  

  pulseWhite(5); 
  pulseWhite2(5); 

  // fullWhite();
  // delay(2000);

  rainbowFade2White(3,3,1);
  rainbowFade2White2(3,3,1);
}


////////////////////////////////////////////////////////////////////////////
// Functions

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

/**************************************************************************/

// NeoPixel functions (currently only contains Adafruit test-code)

/* <NeoPixels> Fill the dots for the outer ring one after the other with a color */
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

/* <NeoPixels> Fill the dots for the inner ring one after the other with a color */
void colorWipe2(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip2.numPixels(); i++) 
  {
    strip2.setPixelColor(i, c);
    strip2.show();
    delay(wait);
  }
}

/**************************/

/* <NeoPixels> Rainbow with white colour chaser for the outer ring */
void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) 
{
  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;

  while(true)
  {
    for(int j=0; j<256; j++) 
    {
      for(uint16_t i=0; i<strip.numPixels(); i++) 
      {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) )
        {
          strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        }
        else
        {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
      }

      if(millis() - lastTime > whiteSpeed) 
      {
        head++;
        tail++;
        if(head == strip.numPixels())
        {
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
        strip.show();
        delay(wait);
    }
  }
}

/* <NeoPixels> Rainbow with white colour chaser for the inner ring */
void whiteOverRainbow2(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) 
{
  if(whiteLength >= strip2.numPixels()) whiteLength = strip2.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;

  while(true)
  {
    for(int j=0; j<256; j++) 
    {
      for(uint16_t i=0; i<strip2.numPixels(); i++) 
      {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) )
        {
          strip2.setPixelColor(i, strip2.Color(0,0,0, 255 ) );
        }
        else
        {
          strip2.setPixelColor(i, Wheel(((i * 256 / strip2.numPixels()) + j) & 255));
        }   
      }

      if(millis() - lastTime > whiteSpeed) 
      {
        head++;
        tail++;
        if(head == strip2.numPixels())
        {
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip2.numPixels();
      tail%=strip2.numPixels();
        strip2.show();
        delay(wait);
    }
  } 
}

/**************************/

/* for the outer strip  */
void pulseWhite(uint8_t wait) 
{
  for(int j = 0; j < 256 ; j++)
  {
      for(uint16_t i=0; i<strip.numPixels(); i++) 
      {
        strip.setPixelColor(i, strip.Color(0,0,0, NEO_gamma[j] ) );
      }
      delay(wait);
      strip.show();
  }

  for(int j = 255; j >= 0 ; j--)
  {
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, strip.Color(0,0,0, NEO_gamma[j] ) );
    }
    delay(wait);
    strip.show();
  }
}

/* for the inner strip  */
void pulseWhite2(uint8_t wait) 
{
  for(int j = 0; j < 256 ; j++)
  {
    for(uint16_t i=0; i<strip2.numPixels(); i++) 
    {
      strip2.setPixelColor(i, strip2.Color(0,0,0, NEO_gamma[j] ) );
    }
    delay(wait);
    strip.show();
  }

  for(int j = 255; j >= 0 ; j--)
  {
    for(uint16_t i=0; i<strip2.numPixels(); i++) 
    {
      strip2.setPixelColor(i, strip2.Color(0,0,0, NEO_gamma[j] ) );
    }
    delay(wait);
    strip2.show();
  }
}

/**************************/

/* for the outer strip  */
void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) 
{
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++)
  {
    for(int j=0; j<256; j++) 
      { // 5 cycles of all colors on wheel
        for(int i=0; i< strip.numPixels(); i++) 
        {
        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
        }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) 
      {
        fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax )
      {
        fadeVal--;
      }
    strip.show();
    delay(wait);
    }
  }
  delay(500);

  for(int k = 0 ; k < whiteLoops ; k ++)
  {
    for(int j = 0; j < 256 ; j++)
    {
      for(uint16_t i=0; i < strip.numPixels(); i++) 
      {
        strip.setPixelColor(i, strip.Color(0,0,0, NEO_gamma[j] ) );
      }
      strip.show();
    }
    delay(2000);
        
    for(int j = 255; j >= 0 ; j--)
    {
      for(uint16_t i=0; i < strip.numPixels(); i++) 
      {
        strip.setPixelColor(i, strip.Color(0,0,0, NEO_gamma[j] ) );
      }
      strip.show();
    }
  }
  delay(500);
}

/* for the inner strip  */
void rainbowFade2White2(uint8_t wait, int rainbowLoops, int whiteLoops) 
{
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++)
  {
    for(int j=0; j<256; j++) 
    { // 5 cycles of all colors on wheel
      for(int i=0; i< strip2.numPixels(); i++) 
      {
        wheelVal = Wheel(((i * 256 / strip2.numPixels()) + j) & 255);
        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip2.setPixelColor( i, strip2.Color( redVal, greenVal, blueVal ) );
      }

    //First loop, fade in!
    if(k == 0 && fadeVal < fadeMax-1) 
    {
      fadeVal++;
    }

    //Last loop, fade out!
    else if(k == rainbowLoops - 1 && j > 255 - fadeMax )
    {
      fadeVal--;
    }

    strip2.show();
    delay(wait);
    }
  }

  delay(500);

  for(int k = 0 ; k < whiteLoops ; k ++)
  {
    for(int j = 0; j < 256 ; j++)
    {
      for(uint16_t i=0; i < strip2.numPixels(); i++) 
      {
        strip2.setPixelColor(i, strip2.Color(0,0,0, NEO_gamma[j] ) );
      }
      strip2.show();
    }
    
    delay(2000);
        
    for(int j = 255; j >= 0 ; j--)
    {
      for(uint16_t i=0; i < strip2.numPixels(); i++) 
      {
        strip2.setPixelColor(i, strip2.Color(0,0,0, NEO_gamma[j] ) );
      }
    strip2.show();
    }
  }
  delay(500);
}

/**************************/
/**************************/

/* Adafruit colour-wheel code for the different rainbow-effect loops */
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) 
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) 
{
  return (c >> 8);
}
uint8_t green(uint32_t c) 
{
  return (c >> 16);
}
uint8_t blue(uint32_t c) 
{
  return (c);
}

/**************************************************************************/


// END OF LINE //









