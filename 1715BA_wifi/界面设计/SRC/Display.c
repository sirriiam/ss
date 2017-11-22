/*======================================================================================
 文件说明：
 1) LCD显示。
 2) 包括段显示和数字显示。
 修改记录：
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\common.h"
#include "INC\display.h"
#include "INC\USER.H"

//=====================================================================================
// 名称：display
// 功能：LCD显示程序
// 输入：
// 输出：
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//=====================================================================================
void display(void)
{
	U8 tmp;
	
	// 关机状态下的显示
	if(gSrc[0]==0)
	{
		for(tmp=0;tmp<32;tmp++)
		{
			gDispData[tmp] = 0x00;//清零所有显示
		}
		// 锁定指示
		if(gSrc[10])
			gDispData[8] |= 0x01;
		if(gSrc[6]|gHour|gMinute)
		{// 有定时
			if(gHour|gMinute)
			{// 已确定
				gDispData[15] |= 0x01;// 分界线
				DispMode();					// 模式
			}
			DispTiming();
		}
		else
		{
			if(gSrc[13])
			{// 自清洁显示
				gDispData[2] |= 0x07;
				gDispData[4] = 0x0f;
			}
		}
		if(gSrc[5])
		{// 净化显示
			gDispData[15] |= 0x08;
			if(gSrc[3]==3)
			{
				gDispData[14] |= 0x03;
				gDispData[13] |= 0x03;
			}
			gDispData[12] |= 0x03;
		}
		disptolcd();
		return;
	}

	// 开机状态下先清零固定显示
	if((gSrc[3]!=0)&&((gWT_EN&0x01)==0))
	{
		gDispData[12] = 0x00;
		gDispData[13] = 0x00;
		gDispData[14] = 0x00;
	}
	else
	{
		gDispData[12] &= 0xf3;	//自动风速不清零,在TailControl()中处理
		switch(gAutoFanCnt)
		{
			case 2:
				gDispData[13] &= 0xf3;
				gDispData[14] &= 0xf3;
				break;
			case 1:
				gDispData[13] &= 0xf3;
				gDispData[14] &= 0x00;
				break;
			default:
				gDispData[13] &= 0x00;
				gDispData[14] &= 0x00;
				break;
		}
	}
	if(gSrc[4])
	{
		gDispData[9] = 0x00;
		gDispData[10] = 0x00;
		gDispData[11] = 0x00;
	}
	else
	{
		if((gVerticalCnt==0)||(gVerticalCnt==4))
		{
			gDispData[9] &= 0xf0;	//自动摆起始时清零
			gDispData[10] &= 0xf0;
			gDispData[11] &= 0xf0;
		}
		else
		{
			gDispData[9] &= 0xf6;	//自动摆其它时候不清零,在TailControl()中处理
			gDispData[10] &= 0xf7;
			gDispData[11] &= 0xf3;
		}
	}
	for(tmp=0;tmp<9;tmp++)
	{
		gDispData[tmp] = 0x00;
	}
	for(tmp=15;tmp<32;tmp++)
	{
		gDispData[tmp] = 0x00;
	}
	
	// 分界线
	gDispData[15] |= 0x01;
	
	// 锁定指示
	if(gSrc[10])
		gDispData[8] |= 0x01;
	
	// 模式
	DispMode();
		
	// 定时时间&温度&自清洁(无法同时显示)
	if(((gSrc[6]|gHour|gMinute)==0)||(gIReturnDec!=0))
	{
		if(gSrc[13])
		{// 自清洁显示
			gDispData[2] |= 0x07;
			gDispData[4] = 0x0f;
		}
		else
		{
			if(gSrc[1]==0)
			{// PMV模式温度显示有所不同
				tmp = 0;
				switch(gSrc[2])
				{
					case 7:
						tmp++;	// 23℃,显示-3℃
					case 8:
						tmp++;	// 24℃,显示-2℃
					case 9:
						tmp++;	// 25℃,显示-1℃
						gDispData[0] |= 0x04;
						gDispData[1] |= 0x08;
						gDispData[3] |= 0x0a;
						DispNum(tmp,2);break;
					case 13:
						tmp++;	// 29℃,显示3℃
					case 12:
						tmp++;	// 28℃,显示2℃
					case 11:
						tmp++;	// 27℃,显示1℃
						gDispData[0] |= 0x04;
						gDispData[1] |= 0x08;
						gDispData[3] |= 0x08;
						DispNum(tmp,2);break;
					default:break;	// 26℃,不显示
				}
			}
			else if(gSrc[1]<=3)
			{
				gDispData[0] |= 0x04;
				gDispData[1] |= 0x08;
				gDispData[3] |= 0x08;
				tmp = gSrc[2]+16;		// 16~30℃
				DispNum(tmp/10,1);
				DispNum(tmp%10,2);
			}
		}
	}
	else
		DispTiming();
	
	// 风速
	gDispData[12] |= 0x03;
	if((gWT_EN&0x01)==0)
	{
		switch(gSrc[3])
		{
			case 3:	// 高
				gDispData[14] |= 0x03;
			case 2:	// 中
				gDispData[13] |= 0x03;
			default:// 低/自动
				break;
		}
	}
	// 上下风向
	if(gSrc[4]!=1)	// 上下固定不显示
	{
		gDispData[10] |= 0x01;
		gDispData[11] |= 0x02;
		switch(gSrc[4])
		{
			case 3:	// 健康气流下吹
				gDispData[10] |= 0x02;
			case 5:	// 位置五
				gDispData[11] |= 0x01;break;
			case 2:	// 健康气流上吹
				gDispData[9] |= 0x04;
			case 4:	// 位置一
				gDispData[9] |= 0x02;break;
			default:// 自动摆
				if(gVerticalCnt==0)
					gDispData[9] |= 0x02;	// 位置一开始
				else if(gVerticalCnt==4)
					gDispData[11] |= 0x01;	// 位置五开始
				break;
		}
	}
	
	// 左右风向
	gDispData[12] |= 0x04;
	gDispData[14] |= 0x08;
	switch(gSrc[12])
	{
		case 0:	// 位置一
			gDispData[12] |= 0x08;
			gDispData[13] |= 0x08;break;
		case 1:	// 位置四
			gDispData[11] |= 0x04;break;
		case 2:	// 位置五
			gDispData[11] |= 0x08;break;
		case 3:	// 位置六
			gDispData[13] |= 0x04;break;
		case 4:	// 位置七
			gDispData[14] |= 0x04;break;
		case 5:	// 自动摆
			gDispData[11] |= 0x0c;
			gDispData[12] |= 0x08;
			gDispData[13] |= 0x0c;
			gDispData[14] |= 0x04;break;
		default:
			break;
	}
		
	// 健康
	if(gSrc[5])
		gDispData[15] |= 0x08;
		
	// 睡眠
	if(gSrc[7])
		gDispData[16] |= 0x04;
	
	// 强力
	switch(gSrc[8])
	{
		case 1:	// 强力
       	   	gDispData[15] |= 0x02;break;
		case 2:	// 安静
       	    gDispData[16] |= 0x01;break;
		default:// 无
			break;
	}
	
	// 辅热
	if(gSrc[9])
		gDispData[16] |= 0x08;
	
	// data to lcd
	disptolcd();
}

//模式显示
void DispMode(void)
{
	switch(gSrc[1])
	{
		case 0:	// PMV
			gDispData[7] |= 0x08;break;
		case 1:	// 制冷
			gDispData[7] |= 0x01;break;
		case 2:	// 除湿
			gDispData[6] |= 0x01;break;
		case 3:	// 制热
			gDispData[6] |= 0x08;break;
		case 4:	// 送风
			gDispData[6] |= 0x04;break;
		default:
			break;
    }
}

//定时开/关显示
void DispTiming(void)
{
	U8 tmp;
	
	if(gITimeSetDec)
		tmp = gSrc[6];
	else
	{
		if(gMinute)
		{
			if(gHour<12)
			{
				if(gMinute>30)
					tmp = gHour*2+2;
				else
					tmp = gHour*2+1;
			}
			else
				tmp = gHour*2+1;
		}
		else
			tmp = gHour*2;
	}
	if(tmp<24)
	{
		if(tmp%2)
			gDispData[0] |= 0x01;	// 半小时显示.5
		if(tmp>19)
		{
			DispNum(1,1);
			DispNum((tmp%20)/2,2);
		}
		else
			DispNum(tmp/2,2);
	}
	else
	{
		if((tmp%20)%2)
			tmp++;								// 定时时间大于12小时,如果未计满小时显示不能减1
		DispNum(tmp/20,1);
		DispNum((tmp%20)/2,2);
	}
	if((gITimeSetDec==0)||(gbPauseTK==1))
	{
		if(gSrc[0])
			gDispData[5] |= 0x08;	// 定时关机
		else
			gDispData[5] |= 0x01;	// 定时开机
	}
}

//===========================================================================
// 名称：DispNum
// 功能：数字显示
// 参数：num: 显示的数字0~9
//		 n: 显示数字的位置或类型
// 返回：
// 调用：
// 嵌套级数：
// 影响：
// 其它：n=1为十位,=2为个位
//===========================================================================
void DispNum(U8 num,U8 n)
{
	switch(n)
	{
		case 1://十位
			gDispData[3] |= g_Number[num]>>4;
			gDispData[4] |= g_Number[num]&0x0F;
			break;
		case 2://个位
			gDispData[1] |= g_Number[num]>>4;
			gDispData[2] |= g_Number[num]&0x0F;
			break;
		default:
			break;
	}
}

//===========================================================================
// 名称: disptolcd
// 功能：显示到LCD
// 参数：
// 返回：
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//===========================================================================
void disptolcd(void)
{
	U8 tmp;
	
	for(tmp=0;tmp<32;tmp++)
	{
		*((vxu8*)(0xff60+tmp)) = gDispData[tmp];
	}
}
