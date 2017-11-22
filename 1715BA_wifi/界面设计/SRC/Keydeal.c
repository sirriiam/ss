/*======================================================================================
 文件说明：
 1) 包括按键扫描，去抖动，按键扫描出来后的相应功能处理。
 2) 一般只需要修改UserKeyDeal()函数，对应用户按键功能处理。
 修改记录：
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\Common.h"	
#include "INC\keydeal.h"
#include "INC\USER.H"

//===========================================================================
// 名称：ACDeal
// 功能：空调数据转换和发码
// 输入：gISendDec
// 输出：
// 调用：RCSend,display
// 嵌套级数：
// 影响：
// 其它：
//===========================================================================
void ACDeal(void)
{
	// 根据按键处理编码数据并发码
	if(gISendDec)
		return;								// 按键按下后不是马上发码，而是延时一段时间后发码
	if(gSendType)
	{
		GetCode();
		RCSend();
		gSrc[11] = 0;					// 定频运转位清零
		gSendType = 0;				// 发码结束，清除标志
	}
}

//===========================================================================
// 名称：KeyDeal
// 功能：按键功能处理(主函数)
// 输入：
// 输出：
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//		
//===========================================================================
void KeyDeal(void)
{
	gbHoldTempKey = 0;
	if(gITimeSetDec)
	{
		if(gbPauseTK==0)
		{
			if(gSrc[0])
				LCDM5 &= 0xf7;		// 定时关机
			else
				LCDM5 &= 0xfe;		// 定时开机
			TKCON |= 0x80;			// 长按定时键释放后恢复闪烁
		}
	}
	if(gkey_value == K_NONE)
		return;
	gIWorkDec = TIMETOIDLE;	// 重新进入待机前的时间(只要有按键检测到，就不进入待机)
	if(gkey_mode != KM_IMME_DOWN)
	{
		if(gkey_mode != KM_LONG_DOWN)
			return;		
		if((gkey_value != KEYTIMING)&&(gkey_value != KEYTEMPUP)&&(gkey_value != KEYTEMPDOWN))
			return;
		if(gbKeyTempSpec)
			return;
		if(gSrc[10])
			return;							// 锁定状态下长按无效
		gbHoldTempKey = 1;
		if(gkey_value == KEYTIMING)
		{	
			TKCON &= 0x7f;			// 长按定时键时暂停闪烁关闭定时开/关标志闪烁
			gITimeSetDec = 200;
			gbPauseTK = 1;
		}
	}
	
	// 用户按键处理
	UserKeyDeal();
	if(gbHoldTempKey)
		gISendDec = 25;				// 放键后才发码(需要大于长按判断时间)
	
	if(gkey_mode != KM_NONE)
	{// 设置定时时间状态或有按键
		
		display();
	}
}

//开关机时的功能处理
void OnOffDeal(void)
{
	U8 tmp;
	
	tmp = gSrc[1];
	gSrc[2] = gTempBuf[tmp];	// 读取各模式温度记忆
	gSrc[3] = gFanBuf[tmp];		// 读取各模式风速记忆
	gWT_EN = 0x00;
	if(gSrc[0])
	{
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		if(tmp==3)
		{
			gSrc[4] = 5;			// 制热模式上下风向起始为位置五
			gSrc[9] = 1;			// 开机为制热模式时自动打开辅热
		}
		else
		{
			gSrc[4] = 4;			// 其它模式上下风向起始为位置一
			gSrc[9] = 0;			// 其它情况下关闭辅热
		}
		gSrc[14] = 1;				// 开机默认情景灯光打开
	}
	else
	{
		gSrc[4] = 1;				// 关机状态下上下固定
		gWindUdBuf[0] = 4;	// 关机时风向不记忆,恢复为初始值
		gWindUdBuf[1] = 4;
		gWindUdBuf[2] = 4;
		gWindUdBuf[3] = 5;
		gWindUdBuf[4] = 4;
		gSrc[5] = 0;				// 关闭净化/健康
		gSrc[9] = 0;				// 关闭辅热
		gSrc[14] = 0;				// 关闭情景灯光
	}
	gSrc[6] = 0;					// 取消定时设定
	gSrc[7] = 0;					// 取消睡眠
	gSleepBak = 0;
	gSrc[8] = 0;					// 取消强力
	gSrc[13] = 0;					// 取消自清洁
	gSrc[15] = 0;					// 取消定位出风
	TKCON &= 0x7f;				// 关闭定时开/关标志闪烁
	gITimeSetDec = 0;
	gAutoFanCnt = 0;
	gSecond = 0;
	gMinute = 0;
	gHour = 0;
	gSpSecond = 0;
	gSpMinute = 0;
	gSpHour = 0;
	gClSecond = 0;
	gClMinute = 0;
}

void ModeFan(void)
{
	gSrc[2] = gTempBuf[4];// 读取温度记忆,不可调
	gSrc[3] = gFanBuf[4];	// 读取风速记忆
	if(gSrc[3]==0)
		gWT_EN |= 0x01;
	else
		gWT_EN &= 0xfe;
	gSrc[4] = 4;					// 上下摆起始为位置一
	gSrc[12] = gWindLrBuf[4];// 读取左右风向记忆
	gSrc[7] = 0;					// 取消睡眠
	gSrc[8] = 0;					// 取消强力
	gSrc[9] = 0;					// 关闭辅热
	gSrc[13] = 0;					// 取消自清洁及定时
	gWT_EN &= 0x7f;
	gISendDec = 0;				// 立即发码
	gSendType = 1;
}

//===========================================================================
// 名称：UserKeyDeal
// 功能：用户按键处理
// 输入：
// 输出：
// 调用：
// 嵌套级数：
// 影响：
// 其它：
//===========================================================================
void UserKeyDeal(void)
{
	U8 tmp;
	
	if(gkey_value == KEYONOFF)
	{// 开关机键
		if(gSrc[10])
			return;							// 锁键状态下此键不起作用
		gKeyID = 0x05;				// 按键识别码
		gSrc[0] ^= 0x01;
		OnOffDeal();
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYMODE)
	{// 模式键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		gKeyID = 0x06;
		gAutoFanCnt = 0;
		gSrc[13] = 0;					// 取消自清洁及定时
		gWT_EN &= 0x7f;
		switch(gSrc[1])
		{
			case 0:
				gSrc[1] = 1;			// PMV-->制冷
				goto TOCOOL;
			case 1:
				gSrc[1] = 2;			// 制冷-->除湿
				goto TODRY;
			case 2:
				gSrc[1] = 4;			// 除湿-->送风
				ModeFan();
			  break;            //versions 1.1  针对第二条反馈意见修改01
			case 4:
				gSrc[1] = 3;			// 送风-->制热
				goto TOHEAT;
			default:
				//gSrc[1] = 0;			// 制热-->PMV
				goto TOPMV;
		}
	}
	else if(gkey_value == KEYPMV)
	{// PMV模式键
		if(gSrc[10])
			return;
		gKeyID = 0x06;
		if(gSrc[0]==0)
		{	
			gSrc[0] = 1;				// 关机状态下按PMV则开机,风速变为自动,如有定时开设定需要取消
			gSrc[3] = 0;
			gSrc[14] = 1;		    //versions 1.1  针对第一条反馈意见修改01
			gAutoFanCnt = 0;
			gSrc[6] = 0;
			gHour = 0;
			gMinute = 0;
			gWT_EN &= 0xdf;
		}
TOPMV:
		TKCON &= 0x7f;				// 关闭闪烁
		gITimeSetDec = 0;
		gSrc[6] = 0;
		gSrc[2] = gTempBuf[0];// 读取温度记忆
		gSrc[12] = gWindLrBuf[0];// 读取左右风向记忆
		if(gSrc[1])
		{
			gSrc[3] = 0;					// 其它模式转换到PMV模式时,风速变为自动,上下摆变为位置一,取消健康
			gAutoFanCnt = 0;
			gSrc[4] = 4;
			gWindUdBuf[0] = gSrc[4];
			gVerticalCnt = 0;
			gSleepBak = gSrc[7];
			gSrc[7] = 0;
			gSrc[9] = 0;				// 模式转换时关闭辅热
		}
		else
			gSrc[4] = gWindUdBuf[0];
		gSrc[1] = 0;
		if(gSrc[3]==0)
			gWT_EN |= 0x01;			// 自动风速则开启WT
		else
			gWT_EN &= 0xfe;
		gSrc[8] = 0;					// 取消强力
		gSrc[13] = 0;					// 取消自清洁及定时
		gWT_EN &= 0x7f;
		gIReturnDec = 0;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYCOOL)
	{// 制冷模式键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		gKeyID = 0x06;
		if(gSrc[1]!=1)
		{
			gSrc[1] = 1;
			gAutoFanCnt = 0;
			gSrc[13] = 0;				// 取消自清洁及定时
			gWT_EN &= 0x7f;
		}
		else
		{
			if(gSrc[13])
				return;						// 当前模式下自清洁已设定时按键无效
		}
TOCOOL:
		gSrc[2] = gTempBuf[1];// 读取温度记忆
		gSrc[3] = gFanBuf[1];	// 读取风速记忆
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		else
			gWT_EN &= 0xfe;
		gSrc[4] = gWindUdBuf[1];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// 上下自动摆风定时刷新开启
		else
			gWT_EN &= 0xfb;
		gVerticalCnt = 0;
		gSrc[12] = gWindLrBuf[1];// 读取左右风向记忆
		gSrc[7] = gSleepBak;
		gSrc[8] = 0;					// 取消强力
		gSrc[9] = 0;					// 关闭辅热
		gIReturnDec = 0;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYDRY)
	{// 除湿模式键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		gKeyID = 0x06;
		if(gSrc[1]!=2)
		{
			gSrc[1] = 2;
			gAutoFanCnt = 0;
			gSrc[13] = 0;				// 取消自清洁及定时
			gWT_EN &= 0x7f;
		}
		else
		{
			if(gSrc[13])
				return;						// 当前模式下自清洁已设定时按键无效
		}
TODRY:
		gSrc[2] = gTempBuf[2];// 读取温度记忆
		gSrc[3] = gFanBuf[2];	// 读取风速记忆
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		else
			gWT_EN &= 0xfe;
		gSrc[4] = gWindUdBuf[2];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// 上下自动摆风定时刷新开启
		else
			gWT_EN &= 0xfb;
		gVerticalCnt = 0;
		gSrc[12] = gWindLrBuf[2];// 读取左右风向记忆
		gSrc[7] = gSleepBak;
		gSrc[8] = 0;					// 取消强力
		gSrc[9] = 0;					// 关闭辅热
		gIReturnDec = 0;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYHEAT)
	{// 制热模式键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		gKeyID = 0x06;
		if(gSrc[1]!=3)
		{
			gSrc[1] = 3;
			gAutoFanCnt = 0;
			gSrc[13] = 0;				// 取消自清洁及定时
			gWT_EN &= 0x7f;
		}
		else
		{
			if(gSrc[13])
				return;						// 当前模式下自清洁已设定时按键无效
		}
TOHEAT:
		gSrc[2] = gTempBuf[3];// 读取温度记忆
		gSrc[3] = gFanBuf[3];	// 读取风速记忆
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		else
			gWT_EN &= 0xfe;
		gSrc[4] = gWindUdBuf[3];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// 上下自动摆风定时刷新开启
		else
			gWT_EN &= 0xfb;
		gVerticalCnt = 4;
		gSrc[12] = gWindLrBuf[3];// 读取左右风向记忆
		gSrc[7] = gSleepBak;
		gSrc[8] = 0;					// 取消强力
		gSrc[9] = 1;					// 自动打开辅热
		gIReturnDec = 0;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYTEMPUP)
	{// 温度加键
		if(gSrc[10])
			return;
		if((gSrc[0]==0)||(gSrc[1]==4))
			return;							// 关机状态或送风模式下此键无效
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		if(gSrc[1]==4)
			return;							// 送风模式下此键无效
		gKeyID = 0x00;
		if(gSrc[6]|gHour|gMinute)
		{											// 定时状态下按键,退出定时状态,刚开始温度不变不发码,超过1秒后开始变化
			TKCON &= 0x7f;			// 关闭闪烁
			gSrc[6] = 0;	
			gITimeSetDec = 0;
			if((gbHoldTempKey==0)&&(gIReturnDec==0))
			{
				gIReturnDec = 200;// 定时显示切换到温度显示的返回计时10秒
				gISendDec = 25;
				gSendType = 0;		// 不发码
				return;
			}
			gIReturnDec = 200;	// 定时显示切换到温度显示的返回计时10秒
		}
		tmp = gSrc[1];
		if(tmp==0)
		{
			if(gSrc[2]<13)
				gSrc[2] += 1;			// PMV模式下温度上限为29℃
		}
		else
		{
			if(gSrc[2]<14)
				gSrc[2] += 1;			// 其它模式下温度上限为30℃
		}
		gTempBuf[tmp] = gSrc[2];	// 记忆
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYTEMPDOWN)
	{// 温度减键
		if(gSrc[10])
			return;
		if((gSrc[0]==0)||(gSrc[1]==4))
			return;							// 关机状态或送风模式下此键无效
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		if(gSrc[1]==4)
			return;							// 送风模式下此键无效
		gKeyID = 0x01;
		if(gSrc[6]|gHour|gMinute)
		{											// 定时状态下按键,退出定时状态,刚开始温度不变不发码,超过1秒后开始变化
			TKCON &= 0x7f;			// 关闭闪烁
			gSrc[6] = 0;	
			gITimeSetDec = 0;
			if((gbHoldTempKey==0)&&(gIReturnDec==0))
			{
				gIReturnDec = 200;// 定时显示切换到温度显示的返回计时10秒
				gISendDec = 25;		// 不发码
				gSendType = 0;
				return;
			}
			gIReturnDec = 200;	// 定时显示切换到温度显示的返回计时10秒
		}
		tmp = gSrc[1];
		if(tmp==0)
		{
			if(gSrc[2]>7)
				gSrc[2] -= 1;			// PMV模式下温度下限为23℃
		}
		else
		{
			if(gSrc[2]>0)
				gSrc[2] -= 1;			// 其它模式下温度下限为16℃
		}
		gTempBuf[tmp] = gSrc[2];	// 记忆
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYFANSPEED)
	{// 风速键
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x04;
		if(gSrc[0]==0)
		{
			if(gSrc[5]==0)
				return;						// 关机且净化关闭状态下此键无效
			if(gSrc[3]==1)			// 关机净化时风速只有高/低两档可调
				gSrc[3] = 3;
			else
				gSrc[3] = 1;
			gISendDec = 0;			// 立即发码
			gSendType = 1;
			return;
		}
		tmp = gSrc[1];
		gSrc[3] += 1;
		if(gSrc[3]==4)
		{
			if(tmp==4)
				gSrc[3] = 1;			// 循环处：高->低(送风模式下没有自动)
			else
				gSrc[3] = 0;			// 循环处：高->自动(送风模式下为低)
		}
		gFanBuf[tmp] = gSrc[3];	// 记忆
		if(gSrc[3]==0)
		{
			gWT_EN |= 0x01;			// 自动风速定时刷新开启
			gDispData[13] &= 0xfc;
			gDispData[14] &= 0xfc;
			LCDM13 &= 0xfc;
			LCDM14 &= 0xfc;
		}
		else
			gWT_EN &= 0xfe;
		gAutoFanCnt = 0;
		gSrc[8] = 0;					// 取消强力
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYWINDVERT)
	{// 上下风向键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		if((gSrc[4]==2)||(gSrc[4]==3))
			return;							// 健康气流时上下风向不可调
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x02;
		if(gSrc[4]==0)
			gSrc[4] = 1;
		else
			gSrc[4] = 0;
		tmp = gSrc[1];
		gWindUdBuf[tmp] = gSrc[4];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// 上下自动摆风定时刷新开启
		else
			gWT_EN &= 0xfb;
		if(gSrc[1]==3)
			gVerticalCnt = 4;		// 制热模式下上下自动摆风的起始位置是五
		else
			gVerticalCnt = 0;		// 其它模式下上下自动摆风的起始位置是一
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYWINDHORIZ)
	{// 左右风向键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x03;
		gSrc[12] += 1;
		if(gSrc[12]==6)
			gSrc[12] = 0;				// 全摆->位置一
		tmp = gSrc[1];
		gWindLrBuf[tmp] = gSrc[12];
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYAUXHEAT)
	{// 辅热键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		if((gSrc[1]!=0)&&(gSrc[1]!=3))
			return;							// 只在PMV和制热模式下有效
		gKeyID = 0x16;
		gSrc[9] ^= 0x01;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYPURIFY)
	{// 净化键(健康)
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x07;
		gSrc[5] ^= 0x01;
//		if(gSrc[5])         //versions 1.1  针对第三条反馈意见修改01
//			gSrc[3] = 1;
//		else
//		{
//			tmp = gSrc[1];
//			gSrc[3] = gFanBuf[tmp];
//		}
		if(gSrc[0]==0)
		{
		if(gSrc[5])         //versions 1.1 针对第三条反馈意见修改02风速关机状态低风
			gSrc[3] = 1;
		else
		{
			tmp = gSrc[1];
			gSrc[3] = gFanBuf[tmp];
		}
			gSrc[14] = gSrc[5];	// 关机状态,情景灯光随净化同时开/关 
		}
		else
		{
			if( gSrc[5])
			{
		     gSrc[14] = gSrc[5];	// 开机状态,净化开，情景灯光开  versions 1.1
			}
		}
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYSLEEP)
	{// 睡眠键
		if(gSrc[10])
			return;
		if((gSrc[0]==0)||(gSrc[1]==4))
			return;							// 关机状态或送风模式下此键无效
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效	
		gKeyID = 0x0b;
		gSrc[7] ^= 0x01;
		if(gSrc[1]!=0)
			gSleepBak = gSrc[7];// PMV下设定的睡眠不传给其它模式
		if(gSrc[7])
		{
			gWT_EN |= 0x40;
			gSpSecond = 0;
			gSpMinute = 0;
			gSpHour = 8;				// 睡眠8小时定时关机
		}
		else
		{
			if(gSleepBak==0)
			{
				gWT_EN &= 0xbf;
				gSpSecond = 0;
				gSpMinute = 0;
				gSpHour = 0;
			}
		}
		if(gSrc[8]==1)
			gSrc[8] = 0;				// 睡眠模式下没有强力档,只能在无和安静之间切换
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	/*else if(gkey_value == KEYAIRFLOW)
	{// 健康气流键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x02;
		if(gSrc[4]==2)
			gSrc[4] = 3;				// 上吹->下吹
		else if(gSrc[4]==3)
		{											// 下吹->标准位置
			if(gSrc[1]==3)
				gSrc[4] = 5;
			else
				gSrc[4] = 4;
		}
		else
			gSrc[4] = 2;				// 第一次按或标准位置->上吹
		tmp = gSrc[1];
		gWindUdBuf[tmp] = gSrc[4];
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}*/
	else if(gkey_value == KEYTURBO)
	{// 强力键
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		if((gSrc[1]==0)||(gSrc[1]==2)||(gSrc[1]==4))
			return;							// PMV、除湿、送风模式下按键无效
		gKeyID = 0x08;
		if(gSrc[7])
		{											// 睡眠模式下没有强力档,只能在无和安静之间切换
			if(gSrc[8])
				gSrc[8] = 0;
			else
				gSrc[8] = 2;
		}
		else
		{											// 无->强力->安静->无
			gSrc[8] += 1;
			if(gSrc[8]==3)
				gSrc[8] = 0;
		}
		if(gSrc[8])
		{
			if(gSrc[3]!=0)
			{
				gWT_EN |= 0x01;			// 强力或安静时风速显示变为自动,定时刷新开启
				if((gSrc[7]!=0)||(gSrc[8]!=2))
					gAutoFanCnt = 0;
			}
		}
		else
		{
			if(gSrc[3]!=0)
				gWT_EN &= 0xfe;		// 恢复原风速显示
		}
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYTIMING)
	{// 定时选择键
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x0d;
		if((gSrc[6]==0)||(gITimeSetDec==0))
		{
			if(gSrc[0])					// 刚进入定时开/关状态时载入起始位置
			{
				gSrc[6] = gTimeOffBak;
				TKADDR = 0x65;		// 选择闪烁点:定时关标志
			}
			else
			{
				gSrc[6] = gTimeOnBak;
				TKADDR = 0x05;		// 选择闪烁点:定时开标志
			}
			TKCON = 0x84;				// 开启单点闪烁,频率1Hz
		}
		else
		{	
			if(gSrc[6]>=24)
				gSrc[6] += 2;			// 12小时开始每按一次加一小时
			else
				gSrc[6] += 1;			// 12小时开始每按一次加一小时
			if(gSrc[6]==50)
				gSrc[6] = 1;			// 24小时->0.5小时
		}
		gIReturnDec = 0;
		gITimeSetDec = 200;		// 定时设置状态计时10秒
		gISendDec = 0;
		gSendType = 0;				// 不发码
	}
	else if(gkey_value == KEYOKCANCEL)
	{// 确定/取消键
		if(gSrc[10])
			return;
		if((gSrc[6]|gHour|gMinute)==0)
			return;							// 无定时时此键无效
		gKeyID = 0x10;
		if(gITimeSetDec)
		{// 确定
			TKCON &= 0x7f;			// 关闭闪烁
			gWT_EN |= 0x20;			// 定时开/关机时间开始递减
			if(gSrc[0])
				gTimeOffBak = gSrc[6];	// 记忆作为起始位置
			else
			{
				gTimeOnBak = gSrc[6];
				gSrc[4] = 1;
			}
			tmp = gSrc[6];
			gSecond = 0;
			gHour = tmp/2;			// 将定时时间起始值换算成小时和分钟
			if(tmp<24)
			{
				if(tmp%2)
					gMinute = 30;
				else
					gMinute = 0;
			}
			else
				gMinute = 0;
		}
		else
		{// 取消
			if(gSrc[0]==0)
				gSrc[4] = 1;			// 关机状态下无定时时上下风向为上下固定
			gWT_EN &= 0xdf;			// 定时开/关机时间停止递减
			gSecond = 0;
			gMinute = 0;
			gHour = 0;
		}
		gSrc[6] = 0;
		gITimeSetDec = 0;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYLIGHT)
	{// 灯光键
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x15;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYCLEAN)
	{// 自清洁键
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		if(gSrc[10])
			return;
		if((gSrc[0]==1)&&(gSrc[1]==0))
			return;							// 开机且为PMV模式时按键无效
		if(gHour|gMinute)
			return;							// 定时有效时按键无效
		if(gSrc[7])
			return;							// 静眠下按键无效
		gKeyID = 0x19;
		gSrc[13] = 0x01;
		gWT_EN |= 0x80;				// 定时21分钟
		gClSecond = 0;
		gClMinute = 21;
		gSrc[6] = 0x00;				// 退出定时设置状态
		gITimeSetDec = 0;
		TKCON &= 0x7f;				// 关闭闪烁
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYSCENELGT)
	{// 情景灯光键
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x0c;
		gSrc[14] ^= 0x01;
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYDIRWIND)
	{// 定位出风键
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		if(gSrc[0]==0)
			return;							// 关机状态下此键无效
		gKeyID = 0x02;
		gSrc[15] += 1;
		if(gSrc[15]==4)
			gSrc[15] = 0;				// 位置3->无
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYLOCK)
	{// 锁定键
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		gKeyID = 0x14;
		gSrc[10] ^= 0x01;
		gISendDec = 0;				// 立即发码
		gSendType = 4;
	}
	/*else if(gkey_value == KEYTDNSET)
	{// 温度减+确认/取消
		if(gSrc[10])
			return;
		if((gSrc[0]!=1)||(gSrc[1]!=1)||(gSrc[2]!=0)||(gSrc[3]!=3)||(gSrc[8]!=0))
			return;							// 开机状态+制冷模式+16℃+高风+无强力时才有效
		gKeyID = 0x01;
		gSrc[11] = 1;					// 定频运转
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}
	else if(gkey_value == KEYTUPSET)
	{// 温度加+确认/取消
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// 自清洁已设定时按键无效
		if((gSrc[0]!=1)||(gSrc[1]!=3)||(gSrc[2]!=14)||(gSrc[3]!=3)||(gSrc[8]!=0))
			return;							// 开机状态+制热模式+30℃+高风+无强力时才有效
		gKeyID = 0x01;
		gSrc[11] = 1;					// 定频运转
		gISendDec = 0;				// 立即发码
		gSendType = 1;
	}*/
}

//===========================================================================
// 名称：GetKey
// 功能：按键扫描
// 输入：gbIKeyFlag(按键中断服务程序被赋值,有按键则gbIKeyFlag=1)
// 输出：gkey_mode(键动作类型)
//		 gkey_value(键码)
// 调用：keyscan,key_compare,key_convert
// 堆栈：
// 影响：
// 其它：该函数放在主循环中被调用。
//       gkey_mode值定义：
//       #define KM_NONE         0   // 无键
//       #define KM_IMME_DOWN    1   // 刚按下
//       #define KM_IMME_UP      2   // 短按抬起
//       #define KM_SHORT_DOWN   3   // 按下刚好1.5秒
//       #define KM_LONG_DOWN    4   // 长按未抬起
//       #define KM_LONG_UP      5   // 长按抬起
//   特征说明:
//   1) 按键检测模块主要由keyscan(),key_compare(),key_convert()三个函数完成。
//   2) 多键和单键扫描出来的键码gkey_value不同，以此来区分。
//   3) 有键按下时才会进行按键扫描，直到按键抬起，无键时无按键扫描波形。
//   4) 键按下过程中，若键码发生变化(例如先是按下KEYA,后又按下KEYB，或先是同
//      时按下KEYA & KEYB,然后又放开了一个键KEYA),但gkey_mode值还是按照第一个
//      先按下的那个键计算,例如先按下KEYA，过2秒后再按下KEYB，此时gkey_mode=4。
//===========================================================================
void GetKey(void)
{
	if(gbIKeyFlag)
	{// 有按键中断才开始扫描
		gKeyCode = keyscan();   // 按键扫描
		key_compare();          // 按键扫描后的处理(去抖动,按键时间长度计数)
		key_convert();          // 转换成直观的按键动作,得到gkey_value,gkey_mode
	}
}

//===========================================================================
// 名称：keyscan
// 功能：按键扫描
// 参数：
// 返回：按键扫描键码
// 调用：无
// 嵌套级数：
// 影响：
// 其它：5x4矩阵键盘
//       P1用到的端口输入(内部弱上拉)
//===========================================================================		
U8 keyscan(void)
{
	U8 i,j,k=0;
	U16 result=0x0000;
	
	ClrWDT();
	EIE2 &= 0xbf;   			// 扫描前禁止按键中断
    
	P2DPL = 0xff;
	for(i=0;i<4;i++)			// 输出口逐行输出低电平:P20->P21->P22->P23
	{
		switch(i)
		{
			case 0:	P2 = 0x0e;break;
			case 1:	P2 = 0x0d;break;
			case 2:	P2 = 0x0b;break;
			default:P2 = 0x07;break;
		}
		delay(24);// 约100us
		if((P1&0xf8)!=0xf8)
		{
			for(j=0;j<5;j++)	// 逐列扫描输入口KI3-KI7
			{
				if((P1&(0x08<<j)) == 0)
				{
					k++;
					if(k>=3)	// 多键按下,无效返回
					{
						P2 = 0x00;
						gbFirstKey = 0;
						return 0xfe;
					}
					else
					{
						result = (result<<8)|((i+1)<<4)|(j+1);
					}
				}
			}			
		}		
	}
	P2 = 0x00;
	if(k==1)
	{
		i = result&0x00ff;
		/*if((i==KEYTEMPUP)||(i==KEYTEMPDOWN))
			gbFirstKey = 1;
		else
			gbFirstKey = 0;*/
		return i;
	}
	else if(k==2)
	{
		/*if(gbFirstKey)
		{*/
			if(result==0x3441)
				return KEYTDNSET;	// 先按下温度-,再按下确认/取消
			else if(result==0x2341)
				return KEYTUPSET;	// 先按下温度+,再按下确认/取消
			else if(result==0x2334)
				return KEYLOCK;		// 按下温度+和温度-
			else
			{
				gbFirstKey = 0;
				return 0xfe;
			}
		/*}
		else
			return 0xfe;*/
	}
	else
	{
		gbFirstKey = 0;
		gbKeyTempSpec = 0;
		return 0x00;
	}
}

//===========================================================================
// 名称：key_compare
// 功能：按键中间处理，去抖动等; 得到gkey_sc_value值和gkey_press值
// 参数：
// 返回：无
// 调用：无
// 嵌套级数：
// 影响：gKeyCode，gkey_sc_ct，gbkey_exist，gkey_sc_value，gkey_press
//===========================================================================
void key_compare(void)
{
    if(gKeyCode != gkey_lt_value)    // 若与上次扫描值不同
    {
        gkey_lt_value = gKeyCode;
        gkey_sc_ct = KEYSC;
        return;
    }
    else
    {
        if(gkey_sc_ct != 0)         // 去抖动时间
        {
            gkey_sc_ct--;
            return;
        }
        else
        {
            gbkey_exist = 1;
            gkey_sc_ct = KEYSC;
            if(gKeyCode == gkey_sc_value)
            {// 与上一次扫描的键值相同
                if(gkey_ct != 0) gkey_ct--;          // 键长计数
            }
            else
            {// 与上一次扫描的键值不同
                gkey_sc_value = gKeyCode; // 保存此次键值
                if(gKeyCode != K_NONE)      // 若此次无键，则不更新按下键值
                {
                    gkey_press = gKeyCode;
                    if(gkey_last == K_NONE)
                    {// 原先无键
                        gkey_ct = KEYCT;
                    }
                }
            }
        }
    }    
}

//===========================================================================
// 名称：key_convert
// 功能：将面板按键扫描出的有效键值处理成直观的键的动作
// 输入：gbkey_exist,gkey_sc_value,gkey_ct,gkey_press
// 输出：gkey_mode(键动作类型),gkey_value(键码)
//       gkey_mode=0~5，见[其它]描述
// 调用：
// 堆栈：
// 影响：gbkey_exist,gkey_sc_value,gkey_ct,gkey_press,gkey_last;
//       本函数一开始先清零gkey_mode,gkey_value.
// 其它：
//       #define KM_NONE         0   // 无键
//       #define KM_IMME_DOWN    1   // 刚按下
//       #define KM_IMME_UP      2   // 短按抬起
//       #define KM_SHORT_DOWN   3   // 按下刚好1.5秒
//       #define KM_LONG_DOWN    4   // 长按未抬起
//       #define KM_LONG_UP      5   // 长按抬起
//===========================================================================
void key_convert(void)
{	
    if(!gbkey_exist) return;
    
    gbkey_exist = 0;
    if(gkey_sc_value == K_NONE) // 当前无键
    {
				gbPauseTK = 0;
        if(gkey_last == K_NONE)    // 以前也无键
        {
            gkey_mode = KM_NONE;
            gkey_value = K_NONE;
        }
        else    // 当前无键，以前有键
        {
            if(gkey_ct>=SHORTRELEASE)
            {
                gkey_mode = KM_IMME_UP;		// 短按抬起
            }
            else
            {
                gkey_mode = KM_LONG_UP;		// 长按抬起
            }
            gISendDec = 0;					// 立即发码
            gkey_value = gkey_press;
            gkey_press = 0;
            gkey_last = 0;
            gbIKeyFlag = 0;
            EIE2 |= 0x40;           // 允许KBI中断
        }
    }
    else    // 当前有键
    {
        if(gkey_last == K_NONE)    // 原来无键，刚按下
        {
            gkey_mode = KM_IMME_DOWN;
            gKeyModeSc = 1;
            gkey_last = gkey_sc_value;
            gkey_value = gkey_sc_value;
            gKey_Prev = 0;
            gbKeyChanged = 0;
        }
        else
        {
            if(gkey_press == gkey_last) // 按着时，按键没发生变化
            {
                if(gkey_ct == SHORTDOWN)
                {
                    gkey_mode = KM_SHORT_DOWN;  // 按下刚好1.5秒
                    gkey_ct--;      // 保证只操作一次
                }
                else if(gkey_ct <= LONGDOWN)
                {
                    gkey_mode = KM_LONG_DOWN;  // 长按下超过1.5秒
										delay(24000);
                }
                gkey_value = gkey_sc_value; 
            }
            else    // 按下过程中，按键发生了变化
            {                
                //if(gkey_ct == KEYCT)
                {// 50ms以内按刚按下处理
                    if(gKey_Prev==0)
                    {
                        gKey_Prev = gkey_last;   // 保存第一次变化前的按键值
                    }
                    gkey_value = gkey_sc_value;
                    if((gkey_value==KEYTDNSET)||(gkey_value==KEYTUPSET))
											gkey_mode = KM_IMME_DOWN;
                    else
										{
											gkey_mode = 0;
											if((gkey_last>0x45)&&((gkey_value==KEYTEMPUP)||(gkey_value==KEYTEMPDOWN)))
												gbKeyTempSpec = 1;
										}
                    gkey_ct = KEYCT;    // 按键动作类型重新处理
                 }
                //else
                //{
                //    gkey_value = K_NONE;
                //    gkey_mode = KM_NONE;
                //}                                   
                gkey_press = gkey_sc_value;        
                gkey_last = gkey_sc_value;     
                gbKeyChanged = 1;           // 按键值发生变化的标志
								gISendDec = 0;
								gSendType = 0;                
            }
        }
    }
}
