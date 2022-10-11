
/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      config.h
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     配置头文件
 ******************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H
/*******************************************************************************
 * 程序编译设置
 ******************************************************************************/
//-------看门狗-------
//看门狗是默认打开的，且无法关闭的
	#define ENABLE_WD			//使用看门狗
//------代码选择------			//以下类型仅能选择一种
//编译模块时首先定义SMOKE HEAT MODULE 类型;然后打开对应的产品类型的编译选项 
//感烟选择的芯片是5542;是5V的
//其他选择的芯片是5541;是3.3V的。
//----------------------------------------------------------------------------------
#define CODE_SEL_SMOKE		//???????
		#ifdef CODE_SEL_SMOKE
			#define CODE_SEL_SMOKE_BUZZLE      		//????????????????? 
		    // #define CODE_SEL_SMOKE_LIGHTON     			//lighton ??????????1.5M 10K 200K
			#define CODE_SEL_SMOKE_HENGRUN     			//???? ??????900K 10k 200K
			#define CODE_SEL_SMOKE_COMPENSATION_BY_NTC	//??????
			#define CODE_SEl_SMOKE_IR_AND_BLUE			//????????
			#define CODE_SEL_SMOKE_AVG                  //Ϊ��ƽ�����ݣ�����2��ƽ��

			#define CODE_SEL_CONFIG_PARA_FIXED			//�̶������ͷǹ̶�����
		#endif
//----------------------------------------------------------------------------------
// #define CODE_SEL_HEAT		//感温代码
	#ifdef CODE_SEL_HEAT 
	// #define DEV_MECH_D20		//D2.0??
	#define DEV_MECH_ND751P		//ND751P??
	#endif //CODE_SEL_HEAT
/*******************************************************************************
 * 端口宏定义
 ******************************************************************************/
//硬件=DEMO
//硬件=感烟
#ifdef CODE_SEL_SMOKE
	//----------逻辑端口----------	
	//旧版硬件和第二版硬件的LED定义在PA3 Yellow ; PA6 Red
	//感烟第三版硬件 PA1 YELLOW;PA4 Red
	#ifdef CODE_SEL_SMOKE_BUZZLE
		#define GPO_SMOKE_YELLOW_ON()    (_pa0 = 1)
		#define GPO_SMOKE_YELLOW_OFF()   (_pa0 = 0)
		#define GPO_SMOKE_RED_ON()       (_pa2 = 1)
		#define GPO_SMOKE_RED_OFF()      (_pa2 = 0)

		#define GPO_SMOKE_LED_ON()       (_pa6 = 1)
		#define GPO_SMOKE_LED_OFF()      (_pa6 = 0)

		#ifdef CODE_SEL_SMOKE_COMPENSATION_BY_NTC
			#define GPO_SEN_PWR_1() 	 (_pa3 = 1)
			#define GPO_SEN_PWR_0() 	 (_pa3 = 0)	
		#endif 
	#else
		#define GPO_SMOKE_YELLOW_ON()    (_pa4 = 1)
		#define GPO_SMOKE_YELLOW_OFF()   (_pa4 = 0)
		#define GPO_SMOKE_RED_ON()       (_pa1 = 1)
		#define GPO_SMOKE_RED_OFF()      (_pa1 = 0)
		#ifdef CODE_SEL_SMOKE_COMPENSATION_BY_NTC
			#define GPO_SEN_PWR_1() 	 (_pa6 = 1)
			#define GPO_SEN_PWR_0() 	 (_pa6 = 0)	
		#endif 

	#endif //CODE_SEL_SMOKE_BUZZLE

	//感烟端口
	//SINK0  IRED
	//SINK1  BLUE
	#define SINK_EN()       (_isgen = 1)
	#define SINK_DISABLE()  (_isgen = 0)

	#define IRED_ON()		(_isgs0 = 1)	//??????1??
	#define BLUE_ON()		(_isgs1 = 1)	//??????2??
	#define IRED_OFF()		(_isgs0 = 0)	//??????1??
	#define BLUE_OFF()		(_isgs1 = 0)	//??????2??

	//AFE????????????
	#define OPAMODE_INIT     (0)
	#define OPAMODE_CHANGAMP (1)

	#define AFE_R3_10K        (0)
	#define AFE_R3_20K		  (1)
	#define AFE_R3_30K		  (2)
	#define AFE_R3_40K	      (3)

#endif



#ifdef CODE_SEL_HEAT
	//感温的IO口
	#define GPO_SEN_PWR_1() 	 (_pa5 = 1)
	#define GPO_SEN_PWR_0() 	 (_pa5 = 0)

	#define GPO_HT_LED_1()		 (_pa1 = 1)
	#define GPO_HT_LED_0()		 (_pa1 = 0)
#endif



// 	#define DA_REF_DIN_MIN	(8)	    //(code/64)*3.3*15 12:9.28V 8:6.1875V 7:5.414V
// 	#define DA_REF_DIN_MAX	(33)	//29V电源电压时，比较阈值设定为25.5V
// 	//根据电源电压测量的档位，确定处于高档还是低档
// 	//上电后，初始化阶段首先分为两档
// 	#define PWR_VOL_TH      (80)    //15.4687V 区分档位
// 	#define DA_CMP_VOL_HIGH (25)    //PTM的翻转电压高档 17.57V
// 	#define DA_CMP_VOL_LOW  (12)     //code 12 :PTM的翻转电压低档 9.28V
// 	//回路电压状态监视
// 	#define LOOP_VOLLOW_TH       (56)   //10.82V  算上全桥和前端压降1.3V 12.12V
// 	#define LOOP_VOLNORMAL_TH	 (71)   //13.7V  算上全桥和前端压降1.3V 15V
// 	//回路电压波动的范围
// 	#define LOOP_VOLWAVE_FAULT_TH    (31)   //波动5.99V的范围
// 	#define LOOP_VOLWAVE_NORMAL_TH   (16)   //波动3V的范围

// #endif

//Heat使用的是5541。是3.3V芯片;因程序空间问题heat单独一个程序
#ifdef CODE_SEL_HEAT
	//拉码电流配置
	//当前拉码电阻是12欧姆
	//电源电压29V时，测得的电源AD值为165；比较电平阈值设置为:
	#define DA_REF_DOUT 	(6)    //(6*3.3/64)/12 = 25.78mA 拉码电阻12Ω
	
//	#define DA_REF_DOUT 	(26)    //(26*3.3/64)/47 = 28.5mA 拉码电阻47欧姆

	//比较阈值范围 8:6.18V  33:25.5V
	//Code7:5.4V比较极限。
	#define MIN_ADJLOOPVOL  (57)    //最小动态调整电压11.02V。小于则固定为最小比较电平
	#define MIN_WORKLOOPVOLT (16)   //总线最小工作电压3.09V。 小于该电压则不工作 
	
	#define DA_REF_DIN_SMOKE_MIN	(8)	
	#define DA_REF_DIN_HEAT_MIN	(13)	    
	#define DA_REF_DIN_MAX	(33)	//29V??????????????????25.5V
	//???????????????????????????????????
	//???????????????????????
	#define PWR_VOL_TH      (80)    //15.4687V ???????
	#define DA_CMP_VOL_HIGH (25)    //PTM?????????? 17.57V
	#define DA_CMP_VOL_LOW  (12)     //code 12 :PTM?????????? 9.28V

	//回路电压状态监视
	#define LOOP_VOLLOW_TH       (56)   //10.82V  算上全桥和前端压降1.3V 12.12V
	#define LOOP_VOLNORMAL_TH	 (71)   //13.7V  算上全桥和前端压降1.3V 15V

	//回路电压波动的范围
	#define LOOP_VOLWAVE_FAULT_TH    (31)   //波动5.99V的范围
	#define LOOP_VOLWAVE_NORMAL_TH   (16)   //波动3V的范围
#endif


//感烟是使用的5542是5V的芯片
#ifdef CODE_SEL_SMOKE
	//拉码电阻修改为12欧姆
	#define DA_REF_DOUT 	(8) 	//code 8：55mA
									//code 6: 39.06mA
									//code  4: 26mA
	#define MIN_ADJLOOPVOL  (38)    //最小动态调整电压11.13V.小于则固定为最小比较电平	
	#define MIN_WORKLOOPVOLT (11)   //总线最小工作电压3.22V。小于该电压则不工作 
	
	#define DA_REF_DIN_MIN	(5)	    //(code/64)*5V*15  code8:9.375V  code5:5.859V    
	#define DA_REF_DIN_MAX	(25)	//29V
	//二极管配回路卡  拉码电流配置  (DAC*VCC/64)/R24
	#define PWR_VOL_TH      (52)    //15.234V档位
    #define DA_CMP_VOL_HIGH (15)    //PTM的翻转电压高档 17.57V
    #define DA_CMP_VOL_LOW  (8)     //code 8 :PTM的翻转电压低档 9.375V

	//回路电压状态监视
	#define LOOP_VOLLOW_TH       (36)   //10.54V  算上全桥和前端压降1.3V 11.84V
	#define LOOP_VOLNORMAL_TH	 (47)   //13.76V  算上全桥和前端压降1.3V 15V

	//回路电压波动的范围
	#define LOOP_VOLWAVE_FAULT_TH    (21)   //波动6.15V的范围
	#define LOOP_VOLWAVE_NORMAL_TH   (10)   //波动2.92V的范围
	
#endif
/*******************************************************************************
 * 函数声明
 ******************************************************************************/
extern void SystemInit (void);
extern void PltOpaSwitchOn (void);
extern void PltOpaSwitchOff (void);
extern void SdCaliOpa (void);
extern uint8 PltLineVolt(void);
extern void SdOpaSwitch (uint8 Flag);

extern uint8 LEDTimer (uint8 Flag);

#ifdef CODE_SEL_HEAT
extern uint16 AdcSample (uint8 Ch);
#else
extern uint16 AdcSample (uint8 Ch);
#endif

#ifdef CODE_SEL_SMOKE
extern void AfeInit(uint8 ucR1Set,uint8 ucR3Set,uint8 ucR2Set,uint8 ucFlagInitOrChangAmp);
#ifdef CODE_SEL_SMOKE_BUZZLE
extern uint8 SmokeBuzzlePwm(uint8 flag);
#endif
#endif

extern uint8 timeLoopCounter;
extern void configPreciseTimer(uint16 duration,uint8 flag);




#endif /* __CONFIG_H */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 *对于45F5541/5542芯片的使用的注意事项：
1.芯片型号中带V字样的才是仿真芯片；F字样是不能仿真的。
2.仿真芯片是3.3V供电电压；5542F的芯片是5V供电电压；5541F的芯片的供电电压是3.3V。
  所以关于拉码电流、PLT的设置都要根据各自的电压等级进行设置。
3.仿真芯片的时钟是没有办法校准的，所以其CLK有5%的偏差。通过E-write Pro的专用烧录器可以校准时钟。
4.在IDE中需要设置芯片类型，同时设置芯片的电压等级和程序配置的主频。烧录器校准时钟是按照这个设置进行的。
5.5541的C0023批次前的芯片PTM存在缺陷，有0.05%的概率，PTM无法捕获到脉宽，推测是PTM未能重新启动。此批次前的应全部丢弃。
6.AFE的DC耦合模式，一级电阻不要超过2M；二级AMP放大不要超过20~50倍。AC耦合模式，二级放大也应遵循20~50倍。
  采样的稳定性主要由二级放大带来。
7.Ram的Bank0和Bank1的寻址方式不一样，Bank0是直接寻址；Bank1是间接寻址。而变量的定义是由IDE自动分配的。
  这样造成ISR的执行时间存在8us@4MHz的差别。进入ISR的时间延迟有8us的误差。
8.AFE的两个OPA校准，必须在上电等待一段时间后（等待电源稳定后进行）。否则OPA校准不正确。偏置OS的值在34左右。
9.ISR中调用的子函数名不要和主循环中的调用的子函数重名。存在一定概率ISR从中断中退出时，跑飞。
10.5541和5542芯片的中断向量表是不一样的。
11.5541和5542的AD的通道配置是不一样的。
12.拉码电流设计为缓升缓降的形式。当3.3V还是5V供电不一样的时候，会导致循环修改DA的时间变化，要重新测定拉码时间。
13.在PTM发送PWM波时，不要重复设置；会导致PTM暂停的。
14.开启和关闭BaseTime0的，不能使其清零CNT。也就是说首次开启BaseTime0，首个中断来临不确定时刻。	
15.PLT的OPA打开后，需要40us才能稳定。所以拉码前需要提前打开40us.为了能保证尽快拉码，所以提前一个CLK打开opa.
16.PLT从采集线电压，切换到拉码功能后，在同一批次硬件的情况下，有的板子出现第一个拉码打开PLTS0时，
出现一个＜5us的大电流脉冲，后续拉码均正常；有的板子正常。已反馈给FAE
17.AFE的两级运放。FAE一级电阻选择1M不要超过2M，二级放大倍数相对小一些。
实测发现，如果按照项目需求配置放大倍数一级想要有一个比较大的量程，则二级的放大倍数会相对比较大。
此时稳定度达到10个AD。
   一二级运放不建议同时开启，否则稳定的时间超过2ms.
18.单一结构体不宜过大，否则编译器在分配RAM0还是RAM1会存在困难
19.目前已经发现5541的AD在将近满量程时，采样值会出现偏小的跳动(连续4次采样)。原因是片内LDO在有一定负载(2~3mA)
通讯属于低电平时，LDO调整会产生一个尖脉冲，且蜂鸣器感烟带来的尖脉冲>5ms，普通通讯>1ms
20.当PLT的比较电平和电源电平处于0.9V以内时，出现当检测总线电压时，切换至AD时，触发通讯的输入捕获，并最终触发P中断。
 
 ******************************************************************************/









/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
