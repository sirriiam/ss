
extern U8 xdata gkey_mode;
extern U8 xdata gkey_value;
extern U8 xdata gKeyCode;
extern U8 xdata gkey_mode;          // 键模式-->长按，短按...

extern bit gbI5ms; 			        // 5ms循环标志
extern bit gbIKeyFlag;              // 按键中断置起的标志

extern void SysInit(void);
extern void GetKey(void);
//extern U8 keyscan(void);
//extern void key_compare(void);
//extern void key_convert(void);
extern void display(void);
extern void KeyDeal(void);
extern void TailControl(void);
extern void ACDeal(void);
extern bit IdleModeDeal(void);
extern void ClrWDT(void);
extern void timeupdate(void);