/*======================================================================================
  PROJECT NAME: Haier-1715AJ
  CREATED DATE: 2016-07-18
  CREATED BY: Francesco Chen
  FUNCTION: 空调遥控器
  MODIFY RECODE: 
  OTHER: 
  	1) KEIL uV4,C51(V9.52.0.0),A51(V8.02b),@Windows 7.
		2) C51代码编译优化级别8(Reuse Common Entry Code).
		3) MCU使用Rb32Pxx5A.
    4) RAM分配: 
         idata:0x80~0xb1 --> gSrc[0]~gSrc[49],数据源区,预留50bytes;
         idata:0xb2~0xd1 --> gSendData[0]~gSendData[31],即发码数据区;
         idata:0xd2~0xff --> 系统堆栈区46字节;
         data:R0~R7工作寄存器及常用变量,位变量;
         xdata:普通变量(非频繁用到的变量,用在执行效率要求不高的程序);
         各区变量剩余空间请查看编译结果。
	  5) 32K倍频再二分频 PCLK = MCLK = 32.768K*128/2 = 2.097MHz。
	  6) 定时器0用于普通定时中断，约5ms中断周期。
	  7) 定时器1未用到。
	  8) 定时器2未用到。
		9) 发码波形时间采用指令延时。
	
======================================================================================*/

#include "INC\head.h"
#include "INC\DataType.h"
#include "INC\main.h"
#include "INC\USER.H"

//===========================================================================
// 名称：main
// 功能：主程序
// 输入：main函数之前，RAM和XRAM内容都已被初始化为0(STARTUP.A51)
// 输出：
// 调用：initial,display,keyscan,key_compare,key_convert,KeyDeal,ClrWDT,ACDeal,
//       IdleModeDeal,Tailcontrol
// 堆栈：系统堆栈起始地址0xd2(in idata),目前总程序开了46字节堆栈
// 影响：
// 其它：
//     1) 正常情况下每5ms循环一次,发码时直到发码结束后才会进入下一次循环
//     2) 无按键一段时间后(3秒,可自定义)进入idle模式，T0被关掉，gbI5ms不再产生
//     3) RTC(时钟定时器)或按键中断(KBI)会唤醒MCU，从而退出idle模式
//     4) 检测到有键按下时，才会进行后续的扫描
//===========================================================================
void main()
{
    U8 bRetMode;    			// IDLE模式返回值
    
    SysInit();						// 系统初始化: 端口，寄存器，LCD显示，全局变量...
    
    /*
    // 留空
    #pragma asm
    jmp _main_bypass
    DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
    DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
    DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
    DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
    DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
    _main_bypass:
    #pragma endasm
    */
    
		//display();					// LCD显示当前数据源信息
    bRetMode = 0;
    
    while(1)
    {// 主循环，约5ms循环一次
		 // 一般情况下，主循环程序中不能加入长延时，发码程序除外
		 // 每过5ms在T0中断服务程序中，gbI5ms=1
			if(gbI5ms)
			{
				gbI5ms = 0;
				gkey_value = 0;		// 键码清零
				gkey_mode = 0;		// 按键动作类型清零
            
				if(!bRetMode)
				{// 不是从WT定时器唤醒
					GetKey();    			
					KeyDeal();			// 按键功能处理
					//TailControl(); 
					ClrWDT();				// 这里清看门狗(溢出时间约2s)
    			ACDeal();				// 空调数据转换及发码
				}
				TailControl();          // 包括显示处理,延时变量处理,相关状态处理
				ClrWDT();					// 清看门狗(溢出时间约2s)
            
				/*
				// 留空
				#pragma asm
				jmp _main_bypass1
				DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
				DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
				DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
				DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
				DB  0ffh,0ffh,0ffh,0ffh,0ffh,0ffh
				_main_bypass1:
				#pragma endasm
				*/
            
				//-----------------------------------------------------------
				// STOP(待机低功耗)模式的进入和退出
				// bRetMode: =1 由WT定时唤醒STOP到正常工作模式
				//           =0 不是从WT定时器唤醒，或本来就是正常工作模式
				//-----------------------------------------------------------
				bRetMode = IdleModeDeal();
				//TailControl();		// 包括显示处理,延时变量处理,相关状态处理
				timeupdate();			// 定时时间走时更新
				if(bRetMode)
					gbI5ms = 1;
			}
		}
}
