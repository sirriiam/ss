
#ifndef _INITIAL_H_
#define _INITIAL_H_

U8 idata gSrc[50] _at_ 0x80;		// �յ�����Դ��
//gSrc[0]�����ػ�,0=�ػ�״̬,1=����״̬
//gSrc[1]��ģʽ,0=PMV(�Զ�),1=����,2=��ʪ,3=����,4=�ͷ�
//gSrc[2]���¶�,0=16��......14=30��
//gSrc[3]������,0=�Զ�,1=��,2=��,3=��
//gSrc[4]�����·���,0=�Զ���,1=���¹̶�,2=���������ϴ�,3=���������´�,4=���°�λ��һ,5=���°�λ����
//gSrc[5]������(����)��0=�ޣ�1=��
//gSrc[6]����ʱ�趨,0=�޶�ʱ,1=0.5Сʱ,2=1Сʱ......23=11.5Сʱ,24=12Сʱ,26=13Сʱ......48=24Сʱ
//gSrc[7]��˯��,0=��,1=��
//gSrc[8]��ǿ��,0=��,1=ǿ��,2=����
//gSrc[9]������,0=��,1=��
//gSrc[10]������,0=��,1=��
//gSrc[11]����Ƶ��ת,0=��,1=��
//gSrc[12]: ���ҷ���,0=λ��1,1=λ��4,2=λ��5,3=λ��6,4=λ��7,5=�Զ���
//gSrc[13]�������,0=��,1=��
//gSrc[14]���龰�ƹ�,0=��,1=��
//gSrc[15]����λ����,0=��,1=λ��1,2=λ��2,3=λ��3

U8 idata gSendData[32] _at_ 0xb2;	// ��������

U8 gTempBuf[5];						// ��ģʽ�¶ȼ�����
U8 gFanBuf[5];						// ��ģʽ���ټ�����
U8 gWindUdBuf[5];					// ��ģʽ���·��������
U8 gWindLrBuf[5];					// ��ģʽ���ҷ��������
U8 gSleepBak;
U8 gTimeOnBak;
U8 gTimeOffBak;
U8 gSendType;							// 0=������,1=����
U8 gWT_EN;
U8 gAutoFanCnt;
U8 gVerticalCnt;
U8 xdata gSecond;
U8 xdata gMinute;
U8 xdata gHour;
U8 xdata gSpSecond;
U8 xdata gSpMinute;
U8 xdata gSpHour;
U8 xdata gClSecond;
U8 xdata gClMinute;
bit gbTimeOut;						// ��ʱ���ó�ʱ�˳���־
bit gbPauseTK;						// ��ʱ��־��˸��ͣ��־
bit gbKeyTempSpec;				// �����¶ȼ���˫/�����Ϊ�¶ȼ�,����ȴ������ͷź������������

extern void Init_Close_RTC(void);
extern void DispNum(U8 num,U8 n);
extern void disptolcd(void);
void delay(U16 tcnt);
//void initial(void);

#endif