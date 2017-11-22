
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

U8 xdata gDispData[32];
extern U8 gWT_EN;
extern U8 gAutoFanCnt;
extern U8 gVerticalCnt;
extern U8 xdata gKeyID;		// 按键识别码
extern U8 xdata gMinute;
extern U8 xdata gHour;
extern bit gbPauseTK;						// 定时标志闪烁暂停标志

const U8 code g_Number[] = // 0～9显示数据(温度十位/个位,定时时间小时十/个位)
{
0x5F,0x50,0x6B,0x79,0x74,0x3D,0x3F,0x58,0x7F,0x7D
};

void display(void);
void DispMode(void);
void DispTemp(void);
void DispFan(void);
void DispTiming(void);
void DispNum(U8 num,U8 n);
void disptolcd(void);

#endif