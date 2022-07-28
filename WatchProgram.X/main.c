/*
 * Program create and display watch
 * Author: Idan Kario
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "System/time.h"
#include "System/system.h"
#include "System/delay.h"
#include "oledDriver/oledC.h"
#include "oledDriver/oledC_colors.h"
#include "oledDriver/oledC_shapes.h"
bool isShowMenu=false;
unsigned short clockType = 0; //Digital-0, analog-1 
char strT[9],strD[12];
short getpatcimeterval(){
    AD1CHS = 8 ;
    // Get ADC sample
    AD1CON1bits.SAMP = 1 ;            //Start sampling
    DELAY_milliseconds(100);          //Sample delay, conversion start automatically
    AD1CON1bits.SAMP = 0 ;            //Start sampling
    DELAY_milliseconds(100);          //Sample delay, conversion start automatically
    while (!AD1CON1bits.DONE) ;       //Wait for conversion to complete
    return ADC1BUF0 ;
}

//Timer 1 Interrupt Settings
void __attribute__((interrupt,no_auto_psv))_T1Interrupt(void){
    if(checkAlarm()){
      int l=1;
      while(l){
        oledC_setBackground(OLEDC_COLOR_BLUE);
        oledC_DrawString(10, 40, 1, 1, (uint8_t*)"Press s1 To", OLEDC_COLOR_SNOW);
        oledC_DrawString(10, 52, 1, 1, (uint8_t*)"Stop Alarm", OLEDC_COLOR_SNOW);
        increase_second();
        oledC_setBackground(OLEDC_COLOR_TOMATO);
        oledC_DrawString(10, 40, 1, 1, (uint8_t*)"Press s1 To", OLEDC_COLOR_SNOW);
        oledC_DrawString(10, 52, 1, 1, (uint8_t*)"Stop Alarm", OLEDC_COLOR_SNOW);
        if(isS1Pressed()){
          oledC_setBackground(OLEDC_COLOR_SNOW);
          l=0;
        }
      }
    }
    displayWatch(isShowMenu);
    IFS0bits.T1IF=0;
}
void menuSetTypeWatch(uint16_t colorBackGround,uint16_t color,int i){
    oledC_DrawRectangle(0, i, 96, i+16, colorBackGround );
    oledC_DrawString(5, 2, 1, 1, (uint8_t *)&"Chose type", colorBackGround);
    oledC_DrawString(5, 17, 1, 1, (uint8_t*)"Digital Watch", color);
    oledC_DrawString(5, 32, 1, 1, (uint8_t*)"Analog Watch", color);
    oledC_DrawString(5, 47, 1, 1, (uint8_t *)&"Exclusive Watch", color);
}
void menuSetNewTime(uint16_t colorBackGround,uint16_t color,int i){
    oledC_DrawRectangle(0, i, 96, i+16, colorBackGround );
    oledC_DrawString(5, 2, 1, 1, (uint8_t *)&"S1 Edit/Enter", colorBackGround);
    oledC_DrawString(5, 17, 1, 1, (uint8_t*)strT, color);
    oledC_DrawString(5, 32, 1, 1, (uint8_t*)strD, color);
    oledC_DrawString(5, 47, 1, 1, (uint8_t *)&"Save", color);
    oledC_DrawString(5, 62, 1, 1, (uint8_t *)&"Back", color);
}
void menuOption(uint16_t colorBackGround,uint16_t color,short i){
    char* t;
    if(getInterval())
        t="12 Display Time";
    else
        t="24 Display Time+";
    oledC_DrawRectangle(0, i, 96, i+16, colorBackGround );
    oledC_DrawString(5, 2, 1, 1, (uint8_t *)&"Change Watch", color);
    oledC_DrawString(5, 17, 1, 1, (uint8_t *)&"Set Time", color);
    oledC_DrawString(5, 32, 1, 1, (uint8_t *)t, color);
    oledC_DrawString(5, 47, 1, 1, (uint8_t *)&"Set alarm clock", color);
    oledC_DrawString(5, 62, 1, 1, (uint8_t *)&"Exit", color);
}
void changeTime(char* t){    
    oledC_DrawRectangle(0, 15, 96, 31, OLEDC_COLOR_SNOW );
    char s[3],m[4],h[4];
    h[0]=t[0],h[1]=t[1],h[2]=t[2],h[3]='\0';
    m[0]=t[3],m[1]=t[4],m[2]=t[5],m[3]='\0';
    s[2]='\0';
    oledC_DrawString(5, 17, 1, 1, (uint8_t*)h, OLEDC_COLOR_CYAN);
    oledC_DrawString(25, 17, 1, 1, (uint8_t*)m, OLEDC_COLOR_CYAN);   
    while(1){   
        short sec=getpatcimeterval()/17;
        if(sec>59)
            sec=59;
        s[0]=sec/10+'0',s[1]=sec%10+'0';
        oledC_DrawString(45, 17, 1, 1, (uint8_t*)s, OLEDC_COLOR_CYAN);
        if(isS1Pressed()){ 
            while(!isS2Pressed()){
                DELAY_milliseconds(100);
                oledC_DrawString(25, 17, 1, 1, (uint8_t*)m, OLEDC_COLOR_SNOW);
                short min=getpatcimeterval()/17+1; 
                if(min>59)
                    min=59;
                 m[0]=min/10+'0',m[1]=min%10+'0';
                oledC_DrawString(25, 17, 1, 1, (uint8_t*)m, OLEDC_COLOR_CYAN);
                if(isS1Pressed()){ 
                    while(!isS2Pressed()){ 
                        DELAY_milliseconds(100);
                        oledC_DrawString(5, 17, 1, 1, (uint8_t*)h, OLEDC_COLOR_SNOW);
                        short hour=getpatcimeterval()/43+1;
                        if(hour>23)
                            hour=23;
                        h[0]=hour/10+'0',h[1]=hour%10+'0';
                        oledC_DrawString(5, 17, 1, 1, (uint8_t*)h , OLEDC_COLOR_CYAN);
                        if(isS1Pressed()){ 
                            strcpy(t, h);
                            strcat(t, m);
                            strcat(t, s);
                            return;
                        }
                    }
                }
            }
         }
        if(isS2Pressed())
            return;
        oledC_DrawString(45, 17, 1, 1, (uint8_t*)s, OLEDC_COLOR_SNOW);
    }
}
short changeDate(char* d){   
    oledC_DrawRectangle(0, 30, 96, 46, OLEDC_COLOR_SNOW );
    char m[3]="00",day[5],y[5];
    day[0]=' ',day[1]=d[4],day[2]=d[5],day[3]=' ',day[4]='\0';
    y[0]=d[7],y[1]= d[8],y[2]=d[9],y[3]=d[10],y[4]='\0';
    oledC_DrawString(20, 32, 1, 1, (uint8_t*)day, OLEDC_COLOR_CYAN);
    oledC_DrawString(45, 32, 1, 1, (uint8_t*)y, OLEDC_COLOR_CYAN);   
    while(1){ 
       short mt=getpatcimeterval()/85+1;
       if(mt>12)
            mt=12;
        m[0]=mt/10+'0',m[1]=mt%10+'0';
        oledC_DrawString(5, 32, 1, 1, (uint8_t*)m, OLEDC_COLOR_CYAN);
        if(isS1Pressed()){ 
            short max=getMaxDay(mt-1);
            while(!isS2Pressed()){ 
                DELAY_milliseconds(100);
                oledC_DrawString(20, 32, 1, 1, (uint8_t*)day, OLEDC_COLOR_SNOW);
                short dayP=getpatcimeterval()/34+1;
                if(dayP>max)
                    dayP=max;
                day[1]=dayP/10+'0',day[2]=dayP%10+'0';
                oledC_DrawString(20, 32, 1, 1, (uint8_t*)day, OLEDC_COLOR_CYAN);
                if(isS1Pressed()){ 
                    while(!isS2Pressed()){ 
                        DELAY_milliseconds(100);
                        oledC_DrawString(45, 32, 1, 1, (uint8_t*)y, OLEDC_COLOR_SNOW); 
                        //Value from 0 to 99
                        short yearP=getpatcimeterval()/10;
                        if(yearP>99)
                            yearP=99;
                        y[2]=yearP/10+'0',y[3]=yearP%10+'0';
                        oledC_DrawString(45, 32, 1, 1, (uint8_t*)y, OLEDC_COLOR_CYAN); 
                        if(isS1Pressed()){ 
                            strcpy(d, getMonthStr(mt-1));
                            strcat(d, day);
                            strcat(d, y);
                            oledC_DrawString(45, 32, 1, 1, (uint8_t*)mt, OLEDC_COLOR_CYAN); 
                            return mt-1;
                        }
                    }
                }
            }
         }
        if(isS2Pressed())
            return 0;
        oledC_DrawString(5, 32, 1, 1, (uint8_t*)m, OLEDC_COLOR_SNOW);
    }
}
int setNewTime(){
    short i=1;
    setStrDate(strD);
    setStrTime(strT);
    menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
    while(1)
    {
        int c=0;
        if(isS1Pressed()){
            if(i==1){
                c=13;              
                changeTime(strT);
                menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
                menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
            }
            else if(i==2){          
                c=changeDate(strD);
                menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
                menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
            }
            else{
                if(i==3){
                    setTime(strT);
                    if(c<13)
                        setDate(strD,c);
                } 
                menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);//else
                return c;
            }
        }
        if(isS2Pressed()){
            menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
            if(i++==4)
                i=1;
            menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
        }
    } 
}
void setAlarm(){
    short i=1;
    setStrDate(strD);
    setStrTime(strT);
    menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
    while(1){
        if(isS1Pressed()){
            if(i==1){  
                changeTime(strT);
                menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
                menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
            }
            else if(i==2){
                changeDate(strD);
                menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
                menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
            }
            else{
                if(i==3)
                    setAlarmWatch(strT,strD);
                menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);//else
                return;
            }
        }
        if(isS2Pressed()){
            menuSetNewTime(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
            if(i++==4)
                i=1;
            menuSetNewTime(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
        }
    } 
}
void ChangeWatch(){
    short i=1;
    setStrDate(strD);
    setStrTime(strT);
    menuSetTypeWatch(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
    while(1){
        if(isS1Pressed()){
            if(i==1)
                setTypeWatch(Digital);
            else if(i==2)
                setTypeWatch(Analog);
             else if(i==3)
                setTypeWatch(Exclusivewatch);
            return;
        }
        if(isS2Pressed()){
            menuSetTypeWatch(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
            if(i++==4)
                i=1;
            menuSetTypeWatch(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
        }
    } 

}
//Display main menu of the program
void menu(){
    short i=0;//Location
    menuOption(0x001F,OLEDC_COLOR_SADDLEBROWN,i);
    while(1){
        if(isS1Pressed()){
            menuOption(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
            //Set the clock and date
            if(i==1){
                
                short c=setNewTime();
                //Go back to watch time to see new time
                if(c<3){
                    returnLastWatchType();
                    return; 
                }
               
                i=0;
                menuOption(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
            }
            else if(i==3){
                setAlarm();
                returnLastWatchType();
                return; 
            }
            else{
                if(i==0)
                    ChangeWatch();
                if(i==2){
                    changeInterval();
                    returnLastWatchType();
                }
                 if(i==4)
                    returnLastWatchType();
                return;//else
            }
        }
        if(isS2Pressed()){
            menuOption(OLEDC_COLOR_SNOW,OLEDC_COLOR_SNOW,i*15);
            if(i++==4)
                i=0;
            menuOption(0x001F,OLEDC_COLOR_SADDLEBROWN,i*15);
        }
    }
}
int main(void){
    SYSTEM_Initialize();
    //Initialize all component in system file
    initilize();
    oledC_setBackground(OLEDC_COLOR_SNOW);
    setTimeDateFromSystem();
    setTypeWatch(Digital);
    while(1){
        if(isS1Pressed()){
            isShowMenu=true;
            oledC_setBackground(OLEDC_COLOR_SNOW);
            setTypeWatch(Smallwatch);
            menu();
            showWatch();
            isShowMenu=false;
        }
    };
}