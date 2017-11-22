/*======================================================================================
 文件说明：
 1) 包括发码数据处理，发码主程序，发码波形采用指令延时。
 修改记录：
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\common.h"

extern U8 xdata gMinute;
extern U8 xdata gHour;
extern U8 xdata gSpMinute;
extern U8 xdata gSpHour;
extern U8 xdata gKeyID;		// 按键识别码,用于发码数据

//===========================================================================
// 名称: Delay554us()
// 功能: 延时554微秒
// 输出：无
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//===========================================================================
void Delay554us(void)
{
	U8 i;
	
	for(i=0;i<114;i++)
		_nop_();
}

//===========================================================================
// 名称: ByteSendH()
// 功能: 发送单字节数据码 
// 输入：userdata发送的字节数据，n发送的位数
// 输出：无
// 调用：
// 嵌套级数：
// 影响：
// 其它：高位先发
//===========================================================================
void ByteSendH(U8 userdata,U8 n)
{
	U8 i,j;
	
	for(i=0;i<n;i++)
	{
		if(userdata & 0x80)
		{
			P30 = 1;			     // 逻辑1高电平
			Delay554us();	
			P30 = 0;			     // 逻辑1低电平
			Delay554us();
			Delay554us();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();		
		}
		else
		{
			P30 = 1;			     // 逻辑0高电平
			Delay554us();
			P30 = 0;			     // 逻辑0低电平
		}
		if (i==(n-1))				 // 最后一位在软件上走的指令多一些
		{
			for(j=0;j<105;j++)
				_nop_();
		}
		else
		{
			for(j=0;j<111;j++)
				_nop_();
		}
    userdata = userdata<<1;
	}
}

//===========================================================================
// 名称: RCSend()
// 功能: 发码主程序
// 输入：
// 输出：
// 调用：
// 嵌套级数：
// 影响：
// 其它：只发一幀
//===========================================================================
void RCSend(void)
{
	U8 i;
		
	LCDM9 |= 0x01;				// 显示发码符号 
	//38KHz，1/3
	CWTD0 = 0xdb;					// 载波低电平计数值
	CWTD1 = 0xee;					// 载波高电平计数值
	CWTCON |= 0x80;				// 开载波
	EIE2 &= 0xbf;					// 屏蔽KBI中断
	ET0 = 0;
	//EA = 0;
	P30 = 1;							// 引导码3047usH+3047usL+3047usH+4432usL
	delay(318);
	P30 = 0;
	delay(317);
	P30 = 1;
	delay(318);
	P30 = 0;
	delay(459);
	//_nop_();
	for(i=0;i<14;i++)			// 数据段(8*14=112位数据)
	{
		ByteSendH(gSendData[i],8);
	}
	for(i=0;i<5;i++)			// 延时补偿
		_nop_();
	P30 = 1;    					// 结束码554usH
	Delay554us();
	P30 = 0;
	//EA = 1;
	EIE2 |= 0x40;   			// 允许KBI中断
	ET0 = 1;
	CWTCON &= 0x7f;				// 关载波
	//delay(2090);    		// 延时20ms
	LCDM9 &= 0xfe;				// 清除发码符号
}

//===========================================================================
// 名称：TimeOffData
// 功能：处理定时关机数据
// 输入：
// 输出：
// 调用：
// 嵌套级数：
// 影响：
// 其它：睡眠和定时关时间同时存在时,取较小值发码
//===========================================================================
void TimeOffData(void)
{
	if(gSpHour<gHour)
	{
		gSendData[5] += gSpHour;
		gSendData[6] += gSpMinute;
	}
	else if(gSpHour==gHour)
	{
		if(gSpMinute<=gMinute)
		{
			gSendData[5] += gSpHour;
			gSendData[6] += gSpMinute;
		}
		else
		{
			gSendData[5] += gHour;
			gSendData[6] += gMinute;
		}
	}
	else
	{
		gSendData[5] += gHour;
		gSendData[6] += gMinute;
	}
}

//===========================================================================
// 名称：GetCode
// 功能：根据按键处理发码数据
// 输入：
// 输出：
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//===========================================================================
void GetCode(void)
{
	U8 tmp,i;
	
	for(i=0;i<32;i++)
		gSendData[i] = 0x00;
	
	//用户码
	gSendData[0] = 0xa6;
	//开关机位			
	if(gSrc[0])
		gSendData[4] |= 0x40;
	//模式数据
	switch(gSrc[1])
	{
		case 0:	gSendData[7] += 0x00;break;//自动
		case 1:	gSendData[7] += 0x20;break;//制冷
		case 2:	gSendData[7] += 0x40;break;//除湿
		case 3:	gSendData[7] += 0x80;break;//制热
		case 4: gSendData[7] += 0xc0;break;//送风
		default:break;
	}
	//温度数据
	tmp = gSrc[2];
	gSendData[1] += (tmp<<4);
	gSendData[10] &= 0xdf;//摄氏
	//风速数据
	switch(gSrc[3])
	{
		case 0:	gSendData[5] += 0xa0;break;//自动
		case 1:	gSendData[5] += 0x60;break;//低风
		case 2:	gSendData[5] += 0x40;break;//中风
		case 3:	gSendData[5] += 0x20;break;//高风
		default:break;
	}
	//上下风向
	switch(gSrc[4])
	{
		case 0:	gSendData[1] += 0x0c;break;//自动摆
		case 1:	gSendData[1] += 0x00;break;//上下固定
		case 2:	gSendData[1] += 0x01;break;//健康气流上吹
		case 3:	gSendData[1] += 0x03;break;//健康气流下吹
		case 4:	gSendData[1] += 0x02;break;//上下摆位置一
		case 5:	gSendData[1] += 0x0a;break;//上下摆位置五
		default:break;
	}
	//左右风向
	switch(gSrc[12])
	{
		case 0:	gSendData[2] += 0x00;break;//左右摆位置一
		case 1:	gSendData[2] += 0x60;break;//左右摆位置四
		case 2:	gSendData[2] += 0x80;break;//左右摆位置五
		case 3:	gSendData[2] += 0xa0;break;//左右摆位置六
		case 4:	gSendData[2] += 0xc0;break;//左右摆位置七
		case 5:	gSendData[2] += 0xe0;break;//自动摆
		default:break;
	}
	//健康位
	if(gSrc[5])
		gSendData[3] |= 0x02;
	//强力位
	tmp = gSrc[8];
	gSendData[6] += (tmp<<6);
	//辅热位
	if(gSrc[9])
		gSendData[4] |= 0x80;
	//自清洁位
	if(gSrc[13])
		gSendData[8] |= 0x40;
	//情景灯光位
	if(gSrc[14])
		gSendData[10] |= 0x10;
	//定位出风
	tmp = gSrc[15];
	gSendData[3] += (tmp<<2);
	//锁定位
	if(gSrc[10])
		gSendData[12] |= 0x20;
	//定频运转位
	if(gSrc[11])
		gSendData[12] |= 0x40;
	//睡眠&定时数据
	if(gHour|gMinute)
	{
		if(gSrc[0])
		{
			gSendData[3] |= 0x20;//定时关
			if(gSrc[7])
			{//睡眠和定时关同时存在时
				gSendData[8] |= 0x80;
				TimeOffData();
			}
			else
			{//有定时关无睡眠
				if(gHour==24)
				{//定时设置为24小时,发码按23小时59分
					gSendData[5] += 23;
					gSendData[6] += 59;
				}
				else
				{
					gSendData[5] += gHour;
					gSendData[6] += gMinute;
				}
			}
		}
		else
		{
			gSendData[3] |= 0x40;//定时开
			if(gHour==24)
			{//定时设置为24小时,发码按23小时59分
				gSendData[7] += 23;
				gSendData[8] += 59;
			}
			else
			{
				gSendData[7] += gHour;
				gSendData[8] += gMinute;
			}
		}
	}
	else
	{
		if(gSrc[7])
		{//无定时关有睡眠
			gSendData[8] |= 0x80;
			gSendData[5] += gSpHour;
			gSendData[6] += gSpMinute;
		}
	}		
	//按键识别码
	gSendData[12] += gKeyID;
	gSendData[12] |= 0x80;//光感设定
	//校验码
	tmp = 0;
	for(i=0;i<13;i++)
		tmp += gSendData[i];
	gSendData[13] = tmp;
}
