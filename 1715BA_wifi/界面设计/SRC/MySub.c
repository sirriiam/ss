/*======================================================================================
 �ļ�˵����
 1) ����ʵʱʱ�Ӷ�ʱ������ʱ�������жϺͶ�Ӧ�Ĵ����������ǰ�ʹӴ������ѵĴ���
 �޸ļ�¼��
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\common.h"
#include "INC\MySub.h"
#include "INC\USER.H"

//===========================================================================
// ���ƣ�TailControl
// ���ܣ�LCD��ʾ��ʱ����
// ������
// ���أ�
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
//===========================================================================
void TailControl(void)
{	
	if(gbLoopDisp==0)
		return;
	if(gSrc[0]==0)
		return;
	gbLoopDisp = 0;
	if((gSrc[3]==0)||(gSrc[8]!=0))
  {// �Զ�����
		if((gWT_EN&0x01)==0x00)
		{
			gWT_EN |= 0x01;
		}
		// ˢ��ǰ�����
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
	{// �����Զ��ڷ�
		if((gWT_EN&0x04)==0x00)
		{
			gWT_EN |= 0x04;
		}
		// ˢ��ǰ�����
		gVerticalCnt++;
		if(gVerticalCnt==8)
			gVerticalCnt=0;
		gDispData[9] &= 0xf9;
		gDispData[10] &= 0xf9;
		gDispData[11] &= 0xfe;
		switch(gVerticalCnt)
		{
			case 0:
				gDispData[9] |= 0x02;break;//�ϵ���
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
				gDispData[11] |= 0x01;break;//�µ���
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
// ���ƣ�IdleModeDeal
// ���ܣ�IDLE(�����͹���)ģʽ�Ľ�����˳�
// ���룺
// �����
// ���ã�
// ��ջ��
// Ӱ�죺
// ������
//===========================================================================
bit IdleModeDeal(void)
{
	volatile U8 temp;
	
	if((gIWorkDec | gkey_value | gITimeSetDec | gIReturnDec)==0x00)
	{// �������
		PortInit();
		if(gbTimeOut)
		{// ��ʱ���ó�ʱ�˳�����Ҫˢ����ʾ
			gbTimeOut = 0;
			display();
		}
		/*P30 = 0;					// ����ܽ������
		P2DPL = 0xaa;				// P2����λ��Ϊ�������
		P2 = 0x00;					// P2����λ�͵�ƽ(δ�õ�)*/
		
		CWTCON = 0x00;			// ���ز�ģ��
		
		ET0 = 0;						// ������T0�ж�
		ET1 = 0;
		TR0 = 0;
		TR1 = 0;
		EA = 0;
		SPKEY = 0x36;
		CKEN &= 0xfd;				// ��ILRC
		EIE2 |= 0x40;				// ����KBI�ж�
		
		temp = KINFLG;			// �ȶ�������
		KINFLG = 0x00;
		temp = KINFLG;			// �ȶ�������
		KINFLG = 0x00;
		gbIKeyFlag = 0;
		
		//SPKEY = 0x36;
		//CKSS = 0x00;				// ѡ��IHRC��ΪMCLK
		_nop_();
		_nop_();
		
		//SPKEY = 0x36;
		//PLLCON = 0x00;			// ��PLL,128��Ƶ
//		BORCON = 0x00; 			// �رյ�ѹ��׼,�رյ�ѹ��λ    versions 1.1  �޸�
	
		EA = 1;
		#if DEBUG
			while((gbIKeyFlag|gbCountDown)==0)
				ClrWDT();
		#else
			SPKEY = 0x36;
			PCON = 0x02;
		#endif
	
		delay(2);
//		BORCON |= 0x80;			// versions 1.1  �޸�
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
//		BORCON |= 0x01;			//versions 1.1  �޸� 
	
		EA = 0;
		SPKEY = 0x36;
		CKDS = 0x01;				// 
	
		SPKEY = 0x36;
		CKEN |= 0x02;				// 
		EA = 1;
		ET0 = 1;
		TR0 = 1;
		ET1 = 1;
		//CWTCON = 0x80;		// ���ز�������
		
		//P1DPL = 0x55;
		//P2DPL = 0xff;			// P2����λ������Ϊ��©���
		if(gbCountDown)			// Ҫע���ٴε��ñ�����ǰ��gbWT�ѱ����㣬��Ȼ���Ƿ���1
			return 1;
	} 
	return 0;
}

//===========================================================================
// ���ƣ�timeupdate
// ���ܣ���ʱʱ����ʱ����
// ������
// ���أ�
// ���ã�
// Ƕ�׼�����
// Ӱ�죺gWT_EN
// ������gWT_EN=0x00ʱ��WTʱ�ӲŻ�ر�
//       gWT_EN.0=1 �Զ�����
//       gWT_EN.1=1 
//       gWT_EN.2=1 ���·����Զ���
//       gWT_EN.5=1 ��ʱ��/��ʱ��ݼ�
//       gWT_EN.6=1 ˯��(8Сʱ��ʱ�ػ�)
//       gWT_EN.7=1 �����(21���Ӻ�ȡ��)
//===========================================================================
void timeupdate(void)
{
	if(gWT_EN)
	{
		if((RTCCON & 0x08)==0x00)
		{
			EIE2 |= 0x20;   // ��RTC�ж�
			RTCCON |= 0x08;	// T8EN
		}
		
		if(gbCountDown)
		{// ��RTC�жϲ���
			gbCountDown = 0;
			
			//��ʱ����/�ػ�
			if(gWT_EN&0x20)
			{
				gSecond++;	// һ��һ����
				if(gSecond==60)
				{// һ����
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
							display();	//ˢ�¶�ʱʱ����ʾ
					}
					else if(gMinute==0)
					{
						//gSrc[6] -= 1;
						if(gHour==0)
						{
							gbCountDown = 0;
							gSrc[0] ^= 0x01;
							OnOffDeal();//���ÿ��ػ�ʱ�Ĺ��ܴ���
						}
						display();	//ˢ�¶�ʱʱ����ʾ
					}
				}
			}
			
			//˯��(8Сʱ��ʱ�ػ�)
			if(gWT_EN&0x40)
			{
				gSpSecond++;	// һ��һ����
				if(gSpSecond==60)
				{// һ����
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
							OnOffDeal();//���ÿ��ػ�ʱ�Ĺ��ܴ���
							display();	//ˢ����ʾ
						}
					}
				}
			}
			//�����(21���Ӻ�ȡ��)
			if(gWT_EN&0x80)
			{
				gClSecond++;	// һ��һ����
				if(gClSecond==60)
				{// һ����
					gClSecond = 0;
					gClMinute--;
					if(gClMinute==0)
					{
						gbCountDown = 0;
						gWT_EN &= 0x7f;
						//Init_Close_WT();
						gSrc[13] = 0;
						display();	//ˢ����ʾ
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
// ���ƣ�Init_Close_RTC
// ���ܣ���ʼ�����ر�RTC
// ���룺
// �����
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
//===========================================================================
void Init_Close_RTC(void)
{
    U8 tmp;
    
    // RTC���³�ʼ��
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
    RTCCNT = 0xc0;			// 256 - 64  1��
    //RTCCNT = 0xe0;		// 256 - 32  0.5��
    //gT8s=0xc0;
    tmp = RTCCON;
    RTCCON = 0x83;			// 64Hz,RTC��
    EIE2 &= 0xdf;				// ��RTC�ж�
}

/*******************************************************************
�������ƣ�ClrWDT()
�������ܣ��忴�Ź�
��ڲ�������
���ڲ�������
��    ע��
*******************************************************************/
void ClrWDT(void)
{
	WDTEN0 = 0x00;
	WDTEN1 = 0x00;
	WDTEN0 = 0x39;
	WDTEN1 = 0xC6;
}
