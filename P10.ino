    
//.............................................................................................
//.VVVV....VVVVV..AAAA.....AARRRRRRRRR..RRII.....AAAA.....AABBBBBBBBB..BBLL.......LEEEEEEEEEE..
//.VVVV....VVVV..AAAAAA....AARRRRRRRRRR.RRII....AAAAAA....AABBBBBBBBBB.BBLL.......LEEEEEEEEEE..
//.VVVV....VVVV..AAAAAA....AARRRRRRRRRR.RRII....AAAAAA....AABBBBBBBBBB.BBLL.......LEEEEEEEEEE..
//.VVVVV..VVVV...AAAAAAA...AARR....RRRR.RRII....AAAAAAA...AABB....BBBB.BBLL.......LEEE.........
//..VVVV..VVVV..AAAAAAAA...AARR....RRRR.RRII...AAAAAAAA...AABB....BBBB.BBLL.......LEEE.........
//..VVVV..VVVV..AAAAAAAA...AARRRRRRRRRR.RRII...AAAAAAAA...AABBBBBBBBBB.BBLL.......LEEEEEEEEEE..
//..VVVVVVVVV...AAAA.AAAA..AARRRRRRRRR..RRII...AAAA.AAAA..AABBBBBBBBB..BBLL.......LEEEEEEEEEE..
//...VVVVVVVV..VAAAAAAAAA..AARRRRRRR....RRII..IAAAAAAAAA..AABBBBBBBBBB.BBLL.......LEEEEEEEEEE..
//...VVVVVVVV..VAAAAAAAAAA.AARR.RRRRR...RRII..IAAAAAAAAAA.AABB.....BBBBBBLL.......LEEE.........
//...VVVVVVV..VVAAAAAAAAAA.AARR..RRRRR..RRII.IIAAAAAAAAAA.AABB.....BBBBBBLL.......LEEE.........
//....VVVVVV..VVAA....AAAA.AARR...RRRRR.RRII.IIAA....AAAA.AABBBBBBBBBBBBBLLLLLLLLLLEEEEEEEEEE..
//....VVVVVV..VVAA....AAAAAAARR....RRRR.RRII.IIAA....AAAAAAABBBBBBBBBB.BBLLLLLLLLLLEEEEEEEEEE..
//....VVVVV..VVVAA.....AAAAAARR.....RRRRRRIIIIIAA.....AAAAAABBBBBBBBB..BBLLLLLLLLLLEEEEEEEEEE..
//.............................................................................................
/////////////////////////////////////
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
#define SELECT_RED        4
#define SELECT_GREEN      5
#define MAX_LENGTH_SIZE   10
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
// SELECT_RED        4
// SELECT_GREEN      5
// PIN_DMD_nOE       9    // D9 active low Output Enable, setting this low lights all the LEDs in the selected rows. Can pwm it at very high frequency for brightness control.
// PIN_DMD_A         6    // D6
// PIN_DMD_B         7    // D7
// PIN_DMD_CLK       13   // D13_SCK  is SPI Clock if SPI is used
// PIN_DMD_SCLK      8    // D8
// PIN_DMD_R_DATA    11   // D11_MOSI is SPI Master Out if SPI is used
unsigned long count=0;// tăng lên sau mỗi 5ms
int second=0;// Tăng lên sau mỗi 500ms
int t_red=60;// Tời gian đèn đỏ/xanh sáng
int count_number=t_red;// Thời gian đếm
int animation_red=sizeof(animation)/sizeof(animation[0]);//Số lượng hiệu ứng đèn xanh, đỏ
int count_animation=0;
bool clear_num_once_time=true;
bool do_animation=true;

byte getNum=0;     ////Biến số lấy từ Bluetooth

enum _STATE{INIT_RED=0,RED,INIT_GREEN,GREEN,INIT_YELLOW,YELLOW,SLOW,NOT_TURN_LEFT,NOT_TURN_RIGHT,WARNING,DEF};
_STATE STATE=NOT_TURN_RIGHT;//Trạng thái của hệ thống
//..............................................................................................
//.FFFFFFFFFF.UUUU...UUUU..NNNN...NNNN....CCCCCCC....TTTTTTTTTTTIIII...OOOOOOO.....NNNN...NNNN..
//.FFFFFFFFFF.UUUU...UUUU..NNNNN..NNNN...CCCCCCCCC...TTTTTTTTTTTIIII..OOOOOOOOOO...NNNNN..NNNN..
//.FFFFFFFFFF.UUUU...UUUU..NNNNN..NNNN..CCCCCCCCCCC..TTTTTTTTTTTIIII.OOOOOOOOOOOO..NNNNN..NNNN..
//.FFFF.......UUUU...UUUU..NNNNNN.NNNN..CCCC...CCCCC....TTTT...TIIII.OOOOO..OOOOO..NNNNNN.NNNN..
//.FFFF.......UUUU...UUUU..NNNNNN.NNNN.NCCC.....CCC.....TTTT...TIIIIIOOOO....OOOOO.NNNNNN.NNNN..
//.FFFFFFFFF..UUUU...UUUU..NNNNNNNNNNN.NCCC.............TTTT...TIIIIIOOO......OOOO.NNNNNNNNNNN..
//.FFFFFFFFF..UUUU...UUUU..NNNNNNNNNNN.NCCC.............TTTT...TIIIIIOOO......OOOO.NNNNNNNNNNN..
//.FFFFFFFFF..UUUU...UUUU..NNNNNNNNNNN.NCCC.............TTTT...TIIIIIOOO......OOOO.NNNNNNNNNNN..
//.FFFF.......UUUU...UUUU..NNNNNNNNNNN.NCCC.....CCC.....TTTT...TIIIIIOOOO....OOOOO.NNNNNNNNNNN..
//.FFFF.......UUUU...UUUU..NNNN.NNNNNN..CCCC...CCCCC....TTTT...TIIII.OOOOO..OOOOO..NNNN.NNNNNN..
//.FFFF.......UUUUUUUUUUU..NNNN..NNNNN..CCCCCCCCCCC.....TTTT...TIIII.OOOOOOOOOOOO..NNNN..NNNNN..
//.FFFF........UUUUUUUUU...NNNN..NNNNN...CCCCCCCCCC.....TTTT...TIIII..OOOOOOOOOO...NNNN..NNNNN..
//.FFFF.........UUUUUUU....NNNN...NNNN....CCCCCCC.......TTTT...TIIII....OOOOOO.....NNNN...NNNN..
//..............................................................................................
void clear_1st_num(){// Clear box include number 1st already show
  dmd.drawLine(18,0,18,3,GRAPHICS_NORMAL);//a
  dmd.drawLine(18,0,16,0,GRAPHICS_NORMAL);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_NORMAL);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_NORMAL);//d
  dmd.drawLine(16,0,14,0,GRAPHICS_NORMAL);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_NORMAL);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_NORMAL);//g
}
void clear_2nd_num(){// Clear box include number 2nd already show
  dmd.drawLine(18,12,18,15,GRAPHICS_NORMAL);//a
  dmd.drawLine(18,12,16,12,GRAPHICS_NORMAL);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_NORMAL);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_NORMAL);//d
  dmd.drawLine(16,12,14,12,GRAPHICS_NORMAL);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_NORMAL);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_NORMAL);//g
}
//Draw number in GREEN or RED state
void not_turn_left();
void not_turn_right();
void draw_SLOW();
void draw0_1st();
void draw1_1st();
void draw2_1st();
void draw3_1st();
void draw4_1st();
void draw5_1st();
void draw6_1st();
void draw7_1st();
void draw8_1st();
void draw9_1st();
void draw0_2nd();
void draw1_2nd();
void draw2_2nd();
void draw3_2nd();
void draw4_2nd();
void draw5_2nd();
void draw6_2nd();
void draw7_2nd();
void draw8_2nd();
void draw9_2nd();
//Draw number in YELLOW STATE
 void draw3_yellow(){
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
void draw2_yellow(){
 dmd.drawLine(24,10,22,10,GRAPHICS_NORMAL);
  dmd.drawLine(25,7,25,9,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_INVERSE);
  dmd.drawLine(25,5,25,10,GRAPHICS_INVERSE);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);
  dmd.drawLine(28,10,26,10,GRAPHICS_INVERSE);
  dmd.drawLine(24,5,22,5,GRAPHICS_INVERSE);

 }
void draw1_yellow(){
  dmd.drawLine(25,5,25,10,GRAPHICS_NORMAL);
  dmd.drawLine(28,10,22,10,GRAPHICS_NORMAL);
  dmd.drawLine(24,5,22,5,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_NORMAL);
  dmd.drawLine(25,7,25,9,GRAPHICS_NORMAL);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);
   dmd.drawLine(22,8,29,8,GRAPHICS_INVERSE);
  dmd.drawLine(27,6,27,7,GRAPHICS_INVERSE);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ScanDMD() { 
//Always 5ms MCU do this function
  dmd.scanDisplayBySPI();
  count++;
  if(count%100==0){
    second++;
    do_animation=true;
    if(second>4*t_red){
      second=0;
      count=0;
    }
    
    }
   if(count%200==0){
    count_number--;
    clear_num_once_time=true;
    if (count_number<0){
      count_number=t_red;
    }
   }
}



//...............................................................
//...SSSSSSS....EEEEEEEEEEE.TTTTTTTTTTTUUUU...UUUU..PPPPPPPPP....
//..SSSSSSSSS...EEEEEEEEEEE.TTTTTTTTTTTUUUU...UUUU..PPPPPPPPPP...
//..SSSSSSSSSS..EEEEEEEEEEE.TTTTTTTTTTTUUUU...UUUU..PPPPPPPPPPP..
//.SSSSS..SSSS..EEEE...........TTTT....UUUU...UUUU..PPPP...PPPP..
//.SSSSS........EEEE...........TTTT....UUUU...UUUU..PPPP...PPPP..
//..SSSSSSS.....EEEEEEEEEE.....TTTT....UUUU...UUUU..PPPPPPPPPPP..
//...SSSSSSSSS..EEEEEEEEEE.....TTTT....UUUU...UUUU..PPPPPPPPPP...
//.....SSSSSSS..EEEEEEEEEE.....TTTT....UUUU...UUUU..PPPPPPPPP....
//........SSSSS.EEEE...........TTTT....UUUU...UUUU..PPPP.........
//.SSSS....SSSS.EEEE...........TTTT....UUUU...UUUU..PPPP.........
//.SSSSSSSSSSSS.EEEEEEEEEEE....TTTT....UUUUUUUUUUU..PPPP.........
//..SSSSSSSSSS..EEEEEEEEEEE....TTTT.....UUUUUUUUU...PPPP.........
//...SSSSSSSS...EEEEEEEEEEE....TTTT......UUUUUUU....PPPP.........
//...............................................................
void setup()
{
    //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

    //clear/init the DMD  pixels held in RAM
   dmd.clearScreen( false );   //true is normal (all pixels off), false is negative (all pixels on)
   pinMode(SELECT_RED,OUTPUT);
   pinMode(SELECT_GREEN,OUTPUT); 
   pinMode(9,OUTPUT); 
   analogWrite(9,120);


  Serial.begin(9600);
  Serial.println("T_Red: 30-99");

}
//.....................................................
//.LLLL.........OOOOOOO.......OOOOOOO.....PPPPPPPPP....
//.LLLL........OOOOOOOOOO....OOOOOOOOOO...PPPPPPPPPP...
//.LLLL.......OOOOOOOOOOOO..OOOOOOOOOOOO..PPPPPPPPPPP..
//.LLLL.......OOOOO..OOOOO..OOOOO..OOOOO..PPPP...PPPP..
//.LLLL......LOOOO....OOOOOOOOOO....OOOOO.PPPP...PPPP..
//.LLLL......LOOO......OOOOOOOO......OOOO.PPPPPPPPPPP..
//.LLLL......LOOO......OOOOOOOO......OOOO.PPPPPPPPPP...
//.LLLL......LOOO......OOOOOOOO......OOOO.PPPPPPPPP....
//.LLLL......LOOOO....OOOOOOOOOO....OOOOO.PPPP.........
//.LLLL.......OOOOO..OOOOO..OOOOO..OOOOO..PPPP.........
//.LLLLLLLLLL.OOOOOOOOOOOO..OOOOOOOOOOOO..PPPP.........
//.LLLLLLLLLL..OOOOOOOOOO....OOOOOOOOOO...PPPP.........
//.LLLLLLLLLL....OOOOOO........OOOOOO.....PPPP.........
//.....................................................
 void loop()
{


  
  
  
  switch(STATE){
    case WARNING:
    if (second%2==0){
      draw_warning();
    }else{
       dmd.clearScreen(false);
    }
    break;
    case NOT_TURN_RIGHT:
    not_turn_right();
    break;
    /////////////////
    case NOT_TURN_LEFT:
    not_turn_left();
    break;
    /////////////////
    case SLOW:
    if (second%2==0){
      draw_SLOW();
    }else{
       dmd.clearScreen(false);
    }
    break;
//................................................................................
//.IIIII.NNNN...NNNN.IIIII.TTTTTTTTTT .....RRRRRRRRRR...EEEEEEEEEEE.DDDDDDDDD.....
//.IIIII.NNNNN..NNNN.IIIII.TTTTTTTTTT .....RRRRRRRRRRR..EEEEEEEEEEE.DDDDDDDDDD....
//.IIIII.NNNNN..NNNN.IIIII.TTTTTTTTTT .....RRRRRRRRRRR..EEEEEEEEEEE.DDDDDDDDDDD...
//.IIIII.NNNNNN.NNNN.IIIII....TTTT.........RRRR...RRRRR.EEEE........DDDD...DDDD...
//.IIIII.NNNNNN.NNNN.IIIII....TTTT.........RRRR...RRRRR.EEEE........DDDD....DDDD..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT.........RRRRRRRRRRR..EEEEEEEEEE..DDDD....DDDD..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT.........RRRRRRRRRRR..EEEEEEEEEE..DDDD....DDDD..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT.........RRRRRRRR.....EEEEEEEEEE..DDDD....DDDD..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT.........RRRR.RRRR....EEEE........DDDD....DDDD..
//.IIIII.NNNN.NNNNNN.IIIII....TTTT.........RRRR..RRRR...EEEE........DDDD...DDDDD..
//.IIIII.NNNN..NNNNN.IIIII....TTTT.........RRRR..RRRRR..EEEEEEEEEEE.DDDDDDDDDDD...
//.IIIII.NNNN..NNNNN.IIIII....TTTT.........RRRR...RRRRR.EEEEEEEEEEE.DDDDDDDDDD....
//.IIIII.NNNN...NNNN.IIIII....TTTT.........RRRR....RRRR.EEEEEEEEEEE.DDDDDDDDD.....
//................................................................................
    case INIT_RED:
    second=0;
    digitalWrite(SELECT_RED,LOW);
    digitalWrite(SELECT_GREEN,HIGH);  
    dmd.clearScreen(false);  
    for(int j=0;j<16;j++){
      for(int i=0;i<32;i++){   
        dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
      }};
    count_number=t_red;
    STATE=RED;

    break;
//........................................
//.RRRRRRRRRR...EEEEEEEEEEE.EDDDDDDDD.....
//.RRRRRRRRRRR..EEEEEEEEEEE.EDDDDDDDDD....
//.RRRRRRRRRRR..EEEEEEEEEEE.EDDDDDDDDDD...
//.RRRR...RRRRR.EEEE........EDDD...DDDD...
//.RRRR...RRRRR.EEEE........EDDD....DDDD..
//.RRRRRRRRRRR..EEEEEEEEEE..EDDD....DDDD..
//.RRRRRRRRRRR..EEEEEEEEEE..EDDD....DDDD..
//.RRRRRRRR.....EEEEEEEEEE..EDDD....DDDD..
//.RRRR.RRRR....EEEE........EDDD....DDDD..
//.RRRR..RRRR...EEEE........EDDD...DDDDD..
//.RRRR..RRRRR..EEEEEEEEEEE.EDDDDDDDDDD...
//.RRRR...RRRRR.EEEEEEEEEEE.EDDDDDDDDD....
//.RRRR....RRRR.EEEEEEEEEEE.EDDDDDDDD.....
//........................................
    case RED:
    if(do_animation){
    dmd.writePixel(animation[second][1],animation[second][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[second][1],animation[second][0],GRAPHICS_INVERSE,0);
    do_animation=false;
    }
     if(second>=2*t_red){
      STATE=INIT_GREEN;
      
     };
    
   if(clear_num_once_time){
     clear_1st_num();
     clear_2nd_num();
   clear_num_once_time=false;
     switch((count_number/10)%10){
      case 0:
      draw0_1st();break;
      case 1:
      draw1_1st();break;
      case 2:
      draw2_1st();break;
      case 3:
      draw3_1st();break;
      case 4:
      draw4_1st();break;
      case 5:
      draw5_1st();break;
      case 6:
      draw6_1st();break;
      case 7:
      draw7_1st();break;
      case 8:
      draw8_1st();break;
      case 9:
      draw9_1st();break;
     }
  switch((count_number)%10){
      case 0:
      draw0_2nd();break;
      case 1:
      draw1_2nd();break;
      case 2:
      draw2_2nd();break;
      case 3:
      draw3_2nd();break;
      case 4:
      draw4_2nd();break;
      case 5:
      draw5_2nd();break;
      case 6:
      draw6_2nd();break;
      case 7:
      draw7_2nd();break;
      case 8:
      draw8_2nd();break;
      case 9:
      draw9_2nd();break;
     }
};
     break;
//......................................................................................................................
//.IIIII.NNNN...NNNN.IIIII.TTTTTTTTTT ....YYYY....YYYY.EEEEEEEEEEE.LLLL.......LLLL.........OOOOOOO...OWWWW..WWWWW...WW..
//.IIIII.NNNNN..NNNN.IIIII.TTTTTTTTTT ....YYYYY..YYYYY.EEEEEEEEEEE.LLLL.......LLLL........OOOOOOOOOO..WWWW..WWWWW..WWW..
//.IIIII.NNNNN..NNNN.IIIII.TTTTTTTTTT .....YYYY..YYYY..EEEEEEEEEEE.LLLL.......LLLL.......OOOOOOOOOOOO.WWWW..WWWWWW.WWW..
//.IIIII.NNNNNN.NNNN.IIIII....TTTT.........YYYYYYYYY...EEEE........LLLL.......LLLL.......OOOOO..OOOOO.WWWW.WWWWWWW.WWW..
//.IIIII.NNNNNN.NNNN.IIIII....TTTT..........YYYYYYYY...EEEE........LLLL.......LLLL......OOOOO....OOOOOWWWW.WWWWWWW.WWW..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT...........YYYYYY....EEEEEEEEEE..LLLL.......LLLL......OOOO......OOOO.WWWWWWWWWWW.WWW..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT...........YYYYYY....EEEEEEEEEE..LLLL.......LLLL......OOOO......OOOO.WWWWWWW.WWWWWWW..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT............YYYY.....EEEEEEEEEE..LLLL.......LLLL......OOOO......OOOO.WWWWWWW.WWWWWWW..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT............YYYY.....EEEE........LLLL.......LLLL......OOOOO....OOOOO.WWWWWWW.WWWWWWW..
//.IIIII.NNNN.NNNNNN.IIIII....TTTT............YYYY.....EEEE........LLLL.......LLLL.......OOOOO..OOOOO..WWWWWWW.WWWWWWW..
//.IIIII.NNNN..NNNNN.IIIII....TTTT............YYYY.....EEEEEEEEEEE.LLLLLLLLLL.LLLLLLLLLL.OOOOOOOOOOOO...WWWWW...WWWWW...
//.IIIII.NNNN..NNNNN.IIIII....TTTT............YYYY.....EEEEEEEEEEE.LLLLLLLLLL.LLLLLLLLLL..OOOOOOOOOO....WWWWW...WWWWW...
//.IIIII.NNNN...NNNN.IIIII....TTTT............YYYY.....EEEEEEEEEEE.LLLLLLLLLL.LLLLLLLLLL....OOOOOO......WWWWW...WWWWW...
//......................................................................................................................
     case INIT_YELLOW:
     
     
     digitalWrite(SELECT_RED,HIGH);
    digitalWrite(SELECT_GREEN,HIGH);  
    dmd.clearScreen(false);  
    for(int j=0;j<16;j++){
      for(int i=0;i<32;i++){   
        dmd.writePixel(i,j,GRAPHICS_INVERSE,test[j*32+i]);
      }};
      STATE=YELLOW;
      second=0;
     break;
//...............................................................................
//.YYYY....YYYY.EEEEEEEEEEE.LLLL.......LLLL.........OOOOOOO...OOWWW..WWWWW...WW..
//.YYYYY..YYYYY.EEEEEEEEEEE.LLLL.......LLLL........OOOOOOOOOO..OWWW..WWWWW..WWW..
//..YYYY..YYYY..EEEEEEEEEEE.LLLL.......LLLL.......OOOOOOOOOOOO.OWWW..WWWWWW.WWW..
//..YYYYYYYYY...EEEE........LLLL.......LLLL.......OOOOO..OOOOO.OWWW.WWWWWWW.WWW..
//...YYYYYYYY...EEEE........LLLL.......LLLL......LOOOO....OOOOOOWWW.WWWWWWW.WWW..
//....YYYYYY....EEEEEEEEEE..LLLL.......LLLL......LOOO......OOOO.WWWWWWWWWWW.WWW..
//....YYYYYY....EEEEEEEEEE..LLLL.......LLLL......LOOO......OOOO.WWWWWWW.WWWWWWW..
//.....YYYY.....EEEEEEEEEE..LLLL.......LLLL......LOOO......OOOO.WWWWWWW.WWWWWWW..
//.....YYYY.....EEEE........LLLL.......LLLL......LOOOO....OOOOO.WWWWWWW.WWWWWWW..
//.....YYYY.....EEEE........LLLL.......LLLL.......OOOOO..OOOOO..WWWWWWW.WWWWWWW..
//.....YYYY.....EEEEEEEEEEE.LLLLLLLLLL.LLLLLLLLLL.OOOOOOOOOOOO...WWWWW...WWWWW...
//.....YYYY.....EEEEEEEEEEE.LLLLLLLLLL.LLLLLLLLLL..OOOOOOOOOO....WWWWW...WWWWW...
//.....YYYY.....EEEEEEEEEEE.LLLLLLLLLL.LLLLLLLLLL....OOOOOO......WWWWW...WWWWW...
//...............................................................................
     case YELLOW:
     if((second>=1)&&(second<=3)&&(do_animation)){
      draw3_yellow();
     }
     if((second>3)&&(second<=5)&&(do_animation)){
      draw2_yellow();
     }
     if((second>5)&&(second<=7)&&(do_animation)){
      draw1_yellow();
     }
     if(second>8){
      STATE=INIT_RED;
     }
     break;

//.........................................................................................................
//.IIIII.NNNN...NNNN.IIIII.TTTTTTTTTT ........GGGGGGG....RRRRRRRRRR...EEEEEEEEEEE.EEEEEEEEEEE.NNNN...NNNN..
//.IIIII.NNNNN..NNNN.IIIII.TTTTTTTTTT ......GGGGGGGGGG...RRRRRRRRRRR..EEEEEEEEEEE.EEEEEEEEEEE.NNNNN..NNNN..
//.IIIII.NNNNN..NNNN.IIIII.TTTTTTTTTT .....GGGGGGGGGGGG..RRRRRRRRRRR..EEEEEEEEEEE.EEEEEEEEEEE.NNNNN..NNNN..
//.IIIII.NNNNNN.NNNN.IIIII....TTTT.........GGGGG..GGGGG..RRRR...RRRRR.EEEE........EEEE........NNNNNN.NNNN..
//.IIIII.NNNNNN.NNNN.IIIII....TTTT........GGGGG....GGG...RRRR...RRRRR.EEEE........EEEE........NNNNNN.NNNN..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT........GGGG...........RRRRRRRRRRR..EEEEEEEEEE..EEEEEEEEEE..NNNNNNNNNNN..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT........GGGG..GGGGGGGG.RRRRRRRRRRR..EEEEEEEEEE..EEEEEEEEEE..NNNNNNNNNNN..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT........GGGG..GGGGGGGG.RRRRRRRR.....EEEEEEEEEE..EEEEEEEEEE..NNNNNNNNNNN..
//.IIIII.NNNNNNNNNNN.IIIII....TTTT........GGGGG.GGGGGGGG.RRRR.RRRR....EEEE........EEEE........NNNNNNNNNNN..
//.IIIII.NNNN.NNNNNN.IIIII....TTTT.........GGGGG....GGGG.RRRR..RRRR...EEEE........EEEE........NNNN.NNNNNN..
//.IIIII.NNNN..NNNNN.IIIII....TTTT.........GGGGGGGGGGGG..RRRR..RRRRR..EEEEEEEEEEE.EEEEEEEEEEE.NNNN..NNNNN..
//.IIIII.NNNN..NNNNN.IIIII....TTTT..........GGGGGGGGGG...RRRR...RRRRR.EEEEEEEEEEE.EEEEEEEEEEE.NNNN..NNNNN..
//.IIIII.NNNN...NNNN.IIIII....TTTT............GGGGGGG....RRRR....RRRR.EEEEEEEEEEE.EEEEEEEEEEE.NNNN...NNNN..
//.........................................................................................................
     
     case INIT_GREEN:
     second=0;
          digitalWrite(SELECT_RED,HIGH);
    digitalWrite(SELECT_GREEN,LOW);  
    dmd.clearScreen(false);  
    for(int j=0;j<16;j++){
      for(int i=0;i<32;i++){   
        dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
      }};
      count_number=t_red;
     STATE=GREEN;

     break;
//..................................................................
//.....GGGGGGG....RRRRRRRRRR...EEEEEEEEEEE.EEEEEEEEEEE.NNNN...NNNN..
//...GGGGGGGGGG...RRRRRRRRRRR..EEEEEEEEEEE.EEEEEEEEEEE.NNNNN..NNNN..
//..GGGGGGGGGGGG..RRRRRRRRRRR..EEEEEEEEEEE.EEEEEEEEEEE.NNNNN..NNNN..
//..GGGGG..GGGGG..RRRR...RRRRR.EEEE........EEEE........NNNNNN.NNNN..
//.GGGGG....GGG...RRRR...RRRRR.EEEE........EEEE........NNNNNN.NNNN..
//.GGGG...........RRRRRRRRRRR..EEEEEEEEEE..EEEEEEEEEE..NNNNNNNNNNN..
//.GGGG..GGGGGGGG.RRRRRRRRRRR..EEEEEEEEEE..EEEEEEEEEE..NNNNNNNNNNN..
//.GGGG..GGGGGGGG.RRRRRRRR.....EEEEEEEEEE..EEEEEEEEEE..NNNNNNNNNNN..
//.GGGGG.GGGGGGGG.RRRR.RRRR....EEEE........EEEE........NNNNNNNNNNN..
//..GGGGG....GGGG.RRRR..RRRR...EEEE........EEEE........NNNN.NNNNNN..
//..GGGGGGGGGGGG..RRRR..RRRRR..EEEEEEEEEEE.EEEEEEEEEEE.NNNN..NNNNN..
//...GGGGGGGGGG...RRRR...RRRRR.EEEEEEEEEEE.EEEEEEEEEEE.NNNN..NNNNN..
//.....GGGGGGG....RRRR....RRRR.EEEEEEEEEEE.EEEEEEEEEEE.NNNN...NNNN..
//..................................................................
     case GREEN:
     if(do_animation){
      do_animation=false;
    dmd.writePixel(animation[second][1],animation[second][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[second][1],animation[second][0],GRAPHICS_INVERSE,0);
    }
    if(second>=2*t_red){
      STATE=INIT_YELLOW;
     
    }
  if(clear_num_once_time){
     clear_1st_num();
     clear_2nd_num();
   clear_num_once_time=false;
  switch((count_number/10)%10){
      case 0:
      draw0_1st();break;
      case 1:
      draw1_1st();break;
      case 2:
      draw2_1st();break;
      case 3:
      draw3_1st();break;
      case 4:
      draw4_1st();break;
      case 5:
      draw5_1st();break;
      case 6:
      draw6_1st();break;
      case 7:
      draw7_1st();break;
      case 8:
      draw8_1st();break;
      case 9:
      draw9_1st();break;
     }
  switch((count_number)%10){
      case 0:
      draw0_2nd();break;
      case 1:
      draw1_2nd();break;
      case 2:
      draw2_2nd();break;
      case 3:
      draw3_2nd();break;
      case 4:
      draw4_2nd();break;
      case 5:
      draw5_2nd();break;
      case 6:
      draw6_2nd();break;
      case 7:
      draw7_2nd();break;
      case 8:
      draw8_2nd();break;
      case 9:
      draw9_2nd();break;
     }
  }
     break;
  }

  demoBluetooth();
  delay(100);

}




//........................................................
//.DDDDDDDDD....RRRRRRRRRR......AAAAA..AAAWW..WWWWW...WW..
//.DDDDDDDDDD...RRRRRRRRRRR.....AAAAA...AAWW..WWWWW..WWW..
//.DDDDDDDDDDD..RRRRRRRRRRR....AAAAAA...AAWW..WWWWWW.WWW..
//.DDDD...DDDD..RRRR...RRRRR...AAAAAAA..AAWW.WWWWWWW.WWW..
//.DDDD....DDDD.RRRR...RRRRR..AAAAAAAA..AAWW.WWWWWWW.WWW..
//.DDDD....DDDD.RRRRRRRRRRR...AAAAAAAA...AWWWWWWWWWW.WWW..
//.DDDD....DDDD.RRRRRRRRRRR...AAAA.AAAA..AWWWWWW.WWWWWWW..
//.DDDD....DDDD.RRRRRRRR.....AAAAAAAAAA..AWWWWWW.WWWWWWW..
//.DDDD....DDDD.RRRR.RRRR....AAAAAAAAAAA.AWWWWWW.WWWWWWW..
//.DDDD...DDDDD.RRRR..RRRR...AAAAAAAAAAA.AWWWWWW.WWWWWWW..
//.DDDDDDDDDDD..RRRR..RRRRR.RAAA....AAAA..WWWWW...WWWWW...
//.DDDDDDDDDD...RRRR...RRRRRRAAA.....AAAA.WWWWW...WWWWW...
//.DDDDDDDDD....RRRR....RRRRRAAA.....AAAA.WWWWW...WWWWW...
//........................................................
void draw9_1st(){

  //Number 9
  dmd.drawLine(19,0,19,3,GRAPHICS_INVERSE);//a
  dmd.drawLine(19,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw8_1st(){
  
  //Number 8
  dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw7_1st(){
  
  //Num
  dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  //dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  //dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw6_1st(){
   
  //Number 6
  dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  //dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw5_1st(){
   
  //Number 5
  dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  //dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw4_1st(){
   
  //Number 4
  //dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  //dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw3_1st(){
  
  //Number 3
  dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw2_1st(){
  
  //Number 2
  dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  //dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw1_1st(){
  
  //Num
  //dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  //dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  //dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
void draw0_1st(){
  
  //Number 0
  dmd.drawLine(18,0,18,3,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,0,16,0,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,3,16,3,GRAPHICS_INVERSE);//c
  //dmd.drawLine(16,0,16,3,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,0,14,0,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,3,14,3,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,0,14,3,GRAPHICS_INVERSE);//g
}
///////////////////////////
// Draw Number 2st
///////////////////////////
void draw0_2nd(){
  
  //Number 0
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  //dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw1_2nd(){
 
  //Number 1
  //dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  //dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  //dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw2_2nd(){
  
  //Number 2
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  //dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw3_2nd(){
   
  //Number 3
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw4_2nd(){
   
  //Number 4
  //dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  //dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw5_2nd(){
   
  //Number 5
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  //dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw6_2nd(){
  
  //Number 0
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  //dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw7_2nd(){
  
  //Number 0
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  //dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  //dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  //dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw8_2nd(){
   
  //Number 8
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
void draw9_2nd(){
   
  //Number 9
  dmd.drawLine(18,12,18,15,GRAPHICS_INVERSE);//a
  dmd.drawLine(18,12,16,12,GRAPHICS_INVERSE);//b
  dmd.drawLine(18,15,16,15,GRAPHICS_INVERSE);//c
  dmd.drawLine(16,12,16,15,GRAPHICS_INVERSE);//d
  //dmd.drawLine(16,12,14,12,GRAPHICS_INVERSE);//e
  dmd.drawLine(16,15,14,15,GRAPHICS_INVERSE);//f
  dmd.drawLine(14,12,14,15,GRAPHICS_INVERSE);//g
}
//Not turn LEFT signal
void not_turn_left(){
  digitalWrite(SELECT_RED,LOW);
  digitalWrite(SELECT_GREEN,HIGH);
  for(int i=1;i<15;i++){
    dmd.writePixel(2*i-1,i,GRAPHICS_INVERSE,1);
    dmd.writePixel(2*i,i,GRAPHICS_INVERSE,1);
    dmd.writePixel(2*i+1,i,GRAPHICS_INVERSE,1);
  }
  //draw retangle

  dmd.drawFilledBox(5,9,19,13,GRAPHICS_INVERSE);
  dmd.drawFilledBox(20,5,24,13,GRAPHICS_INVERSE);
  //draw >>>>
  dmd.drawLine(17,4,27,4,GRAPHICS_INVERSE);
  dmd.drawLine(18,3,26,3,GRAPHICS_INVERSE);
  dmd.drawLine(19,2,25,2,GRAPHICS_INVERSE);
  dmd.drawLine(20,1,24,1,GRAPHICS_INVERSE);
  dmd.drawLine(21,0,23,0,GRAPHICS_INVERSE);
 
}
void not_turn_right(){
    digitalWrite(SELECT_RED,LOW);
  digitalWrite(SELECT_GREEN,HIGH);
  for(int i=1;i<15;i++){
    dmd.writePixel(31-(2*i-1),i,GRAPHICS_INVERSE,1);
    dmd.writePixel(31-(2*i),i,GRAPHICS_INVERSE,1);
    dmd.writePixel(31-(2*i+1),i,GRAPHICS_INVERSE,1);
    //draw retangle

    dmd.drawFilledBox(5,2,19,6,GRAPHICS_INVERSE);
    dmd.drawFilledBox(20,2,24,10,GRAPHICS_INVERSE);
      //draw >>>>
  dmd.drawLine(17,11,27,11,GRAPHICS_INVERSE);
  dmd.drawLine(18,12,26,12,GRAPHICS_INVERSE);
  dmd.drawLine(19,13,25,13,GRAPHICS_INVERSE);
  dmd.drawLine(20,14,24,14,GRAPHICS_INVERSE);
  dmd.drawLine(21,15,23,15,GRAPHICS_INVERSE);
  }
  
}
//........................................................
//...SSSSSSS....LLLL.........OOOOOOO...OOWWW..WWWWW...WW..
//..SSSSSSSSS...LLLL........OOOOOOOOOO..OWWW..WWWWW..WWW..
//..SSSSSSSSSS..LLLL.......OOOOOOOOOOOO.OWWW..WWWWWW.WWW..
//.SSSSS..SSSS..LLLL.......OOOOO..OOOOO.OWWW.WWWWWWW.WWW..
//.SSSSS........LLLL......LOOOO....OOOOOOWWW.WWWWWWW.WWW..
//..SSSSSSS.....LLLL......LOOO......OOOO.WWWWWWWWWWW.WWW..
//...SSSSSSSSS..LLLL......LOOO......OOOO.WWWWWWW.WWWWWWW..
//.....SSSSSSS..LLLL......LOOO......OOOO.WWWWWWW.WWWWWWW..
//........SSSSS.LLLL......LOOOO....OOOOO.WWWWWWW.WWWWWWW..
//.SSSS....SSSS.LLLL.......OOOOO..OOOOO..WWWWWWW.WWWWWWW..
//.SSSSSSSSSSSS.LLLLLLLLLL.OOOOOOOOOOOO...WWWWW...WWWWW...
//..SSSSSSSSSS..LLLLLLLLLL..OOOOOOOOOO....WWWWW...WWWWW...
//...SSSSSSSS...LLLLLLLLLL....OOOOOO......WWWWW...WWWWW...
//........................................................
 void draw_SLOW(){
  digitalWrite(SELECT_GREEN,HIGH);
  digitalWrite(SELECT_RED,HIGH);
 dmd.clearScreen(false);
 //Draw S
 dmd.writePixel(30,10,GRAPHICS_INVERSE,1);
 dmd.drawLine(31,6,31,9,GRAPHICS_INVERSE);
 dmd.drawLine(30,5,29,5,GRAPHICS_INVERSE);
 dmd.drawLine(28,6,28,8,GRAPHICS_INVERSE);
 dmd.writePixel(27,9,GRAPHICS_INVERSE,1);
 dmd.drawLine(31,6,31,9,GRAPHICS_INVERSE);
 dmd.drawLine(26,10,25,10,GRAPHICS_INVERSE);
 dmd.writePixel(25,5,GRAPHICS_INVERSE,1);
 dmd.drawLine(24,6,24,9,GRAPHICS_INVERSE);
 //Draw L
 dmd.drawLine(22,5,16,5,GRAPHICS_INVERSE);dmd.drawLine(16,6,16,10,GRAPHICS_INVERSE);
 //Draw O
 dmd.drawLine(9,5,13,5,GRAPHICS_INVERSE);
 dmd.drawLine(9,10,13,10,GRAPHICS_INVERSE);
 dmd.drawLine(14,6,14,10,GRAPHICS_INVERSE);
 dmd.drawLine(8,6,8,10,GRAPHICS_INVERSE);
 
 //Draw W
 dmd.drawLine(6,5,2,5,GRAPHICS_INVERSE);
 dmd.drawLine(6,11,2,11,GRAPHICS_INVERSE);
 dmd.drawLine(5,8,0,8,GRAPHICS_INVERSE);
 
 
 }
//.........................................................................................
//.WWWW..WWWWW...WWWA..AAAAA.....RRRRRRRRRR...NNNN...NNNN.NIIII.NNNN...NNNN.....GGGGGGG....
//.WWWW..WWWWW..WWWW...AAAAA.....RRRRRRRRRRR..NNNNN..NNNN.NIIII.NNNNN..NNNN...GGGGGGGGGG...
//.WWWW..WWWWWW.WWWW..AAAAAA.....RRRRRRRRRRR..NNNNN..NNNN.NIIII.NNNNN..NNNN..GGGGGGGGGGGG..
//.WWWW.WWWWWWW.WWWW..AAAAAAA....RRRR...RRRRR.NNNNNN.NNNN.NIIII.NNNNNN.NNNN..GGGGG..GGGGG..
//.WWWW.WWWWWWW.WWWW.AAAAAAAA....RRRR...RRRRR.NNNNNN.NNNN.NIIII.NNNNNN.NNNN.NGGGG....GGG...
//..WWWWWWWWWWW.WWW..AAAAAAAA....RRRRRRRRRRR..NNNNNNNNNNN.NIIII.NNNNNNNNNNN.NGGG...........
//..WWWWWWW.WWWWWWW..AAAA.AAAA...RRRRRRRRRRR..NNNNNNNNNNN.NIIII.NNNNNNNNNNN.NGGG..GGGGGGG..
//..WWWWWWW.WWWWWWW.AAAAAAAAAA...RRRRRRRR.....NNNNNNNNNNN.NIIII.NNNNNNNNNNN.NGGG..GGGGGGG..
//..WWWWWWW.WWWWWWW.AAAAAAAAAAA..RRRR.RRRR....NNNNNNNNNNN.NIIII.NNNNNNNNNNN.NGGGG.GGGGGGG..
//..WWWWWWW.WWWWWWW.AAAAAAAAAAA..RRRR..RRRR...NNNN.NNNNNN.NIIII.NNNN.NNNNNN..GGGGG....GGG..
//...WWWWW...WWWWW.WAAA....AAAA..RRRR..RRRRR..NNNN..NNNNN.NIIII.NNNN..NNNNN..GGGGGGGGGGGG..
//...WWWWW...WWWWW.WAAA.....AAAA.RRRR...RRRRR.NNNN..NNNNN.NIIII.NNNN..NNNNN...GGGGGGGGGG...
//...WWWWW...WWWWWWWAAA.....AAAA.RRRR....RRRR.NNNN...NNNN.NIIII.NNNN...NNNN.....GGGGGGG....
//.........................................................................................
void draw_warning(){
  digitalWrite(SELECT_GREEN,HIGH);
  digitalWrite(SELECT_RED,HIGH);
  for (int i=0;i<16;i++){
    for(int j=0;j<32;j++){
      dmd.writePixel(j,i,GRAPHICS_INVERSE,1);
    }
  }
}
void demoBluetooth() {
  
  char str[2];
  if(Serial.available()) {
    Serial.readBytes(str, 2);
    Serial.println(str);
    str[2] = '\0';
    assignNumber(str);
    t_red = getNum;
  }
  
}


void assignNumber(String str) {
  switch (str[1]) {
    case '0':
      switch (str[0]) {
        case '0':
          STATE = INIT_RED;
          break;
        case '1':
          STATE = DEF;
          break;
        case '3':
          getNum = 30;
          break;
        case '4':
          getNum = 40;
          break;
        case '5':
          getNum = 50;
          break;
        case '6':
          getNum = 60;
          break;
        case '7':
          getNum = 70;
          break;
        case '8':
          getNum = 80;
          break;
        case '9':
          getNum = 90;
          break;
        default:
          Serial.println("Er");
          break;
      }
      break;
    case '1':
      switch (str[0]) {
        case '0':
          STATE = RED;
          dmd.clearScreen(false);
          break;
        case '3':
          getNum = 31;
          break;
        case '4':
          getNum = 41;
          break;
        case '5':
          getNum = 51;
          break;
        case '6':
          getNum = 61;
          break;
        case '7':
          getNum = 71;
          break;
        case '8':
          getNum = 81;
          break;
        case '9':
          getNum = 91;
          break;
        default:
          Serial.println("Er");
          break;
      }
      break;
    case '2':
      switch (str[0]) {
          case '0':
            STATE = INIT_GREEN;
            break;
          case '3':
            getNum = 32;
            break;
          case '4':
            getNum = 42;
            break;
          case '5':
            getNum = 52;
            break;
          case '6':
            getNum = 62;
            break;
          case '7':
            getNum = 72;
            break;
          case '8':
            getNum = 82;
            break;
          case '9':
            getNum = 92;
            break;
          default:
            Serial.println("Er");
            break;
      }
      break;
    case '3':
      switch (str[0]) {
          case '0':
            STATE = GREEN;
            dmd.clearScreen(false);
            break;
          case '3':
            getNum = 33;
            break;
          case '4':
            getNum = 43;
            break;
          case '5':
            getNum = 53;
            break;
          case '6':
            getNum = 63;
            break;
          case '7':
            getNum = 73;
            break;
          case '8':
            getNum = 83;
            break;
          case '9':
            getNum = 93;
            break;
          default:
            Serial.println("Er");
            break;
      }
      break;
    case '4':
      switch (str[0]) {
          case '0':
            STATE = INIT_YELLOW;
            
            break;
          case '3':
            getNum = 34;
            break;
          case '4':
            getNum = 44;
            break;
          case '5':
            getNum = 54;
            break;
          case '6':
            getNum = 64;
            break;
          case '7':
            getNum = 74;
            break;
          case '8':
            getNum = 84;
            break;
          case '9':
            getNum = 94;
            break;
          default:
            Serial.println("Er");
            break;
        }
      break;
    case '5':
      switch (str[0]) {
          case '0':
            STATE = YELLOW;
            dmd.clearScreen(false);
            break;
          case '3':
            getNum = 35;
            break;
          case '4':
            getNum = 45;
            break;
          case '5':
            getNum = 55;
            break;
          case '6':
            getNum = 65;
            break;
          case '7':
            getNum = 75;
            break;
          case '8':
            getNum = 85;
            break;
          case '9':
            getNum = 95;
            break;
          default:
            Serial.println("Er");
            break;
        }
      break;
    case '6':
      switch (str[0]) {
          case '0':
            STATE = SLOW;
            break;
          case '3':
            getNum = 36;
            break;
          case '4':
            getNum = 46;
            break;
          case '5':
            getNum = 56;
            break;
          case '6':
            getNum = 66;
            break;
          case '7':
            getNum = 76;
            break;
          case '8':
            getNum = 86;
            break;
          case '9':
            getNum = 96;
            break;
          default:
            Serial.println("Er");
            break;
        }
      break;
    case '7':
      switch (str[0]) {
          case '0':
            STATE = NOT_TURN_LEFT;
            dmd.clearScreen(false);
            break;
          case '3':
            getNum = 37;
            break;
          case '4':
            getNum = 47;
            break;
          case '5':
            getNum = 57;
            break;
          case '6':
            getNum = 67;
            break;
          case '7':
            getNum = 77;
            break;
          case '8':
            getNum = 87;
            break;
          case '9':
            getNum = 97;
            break;
          default:
            Serial.println("Er");
            break;
        }
      break;
    case '8':
      switch (str[0]) {
          case '0':
            STATE = NOT_TURN_RIGHT;
            dmd.clearScreen(false);
            break;
          case '3':
            getNum = 38;
            break;
          case '4':
            getNum = 48;
            break;
          case '5':
            getNum = 58;
            break;
          case '6':
            getNum = 68;
            break;
          case '7':
            getNum = 78;
            break;
          case '8':
            getNum = 88;
            break;
          case '9':
            getNum = 98;
            break;
          default:
            Serial.println("Er");
            break;
        }
        break;
    case '9':
      switch (str[0]) {
          case '0':
            STATE = WARNING;
            break;
          case '3':
            getNum = 39;
            break;
          case '4':
            getNum = 49;
            break;
          case '5':
            getNum = 59;
            break;
          case '6':
            getNum = 69;
            break;
          case '7':
            getNum = 79;
            break;
          case '8':
            getNum = 89;
            break;
          case '9':
            getNum = 99;
            break;
          default:
            Serial.println("Er");
            break;
        }
      break;
    default:
      str = "";
      Serial.println("E");
      break;
  }
}
