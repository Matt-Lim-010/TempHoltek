/********************************文件描述************************************
* (c) Copyright 1995-2018, System Sensor Xi'an.
* ALL RIGHTS RESERVED.
* 文件名称: ThermalAlgorithm.h
* 摘    要:	GB项目：统一感温算法
* 当前版本: V0.1.0
* 作    者: GB Project Team:Uniform algorithm for thermal detector
* 创建日期: 2017-12-18
* 完成日期: 
******************************************************************************/
/*****************************************************************************
* 修改记录1:
* 修改日期:2018-3-23
* 作	者:Houlinfeng
* 内	容:1)报警阈值从固定的R特性40°C，非R特性60°C，修改为标准对应的中值
		   2)补偿值修改为对应的报警中值的补偿值
		   3)温升曲线的BUFF从6点修改为5点
		   4)正常运行过程中的报警阈值修改为在当前温度连续上升35°C后报警；动态报警阈值
		   5)去除单片机特性相关内容，使之适合任何C编译器
		   6)输入参数，如有指针的，进行排空操作
* 当前版本:V0.1.1
******************************************************************************/
/*****************************************************************************
* 修改记录2:
* 修改日期:2018-4-12
* 作	者:Houlinfeng
* 内	容:1)产品是定温型还是差温型根据配置参数来确定。产品类型原则如下:		
			配置为定位型，则按照定温型温度报警
			配置为差温型，但起始温度>=25°C，则按照定温型报警
			配置为差温型，起始温度<25°C，但为低温升速率，按照定温型报警
			配置为差温型，起始温度<25°C，为高升温速率，按照差温报警。
		   2)差温报警，由于报警点温度不定，报警点的补偿值修改为与起始温度相关的一个线性方程
		   3)增加产品类型配置的接口函数。
* 当前版本:V0.1.2
******************************************************************************/
/*****************************************************************************
* 修改记录3:
* 修改日期:2019-01-29
* 作	者:Houlinfeng
* 内	容:1)重点修改滤波算法，以增强对脉冲型的干扰的滤波作用；
		   2)AD的滤波修改为仅对范围做控制，用以判定AD是否存在故障；
		   3)
* 当前版本:V0.1.3
******************************************************************************/
#ifndef __THERMAL_ALGORITHM_H__
#define __THERMAL_ALGORITHM_H__
/******************************  包含头文件 **********************************/
#ifdef CODE_SEL_HEAT
/******************************   宏定义   ***********************************/
//空指针
#define PNULL               ((void*)0)              			//空指针
#define AD_CHANNEL          (0U)
//////////////////////////////
#define TIMEBASE   	   		(4U)								//4秒运行
#define KWINDOW 	  	 	(40U)								//5°C窗口，放大8倍
#define CODE_1C				(8U)								//1°C 放大8倍
#define BUFFSIZE		 	(5U)								//5°C温差，需要5点buff 4-0
//////////////////////////////
//温度限幅滤波器配置
#define TMP_LOW_LIMIT       (0U)								//温度显示范围下限
#define TMP_UP_LIMIT        (800U)								//温度显示范围上限
#define TMP_FILTER_ZONE     (80U)                               //限幅滤波10°C
#define TMP_FILTER_PULSE_MAX (3U)                               //连续限幅抗干扰次数   
//////////////////////////////
//感温探头类型配置
#define HEAT_DEVICE_TYPE_FIXED	(0U)								//定温型
#define HEAT_DEVICE_TYPE_R		(1U)								//差温型
#define CNST_TMP_LINE       (200U)								//25°C作为起始温度分界点
//非R特性和R特性的报警阈值
//改进为报警点取中值而不是固定的60°C和40°C；放大8倍
//	升温曲线   报警点温度	*8阈值
//	恒温	0C/min	60	480
//	From25C 1C/min	61	488
//	From25C 3C/min	58	464
//	From25C 5C/min	58	464
//	From25C 10C/min	57	456
//	From25C 20C/min	60	480
//	From25C 30C/min	63	504
#define TH_ALARMTEMP_NOR_K0C	(480U)						   
#define TH_ALARMTEMP_NOR_K1C	(488U)						   
#define TH_ALARMTEMP_NOR_K3C	(464U)						   
#define TH_ALARMTEMP_NOR_K5C	(464U)						   
#define TH_ALARMTEMP_NOR_K10C	(456U)						   
#define TH_ALARMTEMP_NOR_K20C	(480U)						   
#define TH_ALARMTEMP_NOR_K30C	(504U)						   
//R特性	升温曲线 报警点温度	*8阈值
//  From5C   0C/min 35  280
//	From5C  10C/min	32	256
//	From5C  20C/min	35	280
//	From5C  30C/min	38	304
#define TH_ALARMTEMP_R_K0C		(280U)	
#define TH_ALARMTEMP_R_K10C		(256U)	
#define TH_ALARMTEMP_R_K20C		(280U)	
#define TH_ALARMTEMP_R_K30c		(304U)	
//////////////////////////////
//Section 1 温升5度花费时间的宏定义
//此数据通过实测探头温升曲线，去除开始部分的较大值后的平均值
#ifdef DEV_MECH_D20
	#define K1_5C_PERIOD   (333U)									//1C/min温升曲线 5°C上升时间
	#define K3_5C_PERIOD   (122U)									//3C/min温升曲线 5°C上升时间
	#define K5_5C_PERIOD   (76U)									//5C/min温升曲线 5°C上升时间
#define K10_5C_PERIOD  (43U)									//10C/min温升曲线 5°C上升时间
#define K20_5C_PERIOD  (25U)									//20C/min温升曲线 5°C上升时间
#define K30_5C_PERIOD  (16U)									//30C/min温升曲线 5°C上升时间
#endif //DEV_MECH_D20

#ifdef DEV_MECH_ND751P
	#define K1_5C_PERIOD   (339U)									//1C/min温升曲线 5°C上升时间
#define K3_5C_PERIOD   (127U)									//3C/min温升曲线 5°C上升时间
	#define K5_5C_PERIOD   (83U)									//5C/min温升曲线 5°C上升时间
	#define K10_5C_PERIOD  (42U)									//10C/min温升曲线 5°C上升时间
#define K20_5C_PERIOD  (25U)									//20C/min温升曲线 5°C上升时间
#define K30_5C_PERIOD  (16U)									//30C/min温升曲线 5°C上升时间
#endif //DEV_MECH_ND751P

//Section 2 判断升温斜率阈值的宏定义
#define K0L_CONSTTEMP_PERIOD  (240U)							//判断恒温的时长限制240秒,连续4分钟不升温1°C认为是恒温
#define K1L_5C_PERIOD   (480U)									//1度/分钟的下限时间长度 修改为8min 恒温时间国标规定10min
#define K3L_5C_PERIOD   (( K1_5C_PERIOD+K3_5C_PERIOD)/2)		//3度/分钟的下限时间长度
#define K5L_5C_PERIOD   (( K3_5C_PERIOD+K5_5C_PERIOD)/2)		//5度/分钟的下限时间长度
#define K10L_5C_PERIOD  (( K5_5C_PERIOD+K10_5C_PERIOD)/2)		//10度/分钟的下限时间长度
#define K20L_5C_PERIOD  (( K10_5C_PERIOD+K20_5C_PERIOD)/2)		//20度/分钟的下限时间长度
#define K30L_5C_PERIOD  (( K20_5C_PERIOD+K30_5C_PERIOD)/2)		//30度/分钟的下限时间长度

//Section3 温升曲线编号
#define TEMPRATIO_K0C		(0U)								//恒温过程
#define TEMPRATIO_K1C		(1U)								//1C/min
#define TEMPRATIO_K3C		(2U)								//3C/min
#define TEMPRATIO_K5C		(3U)								//5C/min
#define TEMPRATIO_K10C		(4U)								//10C/min
#define TEMPRATIO_K20C		(5U)								//20C/min
#define TEMPRATIO_K30C		(6U)								//30C/min
//Section4 温度补偿值
//非R特性 即定温型 补偿值
//非R特性	From 25C			
//	升温曲线	报警温度	补偿温度	补偿温度*8
//	恒温		60			0				0
//	1C/min		61			4.7				37.6
//	3C/min		58			7.5				60
//	5C/min		58			8.3				66.4
//	10C/min		57			12.5			100
//	20C/min		60			12.4			99.2
//	30C/min		63			16.2			129.6
#ifdef DEV_MECH_D20
#define K0_COMPT_NOR        (0U)								
	#define K1_COMPT_NOR 		(20U)								
	#define K3_COMPT_NOR 		(55U)								
	#define K5_COMPT_NOR 		(59U)								
	#define K10_COMPT_NOR 		(86U)								
	#define K20_COMPT_NOR 		(123U)								
	#define K30_COMPT_NOR 		(150U)	
#endif //DEV_MECH_D20		

#ifdef DEV_MECH_ND751P
#define K0_COMPT_NOR        (0U)								
	#define K1_COMPT_NOR 		(28U)								
	#define K3_COMPT_NOR 		(66U)								
	#define K5_COMPT_NOR 		(82U)								
	#define K10_COMPT_NOR 		(100U)								
	#define K20_COMPT_NOR 		(132U)								
	#define K30_COMPT_NOR 		(160U)	
#endif //DEV_MECH_ND751P	
/////////////////////////////
//AD码值最大值和最小值范围
#define ADVAL_MAX					  (3889U)             		// >125°C
#define ADVAL_MIN       			  (100U)			    	// <-45°C
#define ADVAL_DEFAULT                 (2048U)                   // 25°C 对应的AD值作为默认AD值
/////////////////////////////
//程序运行状态定义
#define ERRCODE_ADVALBLL               (1U)      				//Below lower limit   低于下限
#define ERRCODE_ADVALABC               (2U)      				//Above the ceiling   高于上限
#define ERRCODE_TMPBLLOW			   (3U)	   					//温度过低
#define ERRCODE_TMPABHIGH			   (4U)						//温度过高
#define ERRCODE_TMPCUROV		       (5U)						//温度曲线超限
#define ERRCODE_MODESELOUT             (6U)						//产品类型超限
#define ERRCODE_PNULL				   (7U)						//输入参数是指针但为空指针

#define FUNCSTATUS_TEMPCURVE_FSTRUN    (10U)    				//首次运行
#define FUNCSTATUS_TEMPCURVE_NONARR	   (11U)    				//时间未到
#define FUNCSTATUS_TEMPCURVE_CUROUT    (12U)    				//曲线输出
#define FUNCSTATUS_TEMPCURVE_TIMOV     (13U)    				//时间超时
#define FUNCSTATUS_TMPALARM		   	   (14U)					//温度报警
/******************************   类型声明  **********************************/
//采样值-温度的线性分段，factor是放大了256倍的斜率系数，保存在FLASH空间
typedef struct 
{
	unsigned short AdStart;										//12位AD采样值 
	unsigned short TempStart;									//折线起点 放大8倍
	unsigned char Factor;		    							//斜率系数 放大256倍
}TempSegRegData;

//heatPoint代表了每一个采样点的温度值和在该温度停留的时间S
typedef struct 
{
	short NtcTemp;												//实际温度的8倍
	unsigned short StayTime;									//在该温度点停留的时间S
}HeatPointRegData;
//R特性报警点补偿值
typedef struct
{
	short ratio;												//斜率 放大256倍保存
	short interept;												//截距 原值
}RTypeCmpRegData;												//

//感温算法使用的全部变量
typedef struct
{
	unsigned char ThermalTypeConfig;							//产品类型配置
	unsigned short AdCodeFilter;								//滤波后的AD值
	short NtcRealTemp;											//Ntc实测温度
	short CompTemp;												//补偿后的温度值
	unsigned char TempCurveVal;									//温度曲线值
	unsigned char AlarmFlag;									//报警标志
	short CnstTmpRecord;										//恒温温度记录  
}HeatAlgorithmData;
/******************************   全局变量声明  ******************************/
extern HeatAlgorithmData HeatAlgorithmRegs;
/******************************   全局函数声明 *******************************/
extern void HeatDetectFunction(void);
extern unsigned char HeatDeviceTypeConfig(unsigned char ucRunModeSel);

#endif
#endif

/////////////////////////////////////////////////////////////////
//The End of File
/////////////////////////////////////////////////////////////////
