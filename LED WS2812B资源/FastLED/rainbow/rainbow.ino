#include <FastLED.h>

#define NUM_LEDS 105
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

//uint8_t beginHue;
uint8_t max_bright = 20;  //0~255 bright

CRGB leds[NUM_LEDS];

CRGB myRGBcolor(0,0,0);

CHSV myHSVcolor(80,255,200); //色调 饱和度 明亮


void setup(){
    Serial.begin(9600);
    delay(1000);

    LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

    FastLED.setBrightness(max_bright);
    
}

void loop(){
//  beginHue++;
//  fill_rainbow(leds, NUM_LEDS, beginHue++,9);

//  fill_gradient_RGB(leds, 0, CRGB::Red, 104, CRGB::Blue);
//  FastLED.show();
//  delay(25);

  fill_gradient(leds, 0, CHSV(50,255,255), 104, CHSV(150,255,255), SHORTEST_HUES);
  addGlitter(60);
  FastLED.show();
  delay(50);
}
