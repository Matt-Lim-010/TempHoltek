/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      smoke.c
 * @author    S
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     烟感程序
 ******************************************************************************/
#include "include.h" 

const uint8 AvGroupConfig[4]=
{
	//SoundTime  LightTime   CycleTime  StrongSoundTime
	//CycleTime = (SoundTime+LightTime)*GroupNum 
	//但是因为只分1组也要延时，为减少特殊处理，多定了一维数组
	//*30为实际持续时间
    11,1,26,3				//CycleTime 780ms

};

void LedCtrl(void)
{
	#if defined(CODE_SEL_SMOKE)
		if(0xA1 == FuncData.EolCmd)
		{
			GPO_SMOKE_YELLOW_ON();
		}
		else
		{
			GPO_SMOKE_YELLOW_OFF();
		}
	#endif	
}

//------------------------------
//业务函数:感烟器算法（红蓝光）
//------------------------------
#ifdef CODE_SEL_SMOKE


//------------------------------
//业务函数:红蓝光感烟程序
//------------------------------
void SmokeDetector (void)
{
	uint16 uiRadioBlueIR = 0;
	uint32 uiBlTemp = 0;
	uint32 uiIrTemp = 0;
	uint32 TempDat = 0;
	//置感烟任务
	SysData.CmdActive |=  ACTIVE_FLAG_FUNC;
	//----------开始检测----------
	if(CommData.Step > COMM_STEP_IDLE)			//通信中不检测
		return;
	//----------特殊工厂命令-----------
	
	// if(0xA1 == FuncData.EolCmd)	//测试黄灯
	// {
	// 	GPO_SMOKE_YELLOW_ON();
	// 	FuncData.CtrlOut = FUNC_CTRL_AV_A_OFF;
	// }
	// else if (0xB1 == FuncData.EolCmd)
	// {
	// 		#ifdef CODE_SEL_SMOKE_BUZZLE
	// 		FuncData.CtrlOut = FUNC_CTRL_AV_A_ON;
	// 		#endif //
	// 		GPO_SMOKE_YELLOW_OFF();
	// }
	// else
	// {
	// 	GPO_SMOKE_YELLOW_OFF();
	// 	FuncData.CtrlOut = FUNC_CTRL_AV_A_OFF;
	// }

	if(0xC1 == FuncData.EolCmd)	//关闭感烟功能
	{
		SysData.CmdActive &=  (~ACTIVE_FLAG_FUNC);  		//感烟任务完成，清感烟任务
		FuncData.Interval = 128;        					//默认间隔是4秒
		return;
	}
	#ifdef CODE_SEL_SMOKE_BUZZLE
	//如果当前蜂鸣器正在工作也不采样
	if(ACTIVE_FLAG_SMK_BEEP == (SysData.CmdActive&ACTIVE_FLAG_SMK_BEEP))
	{
		return;
	}
	#endif
	//----------信号检测----------	
	//开运放
	//目前的暗电流实际就是运放的偏置电压；
	//不论红光、蓝光真实的暗电流实际是基本没有的，放大不出来的。
	//采集暗电流过程中，被中断打断只是延迟，并无影响

	//修改为先点蓝光:因为蓝光压降大，涉及到充电时间
	//更换恒润的双波段发射管后，调整放大倍数 900K 200K 10K
	#ifdef CODE_SEL_SMOKE_LIGHTON
		AfeInit(15,AFE_R3_10K,2,OPAMODE_INIT);
	#endif
	#ifdef CODE_SEL_SMOKE_HENGRUN
		AfeInit(9,AFE_R3_10K,2,OPAMODE_INIT);
	#endif
	//对于此处的特殊处理的解释:
	//片内的LDO电压调整率堪忧，在有一定负载电流的情况下，通讯的低电压造成LDO输出产生一个脉冲。
	//造成了AD采集的不稳定。
	//这个脉冲的下降沿在蜂鸣器响起时，达到5ms。通讯结束达到了1ms。
	//为避免，给通讯中断增加>1ms的延时。SdOpaSwitch(1)本身带有2ms延时，故而将其也封进临界段
	FuncData.FlagBreak = 0;	//初始化中断标志  
	#ifdef CODE_SEL_SMOKE_COMPENSATION_BY_NTC
		GPO_SEN_PWR_1();	//开启NTC供电
	#endif //CODE_SEL_SMOKE_COMPENSATION_BY_NTC
	SdOpaSwitch(1);	//开运放 OPA打开延时2ms+2ms	
	//	GCC_DELAY(500);
	#ifdef CODE_SEL_SMOKE_COMPENSATION_BY_NTC
		#ifdef CODE_SEL_SMOKE_BUZZLE
			FuncData.TempVal = AdcSample(2);	//CH2
		#else
			FuncData.TempVal = AdcSample(3);	//CH3
		#endif
		
		GPO_SEN_PWR_0();	//关闭NTC供电	
		// if((FuncData.TempVal<=660)  &&  ((FuncData.State & FUNC_SD_ST_COMPENSATION)==0))
		// {
		// 	if(FuncData.TempCnt>=10)	//连续10次检测到温度超过后进行补偿
		// 	{
		// 		//启动补偿		
		// 		FuncData.State |= FUNC_SD_ST_COMPENSATION;
		// 	}
		// 	else
		// 	{
		// 		FuncData.TempCnt++;	
		// 	}
		// }
		// else if((FuncData.TempVal>=700)  &&  ((FuncData.State & FUNC_SD_ST_COMPENSATION)!=0))
		// {
		// 	if(FuncData.TempCnt<=0)		
		// 	{
		// 		//启动补偿		
		// 		FuncData.State &= ~FUNC_SD_ST_COMPENSATION;
		// 	}
		// 	else
		// 	{
		// 		FuncData.TempCnt--;
		// 	}
		// }
		if((FuncData.TempVal>=2730))
		{
			if(FuncData.TempCnt>=10)	//连续10次检测到温度超过后进行补偿
			{
				//启动补偿		
				FuncData.State |= FUNC_SD_ST_COMPENSATION;
			}
			else
			{
				FuncData.TempCnt++;	
			}
		}
		else if((FuncData.TempVal<=2630)  &&  ((FuncData.State & FUNC_SD_ST_COMPENSATION)!=0))
		{
			if(FuncData.TempCnt<=0)		
			{
				//启动补偿		
				FuncData.State &= ~FUNC_SD_ST_COMPENSATION;
			}
			else
			{
				FuncData.TempCnt--;
			}
		}
	#endif //CODE_SEL_SMOKE_COMPENSATION_BY_NTC
	//------------------------------------------			
	//采集暗电流
	//FuncData.SD_Dark_OPA0 = AdcSample(5);			//5=运放0输出,暗电流
	//FuncData.SD_Dark_OPA0 = FuncData.SD_Dark_OPA1;	//记录前一次采样值 
	FuncData.SD_Dark_OPA1 = AdcSample(6);			//6=运放1输出,暗电流	
	//------------------------------------------
	//蓝光OPAMODE_INIT
	//仅当当前红外有一定值时，才打开蓝光;原因是蓝光的衰减比较厉害，减少打开蓝光的时间
	//FuncData.SD_IRed_OPA1 是上一个周期，减掉暗值、洁净空气、校准后、漂移补偿后的值
	//当漂移补偿后，虽然红光有本底值也将让蓝光不工作，符合使用逻辑
	//if((FuncData.SD_IRed_OPA1 > 250)
	//FuncData.SD_IRed_OPA0
	#if defined(CODE_SEL_CONFIG_PARA_FIXED)  //���������ﵽ100�����������
		if((FuncData.SD_IRed_OPA1 > 100)
		||(0x55 == FuncData.EolCmd))  
	#else
	TempDat = MemData.Rom[EEPROM_CONFIG1];
	TempDat <<= 1;
	if((FuncData.SD_IRed_OPA1 > TempDat)
	||(0x55 == FuncData.EolCmd))   
	#endif
	{
			if(FuncData.SD_SampleFlag==0)
			{
				FuncData.SD_SampleFlag = 2;				
			}
			SINK_EN();
		BLUE_ON();										//开蓝光
		GCC_DELAY(200); 								//延时200us@4MHz	
		//FuncData.SD_Blue_OPA0 = AdcSample(5);			//6=运放1输出,亮电流
		//FuncData.SD_Blue_OPA0 = FuncData.SD_Blue_OPA1;  //前一次采样值
			FuncData.SD_Blue_OPA1 = AdcSample(6);			//6=运放1输出,亮电流
			BLUE_OFF();
			SINK_DISABLE();	
			if(FuncData.FlagBreak > 0)
			{
				SdOpaSwitch(0);//���˷�
				FuncData.SD_SampleFlag = 0;
				return;
			}
	}
		else
		{
			//��ʼ���״β������	
			//��ֹ���������Ե�ʱ��������
			FuncData.SD_SampleFlag = 0;	
		}
	//------------------------------------------	
	//红外
	SINK_EN();
	IRED_ON();									//开红外
	GCC_DELAY(200);								//延时200us@4MHz	
	//FuncData.SD_IRed_OPA0 = AdcSample(5);			//6=运放0输出,亮电流
	//FuncData.SD_IRed_OPA0 = FuncData.SD_IRed_OPA1; //前一次采样值
	FuncData.SD_IRed_OPA1 = AdcSample(6);			//6=运放1输出,亮电流
	IRED_OFF();
	SINK_DISABLE();
	//关运放
	SdOpaSwitch(0);								//关运放
	if(FuncData.FlagBreak > 0)
	{
		return;
	}
	//输出原始采样值,平均值
	#ifdef CODE_SEL_SMOKE_AVG

	if(FuncData.SD_SampleFlag == 2)
	{
		FuncData.SD_SampleFlag = 1;
		FuncData.SD_Dark_OPA0 = FuncData.SD_Dark_OPA1;
		FuncData.SD_Blue_OPA0 = FuncData.SD_Blue_OPA1;
		FuncData.SD_IRed_OPA0 = FuncData.SD_IRed_OPA1;
	}
	
	FuncData.Value 	  = (FuncData.SD_IRed_OPA1 + FuncData.SD_IRed_OPA0)/2;	//红外采样值
	FuncData.ValueCh1 = (FuncData.SD_Blue_OPA1 + FuncData.SD_Blue_OPA0)/2;	//蓝光采样值
	FuncData.ValueCh2 = (FuncData.SD_Dark_OPA1 + FuncData.SD_Dark_OPA0)/2;	//暗电流值

	FuncData.SD_IRed_OPA1 = FuncData.Value;
	FuncData.SD_Blue_OPA1 = FuncData.ValueCh1;
	FuncData.SD_Dark_OPA1 = FuncData.ValueCh2;
	
	FuncData.SD_Dark_OPA0 = FuncData.SD_Dark_OPA1;	//记录前一次采样值 
	FuncData.SD_Blue_OPA0 = FuncData.SD_Blue_OPA1;  //前一次采样值
	FuncData.SD_IRed_OPA0 = FuncData.SD_IRed_OPA1; //前一次采样值

	#else
	FuncData.Value 	  = FuncData.SD_IRed_OPA1 ;	//红外采样值
	FuncData.ValueCh1 = FuncData.SD_Blue_OPA1 ;	//蓝光采样值
	FuncData.ValueCh2 = FuncData.SD_Dark_OPA1 ;	//暗电流值
	#endif
	/////////////////////////////////////////////////
	//Step1.去暗值
	//红光去暗值
	if(FuncData.SD_IRed_OPA1 > FuncData.SD_Dark_OPA1)
	{
		FuncData.SD_IRed_OPA1 -= FuncData.SD_Dark_OPA1;
	}
	else
	{
		FuncData.SD_IRed_OPA1 = 0;
	}
	//蓝光去暗值
	if(FuncData.SD_Blue_OPA1 >  FuncData.SD_Dark_OPA1)
	{
		FuncData.SD_Blue_OPA1 -= FuncData.SD_Dark_OPA1;
	}
	else
	{
		FuncData.SD_Blue_OPA1 = 0;
	}
	//Step2.去洁净空气值
	if(FuncData.SD_IRed_OPA1 > FuncData.SD_ClrAir_IR)
	{
		FuncData.SD_IRed_OPA1 -= FuncData.SD_ClrAir_IR;
	}
	else
	{
		FuncData.SD_IRed_OPA1 = 0;
	}
	if(FuncData.SD_Blue_OPA1 >  FuncData.SD_ClrAir_BLUE)
	{
		FuncData.SD_Blue_OPA1 -= FuncData.SD_ClrAir_BLUE;
	}
	else
	{
		FuncData.SD_Blue_OPA1 = 0;
	}
	
	//Step3.校准 校准系数放大了128倍
	if(FuncData.State &= FUNC_SD_ST_COMPENSATION)
	{
		//需要温度补偿
	uiIrTemp = (uint32)(FuncData.SD_IRed_OPA1)*MemData.Rom[EEPROM_ID_IR_CALI];
		uiIrTemp = uiIrTemp * (uint32)(MemData.Rom[EEPROM_ID_IR_COMPEN]);//温度补偿
		uiIrTemp >>= 14;  //校准系数放大128 * 164倍存储
		if(uiIrTemp>=4096)  uiIrTemp =4096;//防止错误
		FuncData.SD_IRed_OPA1 = (uint16)uiIrTemp;
	}
	else
	{
		//无需温度补偿
		uiIrTemp = (uint32)(FuncData.SD_IRed_OPA1)*MemData.Rom[EEPROM_ID_IR_CALI];
	uiIrTemp >>= 7;  //校准系数放大128倍存储
	if(uiIrTemp>=4096)  uiIrTemp =4096;//防止错误
	FuncData.SD_IRed_OPA1 = (uint16)uiIrTemp;
	}
	uiBlTemp = (uint32)(FuncData.SD_Blue_OPA1)*MemData.Rom[EEPROM_ID_BL_CALI];
	uiBlTemp >>= 7;
	if(uiBlTemp>=4096)  uiBlTemp =4096;//防止错误
	FuncData.SD_Blue_OPA1 = (uint16)uiBlTemp;

	#ifdef CODE_SEl_SMOKE_IR_AND_BLUE  //红蓝光比值算法
		/////////////////////////////////////////////////////////////
		//故障判断
			//high chamber
			if(FuncData.SD_Dark_OPA1 > FUNC_SD_TH_DARK_H)
			{
				if(FuncData.CntShort_HighCham<3)
				{
					FuncData.CntShort_HighCham++;
				}
				else
				{
					FuncData.State |= FUNC_SD_ST_HIGHCHAM_FAULT;
				}
			}
			else
			{
				if(FuncData.CntShort_HighCham>0)
				{
					FuncData.CntShort_HighCham--;
				}
				else
				{
					FuncData.State &= (~FUNC_SD_ST_HIGHCHAM_FAULT);
				}
			}
			//low chamber
			// 暗值偏小 或者 红光小于洁净值的1/4 
			// 不再判断蓝光。当红光比较小时，实际上不启动蓝光。
			if(FuncData.SD_Dark_OPA1 < FUNC_SD_TH_DARK_L)
			{
				if(FuncData.CntOpen_LowCham < 3)
				{
					FuncData.CntOpen_LowCham++;
				}
				else
				{
					FuncData.State |= FUNC_SD_ST_LOWCHAM_FAULT;
				}
			
			}
			else
			{
				if(FuncData.CntOpen_LowCham > 0)
				{
					FuncData.CntOpen_LowCham--;
				}
				else
				{
					FuncData.State &= (~FUNC_SD_ST_LOWCHAM_FAULT);
				}
			}
	//-------------------------------------------------
		//��������/���ı�ֵ
		if((FuncData.State & FUNC_SD_ST_FAULT) == 0x0000)
		{
			//�޹��Ͻ��к������ֵ����
			#if defined(CODE_SEL_CONFIG_PARA_FIXED)
				//�����������ֵ����150��������ֵ�ļ���
				if(((FuncData.SD_Blue_OPA1 > 150) ||
					(FuncData.SD_IRed_OPA1 > 150)) && 
					(SdkeePotData.SD_KePot_Flag!=2))
			#else 
	TempDat = MemData.Rom[EEPROM_CONFIG2];
	TempDat <<= 1;
				//�����������ֵ����150��������ֵ�ļ���
				if(((FuncData.SD_Blue_OPA1 > TempDat) ||
					(FuncData.SD_IRed_OPA1 > TempDat)) && 
					(SdkeePotData.SD_KePot_Flag!=2))
			#endif
		{	
		SdkeePotData.SD_KePot_Flag = 1;
		SdkeePotData.SD_KePot_QuitCnt = 0x00;
		//max 255*32/6
				//��ֵ�Ŵ�32�� 
				TempDat = FuncData.SD_Blue_OPA1;
				TempDat <<= 5;
				uiRadioBlueIR = TempDat/FuncData.SD_IRed_OPA1;
				//uiRadioBlueIR = ((uint32)FuncData.SD_Blue_OPA1<<5)/FuncData.SD_IRed_OPA1;
		if(uiRadioBlueIR > 512)
			{
			uiRadioBlueIR = 512;
				}
		//比值放大32倍 受限uint8 类型，实际允许的比值最大7.96
		FuncData.SD_Radio_BlueIR = uiRadioBlueIR;
						SdkeePotData.SD_KePot_CheckCnt++;
				#if defined(CODE_SEL_CONFIG_PARA_FIXED)
					if(SdkeePotData.SD_KePot_CheckCnt < 40)
				#else
		if(SdkeePotData.SD_KePot_CheckCnt < MemData.Rom[EEPROM_CONFIG6])
				#endif
					{
			//SdkeePotData.SD_KePot_CheckCnt=0;
			SdkeePotData.SD_KePot_Sum += FuncData.SD_Radio_BlueIR; 	
					}
						else
						{
			//实际比值
#if defined(CODE_SEL_CONFIG_PARA_FIXED)
						FuncData.SD_Radio_BlueIR = SdkeePotData.SD_KePot_Sum/40;
					#else
			FuncData.SD_Radio_BlueIR = SdkeePotData.SD_KePot_Sum/MemData.Rom[EEPROM_CONFIG6];
					#endif
					//if(FuncData.SD_Radio_BlueIR >= BLUE_IR_RADIO_TH)  //<26 ��Ϊ������
					#if defined(CODE_SEL_CONFIG_PARA_FIXED)
						if(FuncData.SD_Radio_BlueIR >= 32)
					#else
						if(FuncData.SD_Radio_BlueIR >= MemData.Rom[EEPROM_CONFIG5])  //<26 ��Ϊ������
					#endif
					{
				//真火
							FuncData.SD_SmokeType_Flag = FUNC_SD_SMOKETYPE_TRUEFIRE;//升烟类型为真火
						}
			SdkeePotData.SD_KePot_CheckCnt=0;
			SdkeePotData.SD_KePot_Flag = 2;
					}
				}
	else
				{
		 //防止烟雾抖动
		 if(SdkeePotData.SD_KePot_Flag ==1)
					{
						SdkeePotData.SD_KePot_QuitCnt++;
			if(SdkeePotData.SD_KePot_QuitCnt>=3)
					{
				SdkeePotData.SD_KePot_QuitCnt = 0x00;
				SdkeePotData.SD_KePot_CheckCnt = 0x00; 
						SdkeePotData.SD_KePot_Flag = 0;
				FuncData.SD_SmokeType_Flag = FUNC_SD_SMOKETYPE_SMOKETEST; //烟测
				FuncData.SD_Radio_BlueIR = 0x00;
				SdkeePotData.SD_KePot_Sum  = 0;
					}
				}
					}

		}
		else
		{
			//��Ʒ�����ҹ���
			SdkeePotData.SD_KePot_QuitCnt = 0x00;
			SdkeePotData.SD_KePot_CheckCnt = 0x00; 
			SdkeePotData.SD_KePot_Flag = 0;
			FuncData.SD_SmokeType_Flag = FUNC_SD_SMOKETYPE_SMOKETEST; //�̲�
			FuncData.SD_Radio_BlueIR = 0x00;
			SdkeePotData.SD_KePot_Sum  = 0;
		}
		//����PW4��ֵ
					if (FuncData.SD_SmokeType_Flag == FUNC_SD_SMOKETYPE_TRUEFIRE)
					{
						//真火
						FuncData.ValuePW4 = 3000;
			#if defined(CODE_SEL_CONFIG_PARA_FIXED)
				if ((FuncData.SD_Blue_OPA1 < 150) && (FuncData.SD_IRed_OPA1 < 150))
			#else
						TempDat = MemData.Rom[EEPROM_CONFIG2];
						TempDat <<= 1;
						if ((FuncData.SD_Blue_OPA1 < TempDat) && (FuncData.SD_IRed_OPA1 < TempDat))
			#endif
						{
							//真火下，如果红蓝光同时小于阈值，恢复到验箱状态
							FuncData.SD_SmokeType_Flag = FUNC_SD_SMOKETYPE_SMOKETEST; //烟测
							SdkeePotData.SD_KePot_Flag = 0;
							FuncData.SD_Radio_BlueIR = 0x00;
							SdkeePotData.SD_KePot_QuitCnt = 0x00;
							SdkeePotData.SD_KePot_CheckCnt = 0x00;
							SdkeePotData.SD_KePot_Sum = 0;
						}
					}
					else
					{
						if (FuncData.SD_IRed_OPA1 < 16)
						{
							FuncData.ValuePW4 = 900;
						}
						else
						{
							TempDat = FuncData.SD_IRed_OPA1;
				#if defined(CODE_SEL_CONFIG_PARA_FIXED)
					if(2 == MemData.Rom[EEPROM_ID_SENSITIVITY])
					{
					TempDat <<= 6;
					TempDat /= 100;
					}
					else
					{
						TempDat <<= 7;
						TempDat /= 100;
					}
					
				#else
							TempDat <<= MemData.Rom[EEPROM_CONFIG4];
							TempDat /= MemData.Rom[EEPROM_CONFIG3];
				#endif
							if (TempDat >= 1100)
								TempDat = 2100;
							FuncData.ValuePW4 = ((uint16)TempDat + 900);
						}
					}
#endif  //CODE_SEl_SMOKE_IR_AND_BLUE


	//----------采样间隔----------	
	//默认定时间隔
	SysData.CmdActive &=  (~ACTIVE_FLAG_FUNC);  //感烟任务完成，清感烟任务

	//默认工作在4秒 31.25ms*128
	FuncData.Interval = 128;        //默认间隔是4秒

	//如果有烟后工作在2秒。
	if((FuncData.SD_IRed_OPA1 > 150)  || (FuncData.SD_Blue_OPA1 >150))
	{
		//红光或者蓝光大于150就启动加速采样
		FuncData.Interval = 64;						//采样间隔2秒
	}

	if(0x55 == FuncData.EolCmd)
	{
		FuncData.Interval = 64;   //加速采样命令2秒	
	}
	
}
//------------------------------
//感烟探测器的蜂鸣器功能
//由于感烟的芯片是5542	AV的芯片是5541
//虽然代码可以通用，为了便于不同系统的条件编译好实现才如此
//资源限制:蜂鸣器使用的STM，同时也在分组查询闪灯定时中时钟。二者有冲突
//------------------------------
#ifdef CODE_SEL_SMOKE_BUZZLE
void SmokeDetectorBuzzle(void)
{	
	
    //更新控制命令后复位状态机和声音,灯的状态
	//避免状态机运行中改变控制逻辑
	#if 1
	if(FuncData.CtrlOut != FuncData.AV_CtrlRecord)
	{
		FuncData.AV_CtrlRecord = FuncData.CtrlOut;

		FuncData.AV_Status = STATUS_AV_IDLE;
		SmokeBuzzlePwm(0);
		FuncData.AV_ActiveCycCnt =0; 
		
		//声命令打开
		if(FUNC_CTRL_AV_A_ON == FuncData.AV_CtrlRecord)
		{
			FuncData.ucAvControlTmp |= FUNC_CTRL_AV_BIT_SOUND; //声打开
			FuncData.State |= FUNC_AV_ST_SOUND_ON;
		}
		else if(FUNC_CTRL_AV_A_OFF == FuncData.AV_CtrlRecord)
		{
			FuncData.ucAvControlTmp &= ~FUNC_CTRL_AV_BIT_SOUND; //声关闭
			FuncData.State &= ~FUNC_AV_ST_SOUND_ON;
		}
		
	}
	#endif

	if((FuncData.ucAvControlTmp&FUNC_CTRL_AV_BIT_AV)
	&&(0 == FuncData.StmQueryLedBusy))  //stm没有被查询LED闪烁功能使能
	{	
		//同步命令在接收到0XFA命令后使能
		//在主循环中的refreshData中进行，确保实时性
		switch(FuncData.AV_Status)
		{
			case STATUS_AV_IDLE:
			{
 				FuncData.AV_Status = STATUS_AV_START;  //转入启动周期
				FuncData.AV_ActiveCycCnt = 2;  //活动状态3各周期 当前周期已经是一个周期了				
				SysData.CmdActive |= ACTIVE_FLAG_RUN;    		
			}
			break;

			case STATUS_AV_START:
			{
				FuncData.AV_Status = STATUS_AV_SOUND_SET;  //延时转入发声状态
				//Interval 30ms -1  
				//FuncData.AV_GroupNum 最小值为1，初始化时赋值，更新值时做范围限制
					
				FuncData.IntervalAV = FuncData.AV_GroupId
				*(AvGroupConfig[AVGRPCFG_SOUNDTIME]
				+AvGroupConfig[AVGRPCFG_LIGHTTIME]); //groupID 0,1,2,3
				//如果是第0组，不延时,保持单片机唤醒状态;直接下一个循环后进入状态机切换
				SysData.CmdActive |= ACTIVE_FLAG_RUN;    		
			}
			break;

			case STATUS_AV_SOUND_SET:
			{				
				//如果声音打开了
				if(FuncData.ucAvControlTmp&FUNC_CTRL_AV_BIT_SOUND)
				{
					SmokeBuzzlePwm(1);
					SysData.CmdActive |= ACTIVE_FLAG_SMK_BEEP;  //声光开始工作
				}
				FuncData.AV_Status = STATUS_AV_SOUNDOFF_LIGHTON;
				FuncData.IntervalAV = AvGroupConfig[AVGRPCFG_SOUNDTIME];
					
			}
			break;

			case STATUS_AV_SOUNDOFF_LIGHTON:
			{
				if(FuncData.ucAvControlTmp & FUNC_CTRL_AV_BIT_SOUND)   //声音
				{
					SmokeBuzzlePwm(0);
					SysData.CmdActive &= ~ACTIVE_FLAG_SMK_BEEP; //声音关后，单片机不必保持唤醒

					//片内的LDO电压调整率堪忧，在有一定负载电流的情况下，通讯的低电压造成LDO输出产生一个脉冲。
					//造成了AD采集的不稳定。
					//这个脉冲的下降沿在蜂鸣器响起时，达到5ms。通讯结束达到了1ms。
					//强制感烟延时至少1个周期后，再进行采样
					if(FuncData.Interval<2)
					{
						FuncData.Interval++;
					}
				}

				FuncData.AV_Status = STATUS_AV_LIGHTOFF_BACKIDLE;
				FuncData.IntervalAV = (AvGroupConfig[AVGRPCFG_LIGHTTIME]);
			}
			break;

			case STATUS_AV_LIGHTOFF_BACKIDLE:
			{
				if(FuncData.AV_ActiveCycCnt)
				{
					FuncData.AV_Status = STATUS_AV_START;
					FuncData.AV_ActiveCycCnt--;
					
				}
				else
				{
					FuncData.AV_Status = STATUS_AV_IDLECYC_START;
				}
				FuncData.IntervalAV = AvGroupConfig[AVGRPCFG_CYCLETIME]
					                -((FuncData.AV_GroupId+1)*
					                (AvGroupConfig[AVGRPCFG_SOUNDTIME]
									+AvGroupConfig[AVGRPCFG_LIGHTTIME]));
				
			}	
			break;

			//2个空闲周期开始
			case STATUS_AV_IDLECYC_START:
			{
				FuncData.IntervalAV = AvGroupConfig[AVGRPCFG_CYCLETIME]*2;
				FuncData.AV_Status = STATUS_AV_IDLE;
			}
			break;


			default:;
			break;
		
		}
	}
	else
	{
		//声光都关闭时
		FuncData.AV_ActiveCycCnt =0; 
		FuncData.AV_Status = STATUS_AV_IDLE;
		
		SysData.CmdActive &= ~ACTIVE_FLAG_SMK_BEEP; //声音关后，单片机不必保持唤醒

		//工厂测试命令
		switch(FuncData.EolCmd)
		{
			//只执行一次
			case 0xB1:{SmokeBuzzlePwm(2);  //工厂测试音
					   SysData.CmdActive |= ACTIVE_FLAG_FUNC;
					   FuncData.EolCmd = 0xFF;}break;
			case 0xB0:{SmokeBuzzlePwm(0);
					   SysData.CmdActive &= ~ACTIVE_FLAG_FUNC;
					   FuncData.EolCmd = 0;}break;
					   
			case 0xFF:
			{
				SysData.CmdActive |= ACTIVE_FLAG_FUNC;  //仅执行一次工厂命令，且不能影响非工厂测试状态
			}
			break;
				
			//不处于工厂测试命令时，关闭声音和灯
			default:
			{
				if(0 == FuncData.StmQueryLedBusy)
				{
					SmokeBuzzlePwm(0);
				}
			};
			break;
		}
		FuncData.IntervalAV = 1;
	}

	//唤醒周期已修改为31.25ms
}
#endif



#endif





#ifdef CODE_SEL_HEAT
/*******************************************************************************
* Function	: HeatDetector
* Brief		: 感温报警器功能函数
*******************************************************************************/
void HeatDetector (void)
{
	//置感烟任务
	SysData.CmdActive |=  ACTIVE_FLAG_FUNC;
	//----------开始检测----------
	if(CommData.Step > COMM_STEP_IDLE)			//通信中不检测
		return;
	HeatDetectFunction();
	//报警判断
	//统一感温算法应用的5度上升判断已经是一种滤波了
	//为提高响应速度不再有报警滤波
	if(FUNCSTATUS_TMPALARM == HeatAlgorithmRegs.AlarmFlag)
	{
		FuncData.State &= ~(FUNC_HT_ST_NTCOPEN);     //清开路故障
		FuncData.State |= FUNC_HT_ST_ALARM;          //置报警
	}

	//AD码值异常低，置开路故障
	//故障时，清报警标志
	if(ERRCODE_ADVALBLL == FuncData.FlagAdCodeStaus)
	{
		if(FuncData.CntFault < 2)
		{
			FuncData.CntFault ++;
		}
		else
		{
			FuncData.State |= FUNC_HT_ST_NTCOPEN;      //置故障
			FuncData.State &= ~(FUNC_HT_ST_ALARM);		//清报警
			FuncData.State &= ~(FUNC_HT_ST_NTCSHORT);   //清短路
		}
	}
	else if(ERRCODE_ADVALABC == FuncData.FlagAdCodeStaus)//异常高，置短路故障
	{
		if(FuncData.CntFault < 2)
		{
			FuncData.CntFault ++;
		}
		else
		{
			FuncData.State |= FUNC_HT_ST_NTCSHORT;      //置故障
			FuncData.State &= ~(FUNC_HT_ST_ALARM);		//清报警
			FuncData.State &= ~(FUNC_HT_ST_NTCOPEN);    //清开路
		}	
	}
	else
	{
		FuncData.CntFault = 0;
		FuncData.State &= ~(FUNC_HT_ST_NTCOPEN);
		FuncData.State &= ~(FUNC_HT_ST_NTCSHORT);
	}

	FuncData.Value	= HeatAlgorithmRegs.NtcRealTemp;//输出NTC实测温度值*8
	FuncData.ValueCh1 = HeatAlgorithmRegs.AdCodeFilter;//AD 12位
	FuncData.ValueCh2 = HeatAlgorithmRegs.CompTemp;//补偿后的温度值
	//感温探测器CH4 用于显示恒温温度值。其他类型ch4用于显示通讯比较点评设置值
	FuncData.ValueCh4 =	HeatAlgorithmRegs.CnstTmpRecord;//恒温温度
	
	SysData.CmdActive &=  (~ACTIVE_FLAG_FUNC); 
	
	//----------采样周期----------
	FuncData.Interval = 134;						//定时间隔4020ms

	if(0x55 == FuncData.EolCmd)
	{
		FuncData.Interval = 67;   //加速采样命令2秒	
	}

}


#endif


void drawForPWOutputTimer(uint16 pw){
	
	#ifdef CODE_SEL_SMOKE
	
		// configDrawCurTimer();
		int16 temp;
		//Enable ptmae for enabling ptm compA interrupt
		// fH / 64 = 16us
		temp = pw >> 4;
		//Optimize adjust timer with running duration
		temp -= 2;
		_ptmaf = 0;
		_ptmal = ((uint8)(temp & 0x00FF));  // 3500 pw4  = max 218 
		_ptmah  = 0;
		//Reset ptm counter STEP1 for draw current
		_pton = 0;
		//Reset ptm counter STEP2 for draw current
		_pton = 1;
		_ptmae = 1;
		// _pa3 = 0;
	#else
		// configDrawCurTimer();
		int16 temp;
		//Enable ptmae for enabling ptm compA interrupt
		// fH / 64 = 16us
		temp = pw >> 4;
		//Optimize adjust timer with running duration
		temp -= 2;
		_ptm0af = 0;
		_ptm0al = ((uint8)(temp & 0x00FF));  // 3500 pw4  = max 218 
		_ptm0ah  = 0;
		//Reset ptm counter STEP1 for draw current
		_pt0on = 0;
		//Reset ptm counter STEP2 for draw current
		_pt0on = 1;
		_ptm0ae = 1;
		// _pa3 = 0;
	#endif
}

/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
