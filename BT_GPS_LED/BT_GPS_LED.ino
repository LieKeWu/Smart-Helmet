#include <SoftwareSerial.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <FastLED.h>
MPU6050 accelgyro;

SoftwareSerial BTSerial(10, 11);

#define NUM_LEDS 105
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
int leftLED[]={5,25,33,57,56,55,54,53,52,51,63,85,95};
int rightLED[]={9,19,41,47,48,49,50,51,52,53,71,79,99};
int allLED[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,29,30,44,45,59,60,74,75,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104};

int flag=1;
int flag2=0;
int js=0;
uint8_t max_bright = 255;  //0~255 bright

CRGB leds[NUM_LEDS];

CRGB myRGBcolor(0,0,0);

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define GpsSerial  Serial
#define DebugSerial Serial
int L = 13; //LED指示灯引脚
int val = 0;  //设置模拟引脚0读取光敏二极管的电压值



//将原始值修正

struct
{
  char GPS_Buffer[80];
  bool isGetData;   //是否获取到GPS数据
  bool isParseData; //是否解析完成
  char UTCTime[11];   //UTC时间
  char latitude[11];    //纬度
  char N_S[2];    //N/S
  char longitude[12];   //经度
  char E_W[2];    //E/W
  bool isUsefull;   //定位信息是否有效
} Save_Data;

const unsigned int gpsRxBufferLength = 600;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int ii = 0;

void setup(){
  Serial.begin(9600);
  BTSerial.begin(9600);
  accelgyro.initialize();

  GpsSerial.begin(9600);      //定义波特率9600
  DebugSerial.begin(9600);
  Save_Data.isGetData = false;
  Save_Data.isParseData = false;
  Save_Data.isUsefull = false;
LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

}

void fix() {
ax /= 16384;
ay /= 16384;
az /= 16384;

gx /= 131;
gy /= 131;
gz /= 131;
}


void loop(){
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  val = analogRead(0);         // 读取电压值0~1023
      //         // 串口查看电压值的变化
  
  //对原始价值做处理
  fix();
     if(val>500){
                             // 一旦小于设定的值，LED灯关闭
             if(flag==1){
                if(gy>15){
                  flag=2;
                }else if(gy<-20){
                  flag=0;
                }
             }
              if(flag==0 or flag==2){
                js++;
              }
              if(js<5){
                if(flag==2) {
                  NLeft();
                }else if(flag==0){
                  NRight();
                }
              }else if(js==5){
                flag=1;
                js=0;
              }
              if(flag==1 and js==0){
                LedAll();
              }
              delay(100);   
      }else{                        // 否则LED亮起
              if(flag==1){
                  if(gy>15){
                    flag=2;
                  }else if(gy<-20){
                    flag=0;
                  }
               }
               if(flag==0 or flag==2){
                  js++;
               }
               if(js<5){
                  if(flag==2) {
                    Left();
                  }else if(flag==0){
                    Right();
                  }
               }else if(js==5){
                  flag=1;
                  endall();
                  js=0;
               }
               delay(100); 

      }
  
  gpsRead();  //获取GPS数据
  parseGpsBuffer();//解析GPS数据
  printGpsBuffer();//输出解析后的数据
  
  if(BTSerial.available()){
     flag2 = 1; 
      }

  if (flag2 == 1){
    char gpsText=BTSerial.read();
    if(gpsText =='A'){
    BTSerial.print(Save_Data.latitude);
    BTSerial.print(",");
    BTSerial.print(Save_Data.N_S);  
    BTSerial.print(",");
    BTSerial.print(Save_Data.longitude); 
    BTSerial.print(","); 
    BTSerial.print(Save_Data.E_W);
    }
  }
}


void errorLog(int num)
{
  DebugSerial.print("ERROR");
  DebugSerial.println(num);
  while (1)
  {
    digitalWrite(L, HIGH);
    delay(300);
    digitalWrite(L, LOW);
    delay(300);
  }
}

void printGpsBuffer()
{ 
  if (flag2 == 1){
  if (Save_Data.isParseData)
  {
    Save_Data.isParseData = false;
    
    DebugSerial.print("Save_Data.UTCTime = ");
    DebugSerial.println(Save_Data.UTCTime);

    if(Save_Data.isUsefull)
    {
      Save_Data.isUsefull = false;
      DebugSerial.print("Save_Data.latitude = ");
      DebugSerial.println(Save_Data.latitude); 
      DebugSerial.print("Save_Data.N_S = ");
      DebugSerial.println(Save_Data.N_S);
      DebugSerial.print("Save_Data.longitude = ");
      DebugSerial.println(Save_Data.longitude);
      DebugSerial.print("Save_Data.E_W = ");
      DebugSerial.println(Save_Data.E_W);
    }
    else
    {
      DebugSerial.println("GPS DATA is not usefull!");
    }
    
  }
  }
}

void parseGpsBuffer()
{ 
  if (flag2 ==1){
  char *subString;
  char *subStringNext;
  if (Save_Data.isGetData)
  {
    Save_Data.isGetData = false;
    DebugSerial.println("**************");
    DebugSerial.println(Save_Data.GPS_Buffer);

    
    for (int i = 0 ; i <= 6 ; i++)
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          errorLog(1);  //解析错误
      }
      else
      {
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL)
        {
          char usefullBuffer[2]; 
          switch(i)
          {
            case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break; //获取UTC时间
            case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break; //获取UTC时间
            case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;  //获取纬度信息
            case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break; //获取N/S
            case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break; //获取纬度信息
            case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break; //获取E/W

            default:break;
          }

          subString = subStringNext;
          Save_Data.isParseData = true;
          if(usefullBuffer[0] == 'A')
            Save_Data.isUsefull = true;
          else if(usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;

        }
        else
        {
          errorLog(2);  //解析错误
        }
      }


    }
  }
  }
}


void gpsRead() {
   if (flag2 ==1){
  while (GpsSerial.available())
  {
    gpsRxBuffer[ii++] = GpsSerial.read();
    if (ii == gpsRxBufferLength)clrGpsRxBuffer();
  }

  char* GPS_BufferHead;
  char* GPS_BufferTail;
  if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL )
  {
    if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) && (GPS_BufferTail > GPS_BufferHead))
    {
      memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, GPS_BufferTail - GPS_BufferHead);
      Save_Data.isGetData = true;

      clrGpsRxBuffer();
    }
  }
   }
}

void clrGpsRxBuffer(void)
{
  memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空
  ii = 0;
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

void endall(){
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void LedAll(){
   for (int x=0;x<40;x++){
    leds[allLED[x]]=CRGB::Yellow;
    FastLED.show();
    delay(1);
  }
  delay(500);
  for (int x=0;x<40;x++){
    leds[allLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
  delay(500);
 
}
void endLedAll(){
  for (int x=0;x<40;x++){
    leds[allLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
}

void NLeft(){
   for (int x=0;x<40;x++){
    leds[allLED[x]]=CRGB::Yellow;
    FastLED.show();
    delay(1);
  }
  for (int x=0;x<=12;x++){
    leds[leftLED[x]]=CRGB::Yellow;
    FastLED.show();
    delay(1);
  }
  delay(500);
  for (int x=0;x<40;x++){
    leds[allLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
   for (int x=0;x<=12;x++){
    leds[leftLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
  delay(500);
}

void NRight(){
   for (int x=0;x<40;x++){
    leds[allLED[x]]=CRGB::Yellow;
    FastLED.show();
    delay(1);
  }
  for (int x=0;x<=12;x++){
    leds[rightLED[x]]=CRGB::Yellow;
    FastLED.show();
    delay(1);
  }
  delay(500);
  for (int x=0;x<40;x++){
    leds[allLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
   for (int x=0;x<=12;x++){
    leds[rightLED[x]]=CRGB::Black;
    FastLED.show();
    delay(1);
  }
  delay(500);

  
}
