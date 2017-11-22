/*======================================================================================
  PROJECT NAME: Haier-1715AJ
  CREATED DATE: 2016-07-18
  CREATED BY: Francesco Chen
  FUNCTION: �յ�ң����
  MODIFY RECODE: 
  OTHER: 
  	1) KEIL uV4,C51(V9.52.0.0),A51(V8.02b),@Windows 7.
		2) C51��������Ż�����8(Reuse Common Entry Code).
		3) MCUʹ��Rb32Pxx5A.
    4) RAM����: 
         idata:0x80~0xb1 --> gSrc[0]~gSrc[49],����Դ��,Ԥ��50bytes;
         idata:0xb2~0xd1 --> gSendData[0]~gSendData[31],������������;
         idata:0xd2~0xff --> ϵͳ��ջ��46�ֽ�;
         data:R0~R7�����Ĵ��������ñ���,λ����;
         xdata:��ͨ����(��Ƶ���õ��ı���,����ִ��Ч��Ҫ�󲻸ߵĳ���);
         ��������ʣ��ռ���鿴��������
	  5) 32K��Ƶ�ٶ���Ƶ PCLK = MCLK = 32.768K*128/2 = 2.097MHz��
	  6) ��ʱ��0������ͨ��ʱ�жϣ�Լ5ms�ж����ڡ�
	  7) ��ʱ��1δ�õ���
	  8) ��ʱ��2δ�õ���
		9) ���벨��ʱ�����ָ����ʱ��
	
======================================================================================*/

#include "INC\head.h"
#include "INC\DataType.h"
#include "INC\main.h"
#include "INC\USER.H"

//===========================================================================
// ���ƣ�main
// ���ܣ�������
// ���룺main����֮ǰ��RAM��XRAM���ݶ��ѱ���ʼ��Ϊ0(STARTUP.A51)
// �����
// ���ã�initial,display,keyscan,key_compare,key_convert,KeyDeal,ClrWDT,ACDeal,
//       IdleModeDeal,Tailcontrol
// ��ջ��ϵͳ��ջ��ʼ��ַ0xd2(in idata),Ŀǰ�ܳ�����46�ֽڶ�ջ
// Ӱ�죺
// ������
//     1) ���������ÿ5msѭ��һ��,����ʱֱ�����������Ż������һ��ѭ��
//     2) �ް���һ��ʱ���(3��,���Զ���)����idleģʽ��T0���ص���gbI5ms���ٲ���
//     3) RTC(ʱ�Ӷ�ʱ��)�򰴼��ж�(KBI)�ỽ��MCU���Ӷ��˳�idleģʽ
//     4) ��⵽�м�����ʱ���Ż���к�����ɨ��
//===========================================================================
void main()
{
    U8 bRetMode;    			// IDLEģʽ����ֵ
    
    SysInit();						// ϵͳ��ʼ��: �˿ڣ��Ĵ�����LCD��ʾ��ȫ�ֱ���...
    
    /*
    // ����
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
    
		//display();					// LCD��ʾ��ǰ����Դ��Ϣ
    bRetMode = 0;
    
    while(1)
    {// ��ѭ����Լ5msѭ��һ��
		 // һ������£���ѭ�������в��ܼ��볤��ʱ������������
		 // ÿ��5ms��T0�жϷ�������У�gbI5ms=1
			if(gbI5ms)
			{
				gbI5ms = 0;
				gkey_value = 0;		// ��������
				gkey_mode = 0;		// ����������������
            
				if(!bRetMode)
				{// ���Ǵ�WT��ʱ������
					GetKey();    			
					KeyDeal();			// �������ܴ���
					//TailControl(); 
					ClrWDT();				// �����忴�Ź�(���ʱ��Լ2s)
    			ACDeal();				// �յ�����ת��������
				}
				TailControl();          // ������ʾ����,��ʱ��������,���״̬����
				ClrWDT();					// �忴�Ź�(���ʱ��Լ2s)
            
				/*
				// ����
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
				// STOP(�����͹���)ģʽ�Ľ�����˳�
				// bRetMode: =1 ��WT��ʱ����STOP����������ģʽ
				//           =0 ���Ǵ�WT��ʱ�����ѣ�����������������ģʽ
				//-----------------------------------------------------------
				bRetMode = IdleModeDeal();
				//TailControl();		// ������ʾ����,��ʱ��������,���״̬����
				timeupdate();			// ��ʱʱ����ʱ����
				if(bRetMode)
					gbI5ms = 1;
			}
		}
}
