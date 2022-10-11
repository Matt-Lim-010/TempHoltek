/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      main.h
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     头文件
 ******************************************************************************/
#ifndef __MAIN_H
#define __MAIN_H

#define MAIN_VER1		(0x01)		//ProductVersion

#if defined(CODE_SEL_SMOKE)
    #define SUB_VER2		(0x07)	//SoftwareVersion    
#endif

#if defined(CODE_SEL_HEAT)
    #define SUB_VER2		(0x05)	//SoftwareVersion    
#endif
/*******************************************************************************
 * 宏定义
 ******************************************************************************/
//TypesId规划
//感温探测器			10
//感烟探测器			20
//烟温复合探测器		30
//Monitor 模块			40
//control 模块			50
//Monitor/control 模块	60
//Hydrant Call Point	70
//Manual Fire Call		80
//AV					90
//Interface Module		110

//产品类型定义
#define CFG_TYPE_HEAT			(2)		//10 感温探测器*
#define CFG_TYPE_HEAT_R         (3)		//11 感温探测器ROR型
#define CFG_TYPE_SMOKE			(20)		//20 感烟探测器*
#define CFG_TYPE_SMOKE_BUZZER   (21)        //21 带蜂鸣器的感烟探测器
//#define CFG_TYPE_SMK_HT		    (30)		//30 烟温复合探测器

#define CFG_TYPE_INPUT          (40)        //40 输入(monitor)模块
#define CFG_TYPE_IPART_PWRIO    (41)        //41 有源IO的输入部分    (自动产生)
#define CFG_TYPE_IPART_PULSEIO  (42)        //42 脉冲IO的输入部分    (自动产生)

#define CFG_TYPE_OUTPUT			(50)		//50 输出(control)模块
#define CFG_TYPE_OPART_PWRIO    (51)        //51 有源IO的输出部分

#define CFG_TYPE_OUTPUT_PLUSE	(60)        //60 输出(control脉冲型)模块
#define CFG_TYPE_OPART_PULSEIO  (61)        //61 脉冲IO的输出部分

//#define CFG_TYPE_INOUT		  (60)		//60 输出输入(monitor control)模块
//#define CFG_TYPE_INOUT_PLUSE    (61)        //61 输出输入(monitor control)脉冲输出

#define CFG_TYPE_HYDRANT		(70)		//70 消火栓(HydrantFire)按钮
#define CFG_TYPE_MCP			(80)		//80 手动报警(ManualFireCallPoint)按钮
#define CFG_TYPE_AV				(90)		//90 声光警报器
#define CFG_TYPE_INTERFACE		(110)		//110 接口模块
#define CFG_TYPE_ISO            (120)		//120 隔离模块
//OEM
#define	CFG_OEM_SYSTEMSENSOR	(1)		//1 System Sensor
#define	CFG_OEM_NOTIFIER		(2)		//2 Notifier
#define	CFG_OEM_PROTECTWELL		(3)		//3 Protectwell
#define	CFG_OEM_HONEYWELL		(4)		//4 Honeyerll
#define	CFG_OEM_NDC				(5)		//5 NDC
#define	CFG_OEM_ZHENGGUANG		(6)		//6 正光
#define	CFG_OEM_JINCHUN			(7)		//7 锦春
/*******************************************************************************
 * 标志定义
 ******************************************************************************/
//激活状态标志
#define ACTIVE_FLAG_IDLE		(0x00)	//空闲
#define ACTIVE_FLAG_COMM		(0x01)	//通信
#define ACTIVE_FLAG_FUNC		(0x02)	//功能
#define ACTIVE_FLAG_LED			(0x04)	//闪灯
#define ACTIVE_FLAG_EEPROM		(0x08)	//存储
#define ACTIVE_FLAG_SMK_BEEP    (0x10)  //该声光的感烟的功能

#define ACTIVE_FLAG_RUN			(0x80)	//单次运行

//LED 控制状态机
#define LEDCTRL_STATUS_INIT       (0U)    //延时初始化
#define LEDCTRL_STATUS_DELAYSTART (1u)    //延时开始
#define LEDCTRL_STATUS_CHECKTIME  (2U)    //检查延时时间
#define LEDCTRL_STATUS_DELAYEND   (3U)    //延时结束

//设备状态	
//#define DEV_SMOKE_REMOTE_TEST_FLAG (0x01)  //远程测试
//#define DEV_SMOKE_NO_PW5 (0x02)  		   //PW5不返回
//#define DEV_COMM_DLIP_LED (0x04)  		   //DLIP FLASH LED
/*******************************************************************************
 * 变量定义
 ******************************************************************************/
typedef struct {
	//功能控制
	uint8	CmdActive;		//激活指令
	uint8	CmdReset;		//复位指令
	uint8 	MyType;         //产品类型


	uint8   MyAddr;
	
}SysDataRegs;



/*******************************************************************************
 * 全局变量引用
 ******************************************************************************/
extern SysDataRegs	SysData;
#ifdef CODE_SEL_SMOKE 
	extern SdkeenPotSetting SdkeePotData; //感烟拐点寻找

#endif

/*******************************************************************************
 * 函数声明
 ******************************************************************************/
void DataInit (void);
void ConfigType(uint8 Typetmp);   
#endif /* __MAIN_H */
/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
