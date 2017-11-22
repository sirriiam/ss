/*======================================================================================
 文件说明：
 1) 包括实时时钟定时，对延时变量的判断和对应的处理，进入待机前和从待机唤醒的处理。
 修改记录：
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\common.h"
#include "INC\MySub.h"
#include "INC\USER.H"

//===========================================================================
// 名称：TailControl
// 功能：LCD显示定时更新
// 参数：
// 返回：
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//===========================================================================
void TailControl(void)
{	
	if(gbLoopDisp==0)
		return;
	if(gSrc[0]==0)
		return;
	gbLoopDisp = 0;
	if((gSrc[3]==0)||(gSrc[8]!=0))
  {// 自动风速
		if((gWT_EN&0x01)==0x00)
		{
			gWT_EN |= 0x01;
		}
		// 刷新前先清除
		gAutoFanCnt++;
		if(gAutoFanCnt==3)
			gAutoFanCnt=0;
		gDispData[13]&=0xfc;
		gDispData[14]&=0xfc;
		switch(gAutoFanCnt)
		{
			case 2:
				gDispData[14]|=0x03;
			case 1:
				gDispData[13]|=0x03;
			default:
				break;
		}
	}
	
	if(gSrc[4]==0)
	{// 上下自动摆风
		if((gWT_EN&0x04)==0x00)
		{
			gWT_EN |= 0x04;
		}
		// 刷新前先清除
		gVerticalCnt++;
		if(gVerticalCnt==8)
			gVerticalCnt=0;
		gDispData[9] &= 0xf9;
		gDispData[10] &= 0xf9;
		gDispData[11] &= 0xfe;
		switch(gVerticalCnt)
		{
			case 0:
				gDispData[9] |= 0x02;break;//上到下
			case 1:
			case 7:
				gDispData[9] |= 0x04;break;
			case 2:
			case 6:
				gDispData[10] |= 0x04;break;
			case 3:
			case 5:
				gDispData[10] |= 0x02;break;
			case 4:
				gDispData[11] |= 0x01;break;//下到上
			default:
				break;
		}
	}
	LCDM9 = gDispData[9];
	LCDM10 = gDispData[10];
	LCDM11 = gDispData[11];
	LCDM13 = gDispData[13];
	LCDM14 = gDispData[14];
}

//===========================================================================
// 名称：IdleModeDeal
// 功能：IDLE(待机低功耗)模式的进入和退出
// 输入：
// 输出：
// 调用：
// 堆栈：
// 影响：
// 其它：
//===========================================================================
bit IdleModeDeal(void)
{
	volatile U8 temp;
	
	if((gIWorkDec | gkey_value | gITimeSetDec | gIReturnDec)==0x00)
	{// 进入待机
		PortInit();
		if(gbTimeOut)
		{// 定时设置超时退出后需要刷新显示
			gbTimeOut = 0;
			display();
		}
		/*P30 = 0;					// 发码管脚输出低
		P2DPL = 0xaa;				// P2低四位设为推挽输出
		P2 = 0x00;					// P2高四位低电平(未用到)*/
		
		CWTCON = 0x00;			// 关载波模块
		
		ET0 = 0;						// 不允许T0中断
		ET1 = 0;
		TR0 = 0;
		TR1 = 0;
		EA = 0;
		SPKEY = 0x36;
		CKEN &= 0xfd;				// 关ILRC
		EIE2 |= 0x40;				// 允许KBI中断
		
		temp = KINFLG;			// 先读后清零
		KINFLG = 0x00;
		temp = KINFLG;			// 先读后清零
		KINFLG = 0x00;
		gbIKeyFlag = 0;
		
		//SPKEY = 0x36;
		//CKSS = 0x00;				// 选择IHRC作为MCLK
		_nop_();
		_nop_();
		
		//SPKEY = 0x36;
		//PLLCON = 0x00;			// 关PLL,128倍频
//		BORCON = 0x00; 			// 关闭电压基准,关闭低压复位    versions 1.1  修改
	
		EA = 1;
		#if DEBUG
			while((gbIKeyFlag|gbCountDown)==0)
				ClrWDT();
		#else
			SPKEY = 0x36;
			PCON = 0x02;
		#endif
	
		delay(2);
//		BORCON |= 0x80;			// versions 1.1  修改
		if(gbIKeyFlag)
		{
			gIWorkDec = TIMETOIDLE;//
			EA = 0;
			SPKEY = 0x36;
			PLLCON = 0x01;		// 
			EA = 1;
			while(1)
			{
				if(PLLCON & 0x10)// 
					break;
			}
			EA = 0;
			SPKEY = 0x36;
			CKSS = 0x02;			// 
			EA = 1;
		}
		delay(2);
//		BORCON |= 0x01;			//versions 1.1  修改 
	
		EA = 0;
		SPKEY = 0x36;
		CKDS = 0x01;				// 
	
		SPKEY = 0x36;
		CKEN |= 0x02;				// 
		EA = 1;
		ET0 = 1;
		TR0 = 1;
		ET1 = 1;
		//CWTCON = 0x80;		// 开载波发生器
		
		//P1DPL = 0x55;
		//P2DPL = 0xff;			// P2低四位重新设为开漏输出
		if(gbCountDown)			// 要注意再次调用本函数前，gbWT已被清零，不然总是返回1
			return 1;
	} 
	return 0;
}

//===========================================================================
// 名称：timeupdate
// 功能：定时时间走时更新
// 参数：
// 返回：
// 调用：
// 嵌套级数：
// 影响：gWT_EN
// 其它：gWT_EN=0x00时，WT时钟才会关闭
//       gWT_EN.0=1 自动风速
//       gWT_EN.1=1 
//       gWT_EN.2=1 上下风向自动摆
//       gWT_EN.5=1 定时开/关时间递减
//       gWT_EN.6=1 睡眠(8小时定时关机)
//       gWT_EN.7=1 自清洁(21分钟后取消)
//===========================================================================
void timeupdate(void)
{
	if(gWT_EN)
	{
		if((RTCCON & 0x08)==0x00)
		{
			EIE2 |= 0x20;   // 开RTC中断
			RTCCON |= 0x08;	// T8EN
		}
		
		if(gbCountDown)
		{// 有RTC中断产生
			gbCountDown = 0;
			
			//定时开机/关机
			if(gWT_EN&0x20)
			{
				gSecond++;	// 一次一秒钟
				if(gSecond==60)
				{// 一分钟
					gSecond = 0;
					if(gMinute==0)
					{
						gMinute = 59;
						gHour--;
					}
					else
						gMinute--;
					if(gMinute==30)
					{
						//gSrc[6] -= 1;
						if(gHour<12)
							display();	//刷新定时时间显示
					}
					else if(gMinute==0)
					{
						//gSrc[6] -= 1;
						if(gHour==0)
						{
							gbCountDown = 0;
							gSrc[0] ^= 0x01;
							OnOffDeal();//调用开关机时的功能处理
						}
						display();	//刷新定时时间显示
					}
				}
			}
			
			//睡眠(8小时定时关机)
			if(gWT_EN&0x40)
			{
				gSpSecond++;	// 一次一秒钟
				if(gSpSecond==60)
				{// 一分钟
					gSpSecond = 0;
					if(gSpMinute==0)
					{
						gSpMinute = 59;
						gSpHour--;
					}
					else
						gSpMinute--;
					if(gSpMinute==0)
					{
						if(gSpHour==0)
						{
							gbCountDown = 0;
							gSrc[0] = 0;
							OnOffDeal();//调用开关机时的功能处理
							display();	//刷新显示
						}
					}
				}
			}
			//自清洁(21分钟后取消)
			if(gWT_EN&0x80)
			{
				gClSecond++;	// 一次一秒钟
				if(gClSecond==60)
				{// 一分钟
					gClSecond = 0;
					gClMinute--;
					if(gClMinute==0)
					{
						gbCountDown = 0;
						gWT_EN &= 0x7f;
						//Init_Close_WT();
						gSrc[13] = 0;
						display();	//刷新显示
					}
				}
			}
		}
	}
	else
	{
		Init_Close_RTC();
		gbCountDown = 0;
		gbLoopDisp = 0;
		gSecond = 0;
		gMinute = 0;
		gHour = 0;
		gSpSecond = 0;
		gSpMinute = 0;
		gSpHour = 0;
		gClSecond = 0;
		gClMinute = 0;
	}
}

//===========================================================================
// 名称：Init_Close_RTC
// 功能：初始化并关闭RTC
// 输入：
// 输出：
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//===========================================================================
void Init_Close_RTC(void)
{
    U8 tmp;
    
    // RTC重新初始化
    tmp = RTCCON;
    RTCCON = 0x81;
    tmp = RTCCNT;
    tmp--;
    RTCCNT = tmp;
    tmp = RTCCON;
    RTCCON = 0x81;
    tmp = RTCCON;
    RTCCON = 0x89;
	
    while(1)
    {
        tmp = RTCCON;
        if(tmp & 0x40)
            break;
    }
    RTCCNT = 0xc0;			// 256 - 64  1秒
    //RTCCNT = 0xe0;		// 256 - 32  0.5秒
    //gT8s=0xc0;
    tmp = RTCCON;
    RTCCON = 0x83;			// 64Hz,RTC关
    EIE2 &= 0xdf;				// 关RTC中断
}

/*******************************************************************
函数名称：ClrWDT()
函数功能：清看门狗
入口参数：无
出口参数：无
备    注：
*******************************************************************/
void ClrWDT(void)
{
	WDTEN0 = 0x00;
	WDTEN1 = 0x00;
	WDTEN0 = 0x39;
	WDTEN1 = 0xC6;
}
