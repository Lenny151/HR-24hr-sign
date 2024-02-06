// Params for width and height

#include <FastLED.h>

#define BRIGHTNESS 50
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB

#define NUM_LEDS_PART_H 93 
#define NUM_LEDS_PART_B 183
#define NUM_LEDS_PART_W 129

CRGB ledsH[NUM_LEDS_PART_H];
CRGB ledsB[NUM_LEDS_PART_B];
CRGB ledsW[NUM_LEDS_PART_W];

uint16_t paletteIndexH = 0;
uint16_t paletteIndexB = 0;
uint16_t paletteIndexW = 0;
//uint16_t paletteIndex = 0;

DEFINE_GRADIENT_PALETTE (hr_colours) {
  0,   255, 145, 0,   //orange
  51,  0,   255, 200,   //blue
  127, 120, 0,  255,   //purple
  204, 255, 0,   200,    //pink
  255, 255, 145, 0,   //orange
};

DEFINE_GRADIENT_PALETTE (hr_colours2) {
  0,   0,   255, 200,   //blue
  27,  0,   255, 200,   //blue
  37,  120, 0,  255,   //purple
  64,  120, 0,  255,   //purple
  91,  120, 0,  255,   //purple
  101, 255, 145, 0,   //orange
  128, 255, 145, 0,   //orange
  155, 255, 145, 0,   //orange
  165, 255, 0,   200,    //pink
  192, 255, 0,   200,    //pink
  219, 255, 0,   200,    //pink
  229, 0,   255, 200,   //blue
  255, 0,   255, 200,   //blue
};

DEFINE_GRADIENT_PALETTE (hr_colours3) {
  0,   255, 145, 0,   //orange 
  64,  120, 0,  255,   //purple
  128, 0,   255, 200,   //blue
  192, 120, 0,  255,   //purple
  255, 255, 145, 0,   //orange
};

uint16_t hardCodedColours[4][3] = {
  {255, 145, 0}, //orange
  {255, 0,   200}, //pink
  {0,   255, 200}, //blue
  {120, 0,  255}, //purple
};

CRGBPalette16 myPal = hr_colours;
CRGBPalette16 myPal2 = hr_colours2;
CRGBPalette16 myPal3 = hr_colours3;

void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds, 93).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(ledsH, NUM_LEDS_PART_H).setCorrection(TypicalLEDStrip);
  
  //FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, 183).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(ledsB, NUM_LEDS_PART_B).setCorrection(TypicalLEDStrip);
  
  //FastLED.addLeds<LED_TYPE, 7, COLOR_ORDER>(leds, 129).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 7, COLOR_ORDER>(ledsW, NUM_LEDS_PART_W).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(57600);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { neon_sign, colour_wipe, neon_sign, mode_3 };

uint16_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void loop()
{ 
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest

  // do some periodic updates
  EVERY_N_SECONDS( 300 ) {
    nextPattern();  // change patterns periodically
//    paletteIndex = 0;
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  
}

void fade_palette(CRGB leds[], int strip, int len, int spd)
{
  fill_palette(leds, strip, paletteIndexB, 255 / len, myPal, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(spd) {
    paletteIndexB--;
  }
}

void fade_palette_short(CRGB leds[], int strip)
{
  fill_palette(leds, strip, paletteIndexW, 255 / 20, myPal3, 255, LINEARBLEND); //may need to play with length "50" to get looking right
 
  EVERY_N_MILLISECONDS(6) {
    paletteIndexW--;
  }
}

void colour_wipe()
{
 for(int j = 0; j < 4; j++){
  for(int i = 0; i < NUM_LEDS_PART_W;){
    ledsW[i].setRGB(hardCodedColours[j][0], hardCodedColours[j][1], hardCodedColours[j][2]);
     fade_palette(ledsB, NUM_LEDS_PART_B, 34, 25);
     fade_colour(ledsH, NUM_LEDS_PART_H);
    FastLED.show();
    //delay(50);
     EVERY_N_MILLISECONDS(60) {
    i++;
    }
  }
 }
}

void fade_colour(CRGB leds[], int strip)
{
  fill_palette(leds, strip, paletteIndexH, 255 / 256, myPal2, 255, LINEARBLEND);
//  addGlitter(150);
  EVERY_N_MILLISECONDS(129) {
    paletteIndexH++;
  }
}

void neon_sign()
{
 fill_solid(ledsH, NUM_LEDS_PART_H, CRGB(255, 145, 0));
 fill_solid(ledsB, NUM_LEDS_PART_B, CRGB(120, 0, 255));
 neon_words();
}

void neon_words()
{
 fill_solid(ledsW, NUM_LEDS_PART_W, CRGB(0, 0, 0));
 FastLED.show();
 delay(500);
 fill_solid(ledsW, 50, CRGB(0, 255, 200));
 FastLED.show();
 delay(700);
 fill_solid(ledsW, 129, CRGB(0, 255, 200));
 FastLED.show();
 delay(500);
 flash_4();
 flash_4();
 delay(100);
 flash_4();
 delay(700);
 flash_H();
 delay(100);
 flash_H();
 delay(1200);
}

void flash_4()
{
int ledsToLightUp [13] = {17,18,19,20,21,22,23,24,25,26,27,28,29};

  for(int i = 0; i < 13; i++) {
    ledsW[ledsToLightUp[i]] = CRGB::Black;
  }
  FastLED.show();
  delay(100);
  
  for(int i = 0; i < 13; i++) {
    ledsW[ledsToLightUp[i]] = CRGB(0, 255, 200);
  }
    FastLED.show();
    delay(100);
}

void flash_H()
{
int ledsToLightUp [12] = {104,105,106,107,108,109,110,111,112,113,114,115};

  for(int i = 0; i < 12; i++) {
    ledsW[ledsToLightUp[i]] = CRGB::Black;
  }
  FastLED.show();
  delay(100);
  
  for(int i = 0; i < 12; i++) {
    ledsW[ledsToLightUp[i]] = CRGB(0, 255, 200);
  }
    FastLED.show();
    delay(100);
}

void mode_3()
{
  fade_palette(ledsH, NUM_LEDS_PART_H, 100, 170);
  fade_palette_short(ledsW, NUM_LEDS_PART_W);
  fade_colour(ledsB, NUM_LEDS_PART_B);
}
