/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      main.c
 * @author    S
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     Firmware for Low cost device HT45F5542
 *            FileNumber: S02-
 ******************************************************************************/
// #define MAIN_VER1		(0x01)		//ProductVersion
// #define SUB_VER2		(0x16)		//SoftwareVersion    //modified by txx 20220609
/******************************************************************************/
#include <STRING.H>
#include "include.h"
//编译版本号 在EE的0X0F位置
//普通感烟 LihgtON 001;HengRun 002
//蜂鸣器感烟 LihgtON 011 ;HengRun 012  
//ISO 110
//AV  120
//HDY MCP INF  130
//OUTPUTPLUSE  140
//单稳态有源输出   150
//磁保持有源输出   151
//感温  200
#ifdef CODE_SEL_SMOKE
	#define COMPLIER_VER_HUNDREDS   (0u)
	//十位定义:蜂鸣器感烟1，普通感烟0
	#ifdef CODE_SEL_SMOKE_BUZZLE 
		#define COMPLIER_VER_TENS  (1U)
	#else
		#define COMPLIER_VER_TENS  (0U)
	#endif
	//个位定义
	#ifdef CODE_SEL_SMOKE_LIGHTON
		#define COMPLIER_VER_ONES   (1u)
	#endif 

	#ifdef CODE_SEL_SMOKE_HENGRUN
		#define COMPLIER_VER_ONES   (2u)
	#endif
#endif



#ifdef CODE_SEL_HEAT
	#define COMPLIER_VER_HUNDREDS   (2u)
	#define COMPLIER_VER_TENS       (0u)
	#define COMPLIER_VER_ONES       (0U)
#endif

/////////////////////////////////////////////
//#define COMPLIER_VER      (COMPLIER_VER_HUNDREDS*100+COMPLIER_VER_TENS*10+COMPLIER_VER_ONES) //编译版本
//------------------------------
//全局变量
//------------------------------
#ifdef CODE_SEL_SMOKE 
SdkeenPotSetting SdkeePotData; //感烟拐点寻找 缩小func结构体 便于两个bank ram的分配
#endif

SysDataRegs		SysData;	//系统数据
MemDataRegs		MemData;	//存储数据
CommDataRegs	CommData;	//通信数据
FuncDataRegs	FuncData;	//功能数据
//------------------------------
//------------------------------
//主程序
//------------------------------
void main()
{
	//系统初始化
	SystemInit();
	//数据初始化
	DataInit();
	//开全局中断
	EnableInterrupts();
	//----------主循环体----------
	while(1)
	{	
		
		#ifdef CODE_SEL_SMOKE  
		//当前主循环空运行100us@4MHz
		//在31.25ms唤醒周期下
		//如果上电没有校准AMP,运行AMP校准程序
		if(0 ==  FuncData.FlagAmpCalid)
		{
			//上电后，延时等待总线电压稳定后，进入校准AMP程序,
			//如果连续两次(唤醒后主循环都运行两次)都没有能成功校准，则下一个31.25ms执行
			if(0 == FuncData.AmpCaliInterval)
			{
				SdCaliOpa(); 
			}
		}
		#endif

		////////////////////////////////////////
		//蜂鸣器感烟增加上电闪三次灯

		#ifdef CODE_SEL_SMOKE_BUZZLE
		if(0 == SdkeePotData.BuzzleSmokeFstComInterval)   //500ms间隔到了
		{
			//如果首次通讯已经来了
			//if(1 == SdkeePotData.BuzzleSmokeFstComFlag)
			{
				if(SdkeePotData.BuzzleSmokeFstComLedCnt > 0)
				{
					SdkeePotData.BuzzleSmokeFstComLedCnt--;
					SdkeePotData.BuzzleSmokeFstComInterval = 16;   //500ms间隔
					GPO_SMOKE_RED_ON();    
					GPO_SMOKE_LED_ON();  //门灯
					GCC_DELAY(10000);
					GPO_SMOKE_RED_OFF();
					GPO_SMOKE_LED_OFF();  //门灯
				}
			}
		}
		#endif
		
	#if 1
	// while(FuncData.FlagBreak)
	// {
	// 	;
	// }
	//TODO tobe added in 5541
	if(FuncData.FlagBreak == 0)
	{
		///////////////////////////////////////
		//功能函数
		if (FuncData.Interval == 0) //定时结束执行
		{
			switch (SysData.MyType)
			{

			//-------------------------------------------------------------------------
			#ifdef CODE_SEL_SMOKE
				case CFG_TYPE_SMOKE: //带蜂鸣器的感烟探测器
				case CFG_TYPE_SMOKE_BUZZER:
				{

					SmokeDetector(); // 31.25ms中断查询一次	 2秒运行一次
				}
				break;

			#endif
//-------------------------------------------------------------------------
			#ifdef CODE_SEL_HEAT
				case CFG_TYPE_HEAT:
				case CFG_TYPE_HEAT_R:
				{
					HeatDetector();
				}
				break;
			#endif

				default:;
					break;
				}
			}

#ifdef CODE_SEL_SMOKE_BUZZLE
			//用于带蜂鸣器的感烟的蜂鸣器部分周期
			if (CFG_TYPE_SMOKE_BUZZER == SysData.MyType)
			{
				if (0 == FuncData.IntervalAV) //运行周期和感烟不一样，所以需要独立的Interval
				{
					SmokeDetectorBuzzle();
				}
			}
			#endif
			//刷新LED
			LedCtrl();
			//刷新状态变量
			RefreshData();
	}

#endif

		//存储器处理程序
		if(MemData.WrBuffCnt) //写EE数据队列中有数据,触发1次写操作
		{
			MemoryWrite();
		}
		MemoryProcess();
		//RefreshData();
		if(SysData.CmdReset > 0)
		{
			if(CommData.Step == COMM_STEP_IDLE)
			{
				SystemReset();//通信完成时操作
			}
		}

		//看门狗复位
		#ifdef ENABLE_WD
		GCC_CLRWDT();
		#endif	

		//休眠控制
		//1.每次唤醒都会运行2次，设计目的是应对:当满足了SysData.CmdActive<2的判断时，
		//出现异步中断，重新置上了相关的任务标志，此时返回主循环后，会立刻进入休眠。
		//需要在第二个周期唤醒后，才能处理。
		//2.该设计，只是降低了上述情况的概率。第二次唤醒周期后，也可能碰到上述情况。
		//3.周期中断唤醒，在休眠机制下实际是同步的进程;在低速时钟运行模式下才是异步进程。
		if(SysData.CmdActive >= ACTIVE_FLAG_RUN)		//单次唤醒
		{
			SysData.CmdActive -= ACTIVE_FLAG_RUN;		//清除
		}
//		else if (SysData.CmdActive == ACTIVE_FLAG_IDLE)	//检查激活状态
		//在通讯的帧间INTB,也进入休眠
		else if (SysData.CmdActive < 1)    					
		{
			GCC_HALT();		//待机	
			GCC_NOP();
		}
	}
	//异常终止
	GCC_HALT();
}

//------------------------------
//数据初始化程序
//------------------------------
void DataInit (void)
{
	//全部变量初始化
	#ifdef CODE_SEL_SMOKE
	(void) memset(((void *) &SdkeePotData),		0,sizeof(SdkeenPotSetting));	
	#endif
	(void) memset(((void *) &SysData),		0,sizeof(SysDataRegs));	
	(void) memset(((void *) &MemData),		0,sizeof(MemDataRegs));	
	(void) memset(((void *) &CommData),		0,sizeof(CommDataRegs));
	(void) memset(((void *) &FuncData),		0,sizeof(FuncDataRegs));
	//读取EEPROM数据
	MemoryInit();
	
	//更新DLIP寄存器和状态寄存器值
	RefreshData();
	//初始化功能函数非零变量
	//上电处于unMonitor状态
	FuncData.State	= (FUNC_ST_ENABLE | FUNC_ST_CONTROLLABLE);	//使能和可输出状态
	FuncData.Interval 	= 32;		//首次FUNC等待默认 32*31.25ms = 1000ms
	#ifdef CODE_SEL_SMOKE
	FuncData.SD_SampleFlag = 0;
	#endif
	FuncData.ValuePW4 = 900;//上电初始化初值，防止第一次通信数据错误
	//根据产品类型进行特定的端口和初始状态设置
	ConfigType(SysData.MyType);	
	FuncData.RemoteTestInd = 0;	
	CommData.CLIPCmdLast = 1;		
}

//------------------------------
//根据不同的产品类型。进行特定的配置信息
//------------------------------
void ConfigType(uint8 Typetmp)
{
	switch(Typetmp)
	{
		//-------------------------------------------------------------------------
		#ifdef CODE_SEL_SMOKE
		case CFG_TYPE_SMOKE:
		case CFG_TYPE_SMOKE_BUZZER:
		{
			//Smoke 单独采用5542 IO已经在 sysInit定义
			//////////////////////////////////////////////////
			FuncData.Interval	= 128;										//首次采样等待-感烟4秒 128*31.25ms = 4000ms
			//感烟校准运放
			FuncData.AmpCaliInterval = 96;									//上电延时3秒校准 96*31.25ms = 3000ms
			FuncData.FlagAmpCalid  = 0 ;									//Amp尚未校准
			FuncData.SD_ClrAir_IR = MemData.Rom[EEPROM_ID_CLEARAIR]; 		//读取洁净空气值
			FuncData.SD_ClrAir_BLUE = MemData.Rom[EEPROM_ID_CLEARAIRBL]; 	//读取蓝光洁净空气制
			
			#ifdef CODE_SEL_SMOKE_COMPENSATION_BY_NTC
				#ifdef CODE_SEL_SMOKE_BUZZLE
				//温度补偿AD配置
				_pac1  = 1;
				_pas03 = 1;
				_pas02 = 0;
				#else
				//温度补偿AD配置
				_pac3  = 1;
				_pas07 = 1;
				_pas06 = 1;
				#endif //CFG_TYPE_SMOKE
			#endif //CODE_SEL_SMOKE_COMPENSATION_BY_NTC
			


			#ifdef CODE_SEL_SMOKE_BUZZLE
			//针对蜂鸣器的感烟探测器
			if(CFG_TYPE_SMOKE_BUZZER == Typetmp)
			{
				//PA4 STPB 选择STPB
				_pas11 = 0;
				_pas10 = 1;
			}
			FuncData.AV_GroupId = 0;				//默认组ID是0
			//FuncData.AV_GroupNum = 1;				//默认总组数是1
			#endif

			//蜂鸣器感烟的新需求
			//1.蜂鸣器感烟的查询闪灯。从闪黄灯修改为红灯
			//2.默认配置为查询不闪灯
			//3.不论配置是否闪灯。蜂鸣器感烟上电闪烁一下。然后首次发生通讯时，闪烁3次。
			#ifdef CODE_SEL_SMOKE_BUZZLE
				SdkeePotData.BuzzleSmokeFstComLedCnt = 3;  //首次通讯闪三次灯
			#endif
		}
		break;
		#endif
		//-------------------------------------------------------------------------
		#ifdef CODE_SEL_HEAT
		case CFG_TYPE_HEAT:
		{
			//PA4  输入AD
			//PA1  LED
			_pac4  = 1;
			_pas11 = 1;
			_pas10 = 0;  //AN0
			HeatDeviceTypeConfig(HEAT_DEVICE_TYPE_FIXED);//定温型感温
		}
		break;

		case CFG_TYPE_HEAT_R:
		{
			//PA4  输入AD
			//PA1  LED
			_pac4  = 1;
			_pas11 = 1;
			_pas10 = 0;  //AN0
			HeatDeviceTypeConfig(HEAT_DEVICE_TYPE_R);//差温型
		}
		break;
		#endif

		

		default:;
		break;

	}

}



 /******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
