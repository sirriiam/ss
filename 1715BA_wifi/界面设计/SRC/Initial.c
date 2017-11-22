/*======================================================================================
 文件说明：
 1) 上电初始化，包括对系统专用寄存器和用户RAM的初始化处理。
 修改记录：
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H> 
#include "INC\DataType.h"
#include "INC\Common.h"
#include "INC\Initial.h"
#include "INC\USER.H"
/*******************************************************************
函数名称：PortInit()
函数功能：端口初始化
入口参数：无
出口参数：无
备    注：PxMDL/H 00-输入无上拉,01-输入上拉,10-推挽输出,11-开漏输出
		  LCDPENx 0-数字输入口,1-LCD
*******************************************************************/
void PortInit(void)
{
	P0DPL = 0xaa;
	P0DPH = 0xaa;
	LCDPC1 = 0xff;   			// SEG0~SEG7
	
	P1DPL = 0x6a;
	P1DPH = 0x55;					// P13~P17用作键盘输入口
	LCDPC3 = 0x01;				// SEG16
	P1 &= 0x07;
	
	P2DPL = 0xff;					// P20~P23用作键盘输出口,设为开漏输出
	P2DPH = 0xaa;
	LCDPC4 = 0x00;
	P2 = 0x00;
    
	P3DPL = 0xaa;
	P3DPH = 0xaa;
	LCDPC0 = 0xf0;				// COM0~COM3
	P3 &= 0xf0;
	
	P4DPL = 0xaa;
	P4DPH = 0xaa;
	LCDPC2 = 0xff;				// SEG8~SEG15
	
	P5DPL = 0x0a;
	P5DPH = 0xaa;
	P5 = 0x00;
}

/*******************************************************************
函数名称：SysInit()
函数功能：系统初始化
入口参数：无
出口参数：无
备    注：
*******************************************************************/
void SysInit(void)
{
	U8 tmp;
	U16 tmp16;
		
	WDTCON = 0xfb;				// 溢出时间约1.433s,第一次调用ClrWDT()时WDT被开启
    
	#if DEBUG
		SPKEY = 0x36;
		CKEN = 0x2b;
	#else
		SPKEY = 0x36;
		CKEN = 0x33;				// 关HCRY,P50,P51用作GPIO;开IHRC,ILRC,LCRY(经P52,P53外接)
	#endif
	PortInit();
	KINEN = 0xf8;					// KI3~KI7
	IOMUXC0 = 0x40;				// P3.0用作载波输出口,发红外码
       
	while(1)
	{
		if(CKSS & 0x20)			// 32K晶振稳定标志
			break;
	}
	delay(1600);
	//SPKEY = 0x36;
	SYSCFG = 0x83;				// P6.0复用作外部复位,ILRC由ILRCEN使能
	SPKEY = 0x36;
	PLLCON = 0x01;				// 开PLL,128倍频
	while(1)
	{
		if(PLLCON & 0x10)		// 等待PLL稳定
			break;
	}
	delay(255);
	SPKEY = 0x36;
	CKSS = 0x02;					// 选择PLL作MCLK
	_nop_();
	_nop_();
	_nop_();
	_nop_();
    
	//SPKEY = 0x36;
	//CKEN = 0x33;					// 关HCRY,P50,P51用作GPIO;开IHRC,ILRC,LCRY(经P52,P53外接)
    
	SPKEY = 0x36;
	CKDS = 0x01;					// 二分频
    
	// 定时器设置
	TIMPRS = 0x77;				// T1:256分频;T0:256分频
	TMOD = 0x12;					// T1:16位定时器,TR1触发;T0:8位自装载,TR0触发
	TR0 = 0;
	TL0 = 215;						// 4194000/2/256/200=41,256-41=215
	TH0 = 215;
	TR1 = 0;
	TL1 = 0x00;						// 3*4194000/2/256=24576,10000H-24576=0xA000
	TH1 = 0xa0;
	T2CON1 = 0x00;				// 16位重载方式
	T2CON2 = 0x00;				// PCLK/2
	Init_Close_RTC();			// 初始化并关闭RTC
    
	CWTPRS = 0x00;				// CWT时钟不分频
	CWTCON = 0x00;				// 关载波定时器
	LVDCON = 0x00;				// 关低压检测
	   
	// LCD驱动设置
	//LCDCON = 0xb7;			// 开启,128Hz,1/3bias,4COM
	LCDCON = 0xa7;				// 开启,256Hz,1/3bias,4COM

	//LCDPCON = 0x01;				// 对比度最高,驱动能力中等
	//LCDPCON = 0x00;				// 对比度最高,驱动能力最小
	//LCDPCON = 0x0c;	
	LCDPCON = 0x03;			// 对比度最高,驱动能力最小
	//LCDPCON = 0x1f;			// 对比度最低,驱动能力最大
	//LCDPCON = 0x1c;			// 对比度最低,驱动能力最小
	
	// LCD上电显示
	for(tmp=0;tmp<32;tmp++)
	{// 先全屏点亮3秒钟
		*((vxu8*)(0xff60+tmp)) = 0xff;
	}
	for(tmp16=0;tmp16<1550;tmp16++)
	{
		for(tmp=0;tmp<180;tmp++);
			ClrWDT();
	}
	for(tmp=0;tmp<32;tmp++)
	{// 再全屏熄灭
		*((vxu8*)(0xff60+tmp)) = 0x00;
	}
	
	// 变量初始化非零值(进入到main前,DATA:00~FF,XDATA:00~FF都已初始为0)
	gSrc[0] = 0;					// 关机状态
	gSrc[1] = 0;					// 上电初始为PMV模式
	gSrc[2] = 10;					// 26℃
	gSrc[3] = 0;					// 自动风速
	gSrc[4] = 1;					// 上下固定
	gSrc[5] = 0;					// 无净化/健康
	gSrc[6] = 0;					// 无定时
	gSrc[7] = 0;					// 无睡眠
	gSrc[8] = 0;					// 无强力
	gSrc[9] = 0;					// 无辅热(只在制热模式下有效)
	gSrc[10] = 0;					// 未锁定
	gSrc[11] = 0;					// 定频运转关闭
	gSrc[13] = 0;					// 无自清洁
	gSrc[14] = 0;					// 情景灯光关闭
	gSrc[15] = 0;					// 无定位出风
	gTempBuf[0] = 10;			// PMV模式初始温度26℃,不显示
	gTempBuf[1] = 10;			// 制冷模式初始温度26℃
	gTempBuf[2] = 10;			// 除湿模式初始温度26℃
	gTempBuf[3] = 10;			// 制热模式初始温度26℃
	gTempBuf[4] = 10;			// 送风模式初始温度26℃,不显示,不可调
	gFanBuf[0] = 0;				// PMV模式初始风速自动
	gFanBuf[1] = 0;				// 制冷模式初始风速自动
	gFanBuf[2] = 0;				// 除湿模式初始风速自动
	gFanBuf[3] = 0;				// 制热模式初始风速自动
	gFanBuf[4] = 1;				// 送风模式初始风速低
	gWindUdBuf[0] = 4;			// PMV模式风向不记忆
	gWindUdBuf[1] = 4;			// 制冷模式初始风向位置一
	gWindUdBuf[2] = 4;			// 除湿模式初始风向位置一
	gWindUdBuf[3] = 5;			// 制热模式初始风向位置五
	gWindUdBuf[4] = 4;			// 送风模式风向不记忆
	gSleepBak = 0;
	gTimeOnBak = 1;				// 定时开/关机起始位置记忆
	gTimeOffBak = 1;
	
	gWT_EN = 0x00;
	gAutoFanCnt = 0;
	gVerticalCnt = 0;
	gSecond = 0;
	gMinute = 0;
	gHour = 0;
	gSpSecond = 0;
	gSpMinute = 0;
	gSpHour = 0;
	gClSecond = 0;
	gClMinute = 0;
	gITimeSetDec = 0;
	gIReturnDec = 0;
	gSendType = 0;
	gbTimeOut = 0;
	gbPauseTK = 0;
	gbKeyTempSpec = 0;
	
	// 中断使能
	EIP2 |= 0x40;					// KBI高优先级
	EIP2 &= 0xdf;					// RTC中断优先级低
	EIE2 |= 0x40;					// 允许KBI中断
	PT0 = 0;							// T0优先级低
	ET0 = 1;							// 允许T0中断
	ET1 = 1;
	EA = 1;								// CPU开放中断
	TR0 = 1;
	TR1 = 0;
}

void delay(U16 tcnt)
{
	do{}while(tcnt--);
}
