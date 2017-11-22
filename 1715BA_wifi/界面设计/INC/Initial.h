
#ifndef _INITIAL_H_
#define _INITIAL_H_

U8 idata gSrc[50] _at_ 0x80;		// 空调数据源区
//gSrc[0]：开关机,0=关机状态,1=开机状态
//gSrc[1]：模式,0=PMV(自动),1=制冷,2=除湿,3=制热,4=送风
//gSrc[2]：温度,0=16℃......14=30℃
//gSrc[3]：风速,0=自动,1=低,2=中,3=高
//gSrc[4]：上下风向,0=自动摆,1=上下固定,2=健康气流上吹,3=健康气流下吹,4=上下摆位置一,5=上下摆位置五
//gSrc[5]：净化(健康)，0=无，1=有
//gSrc[6]：定时设定,0=无定时,1=0.5小时,2=1小时......23=11.5小时,24=12小时,26=13小时......48=24小时
//gSrc[7]：睡眠,0=关,1=开
//gSrc[8]：强力,0=无,1=强力,2=安静
//gSrc[9]：辅热,0=关,1=开
//gSrc[10]：锁定,0=关,1=开
//gSrc[11]：定频运转,0=关,1=开
//gSrc[12]: 左右风向,0=位置1,1=位置4,2=位置5,3=位置6,4=位置7,5=自动摆
//gSrc[13]：自清洁,0=关,1=开
//gSrc[14]：情景灯光,0=关,1=开
//gSrc[15]：定位出风,0=无,1=位置1,2=位置2,3=位置3

U8 idata gSendData[32] _at_ 0xb2;	// 发码数据

U8 gTempBuf[5];						// 各模式温度记忆区
U8 gFanBuf[5];						// 各模式风速记忆区
U8 gWindUdBuf[5];					// 各模式上下风向记忆区
U8 gWindLrBuf[5];					// 各模式左右风向记忆区
U8 gSleepBak;
U8 gTimeOnBak;
U8 gTimeOffBak;
U8 gSendType;							// 0=不发码,1=发码
U8 gWT_EN;
U8 gAutoFanCnt;
U8 gVerticalCnt;
U8 xdata gSecond;
U8 xdata gMinute;
U8 xdata gHour;
U8 xdata gSpSecond;
U8 xdata gSpMinute;
U8 xdata gSpHour;
U8 xdata gClSecond;
U8 xdata gClMinute;
bit gbTimeOut;						// 定时设置超时退出标志
bit gbPauseTK;						// 定时标志闪烁暂停标志
bit gbKeyTempSpec;				// 包括温度键的双/多键变为温度键,必须等待按键释放后才能正常发码

extern void Init_Close_RTC(void);
extern void DispNum(U8 num,U8 n);
extern void disptolcd(void);
void delay(U16 tcnt);
//void initial(void);

#endif