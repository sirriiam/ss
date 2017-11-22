
#ifndef _MYSUB_H_
#define _MYSUB_H_

extern bit gbCountDown;
extern bit gbLoopDisp;
extern bit gbTimeOut;
extern U8 gWT_EN;
extern U8 gAutoFanCnt;
extern U8 gVerticalCnt;
extern U8 xdata gSecond;
extern U8 xdata gMinute;
extern U8 xdata gHour;
extern U8 xdata gSpSecond;
extern U8 xdata gSpMinute;
extern U8 xdata gSpHour;
extern U8 xdata gClSecond;
extern U8 xdata gClMinute;
extern U8 xdata gDispData[32];

extern U8 gWindUdBuf[5];

extern void PortInit(void);
extern void OnOffDeal(void);
extern void display(void);
extern void DispTime(void);
//extern void DispNum(U8 num,U8 n);

//void timedec(U8 tabi);
void TailControl(void);
void timeupdate(void);
bit IdleModeDeal(void);
void Init_Close_RTC(void);

#endif