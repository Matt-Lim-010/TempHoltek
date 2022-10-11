/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      function.h
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     功能函数头文件
 ******************************************************************************/
#ifndef __FUNCTION_H
#define __FUNCTION_H
/*******************************************************************************
 * 宏定义
 ******************************************************************************/
//LED控制
#define LED_L1_ON				(0x01)		//LED1开-通信
#define LED_L2_ON				(0x02)		//LED2开-通信
#define LED_L1_CTRL				(0x04)		//LED1开-控制命令
#define LED_L2_CTRL				(0x08)		//LED2开-控制命令
#define LED_L1_BLINK			(0x10)		//LED1闪烁命令
#define LED_L2_BLINK			(0x20)		//LED2闪烁命令
#define LED_BLINK_ST			(0x80)		//闪烁状态
//蜂鸣器控制
#define BUZZER_ON				(0x01)		//开指令
#define BUZZER_IS_ON			(0x81)		//已经开启
#define BUZZER_IS_OFF			(0x80)		//关指令
//全局状态标志
#define FUNC_ST_ALL				(0x7F7F)	//全部状态标志
#define FUNC_ST_ENABLE			(0x4000)	//事件报出使能
#define FUNC_ST_MONITOR			(0x8000)	//监控状态使能
#define FUNC_ST_CONTROLLABLE    (0x2000)    //可输出命令
#define FUNC_ST_LOOPPWR_LOW     (0x0080)    //回路低电压
#define FUNC_ST_LOOPPWR_WAVE    (0x0002)    //电源波动
//------------------------------------------------------------------------------
//控制指令
#define FUNC_CTRL_AV_ON			(0xB1)	//声光控制开
#define FUNC_CTRL_AV_OFF        (0xB2)  //声光控制关

#define FUNC_CTRL_AV_V_ON		(0xD1)	//光警控制开
#define FUNC_CTRL_AV_V_OFF      (0xD2)  //光警控制关

#define FUNC_CTRL_AV_A_ON		(0xC1)	//声警控制开
#define FUNC_CTRL_AV_A_OFF      (0xC2)  //声警控制关
//控制命令占一个位指示
#define FUNC_CTRL_AV_BIT_SOUND   (0x01) //控制命令 AV sound on
#define FUNC_CTRL_AV_BIT_LIGHT   (0x02) //控制命令 AV light on
#define FUNC_CTRL_AV_BIT_AV      (0x03) //控制命令 AV AV  ON
//-----------------------------------------------------------------------------
//Interface 输出命令
#define FUNC_CTRL_INF_OUTON 	(0xD1)  //Interface 输出
#define FUNC_CTRL_INF_OUTOFF    (0xD2)  //Interface 停止输出

#define FUNC_CTRL_RELAY_ON		(0xD1)	//输出继电器
#define FUNC_CTRL_RELAY_OFF     (0xD2)  //关闭输出

//脉冲型充电命令 修改为工厂测试专用命令
#define FUNC_CTRL_RELAY_CAPON   (0xE1) //脉冲型输出充电命令
#define FUNC_CTRL_RELAY_CAPOFF  (0xE0) //脉冲型输出停止充电
//-----------------------------------------------------------------------------
//感烟SD 以及带buzzer的感烟探测器
//感烟探测器的状态 
#define FUNC_SD_ST_ALARM       					(0x0001)//二线制新的报警格式
#define FUNC_SD_ST_HIGHCHAM_FAULT				(0x0004)//二线制新的high chamber fault
#define FUNC_SD_ST_LOWCHAM_FAULT                (0x0008)//二线制新的low chamber fault
#define FUNC_SD_ST_FAULT                        (0x000C)//SD 故障
#define FUNC_SD_ST_DUST_ALARM                   (0x0010)//灰尘报警
#define FUNC_SD_ST_REMOTTEST_ON                 (2)//(0x0400) //远程测试
#define FUNC_SD_ST_REMOTTEST_OFF                (1)
#define FUNC_SD_ST_BUZZERON                     (0x0200) //带蜂鸣器感烟发出报警音
#define FUNC_SD_ST_NO_PW5                     	(0x0100) //PW5不返回
#define FUNC_SD_ST_DLIP_LED                     (0x0800) // DLIP LED闪烁
#define FUNC_SD_ST_COMPENSATION                 (0x1000) // 温度补偿

//暗值阈值
#define FUNC_SD_DATA_LEN						(15U)		//历史数据长度15*4s=60s
#define FUNC_SD_TH_DARK_H						(1280U)		//暗电流上限阀值ADC[0-255]
#define FUNC_SD_TH_DARK_L						(80U)		//暗电流下限阀值ADC[0-255]
//升烟类型判定		
#define FUNC_SD_SMOKETYPE_SMOKETEST    			(0U)   //升烟类型为烟测
#define FUNC_SD_SMOKETYPE_TRUEFIRE      		(1U)   //升烟类型为真火试验
//蓝光/红光的比例阈值
#ifdef CODE_SEL_SMOKE_LIGHTON
#define BLUE_IR_RADIO_TH        				(24u) //蓝光/红光的比例阈值
#endif
//hengrun 的比值偏大2个值
#ifdef CODE_SEL_SMOKE_HENGRUN
#define BLUE_IR_RADIO_TH        				(26u)  //蓝光/红光的比例阈值
#endif

#define RISETIME_REC_64SEC                      (18u)   //连续监视上升段36秒数据


#define FUNC_SD_OUTREG_DISALARMTEST     		(0xD2)
#define FUNC_SD_OUTREG_ENALARMTEST      		(0xD1)
//脏污阈值
#define FUNC_SD_DUSTALARM_TH 					(35u)   //减掉暗值后 0.1db/m对应的红外之
//慢速漂移补偿

#if 1
//16小时
#define FUNC_SD_SLOWSHIFT_16HOURCNT             ((28800U-1U))  //2sec运行1次 16小时 (28800U)
#define FUNC_SD_SLOWSHIFT_15MINCNT              (450U)    //2sec运行1次 15min 450
#define FUNC_SD_SLOWSHIFT_SHIFT                 (6U)      //共计累加64次，求平均值时右移6位
#define FUNC_SD_SLOWSHIFT_ERRTH					(50U)	  //慢速漂移中采集到异常值
#endif

#if 0
//20sec运行一次
//共计64次 1280sec  共计21.3min
#define FUNC_SD_SLOWSHIFT_16HOURCNT             ((640U-1U))  //2sec运行1次 16小时 (28800U)
#define FUNC_SD_SLOWSHIFT_15MINCNT              (10U)    	//2sec运行1次 15min 450
#define FUNC_SD_SLOWSHIFT_SHIFT                 (6U)      	//共计累加64次，求平均值时右移6位
#define FUNC_SD_SLOWSHIFT_ERRTH					(50U)	  	//慢速漂移中采集到异常值
#endif
//-----------------------------------------------------------------------------
//感温探测器 Heat
#define FUNC_HT_ST_ALARM                        (0x0001) //感温报警
#define FUNC_HT_ST_NTCOPEN                      (0x0010) //NTC开路
#define FUNC_HT_ST_NTCSHORT                     (0x0020) //NTC短路
#define FUNC_HT_ST_REMOTTEST                    (0x0400) //远程测试

#define FUNC_HT_OUTREG_DISALARMTEST   			(0xD2)   //禁止远程测试
#define FUNC_HT_OUTREG_ENALARMTEST     			(0xD1)   //使能远程测试
//-----------------------------------------------------------------------------
//Monitor 和Interface 
//输入状态
#define FUNC_ST_INPUT_SHORT                     (0x0001)  //输入短路
#define FUNC_ST_INPUT_OPEN					    (0x0004)  //输入开路
#define FUNC_ST_INPUT_OUTPUTENABLED             (0x0200)  //输出使能标志
//接口中继模块
#define	FUNC_IF_TH_OPEN							(63U)		//开路上限ADC[0-255]V1=16,V2=50
#define	FUNC_IF_TH_SHORT						(180U)		//短路下限ADC[0-255]V1=95,V2=160
//输入INP
#define	FUNC_IO_TH_OPEN							(230U)		//开路下限ADC[0-255]2.6V
#define	FUNC_IO_TH_SHORT						(68U)		//短路上限ADC[0-255]0.8V
//-----------------------------------------------------------------------------


//???OUT
#define	FUNC_IO_TH_PWR_ON		    			(230U)		//???????????10.8V
//#define	FUNC_IO_TH_PWR_ON						(50U)		//????????????IO64??
#define FUNC_RLY_TRIG_TIME						(200U)		//?????????????????Unit=30ms   6sec
//-----------------------------------------------------------------------------
//MCP
//消火栓和手扭
#define FUNC_MCP_ST_ALARM 					    (0x0001)
//手扭MCP
#define	FUNC_MB_TH_OPEN							(192U)		//开路AD阀值[0-255]75%
#define	FUNC_MB_TH_SHORT						(64U)		//短路AD阀值[0-255]25%
//-----------------------------------------------------------------------------
//AV
#define FUNC_AV_ST_AVON                        (0x0600)   //AV使能了
#define FUNC_AV_ST_SOUND_ON 				   (0x0200)   //声音使能
#define FUNC_AV_ST_LIGHT_ON					   (0x0400)   //声音使能
//声光  顺序显示的状态机
#define STATUS_AV_IDLE			 				(0U)		//AV 空闲状态
#define STATUS_AV_START            			 	(1U)		//AV 启动
#define STATUS_AV_SOUND_SET      				(2U)		//AV 声响开启
#define STATUS_AV_SOUNDOFF_LIGHTON 				(3U)		//AV 声响关闭 开启闪灯
#define STATUS_AV_LIGHTOFF_BACKIDLE 			(4U)		//AV 灯光关闭 返回空闲状态
#define STATUS_AV_IDLECYC_START     			(5U)        //AV 运行3个周期，空闲2个周期
#define STATUS_AV_IDLECYC_LIGHTON   			(6U)        //AV 空闲周期，点灯周期
#define STATUS_AV_IDLECYC_LIGHTOFF  			(7U)        //AV 空闲周期，关闭点灯
//AV 状态机根据分组进行调整
#define AVGRPCFG_SOUNDTIME       				(0U)
#define AVGRPCFG_LIGHTTIME       				(1U)
#define AVGRPCFG_CYCLETIME       				(2U)
#define AVGRPCFG_STRONGSDTIME                   (3U)
//-----------------------------------------------------------------------------
//隔离ISO
#define FUNC_ISO_PWR_ON							(90U)		//电源电压下限[0-255]93=12V
#define FUNC_ISO_NORMAL							(73U)		//10V正常下限[0-255]70=内部9V,93=内部12V
#define FUNC_ISO_SHORT							(58U)		//8V短路上限[0-255]64=外端9V,87=外端12V
#define FUNC_ISO_VDIFF							(10U)		//两端电压差下限[0-255]7~13
#define FUNC_ISO_VDIFF1							(10U)		//两端电压差下限[0-255]7~13
#define FUNC_ISO_VDIFF2							(7U)		//两端电压差下限[0-255]7~13


#define MOVEBUFF_LEN            (45)
/*******************************************************************************
 * 变量定义
 ******************************************************************************/
 //Note:单一结构体太大会导致编译器选择RAM0还是RAM1定位困难
typedef struct {
	//??????
	uint8	Interval;		//处理间隔定时数
	uint8   IntervalAV;     //用于蜂鸣器感烟的蜂鸣器部分定时
	uint8   AdjInterruptCnt; //自动调整被连续打断的次数
	uint8	FlagBreak;		//检测中断标志
	uint8   RemoteTestInd;       //RemoteTestIndicator
	uint8 	LoopVolt;
	//???????
	uint16	State;			//0-状态标志
	uint8	CtrlOut;		//4-控制值
	//uint8	Command;		//5-测试指令
	uint16	Value;			//2-输出值     红光
	uint16	ValueCh1;		//6-通信数据1  蓝光
	uint16	ValueCh2;		//8-通信数据2  暗电流 
	uint16	ValueCh3;		//A-通信数据3
	uint16	ValueCh4;		//C-通信数据4
	uint8	EolCmd;			///-测试命令
	uint16	ValuePW3;		//C-PW3值
	uint16	ValuePW4;		//C-PW4值
	#ifdef CODE_SEL_HEAT
	uint16	ValuePW5;		//C-PW5值
	#endif//
	//
	uint8	TempCnt;		//4-温度补偿计数器
	uint16	TempVal;		//4-温度实时数值

	//uint8   Config;         //?????????

	//故障定时
	uint8	CntFault;		//故障计数

	#ifndef CODE_SEL_HEAT   //Heat不相关
	uint8	CntShort_HighCham;		//短路计数或者high chamber计数
	uint8	CntOpen_LowCham;		//开路计数或者low chamber计数
	#endif
	

	uint8   StmQueryLedBusy;  //stm查询忙标志

#ifdef CODE_SEL_SMOKE 

		uint16   SD_Radio_BlueIR;    //蓝光/红光的比值 
		
	//???
	uint16	SD_Dark_OPA0;		//暗电流采样值一级放大输出
	uint16	SD_IRed_OPA0;		//红亮电流采样值一级放大输出
	uint16	SD_Blue_OPA0;		//蓝亮点流采样值一级放大输出
	uint16	SD_Blue_OPA1;		//蓝亮点流采样值二级放大输出
	uint16	SD_Dark_OPA1;		//暗电流采样值二级放大输出
	uint16	SD_IRed_OPA1;		//红亮电流采样值二级放大输出

	uint8 	SD_SampleFlag;

	uint8   SD_ClrAir_BLUE; //蓝光洁净空气值
	uint8   SD_ClrAir_IR;   //红光洁净空气值

	uint8   SD_SmokeType_Flag; //烟雾类型
	uint8	AmpCaliInterval;  //上电延迟校准
	uint8	FlagAmpCalid;	  //完成校准标志



#endif

	#ifndef CODE_SEL_HEAT
	//声光
	uint8	AV_Status;		//LED和AV 统一规划
	uint8	AV_GroupId; 	//AV的组号ID 0,1,2,3
	//uint8	AV_GroupNum;	//AV的总的组数1,2,3,4. 最大4组
	uint8	AV_CtrlRecord;	//AV控制命令保存
	uint8	ucAvControlTmp; //转化为按位控制命令
	uint8	AV_ActiveCycCnt;//连续活动3各周期，空闲两个周期
	uint8	AV_IdleCycCnt;	//空闲两个周期
	#endif
	


	#ifdef CODE_SEL_HEAT
	//感温
	uint8  FlagAdCodeStaus; //感温AD状态
	#endif
}
FuncDataRegs;

#ifdef CODE_SEL_SMOKE 
typedef struct
{

		//升烟拐点寻找相关
		//KneePoint KePot
	uint8   SD_KePot_CheckCnt;   //确认拐点计数
	uint8   SD_KePot_QuitCnt;    //确认退出拐点计算
	uint8   SD_KePot_Flag;       //拐点FLAG
	uint32  SD_KePot_Sum;        //90sec累加和

	#ifdef CODE_SEL_SMOKE_BUZZLE
	//蜂鸣器感烟首次通讯时，闪3次
	uint8  BuzzleSmokeFstComInterval; //蜂鸣器感烟首次通讯闪灯的间隔
	uint8  BuzzleSmokeFstComLedCnt;   //闪烁三次
	uint8  BuzzleSmokeFstComFlag;   //蜂鸣器感烟首次通讯标志
	#endif
	
}SdkeenPotSetting;


#endif

/*******************************************************************************
 * 全局变量引用
 ******************************************************************************/
extern FuncDataRegs FuncData;
/*******************************************************************************
 * 函数声明
 ******************************************************************************/
extern void SmokeDetector (void);

#ifdef CODE_SEL_SMOKE_BUZZLE
	extern void SmokeDetectorBuzzle(void);
#else 
	extern uint8 SmokePwrShiftProcess(void);
#endif




#ifdef CODE_SEL_HEAT
	extern void HeatDetector (void);
#endif

void drawForPWOutputTimer(uint16 pw);
void LedCtrl(void);

#endif /* __FUNCTION_H */
/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
