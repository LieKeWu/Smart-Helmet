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
    for (int i =0 ; i <= NUM_LEDS ; i++){
        leds[i-1] = CRGB::Red;
        leds[i] = CRGB::Red;
        leds[i+1] = CRGB::Red;
        FastLED.show();
        delay(50);

        leds[i-1] = CRGB::Green;
        leds[i] = CRGB::Green;
        leds[i+1] = CRGB::Green;
        FastLED.show();
        delay(50);

        leds[i-1] = CRGB::Blue;
        leds[i] = CRGB::Blue;
        leds[i+1] = CRGB::Blue;
        FastLED.show();
        delay(50);

        leds[i-1] = CRGB::Black;
        leds[i] = CRGB::Black;
        leds[i+1] = CRGB::Black;
        FastLED.show();
        delay(50);
    }
    delay(500);
}
