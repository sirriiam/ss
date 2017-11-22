/*======================================================================================
 �ļ�˵����
 1) �����������ݴ������������򣬷��벨�β���ָ����ʱ��
 �޸ļ�¼��
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\common.h"

extern U8 xdata gMinute;
extern U8 xdata gHour;
extern U8 xdata gSpMinute;
extern U8 xdata gSpHour;
extern U8 xdata gKeyID;		// ����ʶ����,���ڷ�������

//===========================================================================
// ����: Delay554us()
// ����: ��ʱ554΢��
// �������
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
//===========================================================================
void Delay554us(void)
{
	U8 i;
	
	for(i=0;i<114;i++)
		_nop_();
}

//===========================================================================
// ����: ByteSendH()
// ����: ���͵��ֽ������� 
// ���룺userdata���͵��ֽ����ݣ�n���͵�λ��
// �������
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ��������λ�ȷ�
//===========================================================================
void ByteSendH(U8 userdata,U8 n)
{
	U8 i,j;
	
	for(i=0;i<n;i++)
	{
		if(userdata & 0x80)
		{
			P30 = 1;			     // �߼�1�ߵ�ƽ
			Delay554us();	
			P30 = 0;			     // �߼�1�͵�ƽ
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
			P30 = 1;			     // �߼�0�ߵ�ƽ
			Delay554us();
			P30 = 0;			     // �߼�0�͵�ƽ
		}
		if (i==(n-1))				 // ���һλ��������ߵ�ָ���һЩ
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
// ����: RCSend()
// ����: ����������
// ���룺
// �����
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������ֻ��һ��
//===========================================================================
void RCSend(void)
{
	U8 i;
		
	LCDM9 |= 0x01;				// ��ʾ������� 
	//38KHz��1/3
	CWTD0 = 0xdb;					// �ز��͵�ƽ����ֵ
	CWTD1 = 0xee;					// �ز��ߵ�ƽ����ֵ
	CWTCON |= 0x80;				// ���ز�
	EIE2 &= 0xbf;					// ����KBI�ж�
	ET0 = 0;
	//EA = 0;
	P30 = 1;							// ������3047usH+3047usL+3047usH+4432usL
	delay(318);
	P30 = 0;
	delay(317);
	P30 = 1;
	delay(318);
	P30 = 0;
	delay(459);
	//_nop_();
	for(i=0;i<14;i++)			// ���ݶ�(8*14=112λ����)
	{
		ByteSendH(gSendData[i],8);
	}
	for(i=0;i<5;i++)			// ��ʱ����
		_nop_();
	P30 = 1;    					// ������554usH
	Delay554us();
	P30 = 0;
	//EA = 1;
	EIE2 |= 0x40;   			// ����KBI�ж�
	ET0 = 1;
	CWTCON &= 0x7f;				// ���ز�
	//delay(2090);    		// ��ʱ20ms
	LCDM9 &= 0xfe;				// ����������
}

//===========================================================================
// ���ƣ�TimeOffData
// ���ܣ�����ʱ�ػ�����
// ���룺
// �����
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������˯�ߺͶ�ʱ��ʱ��ͬʱ����ʱ,ȡ��Сֵ����
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
// ���ƣ�GetCode
// ���ܣ����ݰ�������������
// ���룺
// �����
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
//===========================================================================
void GetCode(void)
{
	U8 tmp,i;
	
	for(i=0;i<32;i++)
		gSendData[i] = 0x00;
	
	//�û���
	gSendData[0] = 0xa6;
	//���ػ�λ			
	if(gSrc[0])
		gSendData[4] |= 0x40;
	//ģʽ����
	switch(gSrc[1])
	{
		case 0:	gSendData[7] += 0x00;break;//�Զ�
		case 1:	gSendData[7] += 0x20;break;//����
		case 2:	gSendData[7] += 0x40;break;//��ʪ
		case 3:	gSendData[7] += 0x80;break;//����
		case 4: gSendData[7] += 0xc0;break;//�ͷ�
		default:break;
	}
	//�¶�����
	tmp = gSrc[2];
	gSendData[1] += (tmp<<4);
	gSendData[10] &= 0xdf;//����
	//��������
	switch(gSrc[3])
	{
		case 0:	gSendData[5] += 0xa0;break;//�Զ�
		case 1:	gSendData[5] += 0x60;break;//�ͷ�
		case 2:	gSendData[5] += 0x40;break;//�з�
		case 3:	gSendData[5] += 0x20;break;//�߷�
		default:break;
	}
	//���·���
	switch(gSrc[4])
	{
		case 0:	gSendData[1] += 0x0c;break;//�Զ���
		case 1:	gSendData[1] += 0x00;break;//���¹̶�
		case 2:	gSendData[1] += 0x01;break;//���������ϴ�
		case 3:	gSendData[1] += 0x03;break;//���������´�
		case 4:	gSendData[1] += 0x02;break;//���°�λ��һ
		case 5:	gSendData[1] += 0x0a;break;//���°�λ����
		default:break;
	}
	//���ҷ���
	switch(gSrc[12])
	{
		case 0:	gSendData[2] += 0x00;break;//���Ұ�λ��һ
		case 1:	gSendData[2] += 0x60;break;//���Ұ�λ����
		case 2:	gSendData[2] += 0x80;break;//���Ұ�λ����
		case 3:	gSendData[2] += 0xa0;break;//���Ұ�λ����
		case 4:	gSendData[2] += 0xc0;break;//���Ұ�λ����
		case 5:	gSendData[2] += 0xe0;break;//�Զ���
		default:break;
	}
	//����λ
	if(gSrc[5])
		gSendData[3] |= 0x02;
	//ǿ��λ
	tmp = gSrc[8];
	gSendData[6] += (tmp<<6);
	//����λ
	if(gSrc[9])
		gSendData[4] |= 0x80;
	//�����λ
	if(gSrc[13])
		gSendData[8] |= 0x40;
	//�龰�ƹ�λ
	if(gSrc[14])
		gSendData[10] |= 0x10;
	//��λ����
	tmp = gSrc[15];
	gSendData[3] += (tmp<<2);
	//����λ
	if(gSrc[10])
		gSendData[12] |= 0x20;
	//��Ƶ��תλ
	if(gSrc[11])
		gSendData[12] |= 0x40;
	//˯��&��ʱ����
	if(gHour|gMinute)
	{
		if(gSrc[0])
		{
			gSendData[3] |= 0x20;//��ʱ��
			if(gSrc[7])
			{//˯�ߺͶ�ʱ��ͬʱ����ʱ
				gSendData[8] |= 0x80;
				TimeOffData();
			}
			else
			{//�ж�ʱ����˯��
				if(gHour==24)
				{//��ʱ����Ϊ24Сʱ,���밴23Сʱ59��
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
			gSendData[3] |= 0x40;//��ʱ��
			if(gHour==24)
			{//��ʱ����Ϊ24Сʱ,���밴23Сʱ59��
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
		{//�޶�ʱ����˯��
			gSendData[8] |= 0x80;
			gSendData[5] += gSpHour;
			gSendData[6] += gSpMinute;
		}
	}		
	//����ʶ����
	gSendData[12] += gKeyID;
	gSendData[12] |= 0x80;//����趨
	//У����
	tmp = 0;
	for(i=0;i<13;i++)
		tmp += gSendData[i];
	gSendData[13] = tmp;
}
