#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <FastLED.h>
MPU6050 accelgyro;


#define NUM_LEDS 105
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
int leftLED[]={5,25,33,57,56,55,54,53,52,51,63,85,95};
int rightLED[]={9,19,41,47,48,49,50,51,52,53,71,79,99};
int flag=1;
int js=0;
uint8_t max_bright = 255;  //0~255 bright

CRGB leds[NUM_LEDS];

CRGB myRGBcolor(0,0,0);

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {

//接入i2c总线
Wire.begin();
Serial.begin(38400);
//初始化设备
Serial.println("Initializing I2C devices...");
accelgyro.initialize();

//链接设备
Serial.println("Testing device connections...");
Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

//Serial.begin(9600);

LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

FastLED.setBrightness(max_bright);
}

//将原始值修正
void fix() {
ax /= 16384;
ay /= 16384;
az /= 16384;

gx /= 131;
gy /= 131;
gz /= 131;
}

void Left(){
 for (int x=0;x<=12;x++){
    leds[leftLED[x]]=CRGB::Yellow;
    FastLED.show();
    delay(1);
  }
  delay(500);
  for (int x=0;x<=12;x++){
    leds[leftLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
  delay(500);
}

void Right(){
 for (int x=0;x<=12;x++){
    leds[rightLED[x]]=CRGB::Yellow;
    FastLED.show();
    delay(1);
  }
  delay(500);
  for (int x=0;x<=12;x++){
    leds[rightLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
  delay(500);
}

void all(){
  fill_solid(leds, NUM_LEDS, CRGB::Yellow);
  FastLED.show();
  delay(500);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(500);
}

void endall(){
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void loop() {

accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//对原始价值做处理
fix();

Serial.print("a/g:\t");
//x,y,z轴加速度
//Serial.print(ax); Serial.print("\t");
//Serial.print(ay); Serial.print("\t");
//Serial.print(az); Serial.print("\t");

//x,y,z轴角速度
//Serial.print(gx); Serial.print("\t");
//Serial.println(gy); //Serial.print("\t");
//Serial.println(gz);

Serial.print(gy); Serial.print("\t");
Serial.print(js); Serial.print("\t");
Serial.println(flag); Serial.print("\t");
  if(flag==1){
    if(gy>10){
      flag=2;
    }else if(gy<-30){
      flag=0;
    }
  }
  if(flag==0 or flag==2){
    js++;
  }
  if(js<10){
    if(flag==2) {
      Left();
    }else if(flag==0){
      Right();
    }
  }else if(js==10){
    flag=1;
    endall();
    js=0;
  }
  delay(100);
}
