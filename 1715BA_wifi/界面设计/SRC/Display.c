/*======================================================================================
 �ļ�˵����
 1) LCD��ʾ��
 2) ��������ʾ��������ʾ��
 �޸ļ�¼��
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\common.h"
#include "INC\display.h"
#include "INC\USER.H"

//=====================================================================================
// ���ƣ�display
// ���ܣ�LCD��ʾ����
// ���룺
// �����
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
//=====================================================================================
void display(void)
{
	U8 tmp;
	
	// �ػ�״̬�µ���ʾ
	if(gSrc[0]==0)
	{
		for(tmp=0;tmp<32;tmp++)
		{
			gDispData[tmp] = 0x00;//����������ʾ
		}
		// ����ָʾ
		if(gSrc[10])
			gDispData[8] |= 0x01;
		if(gSrc[6]|gHour|gMinute)
		{// �ж�ʱ
			if(gHour|gMinute)
			{// ��ȷ��
				gDispData[15] |= 0x01;// �ֽ���
				DispMode();					// ģʽ
			}
			DispTiming();
		}
		else
		{
			if(gSrc[13])
			{// �������ʾ
				gDispData[2] |= 0x07;
				gDispData[4] = 0x0f;
			}
		}
		if(gSrc[5])
		{// ������ʾ
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

	// ����״̬��������̶���ʾ
	if((gSrc[3]!=0)&&((gWT_EN&0x01)==0))
	{
		gDispData[12] = 0x00;
		gDispData[13] = 0x00;
		gDispData[14] = 0x00;
	}
	else
	{
		gDispData[12] &= 0xf3;	//�Զ����ٲ�����,��TailControl()�д���
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
			gDispData[9] &= 0xf0;	//�Զ�����ʼʱ����
			gDispData[10] &= 0xf0;
			gDispData[11] &= 0xf0;
		}
		else
		{
			gDispData[9] &= 0xf6;	//�Զ�������ʱ������,��TailControl()�д���
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
	
	// �ֽ���
	gDispData[15] |= 0x01;
	
	// ����ָʾ
	if(gSrc[10])
		gDispData[8] |= 0x01;
	
	// ģʽ
	DispMode();
		
	// ��ʱʱ��&�¶�&�����(�޷�ͬʱ��ʾ)
	if(((gSrc[6]|gHour|gMinute)==0)||(gIReturnDec!=0))
	{
		if(gSrc[13])
		{// �������ʾ
			gDispData[2] |= 0x07;
			gDispData[4] = 0x0f;
		}
		else
		{
			if(gSrc[1]==0)
			{// PMVģʽ�¶���ʾ������ͬ
				tmp = 0;
				switch(gSrc[2])
				{
					case 7:
						tmp++;	// 23��,��ʾ-3��
					case 8:
						tmp++;	// 24��,��ʾ-2��
					case 9:
						tmp++;	// 25��,��ʾ-1��
						gDispData[0] |= 0x04;
						gDispData[1] |= 0x08;
						gDispData[3] |= 0x0a;
						DispNum(tmp,2);break;
					case 13:
						tmp++;	// 29��,��ʾ3��
					case 12:
						tmp++;	// 28��,��ʾ2��
					case 11:
						tmp++;	// 27��,��ʾ1��
						gDispData[0] |= 0x04;
						gDispData[1] |= 0x08;
						gDispData[3] |= 0x08;
						DispNum(tmp,2);break;
					default:break;	// 26��,����ʾ
				}
			}
			else if(gSrc[1]<=3)
			{
				gDispData[0] |= 0x04;
				gDispData[1] |= 0x08;
				gDispData[3] |= 0x08;
				tmp = gSrc[2]+16;		// 16~30��
				DispNum(tmp/10,1);
				DispNum(tmp%10,2);
			}
		}
	}
	else
		DispTiming();
	
	// ����
	gDispData[12] |= 0x03;
	if((gWT_EN&0x01)==0)
	{
		switch(gSrc[3])
		{
			case 3:	// ��
				gDispData[14] |= 0x03;
			case 2:	// ��
				gDispData[13] |= 0x03;
			default:// ��/�Զ�
				break;
		}
	}
	// ���·���
	if(gSrc[4]!=1)	// ���¹̶�����ʾ
	{
		gDispData[10] |= 0x01;
		gDispData[11] |= 0x02;
		switch(gSrc[4])
		{
			case 3:	// ���������´�
				gDispData[10] |= 0x02;
			case 5:	// λ����
				gDispData[11] |= 0x01;break;
			case 2:	// ���������ϴ�
				gDispData[9] |= 0x04;
			case 4:	// λ��һ
				gDispData[9] |= 0x02;break;
			default:// �Զ���
				if(gVerticalCnt==0)
					gDispData[9] |= 0x02;	// λ��һ��ʼ
				else if(gVerticalCnt==4)
					gDispData[11] |= 0x01;	// λ���忪ʼ
				break;
		}
	}
	
	// ���ҷ���
	gDispData[12] |= 0x04;
	gDispData[14] |= 0x08;
	switch(gSrc[12])
	{
		case 0:	// λ��һ
			gDispData[12] |= 0x08;
			gDispData[13] |= 0x08;break;
		case 1:	// λ����
			gDispData[11] |= 0x04;break;
		case 2:	// λ����
			gDispData[11] |= 0x08;break;
		case 3:	// λ����
			gDispData[13] |= 0x04;break;
		case 4:	// λ����
			gDispData[14] |= 0x04;break;
		case 5:	// �Զ���
			gDispData[11] |= 0x0c;
			gDispData[12] |= 0x08;
			gDispData[13] |= 0x0c;
			gDispData[14] |= 0x04;break;
		default:
			break;
	}
		
	// ����
	if(gSrc[5])
		gDispData[15] |= 0x08;
		
	// ˯��
	if(gSrc[7])
		gDispData[16] |= 0x04;
	
	// ǿ��
	switch(gSrc[8])
	{
		case 1:	// ǿ��
       	   	gDispData[15] |= 0x02;break;
		case 2:	// ����
       	    gDispData[16] |= 0x01;break;
		default:// ��
			break;
	}
	
	// ����
	if(gSrc[9])
		gDispData[16] |= 0x08;
	
	// data to lcd
	disptolcd();
}

//ģʽ��ʾ
void DispMode(void)
{
	switch(gSrc[1])
	{
		case 0:	// PMV
			gDispData[7] |= 0x08;break;
		case 1:	// ����
			gDispData[7] |= 0x01;break;
		case 2:	// ��ʪ
			gDispData[6] |= 0x01;break;
		case 3:	// ����
			gDispData[6] |= 0x08;break;
		case 4:	// �ͷ�
			gDispData[6] |= 0x04;break;
		default:
			break;
    }
}

//��ʱ��/����ʾ
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
			gDispData[0] |= 0x01;	// ��Сʱ��ʾ.5
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
			tmp++;								// ��ʱʱ�����12Сʱ,���δ����Сʱ��ʾ���ܼ�1
		DispNum(tmp/20,1);
		DispNum((tmp%20)/2,2);
	}
	if((gITimeSetDec==0)||(gbPauseTK==1))
	{
		if(gSrc[0])
			gDispData[5] |= 0x08;	// ��ʱ�ػ�
		else
			gDispData[5] |= 0x01;	// ��ʱ����
	}
}

//===========================================================================
// ���ƣ�DispNum
// ���ܣ�������ʾ
// ������num: ��ʾ������0~9
//		 n: ��ʾ���ֵ�λ�û�����
// ���أ�
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������n=1Ϊʮλ,=2Ϊ��λ
//===========================================================================
void DispNum(U8 num,U8 n)
{
	switch(n)
	{
		case 1://ʮλ
			gDispData[3] |= g_Number[num]>>4;
			gDispData[4] |= g_Number[num]&0x0F;
			break;
		case 2://��λ
			gDispData[1] |= g_Number[num]>>4;
			gDispData[2] |= g_Number[num]&0x0F;
			break;
		default:
			break;
	}
}

//===========================================================================
// ����: disptolcd
// ���ܣ���ʾ��LCD
// ������
// ���أ�
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
//===========================================================================
void disptolcd(void)
{
	U8 tmp;
	
	for(tmp=0;tmp<32;tmp++)
	{
		*((vxu8*)(0xff60+tmp)) = gDispData[tmp];
	}
}
