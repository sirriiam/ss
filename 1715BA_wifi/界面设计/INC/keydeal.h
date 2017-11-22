
#ifndef _KEYDEAL_H_
#define _KEYDEAL_H_

#define K_NONE 0
#define K_INVALID 0
#define NULL 0

bit gbFirstKey;						// 1=�׼�Ϊ�¶ȼ�
bit	gbHoldTempKey;				// �¶ȼ�/����ʱ������δ̧�𳬹�1��
U8 xdata gKeyID;				// ����ʶ���룬���ڷ�������
U8 xdata gKeyCode;				// ��ɨ��ֵ	

bit gbkey_exist;                // �а���ɨ�赽��,=1
bit gbKeyChanged;               // ���������У��������޷����ı�	
U8 xdata gkey_sc_ct;			// ȥ��������
U8 xdata gkey_lt_value;			// ��һ��ɨ��ֵ
U8 xdata gkey_sc_value;			// ��һ�α���ļ�ֵ
U8 xdata gkey_ct;				// ��������
U8 xdata gkey_press;			// ���µļ�ֵ
U8 xdata gkey_mode;				// ��ģʽ-->�������̰�...
U8 xdata gkey_value;			// ��ֵ
U8 xdata gkey_last;				// ��һ�μ�ֵ
U8 xdata gKey_Prev;             // ����ֵ�仯ǰ��ֵ
U8 xdata gKeyModeSc;

extern bit gbPauseTK;						// ��ʱ��־��˸��ͣ��־
extern bit gbKeyTempSpec;				// �����¶ȼ���˫/�����Ϊ�¶ȼ�,����ȴ������ͷź������������
extern bit gbIKeyFlag;          // �а������±�־,=1�а���
extern U8 gTempBuf[5];			// ��ģʽ�¶ȼ�����
extern U8 gFanBuf[5];			// ��ģʽ���ټ�����
extern U8 gWindUdBuf[5];				// ��ģʽ���·��������
extern U8 gWindLrBuf[5];				// ��ģʽ���ҷ��������
extern U8 gSleepBak;
extern U8 gTimeOnBak;
extern U8 gTimeOffBak;
extern U8 gWT_EN;
extern U8 gAutoFanCnt;
extern U8 gVerticalCnt;
extern U8 xdata gSecond;
extern U8 xdata gMinute;
extern U8 xdata gHour;
extern U8 xdata gSpSecond;
extern U8 xdata gSpMinute;
extern U8 xdata gSpHour;
extern U8 xdata gClSecond;
extern U8 xdata gClMinute;
extern U8 xdata gDispData[32];
extern U8 idata gSendData[32];	// ��������
extern U8 gISendDec;	        // �������º����ٶ೤ʱ���ŷ���

extern void GetCode(void);
extern void RCSend(void);
extern void display(void);
extern void TailControl(void);

void GetKey(void);
U8 keyscan(void);
void key_compare(void);
void key_convert(void);
void KeyDeal(void);
void UserKeyDeal(void);   		// �û���������
//void ACDeal(void);

#endif