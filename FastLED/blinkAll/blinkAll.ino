#include <FastLED.h>

#define NUM_LEDS 105
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

uint8_t max_bright = 20;  //0~255 bright

CRGB leds[NUM_LEDS];

void setup(){
    Serial.begin(9600);
    delay(1000);

    LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

    FastLED.setBrightness(max_bright);
    
}

void loop(){
//    for (int i =0 ; i <= NUM_LEDS ; i++){
//        leds[i] = CRGB::Yellow;
//        FastLED.show();
//    }
//    delay(100);
//
//    for (int i =0 ; i <= NUM_LEDS ; i++){
//        leds[i] = CRGB::Black;
//        FastLED.show();
//    }
//    delay(100);


//  fill_solid(leds, 105, CRGB::Yellow);
//  FastLED.show();
//  delay(100);
//
//  fill_solid(leds, 105, CRGB::Black);
//  FastLED.show();
//  delay(100);

//  fill_solid(leds, 3, CRGB::Yellow);
//  FastLED.show();
//  delay(100);
//
//  fill_solid(leds, 3, CRGB::Black);
//  FastLED.show();
//  delay(100);
//  
//  fill_solid(leds+5, 3, CRGB::Yellow);
//  FastLED.show();
//  delay(100);
//
//  fill_solid(leds+5, 3, CRGB::Black);
//  FastLED.show();
//  delay(100);

//  for(int i = 0; i < NUM_LEDS-3; i++){
//    fill_solid(leds+i, 3, CRGB::Yellow);
//    FastLED.show();
//    delay(10);
//  
//    fill_solid(leds+i, 3, CRGB::Black);
//    FastLED.show();
//    delay(10);
//  }
//
//  for(int i = NUM_LEDS-3; i > 0 ; i--){
//    fill_solid(leds+i, 3, CRGB::Yellow);
//    FastLED.show();
//    delay(10);
//  
//    fill_solid(leds+i, 3, CRGB::Black);
//    FastLED.show();
//    delay(10);
//  }

}
