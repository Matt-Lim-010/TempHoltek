/********************************文件描述************************************
* (c) Copyright 1995-2017, System Sensor Xi'an.
* ALL RIGHTS RESERVED.
* 文件名称: ThermalAlgorithm.c
* 摘    要: GB项目：统一感温算法
* 当前版本: V0.1.0
* 作    者: GB Project Team:Uniform algorithm for thermal detector
* 创建日期: 2017-12-18
* 完成日期: 
******************************************************************************/
/*****************************************************************************
* 修改记录1:
* 修改日期:2018-3-23
* 作    者:Houlinfeng
* 内    容:1)报警阈值从固定的R特性40°C，非R特性60°C，修改为标准对应的中值
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
		   4)HeatDetectFunction()为示例调用，可根据实际情况修改。
		   5)差温报警时，进行补偿值计算时，将一个表达式拆分成3个，避免编译器优化产生错误
		   6)AD转换为AD时增加中间计算的强转unsigned long 防止计算溢出
* 当前版本:V0.1.2
******************************************************************************/
/*****************************************************************************
* 修改记录3:
* 修改日期:2019-01-29
* 作	者:Houlinfeng
* 内	容:1)重点修改滤波算法，以增强对脉冲型的干扰的滤波作用；
		   2)AD的滤波修改为仅对范围做控制，用以判定AD是否存在故障；
		   3)将AD值的4点滤波修改到转换为温度后的4点滑动窗滤波
* 当前版本:V0.1.3
******************************************************************************/

/*****************************************************************************
  注意：在初始化时，应首先根据当前温度给定可靠的当前恒温温度 HeatAlgorithmRegs.CnstTmpRecord
******************************************************************************/
#include "include.h"
#ifdef CODE_SEL_HEAT
/******************************   包含头文件 ********************************/
//#include "ThermalAlgorithm.h"
/******************************   宏定义   **********************************/

/******************************   类型声明  *********************************/

/******************************   全局变量声明  *****************************/
HeatAlgorithmData HeatAlgorithmRegs;
/******************************   局部变量声明 ******************************/
//Ad采集值、温度、斜率对应表
const TempSegRegData TempSegRegs[5]={
//	 AD , Temp,Ratio
//   采用滤波后NTC的AD-Temp表格进行计算
	{1101,0,57},					//0C
	{2570,41<<3,67},				//41C
	{2877,51<<3,88},				//51C
	{3243,66<<3,119},				//66C
	{3502,81<<3,168} 				//81C
};
HeatPointRegData HeatPointRegs[BUFFSIZE]={
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0}
};
/*5°C 耗时 温升曲线判断*/
const unsigned short Th5CTimExpend[6]=
{
	K30L_5C_PERIOD,					//30C/min曲线 5C上升耗时下限阈值
	K20L_5C_PERIOD,					//20C/min曲线 5C上升耗时下限阈值
	K10L_5C_PERIOD,					//10C/min曲线 5C上升耗时下限阈值
	K5L_5C_PERIOD,					//5C/min曲线 5C上升耗时下限阈值
	K3L_5C_PERIOD,					//3C/min曲线 5C上升耗时下限阈值
	K1L_5C_PERIOD					//1C/min曲线 5C上升耗时下限阈值
};
/*定温型报警点温度补偿*/
const unsigned short FixedComptTempVal[7]=
{	
	K0_COMPT_NOR,					//恒温补偿值
	K1_COMPT_NOR,					//1C/min曲线，报警点补偿值
	K3_COMPT_NOR,					//3C/min曲线，报警点补偿值
	K5_COMPT_NOR,					//5C/min曲线，报警点补偿值
	K10_COMPT_NOR,					//10C/min曲线，报警点补偿值
	K20_COMPT_NOR,					//20C/min曲线，报警点补偿值
	K30_COMPT_NOR					//30C/min曲线，报警点补偿值
};
/*差温型报警点温度补偿线性方程*/

const RTypeCmpRegData RorComptTempVal[3]=
{
	#ifdef DEV_MECH_D20
	{13,83},
	{32,107},
	{34,134}
	#endif //DEV_MECH_D20
	#ifdef DEV_MECH_ND751P
	{11,99},
	{22,123},
	{35,158}
	#endif //DEV_MECH_ND751P
};
/*报警点温度阈值*/
//定温型为固定阈值
//差温型为动态阈值
const unsigned short AlarmThVal[2][7]=
{// 0C/min 					1C/min						3C/min			5C/min				10C/min					20C/min					30C/min
	{TH_ALARMTEMP_NOR_K0C,TH_ALARMTEMP_NOR_K1C,TH_ALARMTEMP_NOR_K3C,TH_ALARMTEMP_NOR_K5C,TH_ALARMTEMP_NOR_K10C,TH_ALARMTEMP_NOR_K20C,TH_ALARMTEMP_NOR_K30C},
	{TH_ALARMTEMP_NOR_K0C,TH_ALARMTEMP_NOR_K1C,TH_ALARMTEMP_NOR_K3C,TH_ALARMTEMP_NOR_K5C,  TH_ALARMTEMP_R_K10C,  TH_ALARMTEMP_R_K20C,  TH_ALARMTEMP_R_K30c},
};
/******************************   局部函数声明 ******************************/

/******************************   局部函数实现 ******************************/
/*******************************************************************************
* Function	: AdvalFilter()
* Brief		: AD码值滤波处理:去除极大异常值；4平均。
* Input		: uiAdCodeInput:12位AD码值输入
* OutPut 	: *uipAdCodeOutput:滤波处理后输出AD码值
* Return	: 程序运行状态
*******************************************************************************/
unsigned char AdvalFilter(unsigned short uiAdCodeInput,unsigned short * uipAdCodeOutput)
{
	unsigned char ucFuncStatus = 0; 		//状态0 表示正常
	static unsigned short uiLastAd = ADVAL_DEFAULT; //上一次正确的AD值 初始化幅值为25°C默认AD值
		
	/*Input Check*/
	if(PNULL == uipAdCodeOutput)
	{
		return ERRCODE_PNULL;
	}

	if(uiAdCodeInput < ADVAL_MIN)
	{
		*uipAdCodeOutput = uiLastAd;		  //返回上一次正确的值
		return ERRCODE_ADVALBLL;  //低于下限
		
	}
	if(uiAdCodeInput > ADVAL_MAX)
	{
		*uipAdCodeOutput = uiLastAd;		  //返回上一次正确的值
		return ERRCODE_ADVALABC;  //高于上限
	}
	
	uiLastAd = uiAdCodeInput;
	*uipAdCodeOutput = uiAdCodeInput;
		
	return ucFuncStatus;
}

/*******************************************************************************
* Function	: AdConvertTemp()
* Brief		: AD码值转换为温度。
* Input		: uiAdCode:确保正确范围内的AD值
* OutPut 	: *siTemp:输出对应温度值*8倍（为保证精度不丢失）
* Return	: 程序运行状态
*******************************************************************************/
unsigned char AdConvertTemp(unsigned short uiAdCode,short *siTemp)
{
	unsigned char ucFuncStatus = 0; 		//状态0 表示正常
	short siTempOut = 0;
	unsigned char i = 0;

	static short siTmpBuff[4] ={0,0,0,0}; //温度buff
	static short uiLastTmp = 0; //上一次正确的温度值
	static unsigned char ucFstRunFlag = 0; 	//首次运行标志
	static unsigned char ucRefrshNum = 0;
	short siSumTmp = 0 ;
	static unsigned char s_uiPulseJamCnt = 0;
	
	/*Input Check*/
	if(PNULL == siTemp)
	{
		return ERRCODE_PNULL;
	}
	/*低于0°C  输出0°C*/
	if(uiAdCode < TempSegRegs[0].AdStart)
	{
		ucFuncStatus = ERRCODE_TMPBLLOW;  //置温度过低的标志	
		siTempOut = TMP_LOW_LIMIT;        //显示温度的下限 0°C
	}
	else if(uiAdCode >= TempSegRegs[4].AdStart)
	{	
		//"+"号结合律优先级更高
		siTempOut = TempSegRegs[4].TempStart
			+((uiAdCode - TempSegRegs[4].AdStart)*TempSegRegs[4].Factor>>8);
		
		/*大于100°C，输出100°C*/
		if(siTempOut > TMP_UP_LIMIT)
		{
			ucFuncStatus = ERRCODE_TMPABHIGH;  //置温度过高标志
			siTempOut = TMP_UP_LIMIT;  		   //显示温度的上限100°C
		}
	}
	else
	{
		for(i=0;i<4;i++)
		{
			if((uiAdCode >= TempSegRegs[i].AdStart)
			&&(uiAdCode < TempSegRegs[i+1].AdStart))
			{
				siTempOut = TempSegRegs[i].TempStart
			+((unsigned long)(uiAdCode - TempSegRegs[i].AdStart)*TempSegRegs[i].Factor>>8);
				break;
			}
		}
	}
	/*温度正常范围*/
	/*首次运行填充温度BUFF，给上一次正确温度变量幅值*/
	if(0 == ucFstRunFlag)
	{
		for(i=0;i<4;i++)
		{
			siTmpBuff[i] = siTempOut;
		}
		uiLastTmp = siTempOut;		     //更新正确的上一次AD值
		ucFstRunFlag = 1; 				  //首次运行完成
	}
	
	//限幅型平均滤波，带连续干扰出口特性
	//如果当前温度比上一轮温度超出范围，则认为是干扰。
	if(((siTempOut - uiLastTmp) > TMP_FILTER_ZONE)
	||((uiLastTmp - siTempOut) > TMP_FILTER_ZONE))
	{
		if(s_uiPulseJamCnt < TMP_FILTER_PULSE_MAX)
		{
			s_uiPulseJamCnt++;
			siTmpBuff[(ucRefrshNum++)&0x03] = uiLastTmp; //连续TMP_FILTER_PULSE_MAX内的脉冲干扰，都不影响温度输出
		}	
		else
		{
			siTmpBuff[(ucRefrshNum++)&0x03] = siTempOut;  //如果连续超过TMP_FILTER_PULSE_MAX的脉冲干扰，则认为是正常的值
		}
	}
	else
	{
		s_uiPulseJamCnt = 0;
		siTmpBuff[(ucRefrshNum++)&0x03] = siTempOut; 
	}
	
	/*计算4点BUFF平均*/
	for(i=0;i<4;i++)
	{
		siSumTmp += siTmpBuff[i];
	}
	uiLastTmp = (siSumTmp>>2);		     //更新正确的上一次AD值
	*siTemp = (siSumTmp>>2);			 //siSumTmp 为临时变量，无需再次清零了
	
	return ucFuncStatus;		
}
/*******************************************************************************
* Function	: ResetHeatPointRegs()
* Brief		: 初始化温度点寄存器。
* Input		: siTmpIn:温度输入
* OutPut 	: 无
* Return	: 程序运行状态
*******************************************************************************/
void ResetHeatPointRegs(short siTmpIn)
{
	unsigned char i = 0;
	for(i=0;i<BUFFSIZE;i++)
	{
		HeatPointRegs[i].NtcTemp = siTmpIn;
		HeatPointRegs[i].StayTime = 0;
	}
}
/*******************************************************************************
* Function	: ShiftHeatPointRegs()
* Brief		: 移位存储温度点寄存器。
* Input		: siTmpIn:温度输入
* OutPut 	: 无
* Return	: 程序运行状态
*******************************************************************************/
void ShiftHeatPointRegs(HeatPointRegData TmpPointRegIn)
{
	unsigned char i = 0;
	for(i=0;i<(BUFFSIZE-1);i++)
	{
		HeatPointRegs[i].NtcTemp = HeatPointRegs[i+1].NtcTemp ;
		HeatPointRegs[i].StayTime = HeatPointRegs[i+1].StayTime;
	}
	HeatPointRegs[BUFFSIZE-1].NtcTemp = TmpPointRegIn.NtcTemp;
	HeatPointRegs[BUFFSIZE-1].StayTime = TmpPointRegIn.StayTime;
}
/*******************************************************************************
* Function	: TempRiseCurJudge()
* Brief		: 根据温升情况判断温升速率。
* Input		: siTmpIn:温度输入
* OutPut 	: *uiTempCurve:输出温升速率判断
* Return	: 程序运行状态
*******************************************************************************/
unsigned char TempRiseCurJudge(short siTmpIn,unsigned char *ucTempCurve)
{
	unsigned char ucFuncStatus = 0; 				//状态0 表示正常
	static unsigned char ucFstRunFlag = 0; 			//首次运行标志
	static HeatPointRegData HeatPointTemp ={0,0};	//临时变量
	static unsigned char ucTempCurveTmp = 0;		//温升曲线临时变量
	unsigned char i = 0;
	unsigned short uiSumTimExpendTmp=0;				//5°C上升耗时总计

	/*Input Check*/
	if(PNULL == ucTempCurve)
	{
		return ERRCODE_PNULL;
	}
	
	/*首次运行*/
	if(0 == ucFstRunFlag)
	{
		ResetHeatPointRegs(siTmpIn);
		HeatPointTemp.NtcTemp = siTmpIn;
		HeatPointTemp.StayTime = 0;		
		ucTempCurveTmp = TEMPRATIO_K0C; 	//输出恒温标志
		ucFstRunFlag = 1;

		ucFuncStatus = FUNCSTATUS_TEMPCURVE_FSTRUN; //首次运行标志
	}
	else
	{
		if(HeatPointTemp.StayTime < K0L_CONSTTEMP_PERIOD)
		{
			HeatPointTemp.NtcTemp = siTmpIn;
			HeatPointTemp.StayTime += TIMEBASE;

			ucFuncStatus = FUNCSTATUS_TEMPCURVE_NONARR; //时间未到

			/*下降过程和恒温过程均不能触发此判据*/
			if((HeatPointTemp.NtcTemp - HeatPointRegs[BUFFSIZE-1].NtcTemp) >= (short)CODE_1C) 
			{

				uiSumTimExpendTmp = 0;
				/*判断温升速率*/
				/*每个BUFF都记录的是前1°C到当前温度耗时*/
				for(i=1;i<BUFFSIZE;i++)
				{
					uiSumTimExpendTmp += HeatPointRegs[i].StayTime;
				}
				uiSumTimExpendTmp += HeatPointTemp.StayTime;		//不加Reg[0]的StayTime,而是加上最新的TMP的时间
				/*归一到5°C耗时*/
				/*NtcTemp>BUFF[4],所以NtcTemp>BUFF[0],没有除0风险*/
				uiSumTimExpendTmp = uiSumTimExpendTmp*KWINDOW /(HeatPointTemp.NtcTemp-HeatPointRegs[0].NtcTemp);		
				
				/*温度超过1°C 保存进BUFF*/
				ShiftHeatPointRegs(HeatPointTemp);
				HeatPointTemp.StayTime = 0;

				/*判断温升速率*/
				for(i=0;i<6;i++)
				{
					if(uiSumTimExpendTmp < Th5CTimExpend[i])
					{
						break;
					}
				}
				ucTempCurveTmp = (6-i);   //输出温升速率

				ucFuncStatus = FUNCSTATUS_TEMPCURVE_CUROUT; //曲线输出
			}
		}
		else
		{	
			//超过4min 温度没有变化1°C
			//重置BUFF，认为是恒温过程			
			ResetHeatPointRegs(siTmpIn);
			HeatPointTemp.NtcTemp = siTmpIn;
			HeatPointTemp.StayTime = 0;

			HeatAlgorithmRegs.CnstTmpRecord = siTmpIn;   //记录恒温过程的温度值
			
			ucTempCurveTmp = TEMPRATIO_K0C; 	//输出恒温标志
			ucFuncStatus = FUNCSTATUS_TEMPCURVE_TIMOV;	
		}
	}	
	*ucTempCurve = ucTempCurveTmp;
	return ucFuncStatus;
}
/*******************************************************************************
* Function	: AlarmTmpThConfig()
* Brief		: 根据运行状态选择报警阈值。
* Input		: ucTmpCurve:温升曲线选择
			  ucRunModeSel:产品配置的运行模式
			  siCnstTmpIn:恒温温度输入
* OutPut 	: *uipAlarmThOut:输出温升速率判断
* Return	: 程序运行状态
*******************************************************************************/
unsigned char AlarmTmpThConfig(unsigned char ucTmpCurve,
							   unsigned char ucRunModeSel,
							   short siCnstTmpIn,
							   unsigned short *uipAlarmThOut)
{
	unsigned char ucFuncStatus = 0; 		//状态0 表示正常
	unsigned short uiAlarmThTemp = 0;
	
	/*Input Check*/
	if(PNULL == uipAlarmThOut)
	{
		return ERRCODE_PNULL;
	}

	/*Check ucTmpCurve Scope*/
	if(ucTmpCurve > 6)
	{
		ucFuncStatus = ERRCODE_TMPCUROV;
		return  ucFuncStatus;
	}
	/*报警类型选择取值只有0、1*/
	if(ucRunModeSel > 1)
	{	
		ucFuncStatus = ERRCODE_MODESELOUT;
		return ucFuncStatus;
	}
	/*如果产品配置为差温型探头，
	且处于高升温速率，
	且起始温度为25°C以下，
	则为差温报警*/
	if((ucTmpCurve > TEMPRATIO_K5C)
	&&(HEAT_DEVICE_TYPE_R == ucRunModeSel)
	&&(siCnstTmpIn < CNST_TMP_LINE))
	{
		uiAlarmThTemp = siCnstTmpIn + AlarmThVal[HEAT_DEVICE_TYPE_R][ucTmpCurve];
	}
	else
	{	//否则均为定温型报警
		uiAlarmThTemp = AlarmThVal[HEAT_DEVICE_TYPE_FIXED][ucTmpCurve];
	}
	
	*uipAlarmThOut = uiAlarmThTemp;
	return ucFuncStatus;
}
/*******************************************************************************
* Function	: TempAlarmFunc()
* Brief		: 温度报警功能。
* Input		: siNtcTmpIn:NTC温度输入
			  ucTmpCurve:温升曲线输入
			  ucRunModeSel:产品类型配置
			  siCnstTmpIn:恒温温度输入
			  uiAlarmTh:报警阈值
* OutPut 	: 无
* Return	: 是否报警
*******************************************************************************/
unsigned char TempAlarmFunc(short siNtcTmpIn,
							unsigned char ucTmpCurve,
							unsigned char ucRunModeSel,
							short siCnstTmpIn,
							unsigned short uiAlarmTh)
{
	unsigned char ucFuncStatus = 0; 		//状态0 表示正常
        short siRorCmpValue =0;
        short siRorCmpIncep = 0;
	uint32 Tmp32;
	/*Check ucTmpCurve Scope*/
	if(ucTmpCurve > 6)
	{
		ucFuncStatus = ERRCODE_TMPCUROV;
		return  ucFuncStatus;
	}
	/*运行模式选择取值只有0、1、2*/
	if(ucRunModeSel > 1)
	{	
		ucFuncStatus = ERRCODE_MODESELOUT;
		return ucFuncStatus;
	}

	/*差温型*/
	if((ucRunModeSel == HEAT_DEVICE_TYPE_R)
	&& (ucTmpCurve > TEMPRATIO_K5C)
	&&(siCnstTmpIn < CNST_TMP_LINE))
	{
		//分为3段计算，避免编译器优化造成的异常
		siRorCmpValue = (siCnstTmpIn*(int)RorComptTempVal[ucTmpCurve-4].ratio)>>8;
		siRorCmpIncep = RorComptTempVal[ucTmpCurve-4].interept;
		/*差温型由于报警点是不定的，补偿值采用线性化处理方式*/
		HeatAlgorithmRegs.CompTemp =(siNtcTmpIn + siRorCmpValue+siRorCmpIncep); 
	}
	else
	{	
		/*定温型*/
		HeatAlgorithmRegs.CompTemp = (siNtcTmpIn + FixedComptTempVal[ucTmpCurve]); //补偿后的温度
	}
	//
	//计算PW4
	//
	if(HeatAlgorithmRegs.CompTemp <0 )
	{
		Tmp32 = 0;
	}
	else
	{
		Tmp32 = (uint32)HeatAlgorithmRegs.CompTemp << 5;		
	}
	Tmp32 *= 1400;
	Tmp32 /= (uiAlarmTh >> 3);
	Tmp32 >>= 8;
	Tmp32 = Tmp32 + 600; //底值600
	if(Tmp32>=2000)
	{
		FuncData.ValuePW4 = 2500;
	}
	else if(Tmp32<=600)
	{
			FuncData.ValuePW4 = 600;
	}
	else
	{
		FuncData.ValuePW4 = (uint16)Tmp32;
	}

	
	/*补偿后的温度值>报警阈值，则报警*/
	/*
	if(HeatAlgorithmRegs.CompTemp > uiAlarmTh)
	{
		ucFuncStatus  = FUNCSTATUS_TMPALARM;
	}
	*/
	return ucFuncStatus;

}
/*******************************************************************************
* Function	: HeatDeviceTypeConfig()
* Brief		: 配置产品类型。
* Input		: ucRunModeSel:产品类型配置
			 取值：HEAT_DEVICE_TYPE_FIXED：定温型，兼容S型；
				   HEAT_DEVICE_TYPE_R: 差温型
* OutPut 	: 无
* Return	: 程序运行状态
*******************************************************************************/
unsigned char HeatDeviceTypeConfig(unsigned char ucRunModeSel)
{
	unsigned char ucFuncStatus = 0; 		//状态0 表示正常
	if(ucRunModeSel > 1)
	{
		return ERRCODE_MODESELOUT ;
	}
	HeatAlgorithmRegs.ThermalTypeConfig = ucRunModeSel;
	return ucFuncStatus;
}

/******************************   全局函数实现 ******************************/
/*******************************************************************************
* Function	: HeatDetectFunction
* Brief		: 温度检测报警功能。
* Input		: 无
* OutPut 	: 无
* Return	: 程序运行状态
*******************************************************************************/
void HeatDetectFunction(void)
{
	unsigned short uiAdcode = 0;
	unsigned char i = 0;
	unsigned short uiAlarmThVal = 0;
	static unsigned short s_FirstFlag = 0;

	//第0步
	//配置产品的类型
	//应用HeatDeviceTypeConfig()，在需要的地方进行类型的配置
	//For Example:
	#if 0
	(void)HeatDeviceTypeConfig(HEAT_DEVICE_TYPE_FIXED);//配置产品类型
	#endif
	//第一步
	/*温度AD采集*/
	//注：在此处根据硬件平台添加AD采集代码
	//输出uiAdcode为12位的AD采集值 
	//--------------------------------------------------------------
	//For Example:
	#if 1
	GPO_SEN_PWR_1();								//开启NTC供电
	GCC_DELAY(100);									//延迟消除噪声
	uiAdcode = 0;
	i = 0;
	//为避免LDO在通讯结束后的>1ms的脉冲干扰AD，添加临界段和延时代码
	FuncData.FlagBreak = 0;						//初始化中断标志  
	GCC_DELAY(2000); //延时2ms

	while(i < 4)
	{
		uiAdcode += AdcSample(AD_CHANNEL);	//CH3
		i ++;
	}
	GPO_SEN_PWR_0();								//关闭NTC供电	
	
	if(FuncData.FlagBreak>0)
	{
		return;
	}

	uiAdcode = uiAdcode >> 2;					    //平均值
	#endif
	//--------------------------------------------------------------
	//第二步
	/*AD码值滤波*/
	FuncData.FlagAdCodeStaus = AdvalFilter(uiAdcode,&HeatAlgorithmRegs.AdCodeFilter);
	//第三步
	/*AD码值转换为NTC温度值*/
	(void)AdConvertTemp(HeatAlgorithmRegs.AdCodeFilter,
					   &HeatAlgorithmRegs.NtcRealTemp);

	//如果是首次采样，根据当前温度更新当前恒温温度
	if(0 == s_FirstFlag)
	{
		s_FirstFlag =1;
		HeatAlgorithmRegs.CnstTmpRecord = HeatAlgorithmRegs.NtcRealTemp;
	}
	//第四步
	/*根据输入温度判定温度曲线*/
	(void)TempRiseCurJudge(HeatAlgorithmRegs.NtcRealTemp,
						  &HeatAlgorithmRegs.TempCurveVal);
	//第五步
	/*根据温升曲线和运行模式，确定报警阈值*/
	(void)AlarmTmpThConfig(HeatAlgorithmRegs.TempCurveVal,
						   HeatAlgorithmRegs.ThermalTypeConfig,
						   HeatAlgorithmRegs.CnstTmpRecord,
							 &uiAlarmThVal);
	//第六步
	/*根据输入温度、温度曲线和阈值判定是否报警*/
	HeatAlgorithmRegs.AlarmFlag = TempAlarmFunc(HeatAlgorithmRegs.NtcRealTemp,
							HeatAlgorithmRegs.TempCurveVal,
							HeatAlgorithmRegs.ThermalTypeConfig,
							HeatAlgorithmRegs.CnstTmpRecord,
							uiAlarmThVal);
}

#endif
/////////////////////////////////////////////////////////////////
//The End of File
/////////////////////////////////////////////////////////////////
