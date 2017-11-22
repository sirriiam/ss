
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

U8 xdata gDispData[32];
extern U8 gWT_EN;
extern U8 gAutoFanCnt;
extern U8 gVerticalCnt;
extern U8 xdata gKeyID;		// ����ʶ����
extern U8 xdata gMinute;
extern U8 xdata gHour;
extern bit gbPauseTK;						// ��ʱ��־��˸��ͣ��־

const U8 code g_Number[] = // 0��9��ʾ����(�¶�ʮλ/��λ,��ʱʱ��Сʱʮ/��λ)
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