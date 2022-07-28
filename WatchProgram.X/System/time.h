#ifndef TIME_H
#define	TIME_H
#include <stdbool.h>
#include <stdint.h>
typedef enum {
  Digital,
  Analog,
  Smallwatch,
  Exclusivewatch,
  Empty
} watchType ;

void setTimeDateFromSystem();
void setTime(char* time);
void setStrTime(char* t);
void setStrDate(char* d);
void setDate(char* d,short m);
void setAlarmWatch(char* t,char* d);
bool checkAlarm();
const char* getMonthStr(short i);
short getMaxDay(int m);
void displayWatch(bool isShowMenu);
void hideWatch(short clockType);
void showWatch();
void changeInterval();
bool getInterval();
void increase_hour();
void increase_min();
void increase_second();
#endif	/* XC_HEADER_TEMPLATE_H */

