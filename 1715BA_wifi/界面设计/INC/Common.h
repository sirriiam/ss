#ifndef _COMMON_H_
#define _COMMON_H_

#define ERR 	1
#define NULL	0

// bitΪ����ʱû��ֱ������λ,���Ǳ������Ӧ���ܵ�ָ��
#define bitset(var,bit) ((var)|=(1<<(bit)))    // ��ĳһλΪ��
#define bitclr(var,bit) ((var)&=~(1<<(bit)))   // ��ĳһλΪ��
#define bitsz(var,bit) ((var)&(1<<(bit)))      // ����ĳһλ�Ƿ�Ϊ��

extern U8 idata gSrc[50];    		// �յ�����Դ��
extern U8 idata gSendData[32];      // ��������

// ��������
extern bit gbIKeyFlag;
extern U8 xdata gKeyCode;			// ��ɨ��ֵ
extern U8 xdata gkey_sc_ct;			// ȥ��������
extern U8 xdata gkey_lt_value;		// ��һ��ɨ��ֵ
extern U8 xdata gkey_sc_value;		// ��һ�α���ļ�ֵ
extern U8 xdata gkey_ct;			// ��������
extern U8 xdata gkey_press;			// ���µļ�ֵ
extern U8 xdata gkey_mode;			// ��ģʽ-->�������̰�...
extern U8 xdata gkey_value;			// ��ֵ
extern U8 xdata gkey_last;			// ��һ�μ�ֵ

extern U8 gSendType;				// 0=�����룬1=����ͨ�����룬2=����ʱ�趨/ȡ��������
extern U8 gIWorkDec;
extern U8 gITimeSetDec;
extern U8 gIReturnDec;    	// ��ʱ��ʾ����ʱ�䳤��
//extern U8 gInt_50ms;

extern void ClrWDT(void);
extern void delay(U16 tcnt);

#endif