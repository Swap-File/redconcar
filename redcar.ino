#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define NUM_LEDS    193
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255

void setup() {

  FastLED.addLeds<LPD8806, 2, 3, BRG>(leds, NUM_LEDS);

  pinMode(A3, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  FastLED.setBrightness(BRIGHTNESS);
}

#define CENTER_LED 113
#define SIDE_LENGTH 81

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{

  fadeToBlackBy( leds, NUM_LEDS, 20);

  if ( digitalRead(A2) && digitalRead(A5)) {
    byte dothue = 0;
    for ( int i = 0; i < 8; i++) {
      int pos = beatsin16(i + 7, 0, SIDE_LENGTH);
      leds[CENTER_LED + pos] |= CHSV(dothue, 200, 255);
      leds[CENTER_LED - pos] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }

#define REAR_WIDTH 18
#define FRONT_WIDTH 18
#define FRONT_START_POS 12
#define REAR_START_POS 53

  int rear_right = random16(CENTER_LED + REAR_START_POS , CENTER_LED + REAR_START_POS + REAR_WIDTH);
  int front_right = random16(CENTER_LED + FRONT_START_POS , CENTER_LED + FRONT_START_POS + FRONT_WIDTH);
  int front_left = random16(CENTER_LED - FRONT_START_POS - FRONT_WIDTH, CENTER_LED - FRONT_START_POS);
  int rear_left = random16(CENTER_LED - REAR_START_POS - REAR_WIDTH, CENTER_LED - REAR_START_POS);

  if (!digitalRead(A2)) { //left switch
    leds[front_left] += CHSV( gHue + random8(64), 200, 255);
    leds[front_right] += CHSV( gHue + random8(64), 200, 180);
    leds[rear_left] += CHSV( gHue + random8(64), 200, 96);
    leds[rear_right] += CHSV( gHue + random8(64), 200, 64);
  } else if (!digitalRead(A5)) { //right switch
    leds[front_right] += CHSV( gHue + random8(64), 200, 255);
    leds[front_left] += CHSV( gHue + random8(64), 200, 180);
    leds[rear_right] += CHSV( gHue + random8(64), 200, 96);
    leds[rear_left] += CHSV( gHue + random8(64), 200, 64);
  }

  //off switch
  if (digitalRead(A3)) FastLED.clear();
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show();

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

}


