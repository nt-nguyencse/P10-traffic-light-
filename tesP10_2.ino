#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include <avr/pgmspace.h>
#include "sand_box.h"
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

unsigned long long count=0;
int second=0;


void draw3(){
  dmd.drawLine(22,8,29,8,GRAPHICS_NORMAL);
  dmd.drawLine(27,6,27,7,GRAPHICS_NORMAL);
  dmd.drawLine(24,5,22,5,GRAPHICS_NORMAL);
  dmd.drawLine(22,9,29,9,GRAPHICS_NORMAL);
  dmd.drawLine(27,7,27,8,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_INVERSE);
  dmd.drawLine(25,7,25,9,GRAPHICS_INVERSE);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);
  dmd.drawLine(28,10,22,10,GRAPHICS_INVERSE);
  
};
void draw2(){
  dmd.drawLine(24,10,22,10,GRAPHICS_NORMAL);
  dmd.drawLine(25,7,25,9,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_INVERSE);
  dmd.drawLine(25,5,25,10,GRAPHICS_INVERSE);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);
  dmd.drawLine(28,10,26,10,GRAPHICS_INVERSE);
  dmd.drawLine(24,5,22,5,GRAPHICS_INVERSE);
  
}
void draw1(){
  dmd.drawLine(25,5,25,10,GRAPHICS_NORMAL);
  dmd.drawLine(28,10,22,10,GRAPHICS_NORMAL);
  dmd.drawLine(24,5,22,5,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_NORMAL);
  dmd.drawLine(25,7,25,9,GRAPHICS_NORMAL);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);
  
  dmd.drawLine(22,8,29,8,GRAPHICS_INVERSE);
  dmd.drawLine(27,6,27,7,GRAPHICS_INVERSE);
}
void ScanDMD()
{ 
  /*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
  dmd.scanDisplayBySPI();
  count++;
  if(count>200){
    count=0;
  }
}
void setup(void)
{
/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/

   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
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
 
  for(int j=0;j<16;j++){
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
  draw3();
  delay(1000);
  draw2();
  delay(1000);
  draw1();
  delay(1000);*/

  
    
};
