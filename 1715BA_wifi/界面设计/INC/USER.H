
#ifndef _USER_H_
#define _USER_H_

// 仿真调试
#define DEBUG						1					// 0-关闭,1-开启

// 按键扫描参数
#define K_NONE	        0					// 无键按下
#define KEYSC	        	4					// 按键去抖动时间,4-->20ms,3-->15ms,2-->10ms
#define KEYSCDLY        8					// 双键扫描时,去抖动时间
#define KEYCT	        	60				// 按键动作最大判断时间(KEYCT/20)秒
#define SHORTRELEASE	(KEYCT-30)	// 短按抬起
#define SHORTDOWN	    (KEYCT-15)	// 按下刚好0.6秒
#define LONGDOWN	    (KEYCT-22)	// 长按未抬起超过1秒
#define LONGHOLD	    (KEYCT-38)	// 长按未抬起达到5秒

#define KM_NONE         0   	// 无按键动作
#define KM_IMME_DOWN    1   	// 刚按下
#define KM_IMME_UP      2   	// 短按抬起
#define KM_SHORT_DOWN   3   	// 按下刚好0.6秒
#define KM_LONG_DOWN    4   	// 长按超过1秒
#define KM_LONG_UP      5   	// 长按抬起

// 按键位置定义(即扫描得到的键值)
// 高4位为输出口序号(1~4),低4位为输入口序号(1~5)
#define KEYONOFF				0x45	// 开关机键
#define KEYPMV          0x25	// PMV模式
#define KEYMODE	      	0x15	// 模式
#define KEYSCENELGT     0x24	// 情景灯光
#define KEYCLEAN        0x44	// 自清洁
#define KEYTEMPUP     	0x23	// 温度加
#define KEYTEMPDOWN     0x34	// 温度减
#define KEYFANSPEED     0x35	// 风速
#define KEYWINDVERT     0x43	// 上下风向
#define KEYWINDHORIZ    0x14	// 左右风向
#define KEYAUXHEAT			0x33	// 辅热
#define KEYPURIFY 			0x13	// 净化/健康
//#define KEYAIRFLOW			0x42	// 健康气流
#define KEYDIRWIND			0x42	// 定位出风
#define KEYSLEEP      	0x12	// 睡眠
#define KEYTIMING				0x32	// 定时
#define KEYTURBO				0x41	// 强力
#define KEYLIGHT				0x11	// 屏显(灯光)
#define KEYOKCANCEL			0x31	// 确定/取消

#define KEYCOOL         0xEE	// 制冷模式(不存在)
#define KEYHEAT         0xEE	// 制热模式(不存在)
#define KEYDRY          0xEE	// 除湿模式(不存在)

#define KEYTDNSET				0x51	// 温度减+确认/取消
#define KEYTUPSET				0x52	// 温度加+确认/取消
#define KEYLOCK					0x53	// 锁定

/*// 定时步进
#define TMDECSTEP       19		// 19表示1小时递减一次,0表示每3分钟递减一次
#define TMDECSTEP				1			// 10表示半小时递减一次,0表示每3分钟递减一次
#define TMDECSTEP1      10		// 10表示半小时递减一次,0表示每3分钟递减一次,超强半小时自动返回*/

/*// 定时设置无按键后退出时间
#define EXITCOUNT       100*/

// 无按键后进入IDLE模式的时间
//#define TIMETOIDLE      60    // 3秒(需要大于发码时间长度)
#define TIMETOIDLE      20		// 1秒(需要大于发码时间长度)

/*// 定时递减
#define TM60  60
*/

#endif