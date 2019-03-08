#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include <avr/pgmspace.h>
#include "sand_box.h"
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#define SELECT_RED        4//Chọn màu Đỏ
#define SELECT_GREEN      5//CHọn màu Xanh
//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
// SELECT_RED        4
// SELECT_GREEN      5
// PIN_DMD_nOE       9    // D9 active low Output Enable, setting this low lights all the LEDs in the selected rows. Can pwm it at very high frequency for brightness control.
// PIN_DMD_A         6    // D6
// PIN_DMD_B         7    // D7
// PIN_DMD_CLK       13   // D13_SCK  is SPI Clock if SPI is used
// PIN_DMD_SCLK      8    // D8
// PIN_DMD_R_DATA    11   // D11_MOSI is SPI Master Out if SPI is used
static long long count=0;
static int second=0;//đếm giây
static int t_red=60;//Lưu giá trị tời gian đèn đỏ trước khi cập nhật (nhận từ blue tooth)
static int t_green=57;//Lưu giá trị tời gian đèn đỏ trước khi cập nhật (nhận từ blue tooth)
unsigned long long red_light=60,green_light=57,orange_light=3;//Lưu giá trị thời gian đang xử lý.
enum _STATE{GREEN=0,RED,YELLOW,INIT};// Trạng thái của biển báo
_STATE STATE= INIT;
static int range_animation=(int)(sizeof(animation)/sizeof(animation[0]));// Số lượng các hoạt động
int id_red_animation=0;
int bright=30;//Độ sáng của LED
void ScanDMD()
{ 
  /*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
  dmd.scanDisplayBySPI();
  count++;
  if(count>400){
    count=0;
    second++;
    if(second>=(2*t_red+1)){
      second=0;
    }
  }
}
void setup(void)
{
  pinMode(SELECT_RED,OUTPUT);
  pinMode(SELECT_GREEN,OUTPUT);
/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/

   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 2500 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD  pixels held in RAM
   dmd.clearScreen( false );   //true is normal (all pixels off), false is negative (all pixels on)
   Serial.begin(9600);
}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
void loop(void)
{
  switch(STATE){
//**************************************
    case RED:
    
    digitalWrite(SELECT_GREEN,LOW);
    digitalWrite(SELECT_RED,HIGH);
     for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
      }};
    if(second<red_light){
      int k=(second+count/200)*(range_animation/red_light);
      dmd.writePixel(animation[k][1],animation[k][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[k][1],animation[k][0],GRAPHICS_INVERSE,0);
    };
    if(second>red_light){
      dmd.clearScreen( false );
      STATE=GREEN;
    };
    break;
//**************************************
    case GREEN:
    digitalWrite(SELECT_GREEN,HIGH);
    digitalWrite(SELECT_RED,LOW);
      for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
      }};
     if(second<red_light){
      int k=(second+(count/400))*(range_animation/green_light);
      dmd.writePixel(animation[k][1],animation[k][0],GRAPHICS_INVERSE,1);
      dmd.writePixel(30-animation[k][1],animation[k][0],GRAPHICS_INVERSE,0);
    };
    if(second>(2*t_red-3)){
      dmd.clearScreen( false );
      STATE=YELLOW;
    };
    break;
//*****************************************
    case YELLOW:
    digitalWrite(SELECT_GREEN,HIGH);
    digitalWrite(SELECT_RED,HIGH);
     for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
      }};
    if(second==2*red_light-3){
      draw3(dmd);
    };
    if(second==2*red_light-2){
      draw2(dmd);
    };
    if(second==2*red_light-1){
      draw1(dmd);
    };
    if(second>=(2*red_light)){
      dmd.clearScreen( false );
      STATE=INIT;
    };
    break;
//***************************************
    case INIT:
    digitalWrite(SELECT_GREEN,HIGH);
    digitalWrite(SELECT_RED,HIGH);
    dmd.clearScreen( false );
    for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
        }};
    STATE=RED;
    break;
    default:
    digitalWrite(SELECT_GREEN,HIGH);
    digitalWrite(SELECT_RED,HIGH);
    
    if(second%2==0){
      for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
        }};
    }
    else{
      for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_NORMAL,RG[j*32+i]);
        }};
    }
    break;
    
  }
  analogWrite(9,1023*(bright/100));
  delay(5);
 };
  /*for(int j=0;j<16;j++){
  for(int i=0;i<32;i++){   
      dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
    }};
  delay(500);
  for(int k = 0; k < sizeof(animation)/sizeof(animation[0]); k++)
  {
    dmd.writePixel(animation[k][1],animation[k][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[k][1],animation[k][0],GRAPHICS_INVERSE,0);
    delay(200);
  }
  delay(2000);
  /*for(int j=0;j<16;j++){
  for(int i=0;i<32;i++){   
      dmd.writePixel(i,j,GRAPHICS_INVERSE,test[j*32+i]);
    }};
  delay(500);
  draw3(dmd);
  delay(1000);
  draw2(dmd);
  delay(1000);
  draw1(dmd);
  delay(1000);*/

  
    

