/*======================================================================================
 �ļ�˵����
 1) �ϵ��ʼ����������ϵͳר�üĴ������û�RAM�ĳ�ʼ������
 �޸ļ�¼��
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H> 
#include "INC\DataType.h"
#include "INC\Common.h"
#include "INC\Initial.h"
#include "INC\USER.H"
/*******************************************************************
�������ƣ�PortInit()
�������ܣ��˿ڳ�ʼ��
��ڲ�������
���ڲ�������
��    ע��PxMDL/H 00-����������,01-��������,10-�������,11-��©���
		  LCDPENx 0-���������,1-LCD
*******************************************************************/
void PortInit(void)
{
	P0DPL = 0xaa;
	P0DPH = 0xaa;
	LCDPC1 = 0xff;   			// SEG0~SEG7
	
	P1DPL = 0x6a;
	P1DPH = 0x55;					// P13~P17�������������
	LCDPC3 = 0x01;				// SEG16
	P1 &= 0x07;
	
	P2DPL = 0xff;					// P20~P23�������������,��Ϊ��©���
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
�������ƣ�SysInit()
�������ܣ�ϵͳ��ʼ��
��ڲ�������
���ڲ�������
��    ע��
*******************************************************************/
void SysInit(void)
{
	U8 tmp;
	U16 tmp16;
		
	WDTCON = 0xfb;				// ���ʱ��Լ1.433s,��һ�ε���ClrWDT()ʱWDT������
    
	#if DEBUG
		SPKEY = 0x36;
		CKEN = 0x2b;
	#else
		SPKEY = 0x36;
		CKEN = 0x33;				// ��HCRY,P50,P51����GPIO;��IHRC,ILRC,LCRY(��P52,P53���)
	#endif
	PortInit();
	KINEN = 0xf8;					// KI3~KI7
	IOMUXC0 = 0x40;				// P3.0�����ز������,��������
       
	while(1)
	{
		if(CKSS & 0x20)			// 32K�����ȶ���־
			break;
	}
	delay(1600);
	//SPKEY = 0x36;
	SYSCFG = 0x83;				// P6.0�������ⲿ��λ,ILRC��ILRCENʹ��
	SPKEY = 0x36;
	PLLCON = 0x01;				// ��PLL,128��Ƶ
	while(1)
	{
		if(PLLCON & 0x10)		// �ȴ�PLL�ȶ�
			break;
	}
	delay(255);
	SPKEY = 0x36;
	CKSS = 0x02;					// ѡ��PLL��MCLK
	_nop_();
	_nop_();
	_nop_();
	_nop_();
    
	//SPKEY = 0x36;
	//CKEN = 0x33;					// ��HCRY,P50,P51����GPIO;��IHRC,ILRC,LCRY(��P52,P53���)
    
	SPKEY = 0x36;
	CKDS = 0x01;					// ����Ƶ
    
	// ��ʱ������
	TIMPRS = 0x77;				// T1:256��Ƶ;T0:256��Ƶ
	TMOD = 0x12;					// T1:16λ��ʱ��,TR1����;T0:8λ��װ��,TR0����
	TR0 = 0;
	TL0 = 215;						// 4194000/2/256/200=41,256-41=215
	TH0 = 215;
	TR1 = 0;
	TL1 = 0x00;						// 3*4194000/2/256=24576,10000H-24576=0xA000
	TH1 = 0xa0;
	T2CON1 = 0x00;				// 16λ���ط�ʽ
	T2CON2 = 0x00;				// PCLK/2
	Init_Close_RTC();			// ��ʼ�����ر�RTC
    
	CWTPRS = 0x00;				// CWTʱ�Ӳ���Ƶ
	CWTCON = 0x00;				// ���ز���ʱ��
	LVDCON = 0x00;				// �ص�ѹ���
	   
	// LCD��������
	//LCDCON = 0xb7;			// ����,128Hz,1/3bias,4COM
	LCDCON = 0xa7;				// ����,256Hz,1/3bias,4COM

	//LCDPCON = 0x01;				// �Աȶ����,���������е�
	//LCDPCON = 0x00;				// �Աȶ����,����������С
	//LCDPCON = 0x0c;	
	LCDPCON = 0x03;			// �Աȶ����,����������С
	//LCDPCON = 0x1f;			// �Աȶ����,�����������
	//LCDPCON = 0x1c;			// �Աȶ����,����������С
	
	// LCD�ϵ���ʾ
	for(tmp=0;tmp<32;tmp++)
	{// ��ȫ������3����
		*((vxu8*)(0xff60+tmp)) = 0xff;
	}
	for(tmp16=0;tmp16<1550;tmp16++)
	{
		for(tmp=0;tmp<180;tmp++);
			ClrWDT();
	}
	for(tmp=0;tmp<32;tmp++)
	{// ��ȫ��Ϩ��
		*((vxu8*)(0xff60+tmp)) = 0x00;
	}
	
	// ������ʼ������ֵ(���뵽mainǰ,DATA:00~FF,XDATA:00~FF���ѳ�ʼΪ0)
	gSrc[0] = 0;					// �ػ�״̬
	gSrc[1] = 0;					// �ϵ��ʼΪPMVģʽ
	gSrc[2] = 10;					// 26��
	gSrc[3] = 0;					// �Զ�����
	gSrc[4] = 1;					// ���¹̶�
	gSrc[5] = 0;					// �޾���/����
	gSrc[6] = 0;					// �޶�ʱ
	gSrc[7] = 0;					// ��˯��
	gSrc[8] = 0;					// ��ǿ��
	gSrc[9] = 0;					// �޸���(ֻ������ģʽ����Ч)
	gSrc[10] = 0;					// δ����
	gSrc[11] = 0;					// ��Ƶ��ת�ر�
	gSrc[13] = 0;					// �������
	gSrc[14] = 0;					// �龰�ƹ�ر�
	gSrc[15] = 0;					// �޶�λ����
	gTempBuf[0] = 10;			// PMVģʽ��ʼ�¶�26��,����ʾ
	gTempBuf[1] = 10;			// ����ģʽ��ʼ�¶�26��
	gTempBuf[2] = 10;			// ��ʪģʽ��ʼ�¶�26��
	gTempBuf[3] = 10;			// ����ģʽ��ʼ�¶�26��
	gTempBuf[4] = 10;			// �ͷ�ģʽ��ʼ�¶�26��,����ʾ,���ɵ�
	gFanBuf[0] = 0;				// PMVģʽ��ʼ�����Զ�
	gFanBuf[1] = 0;				// ����ģʽ��ʼ�����Զ�
	gFanBuf[2] = 0;				// ��ʪģʽ��ʼ�����Զ�
	gFanBuf[3] = 0;				// ����ģʽ��ʼ�����Զ�
	gFanBuf[4] = 1;				// �ͷ�ģʽ��ʼ���ٵ�
	gWindUdBuf[0] = 4;			// PMVģʽ���򲻼���
	gWindUdBuf[1] = 4;			// ����ģʽ��ʼ����λ��һ
	gWindUdBuf[2] = 4;			// ��ʪģʽ��ʼ����λ��һ
	gWindUdBuf[3] = 5;			// ����ģʽ��ʼ����λ����
	gWindUdBuf[4] = 4;			// �ͷ�ģʽ���򲻼���
	gSleepBak = 0;
	gTimeOnBak = 1;				// ��ʱ��/�ػ���ʼλ�ü���
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
	
	// �ж�ʹ��
	EIP2 |= 0x40;					// KBI�����ȼ�
	EIP2 &= 0xdf;					// RTC�ж����ȼ���
	EIE2 |= 0x40;					// ����KBI�ж�
	PT0 = 0;							// T0���ȼ���
	ET0 = 1;							// ����T0�ж�
	ET1 = 1;
	EA = 1;								// CPU�����ж�
	TR0 = 1;
	TR1 = 0;
}

void delay(U16 tcnt)
{
	do{}while(tcnt--);
}
