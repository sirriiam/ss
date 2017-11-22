/*======================================================================================
 文件说明：
 1) T0/T1中断，RTC(实时时钟定时器)中断，按键中断(P1相关端口下降沿触发)
 修改记录：
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\USER.H"

bit gbIKeyFlag;				// 按键中断置起的标志
bit gbCountDown;			// WT(时钟定时器)中断置起的标志，用于倒计时
bit gbLoopDisp;				// WT(时钟定时器)中断置起的标志，用于自动风速、内/外风门的循环显示
bit gbI5ms;						// 基本时基5ms

U8 gInt_50ms;					// 基本时基50ms
U8 gIWorkDec;		    	// 待机前的工作时长
U8 gISendDec;	        // 按键按下后至少多长时间后才发码
U8 gITimeSetDec;    	// 定时设定时间长度
U8 gIReturnDec;    		// 定时显示返回时间长度

extern bit gbTimeOut;
extern U8 idata gSrc[];
extern U8 xdata gDispData[32];
extern U8 xdata gKeyID;// 按键识别码,用于发码数据
extern U8 gWT_EN;

extern void TailControl(void);

//===========================================================================
// 名称：isr_t0
// 功能：中断服务程序 T0溢出中断
// 返回：
// 堆栈：
// 影响：
// 其它：
//      5ms中断一次
//      MCU正常工作模式(非低功耗模式)的延时变量递减
//===========================================================================
void isr_t0(void) interrupt 1
{
	gbI5ms = 1;
	if(gInt_50ms<10)
		gInt_50ms++;
	else
		gInt_50ms = 0;
        
	if(gInt_50ms==0)
	{
		// 待机前的工作时长
		if(gIWorkDec>0)
			gIWorkDec--;
		
		// 按键操作后到开始发码的延时
		if(gISendDec>0)
			gISendDec--;
		
		// 定时设置状态计时
		if(gITimeSetDec>0)
		{
			gITimeSetDec--;
			if(gITimeSetDec==0)
			{
				gSrc[6] = 0;
				TKCON &= 0x7f;			// 关闭闪烁
				gbTimeOut = 1;
			}
		}
		
		// 定时显示返回计时
		if(gIReturnDec>0)
		{
			gIReturnDec--;
			if(gIReturnDec==0)
			{
				if(gWT_EN&0x20)
					gKeyID = 0x0D;		// 定时已确定才需要还原显示
				gbTimeOut = 1;
			}
		}
	}
}

//===========================================================================
// 名称：isr_t1
// 功能：中断服务程序 T1溢出中断,500ms闪烁中断
// 返回：
// 堆栈：
// 影响：
// 其它：
//===========================================================================
void isr_t1(void) interrupt 3
{
	TR1 = 0;
	TF1 = 0;
	TL1 = 0x00;
	TH1 = 0xf0;
}

//===========================================================================
// 名称：isr_wt
// 功能：时钟定时器中断唤醒程序，一般是主程序设定每秒/分钟唤醒一次
// 返回：
// 堆栈：
// 影响：
// 其它：
//===========================================================================
void isr_wt(void) interrupt 19
{
//	BORCON = 0x80;            // 开电压基准，稍后再开启低压复位 versions 1.1  修改
    
	gbCountDown = 1;
	gbLoopDisp = 1;
	if(RTCCON & 0x40)
	{
		RTCCON &= 0xbf;
	}
	//TailControl();
}

//===========================================================================
// 名称：isr_kbi
// 功能：按键中断唤醒
// 返回：
// 堆栈：
// 影响：
// 其它：
//===========================================================================
void isr_kbi(void) interrupt 20
{
    volatile U8 temp;
	
//    BORCON = 0x80;					// 开电压基准，稍后(主程序)再开启低压复位 versions 1.1  修改
    gbIKeyFlag = 1;
    
    EIE2 &= 0xbf;						// 先关按键中断
    temp = KINFLG;					// 先读后清零
    KINFLG = 0x00;
    temp = KINFLG;					// 先读后清零
    KINFLG = 0x00;
    EIE2 |= 0x40;						// 重新开按键中断
}
