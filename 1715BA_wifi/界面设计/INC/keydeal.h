
#ifndef _KEYDEAL_H_
#define _KEYDEAL_H_

#define K_NONE 0
#define K_INVALID 0
#define NULL 0

bit gbFirstKey;						// 1=首键为温度键
bit	gbHoldTempKey;				// 温度加/减或定时键长按未抬起超过1秒
U8 xdata gKeyID;				// 按键识别码，用于发码数据
U8 xdata gKeyCode;				// 键扫描值	

bit gbkey_exist;                // 有按键扫描到了,=1
bit gbKeyChanged;               // 按键过程中，键码有无发生改变	
U8 xdata gkey_sc_ct;			// 去抖动计数
U8 xdata gkey_lt_value;			// 上一次扫描值
U8 xdata gkey_sc_value;			// 上一次保存的键值
U8 xdata gkey_ct;				// 键长计数
U8 xdata gkey_press;			// 按下的键值
U8 xdata gkey_mode;				// 键模式-->长按，短按...
U8 xdata gkey_value;			// 键值
U8 xdata gkey_last;				// 上一次键值
U8 xdata gKey_Prev;             // 按键值变化前的值
U8 xdata gKeyModeSc;

extern bit gbPauseTK;						// 定时标志闪烁暂停标志
extern bit gbKeyTempSpec;				// 包括温度键的双/多键变为温度键,必须等待按键释放后才能正常发码
extern bit gbIKeyFlag;          // 有按键按下标志,=1有按键
extern U8 gTempBuf[5];			// 各模式温度记忆区
extern U8 gFanBuf[5];			// 各模式风速记忆区
extern U8 gWindUdBuf[5];				// 各模式上下风向记忆区
extern U8 gWindLrBuf[5];				// 各模式左右风向记忆区
extern U8 gSleepBak;
extern U8 gTimeOnBak;
extern U8 gTimeOffBak;
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
extern U8 idata gSendData[32];	// 发码数据
extern U8 gISendDec;	        // 按键按下后至少多长时间后才发码

extern void GetCode(void);
extern void RCSend(void);
extern void display(void);
extern void TailControl(void);

void GetKey(void);
U8 keyscan(void);
void key_compare(void);
void key_convert(void);
void KeyDeal(void);
void UserKeyDeal(void);   		// 用户按键处理
//void ACDeal(void);

#endif