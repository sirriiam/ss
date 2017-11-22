#ifndef _COMMON_H_
#define _COMMON_H_

#define ERR 	1
#define NULL	0

// bit为常数时没有直正的移位,而是编译成相应功能的指令
#define bitset(var,bit) ((var)|=(1<<(bit)))    // 置某一位为高
#define bitclr(var,bit) ((var)&=~(1<<(bit)))   // 置某一位为低
#define bitsz(var,bit) ((var)&(1<<(bit)))      // 测试某一位是否为低

extern U8 idata gSrc[50];    		// 空调数据源区
extern U8 idata gSendData[32];      // 发码数据

// 常量引用
extern bit gbIKeyFlag;
extern U8 xdata gKeyCode;			// 键扫描值
extern U8 xdata gkey_sc_ct;			// 去抖动计数
extern U8 xdata gkey_lt_value;		// 上一次扫描值
extern U8 xdata gkey_sc_value;		// 上一次保存的键值
extern U8 xdata gkey_ct;			// 键长计数
extern U8 xdata gkey_press;			// 按下的键值
extern U8 xdata gkey_mode;			// 键模式-->长按，短按...
extern U8 xdata gkey_value;			// 键值
extern U8 xdata gkey_last;			// 上一次键值

extern U8 gSendType;				// 0=不发码，1=按普通键发码，2=按定时设定/取消键发码
extern U8 gIWorkDec;
extern U8 gITimeSetDec;
extern U8 gIReturnDec;    	// 定时显示返回时间长度
//extern U8 gInt_50ms;

extern void ClrWDT(void);
extern void delay(U16 tcnt);

#endif