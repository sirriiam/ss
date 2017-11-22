/*======================================================================================
 �ļ�˵����
 1) ��������ɨ�裬ȥ����������ɨ����������Ӧ���ܴ���
 2) һ��ֻ��Ҫ�޸�UserKeyDeal()��������Ӧ�û��������ܴ���
 �޸ļ�¼��
======================================================================================*/

#include "INC\head.h"
#include <INTRINS.H>
#include "INC\DataType.h"
#include "INC\Common.h"	
#include "INC\keydeal.h"
#include "INC\USER.H"

//===========================================================================
// ���ƣ�ACDeal
// ���ܣ��յ�����ת���ͷ���
// ���룺gISendDec
// �����
// ���ã�RCSend,display
// Ƕ�׼�����
// Ӱ�죺
// ������
//===========================================================================
void ACDeal(void)
{
	// ���ݰ�������������ݲ�����
	if(gISendDec)
		return;								// �������º������Ϸ��룬������ʱһ��ʱ�����
	if(gSendType)
	{
		GetCode();
		RCSend();
		gSrc[11] = 0;					// ��Ƶ��תλ����
		gSendType = 0;				// ��������������־
	}
}

//===========================================================================
// ���ƣ�KeyDeal
// ���ܣ��������ܴ���(������)
// ���룺
// �����
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
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
				LCDM5 &= 0xf7;		// ��ʱ�ػ�
			else
				LCDM5 &= 0xfe;		// ��ʱ����
			TKCON |= 0x80;			// ������ʱ���ͷź�ָ���˸
		}
	}
	if(gkey_value == K_NONE)
		return;
	gIWorkDec = TIMETOIDLE;	// ���½������ǰ��ʱ��(ֻҪ�а�����⵽���Ͳ��������)
	if(gkey_mode != KM_IMME_DOWN)
	{
		if(gkey_mode != KM_LONG_DOWN)
			return;		
		if((gkey_value != KEYTIMING)&&(gkey_value != KEYTEMPUP)&&(gkey_value != KEYTEMPDOWN))
			return;
		if(gbKeyTempSpec)
			return;
		if(gSrc[10])
			return;							// ����״̬�³�����Ч
		gbHoldTempKey = 1;
		if(gkey_value == KEYTIMING)
		{	
			TKCON &= 0x7f;			// ������ʱ��ʱ��ͣ��˸�رն�ʱ��/�ر�־��˸
			gITimeSetDec = 200;
			gbPauseTK = 1;
		}
	}
	
	// �û���������
	UserKeyDeal();
	if(gbHoldTempKey)
		gISendDec = 25;				// �ż���ŷ���(��Ҫ���ڳ����ж�ʱ��)
	
	if(gkey_mode != KM_NONE)
	{// ���ö�ʱʱ��״̬���а���
		
		display();
	}
}

//���ػ�ʱ�Ĺ��ܴ���
void OnOffDeal(void)
{
	U8 tmp;
	
	tmp = gSrc[1];
	gSrc[2] = gTempBuf[tmp];	// ��ȡ��ģʽ�¶ȼ���
	gSrc[3] = gFanBuf[tmp];		// ��ȡ��ģʽ���ټ���
	gWT_EN = 0x00;
	if(gSrc[0])
	{
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		if(tmp==3)
		{
			gSrc[4] = 5;			// ����ģʽ���·�����ʼΪλ����
			gSrc[9] = 1;			// ����Ϊ����ģʽʱ�Զ��򿪸���
		}
		else
		{
			gSrc[4] = 4;			// ����ģʽ���·�����ʼΪλ��һ
			gSrc[9] = 0;			// ��������¹رո���
		}
		gSrc[14] = 1;				// ����Ĭ���龰�ƹ��
	}
	else
	{
		gSrc[4] = 1;				// �ػ�״̬�����¹̶�
		gWindUdBuf[0] = 4;	// �ػ�ʱ���򲻼���,�ָ�Ϊ��ʼֵ
		gWindUdBuf[1] = 4;
		gWindUdBuf[2] = 4;
		gWindUdBuf[3] = 5;
		gWindUdBuf[4] = 4;
		gSrc[5] = 0;				// �رվ���/����
		gSrc[9] = 0;				// �رո���
		gSrc[14] = 0;				// �ر��龰�ƹ�
	}
	gSrc[6] = 0;					// ȡ����ʱ�趨
	gSrc[7] = 0;					// ȡ��˯��
	gSleepBak = 0;
	gSrc[8] = 0;					// ȡ��ǿ��
	gSrc[13] = 0;					// ȡ�������
	gSrc[15] = 0;					// ȡ����λ����
	TKCON &= 0x7f;				// �رն�ʱ��/�ر�־��˸
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
	gSrc[2] = gTempBuf[4];// ��ȡ�¶ȼ���,���ɵ�
	gSrc[3] = gFanBuf[4];	// ��ȡ���ټ���
	if(gSrc[3]==0)
		gWT_EN |= 0x01;
	else
		gWT_EN &= 0xfe;
	gSrc[4] = 4;					// ���°���ʼΪλ��һ
	gSrc[12] = gWindLrBuf[4];// ��ȡ���ҷ������
	gSrc[7] = 0;					// ȡ��˯��
	gSrc[8] = 0;					// ȡ��ǿ��
	gSrc[9] = 0;					// �رո���
	gSrc[13] = 0;					// ȡ������༰��ʱ
	gWT_EN &= 0x7f;
	gISendDec = 0;				// ��������
	gSendType = 1;
}

//===========================================================================
// ���ƣ�UserKeyDeal
// ���ܣ��û���������
// ���룺
// �����
// ���ã�
// Ƕ�׼�����
// Ӱ�죺
// ������
//===========================================================================
void UserKeyDeal(void)
{
	U8 tmp;
	
	if(gkey_value == KEYONOFF)
	{// ���ػ���
		if(gSrc[10])
			return;							// ����״̬�´˼���������
		gKeyID = 0x05;				// ����ʶ����
		gSrc[0] ^= 0x01;
		OnOffDeal();
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYMODE)
	{// ģʽ��
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		gKeyID = 0x06;
		gAutoFanCnt = 0;
		gSrc[13] = 0;					// ȡ������༰��ʱ
		gWT_EN &= 0x7f;
		switch(gSrc[1])
		{
			case 0:
				gSrc[1] = 1;			// PMV-->����
				goto TOCOOL;
			case 1:
				gSrc[1] = 2;			// ����-->��ʪ
				goto TODRY;
			case 2:
				gSrc[1] = 4;			// ��ʪ-->�ͷ�
				ModeFan();
			  break;            //versions 1.1  ��Եڶ�����������޸�01
			case 4:
				gSrc[1] = 3;			// �ͷ�-->����
				goto TOHEAT;
			default:
				//gSrc[1] = 0;			// ����-->PMV
				goto TOPMV;
		}
	}
	else if(gkey_value == KEYPMV)
	{// PMVģʽ��
		if(gSrc[10])
			return;
		gKeyID = 0x06;
		if(gSrc[0]==0)
		{	
			gSrc[0] = 1;				// �ػ�״̬�°�PMV�򿪻�,���ٱ�Ϊ�Զ�,���ж�ʱ���趨��Ҫȡ��
			gSrc[3] = 0;
			gSrc[14] = 1;		    //versions 1.1  ��Ե�һ����������޸�01
			gAutoFanCnt = 0;
			gSrc[6] = 0;
			gHour = 0;
			gMinute = 0;
			gWT_EN &= 0xdf;
		}
TOPMV:
		TKCON &= 0x7f;				// �ر���˸
		gITimeSetDec = 0;
		gSrc[6] = 0;
		gSrc[2] = gTempBuf[0];// ��ȡ�¶ȼ���
		gSrc[12] = gWindLrBuf[0];// ��ȡ���ҷ������
		if(gSrc[1])
		{
			gSrc[3] = 0;					// ����ģʽת����PMVģʽʱ,���ٱ�Ϊ�Զ�,���°ڱ�Ϊλ��һ,ȡ������
			gAutoFanCnt = 0;
			gSrc[4] = 4;
			gWindUdBuf[0] = gSrc[4];
			gVerticalCnt = 0;
			gSleepBak = gSrc[7];
			gSrc[7] = 0;
			gSrc[9] = 0;				// ģʽת��ʱ�رո���
		}
		else
			gSrc[4] = gWindUdBuf[0];
		gSrc[1] = 0;
		if(gSrc[3]==0)
			gWT_EN |= 0x01;			// �Զ���������WT
		else
			gWT_EN &= 0xfe;
		gSrc[8] = 0;					// ȡ��ǿ��
		gSrc[13] = 0;					// ȡ������༰��ʱ
		gWT_EN &= 0x7f;
		gIReturnDec = 0;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYCOOL)
	{// ����ģʽ��
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		gKeyID = 0x06;
		if(gSrc[1]!=1)
		{
			gSrc[1] = 1;
			gAutoFanCnt = 0;
			gSrc[13] = 0;				// ȡ������༰��ʱ
			gWT_EN &= 0x7f;
		}
		else
		{
			if(gSrc[13])
				return;						// ��ǰģʽ����������趨ʱ������Ч
		}
TOCOOL:
		gSrc[2] = gTempBuf[1];// ��ȡ�¶ȼ���
		gSrc[3] = gFanBuf[1];	// ��ȡ���ټ���
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		else
			gWT_EN &= 0xfe;
		gSrc[4] = gWindUdBuf[1];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// �����Զ��ڷ綨ʱˢ�¿���
		else
			gWT_EN &= 0xfb;
		gVerticalCnt = 0;
		gSrc[12] = gWindLrBuf[1];// ��ȡ���ҷ������
		gSrc[7] = gSleepBak;
		gSrc[8] = 0;					// ȡ��ǿ��
		gSrc[9] = 0;					// �رո���
		gIReturnDec = 0;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYDRY)
	{// ��ʪģʽ��
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		gKeyID = 0x06;
		if(gSrc[1]!=2)
		{
			gSrc[1] = 2;
			gAutoFanCnt = 0;
			gSrc[13] = 0;				// ȡ������༰��ʱ
			gWT_EN &= 0x7f;
		}
		else
		{
			if(gSrc[13])
				return;						// ��ǰģʽ����������趨ʱ������Ч
		}
TODRY:
		gSrc[2] = gTempBuf[2];// ��ȡ�¶ȼ���
		gSrc[3] = gFanBuf[2];	// ��ȡ���ټ���
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		else
			gWT_EN &= 0xfe;
		gSrc[4] = gWindUdBuf[2];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// �����Զ��ڷ綨ʱˢ�¿���
		else
			gWT_EN &= 0xfb;
		gVerticalCnt = 0;
		gSrc[12] = gWindLrBuf[2];// ��ȡ���ҷ������
		gSrc[7] = gSleepBak;
		gSrc[8] = 0;					// ȡ��ǿ��
		gSrc[9] = 0;					// �رո���
		gIReturnDec = 0;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYHEAT)
	{// ����ģʽ��
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		gKeyID = 0x06;
		if(gSrc[1]!=3)
		{
			gSrc[1] = 3;
			gAutoFanCnt = 0;
			gSrc[13] = 0;				// ȡ������༰��ʱ
			gWT_EN &= 0x7f;
		}
		else
		{
			if(gSrc[13])
				return;						// ��ǰģʽ����������趨ʱ������Ч
		}
TOHEAT:
		gSrc[2] = gTempBuf[3];// ��ȡ�¶ȼ���
		gSrc[3] = gFanBuf[3];	// ��ȡ���ټ���
		if(gSrc[3]==0)
			gWT_EN |= 0x01;
		else
			gWT_EN &= 0xfe;
		gSrc[4] = gWindUdBuf[3];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// �����Զ��ڷ綨ʱˢ�¿���
		else
			gWT_EN &= 0xfb;
		gVerticalCnt = 4;
		gSrc[12] = gWindLrBuf[3];// ��ȡ���ҷ������
		gSrc[7] = gSleepBak;
		gSrc[8] = 0;					// ȡ��ǿ��
		gSrc[9] = 1;					// �Զ��򿪸���
		gIReturnDec = 0;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYTEMPUP)
	{// �¶ȼӼ�
		if(gSrc[10])
			return;
		if((gSrc[0]==0)||(gSrc[1]==4))
			return;							// �ػ�״̬���ͷ�ģʽ�´˼���Ч
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		if(gSrc[1]==4)
			return;							// �ͷ�ģʽ�´˼���Ч
		gKeyID = 0x00;
		if(gSrc[6]|gHour|gMinute)
		{											// ��ʱ״̬�°���,�˳���ʱ״̬,�տ�ʼ�¶Ȳ��䲻����,����1���ʼ�仯
			TKCON &= 0x7f;			// �ر���˸
			gSrc[6] = 0;	
			gITimeSetDec = 0;
			if((gbHoldTempKey==0)&&(gIReturnDec==0))
			{
				gIReturnDec = 200;// ��ʱ��ʾ�л����¶���ʾ�ķ��ؼ�ʱ10��
				gISendDec = 25;
				gSendType = 0;		// ������
				return;
			}
			gIReturnDec = 200;	// ��ʱ��ʾ�л����¶���ʾ�ķ��ؼ�ʱ10��
		}
		tmp = gSrc[1];
		if(tmp==0)
		{
			if(gSrc[2]<13)
				gSrc[2] += 1;			// PMVģʽ���¶�����Ϊ29��
		}
		else
		{
			if(gSrc[2]<14)
				gSrc[2] += 1;			// ����ģʽ���¶�����Ϊ30��
		}
		gTempBuf[tmp] = gSrc[2];	// ����
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYTEMPDOWN)
	{// �¶ȼ���
		if(gSrc[10])
			return;
		if((gSrc[0]==0)||(gSrc[1]==4))
			return;							// �ػ�״̬���ͷ�ģʽ�´˼���Ч
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		if(gSrc[1]==4)
			return;							// �ͷ�ģʽ�´˼���Ч
		gKeyID = 0x01;
		if(gSrc[6]|gHour|gMinute)
		{											// ��ʱ״̬�°���,�˳���ʱ״̬,�տ�ʼ�¶Ȳ��䲻����,����1���ʼ�仯
			TKCON &= 0x7f;			// �ر���˸
			gSrc[6] = 0;	
			gITimeSetDec = 0;
			if((gbHoldTempKey==0)&&(gIReturnDec==0))
			{
				gIReturnDec = 200;// ��ʱ��ʾ�л����¶���ʾ�ķ��ؼ�ʱ10��
				gISendDec = 25;		// ������
				gSendType = 0;
				return;
			}
			gIReturnDec = 200;	// ��ʱ��ʾ�л����¶���ʾ�ķ��ؼ�ʱ10��
		}
		tmp = gSrc[1];
		if(tmp==0)
		{
			if(gSrc[2]>7)
				gSrc[2] -= 1;			// PMVģʽ���¶�����Ϊ23��
		}
		else
		{
			if(gSrc[2]>0)
				gSrc[2] -= 1;			// ����ģʽ���¶�����Ϊ16��
		}
		gTempBuf[tmp] = gSrc[2];	// ����
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYFANSPEED)
	{// ���ټ�
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x04;
		if(gSrc[0]==0)
		{
			if(gSrc[5]==0)
				return;						// �ػ��Ҿ����ر�״̬�´˼���Ч
			if(gSrc[3]==1)			// �ػ�����ʱ����ֻ�и�/�������ɵ�
				gSrc[3] = 3;
			else
				gSrc[3] = 1;
			gISendDec = 0;			// ��������
			gSendType = 1;
			return;
		}
		tmp = gSrc[1];
		gSrc[3] += 1;
		if(gSrc[3]==4)
		{
			if(tmp==4)
				gSrc[3] = 1;			// ѭ��������->��(�ͷ�ģʽ��û���Զ�)
			else
				gSrc[3] = 0;			// ѭ��������->�Զ�(�ͷ�ģʽ��Ϊ��)
		}
		gFanBuf[tmp] = gSrc[3];	// ����
		if(gSrc[3]==0)
		{
			gWT_EN |= 0x01;			// �Զ����ٶ�ʱˢ�¿���
			gDispData[13] &= 0xfc;
			gDispData[14] &= 0xfc;
			LCDM13 &= 0xfc;
			LCDM14 &= 0xfc;
		}
		else
			gWT_EN &= 0xfe;
		gAutoFanCnt = 0;
		gSrc[8] = 0;					// ȡ��ǿ��
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYWINDVERT)
	{// ���·����
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		if((gSrc[4]==2)||(gSrc[4]==3))
			return;							// ��������ʱ���·��򲻿ɵ�
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x02;
		if(gSrc[4]==0)
			gSrc[4] = 1;
		else
			gSrc[4] = 0;
		tmp = gSrc[1];
		gWindUdBuf[tmp] = gSrc[4];
		if(gSrc[4]==0)
			gWT_EN |= 0x04;			// �����Զ��ڷ綨ʱˢ�¿���
		else
			gWT_EN &= 0xfb;
		if(gSrc[1]==3)
			gVerticalCnt = 4;		// ����ģʽ�������Զ��ڷ����ʼλ������
		else
			gVerticalCnt = 0;		// ����ģʽ�������Զ��ڷ����ʼλ����һ
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYWINDHORIZ)
	{// ���ҷ����
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x03;
		gSrc[12] += 1;
		if(gSrc[12]==6)
			gSrc[12] = 0;				// ȫ��->λ��һ
		tmp = gSrc[1];
		gWindLrBuf[tmp] = gSrc[12];
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYAUXHEAT)
	{// ���ȼ�
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		if((gSrc[1]!=0)&&(gSrc[1]!=3))
			return;							// ֻ��PMV������ģʽ����Ч
		gKeyID = 0x16;
		gSrc[9] ^= 0x01;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYPURIFY)
	{// ������(����)
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x07;
		gSrc[5] ^= 0x01;
//		if(gSrc[5])         //versions 1.1  ��Ե�������������޸�01
//			gSrc[3] = 1;
//		else
//		{
//			tmp = gSrc[1];
//			gSrc[3] = gFanBuf[tmp];
//		}
		if(gSrc[0]==0)
		{
		if(gSrc[5])         //versions 1.1 ��Ե�������������޸�02���ٹػ�״̬�ͷ�
			gSrc[3] = 1;
		else
		{
			tmp = gSrc[1];
			gSrc[3] = gFanBuf[tmp];
		}
			gSrc[14] = gSrc[5];	// �ػ�״̬,�龰�ƹ��澻��ͬʱ��/�� 
		}
		else
		{
			if( gSrc[5])
			{
		     gSrc[14] = gSrc[5];	// ����״̬,���������龰�ƹ⿪  versions 1.1
			}
		}
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYSLEEP)
	{// ˯�߼�
		if(gSrc[10])
			return;
		if((gSrc[0]==0)||(gSrc[1]==4))
			return;							// �ػ�״̬���ͷ�ģʽ�´˼���Ч
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч	
		gKeyID = 0x0b;
		gSrc[7] ^= 0x01;
		if(gSrc[1]!=0)
			gSleepBak = gSrc[7];// PMV���趨��˯�߲���������ģʽ
		if(gSrc[7])
		{
			gWT_EN |= 0x40;
			gSpSecond = 0;
			gSpMinute = 0;
			gSpHour = 8;				// ˯��8Сʱ��ʱ�ػ�
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
			gSrc[8] = 0;				// ˯��ģʽ��û��ǿ����,ֻ�����޺Ͱ���֮���л�
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	/*else if(gkey_value == KEYAIRFLOW)
	{// ����������
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x02;
		if(gSrc[4]==2)
			gSrc[4] = 3;				// �ϴ�->�´�
		else if(gSrc[4]==3)
		{											// �´�->��׼λ��
			if(gSrc[1]==3)
				gSrc[4] = 5;
			else
				gSrc[4] = 4;
		}
		else
			gSrc[4] = 2;				// ��һ�ΰ����׼λ��->�ϴ�
		tmp = gSrc[1];
		gWindUdBuf[tmp] = gSrc[4];
		gISendDec = 0;				// ��������
		gSendType = 1;
	}*/
	else if(gkey_value == KEYTURBO)
	{// ǿ����
		if(gSrc[10])
			return;
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		if((gSrc[1]==0)||(gSrc[1]==2)||(gSrc[1]==4))
			return;							// PMV����ʪ���ͷ�ģʽ�°�����Ч
		gKeyID = 0x08;
		if(gSrc[7])
		{											// ˯��ģʽ��û��ǿ����,ֻ�����޺Ͱ���֮���л�
			if(gSrc[8])
				gSrc[8] = 0;
			else
				gSrc[8] = 2;
		}
		else
		{											// ��->ǿ��->����->��
			gSrc[8] += 1;
			if(gSrc[8]==3)
				gSrc[8] = 0;
		}
		if(gSrc[8])
		{
			if(gSrc[3]!=0)
			{
				gWT_EN |= 0x01;			// ǿ���򰲾�ʱ������ʾ��Ϊ�Զ�,��ʱˢ�¿���
				if((gSrc[7]!=0)||(gSrc[8]!=2))
					gAutoFanCnt = 0;
			}
		}
		else
		{
			if(gSrc[3]!=0)
				gWT_EN &= 0xfe;		// �ָ�ԭ������ʾ
		}
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYTIMING)
	{// ��ʱѡ���
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x0d;
		if((gSrc[6]==0)||(gITimeSetDec==0))
		{
			if(gSrc[0])					// �ս��붨ʱ��/��״̬ʱ������ʼλ��
			{
				gSrc[6] = gTimeOffBak;
				TKADDR = 0x65;		// ѡ����˸��:��ʱ�ر�־
			}
			else
			{
				gSrc[6] = gTimeOnBak;
				TKADDR = 0x05;		// ѡ����˸��:��ʱ����־
			}
			TKCON = 0x84;				// ����������˸,Ƶ��1Hz
		}
		else
		{	
			if(gSrc[6]>=24)
				gSrc[6] += 2;			// 12Сʱ��ʼÿ��һ�μ�һСʱ
			else
				gSrc[6] += 1;			// 12Сʱ��ʼÿ��һ�μ�һСʱ
			if(gSrc[6]==50)
				gSrc[6] = 1;			// 24Сʱ->0.5Сʱ
		}
		gIReturnDec = 0;
		gITimeSetDec = 200;		// ��ʱ����״̬��ʱ10��
		gISendDec = 0;
		gSendType = 0;				// ������
	}
	else if(gkey_value == KEYOKCANCEL)
	{// ȷ��/ȡ����
		if(gSrc[10])
			return;
		if((gSrc[6]|gHour|gMinute)==0)
			return;							// �޶�ʱʱ�˼���Ч
		gKeyID = 0x10;
		if(gITimeSetDec)
		{// ȷ��
			TKCON &= 0x7f;			// �ر���˸
			gWT_EN |= 0x20;			// ��ʱ��/�ػ�ʱ�俪ʼ�ݼ�
			if(gSrc[0])
				gTimeOffBak = gSrc[6];	// ������Ϊ��ʼλ��
			else
			{
				gTimeOnBak = gSrc[6];
				gSrc[4] = 1;
			}
			tmp = gSrc[6];
			gSecond = 0;
			gHour = tmp/2;			// ����ʱʱ����ʼֵ�����Сʱ�ͷ���
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
		{// ȡ��
			if(gSrc[0]==0)
				gSrc[4] = 1;			// �ػ�״̬���޶�ʱʱ���·���Ϊ���¹̶�
			gWT_EN &= 0xdf;			// ��ʱ��/�ػ�ʱ��ֹͣ�ݼ�
			gSecond = 0;
			gMinute = 0;
			gHour = 0;
		}
		gSrc[6] = 0;
		gITimeSetDec = 0;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYLIGHT)
	{// �ƹ��
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x15;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYCLEAN)
	{// ������
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		if(gSrc[10])
			return;
		if((gSrc[0]==1)&&(gSrc[1]==0))
			return;							// ������ΪPMVģʽʱ������Ч
		if(gHour|gMinute)
			return;							// ��ʱ��Чʱ������Ч
		if(gSrc[7])
			return;							// �����°�����Ч
		gKeyID = 0x19;
		gSrc[13] = 0x01;
		gWT_EN |= 0x80;				// ��ʱ21����
		gClSecond = 0;
		gClMinute = 21;
		gSrc[6] = 0x00;				// �˳���ʱ����״̬
		gITimeSetDec = 0;
		TKCON &= 0x7f;				// �ر���˸
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYSCENELGT)
	{// �龰�ƹ��
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x0c;
		gSrc[14] ^= 0x01;
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYDIRWIND)
	{// ��λ�����
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		if(gSrc[0]==0)
			return;							// �ػ�״̬�´˼���Ч
		gKeyID = 0x02;
		gSrc[15] += 1;
		if(gSrc[15]==4)
			gSrc[15] = 0;				// λ��3->��
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYLOCK)
	{// ������
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		gKeyID = 0x14;
		gSrc[10] ^= 0x01;
		gISendDec = 0;				// ��������
		gSendType = 4;
	}
	/*else if(gkey_value == KEYTDNSET)
	{// �¶ȼ�+ȷ��/ȡ��
		if(gSrc[10])
			return;
		if((gSrc[0]!=1)||(gSrc[1]!=1)||(gSrc[2]!=0)||(gSrc[3]!=3)||(gSrc[8]!=0))
			return;							// ����״̬+����ģʽ+16��+�߷�+��ǿ��ʱ����Ч
		gKeyID = 0x01;
		gSrc[11] = 1;					// ��Ƶ��ת
		gISendDec = 0;				// ��������
		gSendType = 1;
	}
	else if(gkey_value == KEYTUPSET)
	{// �¶ȼ�+ȷ��/ȡ��
		if(gSrc[10])
			return;
		if(gSrc[13])
			return;							// ��������趨ʱ������Ч
		if((gSrc[0]!=1)||(gSrc[1]!=3)||(gSrc[2]!=14)||(gSrc[3]!=3)||(gSrc[8]!=0))
			return;							// ����״̬+����ģʽ+30��+�߷�+��ǿ��ʱ����Ч
		gKeyID = 0x01;
		gSrc[11] = 1;					// ��Ƶ��ת
		gISendDec = 0;				// ��������
		gSendType = 1;
	}*/
}

//===========================================================================
// ���ƣ�GetKey
// ���ܣ�����ɨ��
// ���룺gbIKeyFlag(�����жϷ�����򱻸�ֵ,�а�����gbIKeyFlag=1)
// �����gkey_mode(����������)
//		 gkey_value(����)
// ���ã�keyscan,key_compare,key_convert
// ��ջ��
// Ӱ�죺
// �������ú���������ѭ���б����á�
//       gkey_modeֵ���壺
//       #define KM_NONE         0   // �޼�
//       #define KM_IMME_DOWN    1   // �հ���
//       #define KM_IMME_UP      2   // �̰�̧��
//       #define KM_SHORT_DOWN   3   // ���¸պ�1.5��
//       #define KM_LONG_DOWN    4   // ����δ̧��
//       #define KM_LONG_UP      5   // ����̧��
//   ����˵��:
//   1) �������ģ����Ҫ��keyscan(),key_compare(),key_convert()����������ɡ�
//   2) ����͵���ɨ������ļ���gkey_value��ͬ���Դ������֡�
//   3) �м�����ʱ�Ż���а���ɨ�裬ֱ������̧���޼�ʱ�ް���ɨ�貨�Ρ�
//   4) �����¹����У������뷢���仯(�������ǰ���KEYA,���ְ���KEYB��������ͬ
//      ʱ����KEYA & KEYB,Ȼ���ַſ���һ����KEYA),��gkey_modeֵ���ǰ��յ�һ��
//      �Ȱ��µ��Ǹ�������,�����Ȱ���KEYA����2����ٰ���KEYB����ʱgkey_mode=4��
//===========================================================================
void GetKey(void)
{
	if(gbIKeyFlag)
	{// �а����жϲſ�ʼɨ��
		gKeyCode = keyscan();   // ����ɨ��
		key_compare();          // ����ɨ���Ĵ���(ȥ����,����ʱ�䳤�ȼ���)
		key_convert();          // ת����ֱ�۵İ�������,�õ�gkey_value,gkey_mode
	}
}

//===========================================================================
// ���ƣ�keyscan
// ���ܣ�����ɨ��
// ������
// ���أ�����ɨ�����
// ���ã���
// Ƕ�׼�����
// Ӱ�죺
// ������5x4�������
//       P1�õ��Ķ˿�����(�ڲ�������)
//===========================================================================		
U8 keyscan(void)
{
	U8 i,j,k=0;
	U16 result=0x0000;
	
	ClrWDT();
	EIE2 &= 0xbf;   			// ɨ��ǰ��ֹ�����ж�
    
	P2DPL = 0xff;
	for(i=0;i<4;i++)			// �������������͵�ƽ:P20->P21->P22->P23
	{
		switch(i)
		{
			case 0:	P2 = 0x0e;break;
			case 1:	P2 = 0x0d;break;
			case 2:	P2 = 0x0b;break;
			default:P2 = 0x07;break;
		}
		delay(24);// Լ100us
		if((P1&0xf8)!=0xf8)
		{
			for(j=0;j<5;j++)	// ����ɨ�������KI3-KI7
			{
				if((P1&(0x08<<j)) == 0)
				{
					k++;
					if(k>=3)	// �������,��Ч����
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
				return KEYTDNSET;	// �Ȱ����¶�-,�ٰ���ȷ��/ȡ��
			else if(result==0x2341)
				return KEYTUPSET;	// �Ȱ����¶�+,�ٰ���ȷ��/ȡ��
			else if(result==0x2334)
				return KEYLOCK;		// �����¶�+���¶�-
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
// ���ƣ�key_compare
// ���ܣ������м䴦��ȥ������; �õ�gkey_sc_valueֵ��gkey_pressֵ
// ������
// ���أ���
// ���ã���
// Ƕ�׼�����
// Ӱ�죺gKeyCode��gkey_sc_ct��gbkey_exist��gkey_sc_value��gkey_press
//===========================================================================
void key_compare(void)
{
    if(gKeyCode != gkey_lt_value)    // �����ϴ�ɨ��ֵ��ͬ
    {
        gkey_lt_value = gKeyCode;
        gkey_sc_ct = KEYSC;
        return;
    }
    else
    {
        if(gkey_sc_ct != 0)         // ȥ����ʱ��
        {
            gkey_sc_ct--;
            return;
        }
        else
        {
            gbkey_exist = 1;
            gkey_sc_ct = KEYSC;
            if(gKeyCode == gkey_sc_value)
            {// ����һ��ɨ��ļ�ֵ��ͬ
                if(gkey_ct != 0) gkey_ct--;          // ��������
            }
            else
            {// ����һ��ɨ��ļ�ֵ��ͬ
                gkey_sc_value = gKeyCode; // ����˴μ�ֵ
                if(gKeyCode != K_NONE)      // ���˴��޼����򲻸��°��¼�ֵ
                {
                    gkey_press = gKeyCode;
                    if(gkey_last == K_NONE)
                    {// ԭ���޼�
                        gkey_ct = KEYCT;
                    }
                }
            }
        }
    }    
}

//===========================================================================
// ���ƣ�key_convert
// ���ܣ�����尴��ɨ�������Ч��ֵ�����ֱ�۵ļ��Ķ���
// ���룺gbkey_exist,gkey_sc_value,gkey_ct,gkey_press
// �����gkey_mode(����������),gkey_value(����)
//       gkey_mode=0~5����[����]����
// ���ã�
// ��ջ��
// Ӱ�죺gbkey_exist,gkey_sc_value,gkey_ct,gkey_press,gkey_last;
//       ������һ��ʼ������gkey_mode,gkey_value.
// ������
//       #define KM_NONE         0   // �޼�
//       #define KM_IMME_DOWN    1   // �հ���
//       #define KM_IMME_UP      2   // �̰�̧��
//       #define KM_SHORT_DOWN   3   // ���¸պ�1.5��
//       #define KM_LONG_DOWN    4   // ����δ̧��
//       #define KM_LONG_UP      5   // ����̧��
//===========================================================================
void key_convert(void)
{	
    if(!gbkey_exist) return;
    
    gbkey_exist = 0;
    if(gkey_sc_value == K_NONE) // ��ǰ�޼�
    {
				gbPauseTK = 0;
        if(gkey_last == K_NONE)    // ��ǰҲ�޼�
        {
            gkey_mode = KM_NONE;
            gkey_value = K_NONE;
        }
        else    // ��ǰ�޼�����ǰ�м�
        {
            if(gkey_ct>=SHORTRELEASE)
            {
                gkey_mode = KM_IMME_UP;		// �̰�̧��
            }
            else
            {
                gkey_mode = KM_LONG_UP;		// ����̧��
            }
            gISendDec = 0;					// ��������
            gkey_value = gkey_press;
            gkey_press = 0;
            gkey_last = 0;
            gbIKeyFlag = 0;
            EIE2 |= 0x40;           // ����KBI�ж�
        }
    }
    else    // ��ǰ�м�
    {
        if(gkey_last == K_NONE)    // ԭ���޼����հ���
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
            if(gkey_press == gkey_last) // ����ʱ������û�����仯
            {
                if(gkey_ct == SHORTDOWN)
                {
                    gkey_mode = KM_SHORT_DOWN;  // ���¸պ�1.5��
                    gkey_ct--;      // ��ֻ֤����һ��
                }
                else if(gkey_ct <= LONGDOWN)
                {
                    gkey_mode = KM_LONG_DOWN;  // �����³���1.5��
										delay(24000);
                }
                gkey_value = gkey_sc_value; 
            }
            else    // ���¹����У����������˱仯
            {                
                //if(gkey_ct == KEYCT)
                {// 50ms���ڰ��հ��´���
                    if(gKey_Prev==0)
                    {
                        gKey_Prev = gkey_last;   // �����һ�α仯ǰ�İ���ֵ
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
                    gkey_ct = KEYCT;    // ���������������´���
                 }
                //else
                //{
                //    gkey_value = K_NONE;
                //    gkey_mode = KM_NONE;
                //}                                   
                gkey_press = gkey_sc_value;        
                gkey_last = gkey_sc_value;     
                gbKeyChanged = 1;           // ����ֵ�����仯�ı�־
								gISendDec = 0;
								gSendType = 0;                
            }
        }
    }
}
