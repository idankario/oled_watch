#include "./time.h"
#include <string.h>
#include <stdlib.h>
#include "../oledDriver/oledC.h"
#include "../oledDriver/oledC_shapes.h"
#include <math.h>
const uint16_t COLOR_SADDLEBROWN = 0x8a22; // converted from RGB(139,69,19) 
const uint16_t COLOR_DARKRED = 0x8800; // converted from RGB(139,0,0)
const uint16_t COLOR_SNOW = 0xffdf; // converted from RGB(255,250,250)
const uint16_t COLOR_TOMATO = 0xfb08; // converted from RGB(255,99,71)
const uint16_t COLOR_BLUE = 0x1f; // converted from RGB(0,0,255)
const uint16_t COLOR_BLACK = 0x0; // converted from RGB(0,0,0)
#define CENTER_X      48
#define CENTER_Y      36
bool is24 = true;
bool show=true;
bool isAm;
bool alarm=false; 
short Thour=0;
bool isfirst;
//Day Month Year
short indexM;
char time[9];
char date[12];
char timeAlarm[9];
char dateAlarm[12];
watchType watch,wt;//watch type
short months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
uint16_t xs, ys,xm,ym,xh,yh;
const char *monthStr[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
void changeInterval(){
    if(is24)
        is24=false;
    else
        is24=true;
}
//Set Month string
void setMonthStr()
{
    date[0]=monthStr[indexM][0];
    date[1]=monthStr[indexM][1];
    date[2]=monthStr[indexM][2];
}
void setTypeWatch(watchType w){
    if(watch!=Empty||watch!=Smallwatch)
        wt=watch;
    watch=w;
    isfirst=true;
}
void returnLastWatchType(){
    watch=wt;
}
//Set Month by the system
void setMonth(){
    indexM=(__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n') ? 0 : \
        (__DATE__[0] == 'F') ? 1 : \
        (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r') ? 2 : \
        (__DATE__[0] == 'A' && __DATE__[1] == 'p') ? 3 : \
        (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y') ? 4 : \
        (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n') ? 5 : \
        (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l') ? 6 : \
        (__DATE__[0] == 'A' && __DATE__[1] == 'u') ? 7 : \
        (__DATE__[0] == 'S') ? 8 : \
        (__DATE__[0] == 'O') ? 9 : \
        (__DATE__[0] == 'N') ? 10 : \
        (__DATE__[0] == 'D') ? 11 : \
        /* error default */    '?' ;
}
//Set date of month and year
void setYearDay(char* d){
    //Day
    date[4] = d[4],date[5] = d[5];
    //Year
    date[7] = d[7],date[8] = d[8],date[9] = d[9],date[10] = d[10];
}
//Swap string of month namer to name
void setM(char m){
    indexM=m+'0'-1;
    setMonthStr();
}
void setStrTime(char* t){
    strcpy(t,time);
}
void setStrDate(char* d){
    strcpy(d,date);
}
void setDate(char* d,short m){
    setYearDay(d);
    indexM=m;
    setMonthStr();
}
bool checkAlarm(){
    char t[9];
    strcpy(t,time);
    t[0] = Thour/10+'0';
    t[1] = Thour%10+'0';
    if(alarm&&(strcmp(timeAlarm,t)==0)&&(strcmp(dateAlarm,date)==0)){
        alarm=false;
        return true;
    }
    else
        return false;
}
void setAlarmWatch(char* t,char* d){
    alarm=true;
    strcpy(timeAlarm,t);
    strcpy(dateAlarm,d);
}
void setTime(char* t){
    char hc[3];
    hc[0] = t[0],hc[1] = t[1],hc[2] = '\0';
    Thour=atoi(hc);
    if(Thour<12){
        isAm=true;
        oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"AM", COLOR_SADDLEBROWN);
    }
    else{
        isAm=false;
        oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"PM", COLOR_SADDLEBROWN);
    }
    time[2] = ':';
    time[3] = t[3],time[4] = t[4];//min
    time[5] = ':';
    time[6] = t[6],time[7] = t[7];//sec
    time[8] = '\0';
}
void setTimeDateFromSystem(){
    setMonth();
    setMonthStr();
    date[3]=' ';
    setYearDay(__DATE__);
    date[6] =' ';
    date[11] = '\0';
    setTime(__TIME__);
    //Draw date
    oledC_DrawString(30, 80, 1, 1, (uint8_t*)date, COLOR_SADDLEBROWN);
}
//Get name of month 
const char* getMonthStr(short i){
    return monthStr[i];
}
//Get bool for is24 to know current stage of watch
bool getInterval(){
    return is24;
}
//Get maximud day on the month
short getMaxDay(int m){
    return months[m];
}
//Display Am of Pm 
void displayAmPm(){
    if(Thour<12){
        if(!isAm){
            isAm=true;
            oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"PM", COLOR_SNOW);
            oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"AM", COLOR_SADDLEBROWN);
        }
    }
    else{
        if(isAm){
            isAm=false;
            oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"AM", COLOR_SNOW);
            oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"PM", COLOR_SADDLEBROWN);
            oledC_DrawLine(21, 36, 27, 36, 2, COLOR_SNOW);
            oledC_DrawLine(69, 36, 75, 36, 2, COLOR_SNOW);
        }
    }
}

// Begin at 0 degree and stop at 360 degree
// To convert degree into radians, we have to multiply degree by (pi/180)
// degree (in radians) = degree * pi / 180
// degree (in radians) = degree / (180/pi)
// pi = 57.29577951 ( Python math.pi value )
void displayanalog(  )
{
    uint16_t a = 0;
    uint16_t x1, x2, y1, y2;
    oledC_DrawRectangle(0, 26, 96, 46, COLOR_SADDLEBROWN);
    oledC_DrawRectangle(0, 36, 96, 38, COLOR_SNOW);
    oledC_DrawCircle(48, 36, 36, COLOR_SADDLEBROWN);
    oledC_DrawCircle(48, 36, 3, COLOR_SNOW);
    isfirst=false; 
    // Draw Small Mark for Every Hour 12 3 6 9 is thick
    for( a=0; a<360; a+=30 )
    {
      uint8_t thickness=1;
      float angle = a ;
      angle = (angle/57.29577951) ;         //Convert degrees to radians
      x1 = (CENTER_X + (sin(angle)*32));
      y1 = (CENTER_Y - (cos(angle)*32));
      x2 = (CENTER_X + (sin(angle)*(32-5)));
      y2 = (CENTER_Y - (cos(angle)*(32-5)));
      if(a%90==0)
          thickness=2;
      oledC_DrawLine(x1, y1, x2, y2, thickness,COLOR_SNOW);
    }
}
//Display time hand
void display_time(uint16_t cs,uint16_t cm, uint16_t ch)
{
    oledC_DrawCircle(CENTER_X, CENTER_Y, 3, ch);
    // display second hand
    int hour=time[1]-'0'+(time[0]-'0')*10;
    int minute=time[4]-'0'+(time[3]-'0')*10;
    int second=time[7]-'0'+(time[6]-'0')*10;
    float angle = 0.0;
    uint16_t temp = 0;
    // display second hand
    temp = (uint16_t)second * (uint16_t)6;
    angle= ((float)temp/57.29577951) ;      //Convert degrees to radians  
    xs = ( CENTER_X + (sin(angle)*(25)) );
    ys = ( CENTER_Y - (cos(angle)*(25)) );
    oledC_DrawLine( CENTER_X, CENTER_Y, xs, ys, 1,cs);
    // display minute hand
    temp = minute * 6;
    angle= ((float)temp/57.29577951) ;      //Convert degrees to radians  
    xm = ( CENTER_X + (sin(angle)*(18)) );
    ym = ( CENTER_Y - (cos(angle)*(18)) );
    oledC_DrawLine( CENTER_X, CENTER_Y, xm, ym, 1,cm);
    // display hour hand
    angle = hour * 30 + ((minute*6)/12 )   ;
    angle=(angle/57.29577951) ;             //Convert degrees to radians  
    xh = ( CENTER_X + (sin(angle)*(8)) );
    yh = ( CENTER_Y - (cos(angle)*(8)) );
    oledC_DrawLine( CENTER_X, CENTER_Y, xh, yh, 3,ch);
}
void displayExclusivewatch(  )
{
    uint16_t a = 0;
    uint16_t x1, x2, y1, y2;
    oledC_DrawRing(48, 36, 36, 3, COLOR_BLACK);
    oledC_DrawCircle(48, 36, 3, COLOR_BLACK);
    isfirst=false; 
    // Draw Small Mark for Every Hour 12 3 6 9 is thick
    for( a=0; a<360; a+=30 )
    {
      uint8_t thickness=1;
      float angle = a ;
      angle = (angle/57.29577951) ;         //Convert degrees to radians
      x1 = (CENTER_X + (sin(angle)*32));
      y1 = (CENTER_Y - (cos(angle)*32));
      x2 = (CENTER_X + (sin(angle)*(32-5)));
      y2 = (CENTER_Y - (cos(angle)*(32-5)));
      if(a%90==0)
          thickness=2;
      oledC_DrawLine(x1, y1, x2, y2, thickness,COLOR_BLACK);
    }
}
void updateTime(){
    short th1,th2;
    if(is24){
        th1=Thour/10;
        th2=Thour%10;
    }
    else{
        short th=Thour%12;
        th1=th/10;
        th2=th%10;
    }
    time[0] = th1+'0';
    time[1] = th2+'0';
}
//Display watch by type 0 for digital and 1 for analog
void displayWatch(bool isShowMenu){
    if(isShowMenu){
        if(watch==Smallwatch){//Small watch
            oledC_DrawString(45, 77, 1, 1, (uint8_t*)time, COLOR_SNOW);  
            increase_second();
            updateTime();
            oledC_DrawString(45, 77, 1, 1, (uint8_t*)time, COLOR_SADDLEBROWN);  
        }
        else
          increase_second();  
    }
    else if(watch==Exclusivewatch ){//analog
        increase_second();
        if(isfirst)
            displayanalog();
        else{
            oledC_DrawLine( CENTER_X, CENTER_Y, xs, ys, 1,COLOR_SADDLEBROWN);
            oledC_DrawLine( CENTER_X, CENTER_Y, xm, ym, 1,COLOR_SADDLEBROWN);
            oledC_DrawLine( CENTER_X, CENTER_Y, xh, yh, 2,COLOR_SADDLEBROWN);
        }
        display_time(COLOR_SNOW,COLOR_SNOW,COLOR_SNOW);
    }
    else if(watch==Digital){//digital
        oledC_DrawString(5, 40, 2, 2, (uint8_t*)time, COLOR_SNOW);  
        increase_second();
        updateTime();
        oledC_DrawString(5, 40, 2, 2, (uint8_t*)time, COLOR_SADDLEBROWN);     
    }
    else if(watch==Analog){
        increase_second();
        if(isfirst)
            displayExclusivewatch(  );
        else{
            oledC_DrawLine( CENTER_X, CENTER_Y, xs, ys, 1,COLOR_SNOW);
            oledC_DrawLine( CENTER_X, CENTER_Y, xm, ym, 1,COLOR_SNOW);
            oledC_DrawLine( CENTER_X, CENTER_Y, xh, yh, 2,COLOR_SNOW);
        }
        display_time(COLOR_DARKRED,COLOR_BLUE,COLOR_BLACK);
    }   
}

void showWatch(){
    show=true;
    oledC_setBackground(COLOR_SNOW);
    oledC_DrawString(30, 80, 1, 1, (uint8_t*)date, COLOR_SADDLEBROWN);
    if(Thour<12)
        oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"AM", COLOR_SADDLEBROWN);
    else
        oledC_DrawString(5, 80, 1, 1, (uint8_t *)&"PM", COLOR_SADDLEBROWN);
}
//Increase time
void increase_year(){
    short nextYear=(date[7]-'0')*1000+(date[8]-'0')*100+(date[9]-'0')*10+(date[10]-'0')+1;
    date[7]=nextYear/1000+'0';
    date[8]=nextYear/100%10+'0';
    date[9]=nextYear/10%100+'0';
    date[10]=nextYear%10+'0';
}
void increase_month(){
    indexM++;
    if(indexM>11){
        indexM=0;
        increase_year();
    }
    setMonthStr();
}
//Draw and update date
void increase_day(){
    oledC_DrawString(30, 80, 1, 1, (uint8_t*)date, COLOR_SNOW);
    short nextDay=(date[4]-'0')*10+(date[5]-'0')+1;
    if(nextDay>months[indexM]){
        date[4]='0',date[5]='1';
        increase_month();
    }
    else{
        date[4]=(nextDay/10)+'0',date[5]=(nextDay%10)+'0';
    }
    if(show)
        oledC_DrawString(30, 80, 1, 1, (uint8_t*)date, COLOR_SADDLEBROWN);
}

//Draw Am or Pm 
void increase_hour(){
    Thour++;
    if(show)
        displayAmPm();
    if(Thour==24){
        Thour=0;
        increase_day();
    }
}
void increase_min()
{
    time[4] = (time[4]-'0'+1)+'0';
    if(time[4]==':')
    {
        time[4]='0';
        time[3] = (time[3]-'0'+1)+'0';  
        if(time[3]=='6')
        {
            time[3]='0';
            increase_hour();
        }
    }
}
void increase_second()
{  
    time[7] = (time[7]-'0'+1)+'0';
    if(time[7]==':')
    {
        time[7]='0';
        time[6] = (time[6]-'0'+1)+'0';
        if(time[6]=='6')
        {
            time[6]='0';
            increase_min();
        }
    }
}

