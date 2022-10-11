/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      config.c
 * @author    S
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     配置程序
 ******************************************************************************/
 #include "include.h"
void PtmInit(void);
void PltCaliCmp (void);
void PltCaliOpa (void);
void PltInit(uint8 ucDacCode);  //PLT的初始化配置
//AV的首三次渐强音。首先载入BUFF[2]，然后BUFF[1],最后BUFF[0]
// const uint16 uiSoundGraBuff[3]={360,360,360};  
// const uint16 ucSoundHighBuff[3] = {70,30,1};
uint8 timeLoopCounter = 0;
// void configPreciseTimer(uint16 duration,uint8 flag);;
//------------------------------
//单片机系统初始化
//------------------------------
void SystemInit (void)
{
	//---------关闭全局中断---------
	DisableInterrupts();
	
	//----------看门狗设置----------
#ifdef ENABLE_WD
	if((_pdf == 0) && (_to == 0))	//检查是上电复位
	{
		//看门狗4秒,时钟32.768kHz
		_wdtc = 0B01010110;			//WE[7:3]=10101禁用/01010开启,WS[2:0]=110=2^17分频4秒
	}
#endif
	
	//-----------时钟设置-----------
	//FAE介绍不推荐8M主频，不稳定；推荐使用4M主频
	//主频使用4M
    _hircc 	= 0B00000101;			//高速时钟,HIRC[3:2]:00=2M,01=4M,10=8M;HIRCF[1],HIRCEN[0]
    //不能增加超时处理:
    //1.时钟持续不能稳定，触发watchDog进行复位。
    //2.等待超时后，没有合理的处理错误，继续运行风险更大。
	while(_hircf == 0){;}			//等待时钟稳定 
    //主时钟=FH,休眠:高速关闭,低速开启
    //主时钟选择4H；休眠时关闭高速时钟；开启低速时钟
    _scc 	= 0B00000001;			//CKS[7:5]分频,FHIDEN[1]高开,FSIDEN[0]低开

#ifdef CODE_SEL_SMOKE					
	//-----------IO口设置-----------
	//不使用的IO口的配置输出0
	//PA0   NC    OCDSDA    SCL(IIC CLK)
	//PA1   NC    NC
	//PA2   NC    OCDSCK
	//PA3   DO    LED_Y
	//PA4   NC    NC
	//PA5   NC    
	//PA6   DO    LED_R
	//PA7   NA
	_pa 	= 0B00000000;			//PA输出
	_pac 	= 0B00000000;			//PA输入输出0=输出,1=输入(PA3通信电压检测时=1)
	_papu	= 0B00000000;			//PA上拉电阻0=关闭,1=使能
	_pawu	= 0B00000000;			//PA唤醒中断0=关闭,1=使能
	_pas0	= 0B00000000;			//PA3-PA0复用功能(PA3通信电压检测时=11)
	_pas1	= 0B00000000;			//PA7-PA4复用功能
	//PB1 PB2 PB3 
	//内部连接引脚
	//PB	7	6	5	4	3	2	1	0
	//FN	-	-	-	-	PLT	PLT	PLT	-
	//DIR	O	O	O	O	0	0	0	O
	//PU	N	N	N	N	N	N	N	N
	_pb		= 0B00000000;			//PB输出
	_pbc	= 0B00000000;			//PB输入输出0=输出,1=输入
	_pbpu	= 0B00000000;			//PB上拉电阻0=关闭,1=使能
	_pbs0	= 0B01010100;			//PB3-PB0复用功能
	//----------红蓝光发射电流设置---------
	//源电流选择
	_sledc	= 0B00000000;			//源电流00=最小11=最大
	//灌电流发生器
	//烟感器光电驱动电流:
	_isgenc	= 0x00;					//功能使能EN[7]使能S1[1]使能S0[0]使能
	//sink0 是 50+10*data
	//sink1 是 50+5*data
	_isgdata0 = 11;				 	//ISINK0[4:0]=>[50+10*d]mA,170mA
	_isgdata1 = 7;					//ISINK1[4:0]=>[50+5*d]mA,85mA	
	//IFS0 寄存器
	//需要配置PTPI引脚  CxCap
	_ifs0	= 0B00000000;			//外设输入源选择

	//----------烟感前端设置----------
	//AFE的运放便宜校准值
	//初始化给定一个固定的VOS值。
	//在主循环中，空闲时刻进行自动校准
	_sda0vos = 34;					
	_sda1vos = 34;
	#ifdef CODE_SEL_SMOKE_HENGRUN
	AfeInit(9,AFE_R3_10K,2,OPAMODE_INIT);
	#endif
	SdOpaSwitch(0);  //关闭烟感放大电路  放大电流开启耗电1mA	
	//------------时基设置------------
	//TB 设置
	//低速时钟Fsub=32.768k,TB0=1秒,TB1=禁用
	//fsub 典型值32K  timebase0 溢出定时约1.024sec
	//TimeBase0的溢出周期修改为 1024个周期，31.25ms。用于快速调整通讯比较电平
	_pscr = 0B00000010;				//时基时钟,CLK[1:0],00=Fsys,01=Fsys/4,1X=Fsub
//	_tb0c = 0B10000110;				//TB0设置,ON[7],TB[2:0]周期0=256,7=32768
	_tb0c = 0B10000010;             //临时调整为 1024个周期
									//约31.25ms
	_tb1c = 0B00000000;				//TB1设置,ON[7],TB[2:0]周期0=256,7=32768
	_tb0e = 1;   					//时基0中断使能

#else

	//-----------IO口设置-----------
	//模块的默认IO配置	
	//修改为PA5 PTP1B
	//PA0  NC   OCDSDA
	//PA1  NC   原配置为通讯的CLK
	//PA2  NC   OCDSCK
	//PA3  NC 
	//PA4  OUT  LED
	//PA5  NC
	//PA6  NC
	//PA7  NC
	_pa = 0B00000000;
	_pac = 0B00000000;
	_papu = 0B00000000;
	_pawu = 0B00000000;
	_pas0 = 0B00000000;  //PA3 在PWM开关函数中修改配置，默认配置输出低以降功耗
	_pas1 = 0B00000000;
    //PB1 PB2 PB3 为PLT内部引脚
    //经确认输入输出无关PLT的操作；
	_pb		= 0B00000000;			//PB输出
	_pbc	= 0B00000000;			//PB输入输出0=输出,1=输入
	_pbpu	= 0B00000000;			//PB上拉电阻0=关闭,1=使能
	_pbs0	= 0B01010100;			//PB3-PB0复用功能
	//PC0  PTP1 SOUND   PWM
	_pc = 0;
	_pcc = 0;
	_pcpu = 0;
	
	//源电流配置为最小
	_sledc0 = 0x00;
	_sledc1 = 0x00;
	//PTP0I输入源引脚  RX/TX输入源引脚 
	_ifs = 0x00;//引脚公共功能选择 

	//AV由于要实现总线同步的功能，需要同步各自的运行周期
	//而BaseTime 在关闭和开启后不能清其CNT，导致首次进入BaseTime ISR时间不确定
	//Fsub 32.768k
	//30ms 时基定时
	_ctm1c0 = 0B01000000;		//时钟设置PAU[7]=0,CK[6:4]=011-/64,ON[3]=0,RP[2:0]=000
	//CCRA 周期 A匹配清零
	_ctm1c1 = 0B11000001;		//模式设置M[7:6]=10,IO[5:4]=11,OC[3]=1,POL[2]=0,DPX[1]=0
	_ctm1al = 0xd4;				//定时值980=30ms
	_ctm1ah = 0x03;
	_ctm1af = 0;	
	//清除中断标志
	_ctm1ae = 1;                // 打开相应中断开关
    _mf4e = 1;
	_ct1on = 1;					//开定时器ON[3]=1
#endif

	//-----------回路总线PLT设置---------
	PltCaliCmp();  			       //校准电源收发器比较器
	PltCaliOpa();                  //校准电源收发器plt的运放
	
	PltInit(9);       //根据默认比较电平 配置PLT
	
	PtmInit();                     //边沿捕获设置
	//---------低电压检测设置---------
	//低电压复位硬件始终使能 固定为2.1V
	//LVDC只是检测供电电压，并不是回路电压，在本应用中没有实际意义，而且还耗电。
//	_lvdc = 0B00011010;				//LVDO[5]输出,EN[4],VBGEN[3],VLVD[2:0]0=2V,2=2.4V
	// _pltc0e = 1;
	_pltc0f = 0;
	#ifdef CODE_SEL_SMOKE	
	_stmae = 1;
	#else
	_mf3e = 1;
	_ctm0ae = 1;
	#endif
	
}

#ifdef CODE_SEL_SMOKE
//------------------------------
//AfeInit 初始化
//R1Set 0~63 *100K
//R3Set 0:10K 1:20K 2:30K 3:40K
//R2Set 0~63 *100K
//------------------------------
void AfeInit(uint8 ucR1Set,uint8 ucR3Set,uint8 ucR2Set,uint8 ucFlagInitOrChangAmp)
{
	//如果是初始化则设置带宽
	if(OPAMODE_INIT == ucFlagInitOrChangAmp)
	{
		//两级运放设置
		_sda0c	= 0B00000011;			//放大器设置,EN[6],AO[5],BW[1:0]带宽5,40,600k,2M
		_sda1c	= 0B00000011;			//放大器设置,EN[6],AO[5],BW[1:0]带宽5,40,600k,2M
	}
	//AC耦合模式
	//SDS0,SDS1 和SDS2 打开
	#if 0
	_sdsw  = 0B00100111; 
	//_sdpgac0 = 60;	//R1 6M
	//_sdpgac1 = 0B01001110;	//R3 20k R2 1400K	 OPA1放大倍数70
	
	_sdpgac0 = ucR1Set;
	_sdpgac1 = ((ucR3Set<<6) | ucR2Set); 
	#endif
	
	//DC 耦合模式
	#if 1
	//DC耦合模式下 R1 1M R3 20K R3 1200K ；0.5db/m时，一级53 二级255
	//
	_sdsw = 0B01110111;     //S0打开的话 会把偏置也放大很多
//	_sdpgac0 = 10;			//R1  1.0M
//  _sdpgac1 = 0B01001100;  //1+1200/20  放大61倍
	_sdpgac0 = ucR1Set;
	_sdpgac1 = ((ucR3Set<<6) | ucR2Set); 
	#endif

}
#endif
//------------------------------
//Plt 初始化
//------------------------------
void PltInit(uint8 ucDacCode)
{
#ifdef CODE_SEL_SMOKE
	_pbs0 &= 0B00000011;
	_pbs0 |= 0B01010100; 	   // 设置二总线通信 I/O端口 功能
	_pltsw = 0B00000000; 	   // 设置PLT 内部开关	PLTS2 PLTS1 PLTS0 

	//仿真芯片用的是3.3V 
	//Flash芯片用的是5V 
	//比较电平不同
	_pltda0l = ucDacCode;			  //设置比较器0 总线翻转电平20.1V
	/*
	   _pltda0l 配合 比较器0的总线翻转电压值计算方法
	   TRX 对地的分压电阻阻值为 2.24M : 160K，即 PLRX 电压 等于TRX电压 / 15
	   比较器翻转电压 = DAC输出电压 * 15
	   DAC输出电压 = (VDD/64) * _pltda0l
	   
	   结果套入计算：
	   DAC输出电压 = (VDD * _pltda0l)/64 = 3.3V * 19 / 64 = 0.9797V
	   总线翻转电压 = DAC输出电压 * 15 = 0.9797V * 15 = 14.6955V
	*/
	_pltda1l = 3;			   // 设置 比较器1 总线翻转电压为0V 暂时不用比较器1
	_pltda2l = 0;			   // 回码电流控制DAC DAC输出0V 无回码拉电流
	/*
	   _pltda2l 配合 运放 实现回码电流的电流值计算
	   回码电流 = DAC2输出电压/ Ris电阻（IS脚位对地电阻，DEMO板为20欧姆）
	   如果需要回码电流 = 50MA，DAC2输出电压需要 = 1V
	   DAC输出电压 = (VDD/64) * _pltda2l
	   
	   推到出，如果需要回码电流50MA，配合IS电阻为20欧姆情况下，DAC输出1V，_pltda2l应设置为19
	   如果_pltda2l = 19，实际回码电流 = [ (VDD / 64) * _pltda2l ] / 20 = [ (3.3 * 19) / 64] / 20 = 48.98MA
	*/
	_pltdacc =  0B00000011;		// 关闭 DAC2和DAC1，打开 DAC0
	//为了探究通讯的极限电源电平和比较电平
	//比较器驱动电流设置为01，迟滞宽度0.9Vmax。
	//极限通讯比较阈值下限8:6.1875V;Vcom 6.96V。
	
	//比较器驱动电流设置为 01.  2.5us相应14~22uA电流消耗
	_pltc0c = 0B01000101;	   // 设置比较器0：
							   //			   比较器输出电压 正向跟随
							   //			   比较器 除能
							   //			   比较器去抖功能 31~32tsys
							   //			   比较器功耗设置：58UA，比较器响应时间0.7US（最大极限）
	_pltc1c = 0B01000101;	   // 设置比较器1：关闭
	//电压迟滞窗口 20~60mV  对应到比较电平20~60mV * 15 0.3~0.9V
	_pltchyc = 0B00010101;	   // 设置：
							   //	   比较器0连入PLT0捕获输入功能
							   //	   比较器0和比较器1，比较器输出：同向
							   //	   比较器0迟滞电压窗口：0.9V （最大）
							   //	   比较器1迟滞电压窗口：5MV （最大）
	_pltac = 0B00000001; 	   // 设置回码运放，功能除能，正逻辑电平，2MHz频宽

	_pltc0en = 1;	   // 打开比较器0供电 准备接收总线电平信号
	_pltc1en = 1;      //
	
	//_pltc0e  = 1;	
	//_pltc1e = 1;
    _pltc0pol = 0;
    _pltc1pol = 0;	
	
#endif //CODE_SEL_SMOKE

#ifdef CODE_SEL_HEAT
	_pbs0 &= 0B00000011;
	_pbs0 |= 0B01010100; 	   // 设置二总线通信 I/O端口 功能
	_pltsw = 0B00000000; 	   // 设置PLT 内部开关	PLTS2 PLTS1 PLTS0 

	//仿真芯片用的是3.3V 
	//Flash芯片用的是5V 
	//比较电平不同
	//_pltda0l = ucDacCode;			  //设置比较器0 总线翻转电平20.1V
	_pltda0l = 13;			  //设置比较器0 总线翻转电平20.1V
	/*
	   _pltda0l 配合 比较器0的总线翻转电压值计算方法
	   TRX 对地的分压电阻阻值为 2.24M : 160K，即 PLRX 电压 等于TRX电压 / 15
	   比较器翻转电压 = DAC输出电压 * 15
	   DAC输出电压 = (VDD/64) * _pltda0l
	   
	   结果套入计算：
	   DAC输出电压 = (VDD * _pltda0l)/64 = 3.3V * 19 / 64 = 0.9797V
	   总线翻转电压 = DAC输出电压 * 15 = 0.9797V * 15 = 14.6955V
	*/
	_pltda1l = 4;			   // 设置 比较器1 总线翻转电压为0V 暂时不用比较器1
	_pltda2l = 0;			   // 回码电流控制DAC DAC输出0V 无回码拉电流
	/*
	   _pltda2l 配合 运放 实现回码电流的电流值计算
	   回码电流 = DAC2输出电压/ Ris电阻（IS脚位对地电阻，DEMO板为20欧姆）
	   如果需要回码电流 = 50MA，DAC2输出电压需要 = 1V
	   DAC输出电压 = (VDD/64) * _pltda2l
	   
	   推到出，如果需要回码电流50MA，配合IS电阻为20欧姆情况下，DAC输出1V，_pltda2l应设置为19
	   如果_pltda2l = 19，实际回码电流 = [ (VDD / 64) * _pltda2l ] / 20 = [ (3.3 * 19) / 64] / 20 = 48.98MA
	*/
	_pltdacc =  0B00000011;		// 关闭 DAC2和DAC1，打开 DAC0
	//为了探究通讯的极限电源电平和比较电平
	//比较器驱动电流设置为01，迟滞宽度0.9Vmax。
	//极限通讯比较阈值下限8:6.1875V;Vcom 6.96V。
	
	//比较器驱动电流设置为 01.  2.5us相应14~22uA电流消耗
	_pltc0c = 0B01000101;	   // 设置比较器0：
							   //			   比较器输出电压 正向跟随
							   //			   比较器 除能
							   //			   比较器去抖功能 31~32tsys
							   //			   比较器功耗设置：58UA，比较器响应时间0.7US（最大极限）
	_pltc1c = 0B01000101;	   // 设置比较器1：关闭
	//电压迟滞窗口 20~60mV  对应到比较电平20~60mV * 15 0.3~0.9V
	_pltchyc = 0B00010101;	   // 设置：
							   //	   比较器0连入PLT0捕获输入功能
							   //	   比较器0和比较器1，比较器输出：同向
							   //	   比较器0迟滞电压窗口：0.9V （最大）
							   //	   比较器1迟滞电压窗口：5MV （最大）
	_pltac = 0B00000001; 	   // 设置回码运放，功能除能，正逻辑电平，2MHz频宽

	_pltc0en = 1;	   // 打开比较器0供电 准备接收总线电平信号
	_pltc1en = 1;      //
	
	//_pltc0e  = 1;	
	//_pltc1e = 1;
    _pltc0pol = 0;
    _pltc1pol = 0;
	
#endif //CODE_SEL_HEAT
}

//------------------------------
//电源收发器-比较器-失调校准
//------------------------------
void PltCaliCmp (void)
{	
	uint8 vos1 = 0, vos2 = 0;
	uint8 out1 = 0, out2 = 0;
	//比较器0
	//递增查找
	_pltc0vos = 0B01100000;			//OFM=1,RSP=1,OF=0
	GCC_DELAY(50);					//等待
	while(vos1 < 31)				//VOS范围0-31
	{
		out1 = _pltc0out;			//比较器输出状态
		_pltc0vos ++;				//递增
		vos1 ++;
		GCC_DELAY(50);				//等待50us@4MHz
		out2 = _pltc0out;			//比较器输出状态
		if(out1 != out2)
		{
			break;
		}
	}
	//递减查找
	_pltc0vos = 0B01111111;			//OFM=1,RSP=1,OF=0
	GCC_DELAY(50);					//等待
	while(vos2 < 31)				//VOS范围0-31
	{
		out1 = _pltc0out;			//比较器输出状态
		_pltc0vos --;				//递增
		vos2 ++;
		GCC_DELAY(50);				//等待50us@4MHz
		out2 = _pltc0out;			//比较器输出状态
		if(out1 != out2)
		{
			break;
		}
	}
	vos2 = 31 - vos2;
	//计算
	vos1 = (vos1 + vos2) / 2;		//取中值
	_pltc0vos = vos1;				//保存VOS，不能禁用CMP
	
	//下一步
	vos1 = 0;
	vos2 = 0;
	
	//比较器1
	//递增查找
	_pltc1vos = 0B01100000;			//OFM=1,RSP=1,OF=0
	GCC_DELAY(50);					//等待
	while(vos1 < 31)				//VOS范围0-31
	{
		out1 = _pltc1out;			//比较器输出状态
		_pltc1vos ++;				//递增
		vos1 ++;
		GCC_DELAY(50);				//等待50us@2MHz
		out2 = _pltc1out;			//比较器输出状态
		if(out1 != out2)
		{
			break;
		}
	}
	//递减查找
	_pltc1vos = 0B01111111;			//OFM=1,RSP=1,OF=31
	GCC_DELAY(50);					//等待
	while(vos2 < 31)				//VOS范围0-31
	{
		out1 = _pltc1out;			//比较器输出状态
		_pltc1vos --;				//递增
		vos2 ++;
		GCC_DELAY(50);				//等待50us@2MHz
		out2 = _pltc1out;			//比较器输出状态
		if(out1 != out2)
		{
			break;
		}
	}
	vos2 = 31 - vos2;
	//计算
	vos1 = (vos1 + vos2) / 2;		//取中值
	_pltc1vos = vos1;				//保存VOS，不能禁用CMP
}

//------------------------------
//PtmInit  PTM  配置为上升沿、下降沿捕获
//注意:5541芯片的C0023批次前的PTM存在芯片问题，0.05%的概率PTM捕获值为0
//------------------------------
void PtmInit(void)
{
#ifdef CODE_SEL_SMOKE
    _ifs0 &= 0B00111111;
    // _ptmc0 = 0B00110000;
	_ptmc0 = 0B00100000;
    _ptmc1 = 0B01100000;
    _ptmc2 = 0B00000110; //TODO pltvf
    // _ptmrpl = 0x77;
    // _ptmrph = 0x01;
	// configPreciseTimer(0,TIMER_FALLING_TIMEOUT);
    _ptmpe = 1;
    _ptmae = 1;
    _pton = 1;
#else

    //使用5541芯片.有两个PTM0和PTM1。其中只有PTM0可以用作PLT的输入
    // _ifs &=0B00000011;      //IFS3~IFS2清零 PTP0I 输入选择CACAP
    // _ptm0c0 = 0B00100000;    // Fptm0 = Fh/16 = 4MHz/16 = 250KHz ;   PTM0 停止运行
    // _ptm0c1 = 0B01010000;    // PTM0 设置为捕获输入模式；比较器下降沿捕获输入，下降沿计数器值锁存至CCRA
    // _ptm0c2 = 0B00000100;    // 设置ptm0计数器清零条件为：比较器P发生匹配或捕获到下降沿

    // _ptm0rpl = 0xF4;		
    // _ptm0rph = 0x01;            //设置值为2000us，即 计数达到500后 发生PTM0 P匹配中断 并清零计数器

    // _ptm0pe = 1;
    // _ptm0ae = 1;

    // _mf1e = 1;              // 打开 多功能向量中断开关
    // //开启PTM计数
    // _pt0on = 1;

    _ifs &= 0B00000011;
    // _ptm0c0 = 0B00110000;
	_ptm0c0 = 0B00100000;
    _ptm0c1 = 0B01100000;
    _ptm0c2 = 0B00000110;
    // _ptm0rpl = 0x77;
    // _ptm0rph = 0x01;
	// configPreciseTimer(0,TIMER_FALLING_TIMEOUT);
	// timeLoopCounter = 1;
	// _ptm0rpl = 0xFF;
	// _ptm0rph = 0x03;//  1DD remainning
    _ptm0pe = 1;
    _ptm0ae = 1;
    _mf1e = 1;
    _pt0on = 1;
	
	// _pt0pau = 0;
#endif

}

//------------------------------
//电源收发器-运放-失调校准
//------------------------------
void PltCaliOpa (void)
{	
	uint8 vos1 = 0, vos2 = 0;
	uint8 out1 = 0, out2 = 0;
	//递增查找
	_pltavos = 0B11000000;			//OFM=1,RSP=1,OF=0
	GCC_DELAY(25);					//等待
	while(vos1 < 63)				//VOS范围0-63
	{
		out1 = _pltao;				//运放输出状态
		_pltavos ++;				//递增
		vos1 ++;
		GCC_DELAY(25);				//等待50us@2MHz
		out2 = _pltao;				//运放输出状态
		if(out1 != out2)
		{
			break;
		}
	}
	//递减查找
	_pltavos = 0B11111111;			//OFM=1,RSP=1,OF=63
	GCC_DELAY(25);					//等待
	while(vos2 < 63)				//VOS范围0-63
	{
		out1 = _pltao;				//比较器输出状态
		_pltavos --;				//递增
		vos2 ++;
		GCC_DELAY(25);				//等待50us@2MHz
		out2 = _pltao;				//比较器输出状态
		if(out1 != out2)
		{
			break;
		}
	}
	vos2 = 63 - vos2;
	//计算
	vos1 = (vos1 + vos2) / 2;		//取中值
	_pltavos = vos1;				//保存VOS，不能禁用CMP
}

//------------------------------
//电源收发器-回码-发送开
//------------------------------
// void PltOpaSwitchOn (void)
// {
// 	_pltsw |= 0B00000001;	//S2[2]=0-DA,S1[1]=0-IS,S0[0]=0-G	
// 	_pltda2l = DA_REF_DOUT;
// }

//------------------------------
//电源收发器-回码-发送关
//------------------------------
void PltOpaSwitchOff (void)
{
	
	_pltda2l = 0; 
	
	_pltsw &= ~0B00000001;	//S2[2]=0-DA,S1[1]=1-LV,S0[0]=0-G
	_pltdac2en = 0;			//关DAC
	_pltaen = 0;

	//GPO_DBG_0();			//测试用
}

//------------------------------
//PLT电压测量
//------------------------------
// uint8 PltLineVolt(void)
// {
// 	uint8 VoltTemp = 0;
// 	uint8 MaxVolt = 0;
// 	uint8 MinVolt = 255;
// 	uint16 SumVolt = 0;
// 	uint8 i = 0;
// 	uint16 cnt = 0;
// 	//开始
// 	_pltsw = 0B00000110;		//S2[2]=1-RX,S1[1]=1-LV,S0[0]=0-G
// 	_pltaen = 1;                // OPA使能

// 	//Smoke是5542其他是5541.两个芯片的AD定义是不一样的
// 	#ifdef CODE_SEL_SMOKE
// 	_sadc1 = 0B10001010;	//[4:3]SAVRS=01内部VDD,[2:0]SACKS=000时钟分频1
// 	#else
// 	_sadc1 = 0B01001010;	//[4:3]SAVRS=01内部VDD,[2:0]SACKS=000时钟分频1
// 	#endif
// 	//使能外设
// 	_sadc0 = 0B00101111;	//[5]ADCEN=1使能,[4]ADRFS=0高对齐
// 	//转换	
// 	GCC_DELAY(40);			// 延时40US @ 4MHz PLT 的OPA 使能后稳定需要40us

// 	//连续采样4次
// 	for(i=0;i<4;i++)
// 	{
// 		_start = 0;
// 		_start = 1;
// 		_start = 0;
// 		while((_adbz)&&(cnt<1000)){cnt++;}     //等待Ad转换完成  增加超时机制

// 		SumVolt += _sadoh;
// 		//更新最小值
// 		if(_sadoh < MinVolt)
// 		{
// 			MinVolt = _sadoh;
// 		}
// 		//更新最大值
// 		if(_sadoh> MaxVolt)
// 		{
// 			MaxVolt = _sadoh;
// 		}
// 	}
//     _adcen = 0;				//关闭ADC
    
// 	//去极值
// 	SumVolt -= MinVolt;
// 	SumVolt -= MaxVolt;

// 	VoltTemp = (uint8)(SumVolt>>1);
	
// 	FuncData.ValueCh3 = VoltTemp;  //取2平均

// 	return VoltTemp;   //直接返回AD值即可 因为要赋值给PLT的比较电平
// }

//------------------------------
//烟感放大电路失调校准
//------------------------------
#ifdef CODE_SEL_SMOKE
void SdCaliOpa (void)
{
	//运行时间10ms @4MHz主频
	uint8 tmp0=0,tmp1=0,Value1=0,Value2=0;

	//通讯中不校准
	FuncData.FlagBreak = 0;	
		
	_sdsw =0b00000011;
	_sda0c = 0b01000011;
	_sda0vos = 0b11000000;
	GCC_DELAY(8);
	tmp0 = _sda0o;
	tmp1 = _sda0o;
	while(tmp0 == tmp1)
	{
		_sda0vos++;
		GCC_CLRWDT();
		GCC_DELAY(52);	   //52Us@4MHz
		tmp1 = _sda0o;
		if(_sda0vos == 0xff)
		{
			break;
		}
	}
	Value1 = _sda0vos & 0b00111111;

	_sda0vos = 0b11111111;
	GCC_DELAY(8);
	tmp0 = _sda0o;
	tmp1 = _sda0o;
	while(tmp0 == tmp1)
	{
		_sda0vos--;
		GCC_CLRWDT();
		GCC_DELAY(52);	   //52Us@1MHz
		tmp1 = _sda0o;
		if(_sda0vos == 0xc0)
		{
			break;
		}
	}
	Value2 = _sda0vos & 0b00111111;

	Value1 = ((Value1+Value2)>>1);
	_sda0vos = Value1;

	//************************************************
	_sdsw =0b00100000;
	_sda1c = 0b01000011;

	_sda1vos = 0b11000000;
	GCC_DELAY(8);
	tmp0 = _sda1o;
	tmp1 = _sda1o;
	while(tmp0 == tmp1)
	{
		_sda1vos++;
		GCC_CLRWDT();
		GCC_DELAY(52);	   //52Us@1MHz
		tmp1 = _sda1o;
		if(_sda1vos == 0xff)
		{
			break;
		}
	}
	Value1 = _sda1vos & 0b00111111;

	_sda1vos = 0b11111111;
	GCC_DELAY(8);
	tmp0 = _sda1o;
	tmp1 = _sda1o;
	while(tmp0 == tmp1)
	{
		_sda1vos--;
		GCC_CLRWDT();
		GCC_DELAY(52);	   //52Us@1MHz
		tmp1 = _sda1o;
		if(_sda1vos == 0xc0)
		{
			break;
		}
	}
	Value2 = _sda1vos & 0b00111111;
	Value1 = ((Value1+Value2)>>1);
	_sda1vos = Value1;

#ifdef CODE_SEL_SMOKE_HENGRUN
	AfeInit(9,AFE_R3_10K,2,OPAMODE_INIT);
#endif
	SdOpaSwitch(0);  //关运放省电
	//校准过程中如果发生通讯中断，则放弃本次校准
	if(FuncData.FlagBreak > 0)
	{
    	FuncData.FlagAmpCalid = 0;
		return;
	}
	FuncData.FlagAmpCalid = 1;   //置校准成功标志
}
#endif
//------------------------------
//烟感放大电路开关
//输入0：关闭
//输入1：开启
//------------------------------
#ifdef CODE_SEL_SMOKE
void SdOpaSwitch (uint8 Flag)
{
	if(Flag == 0)			//关闭
	{	
		//SDS0控制的偏置电路有2uA的电流，在使用时再开启
		_sds0 = 0;			//关一级OPA 偏置
		_sda0en = 0;		//关一级OPA
		_sda1en = 0;        //关二级OPA
	}
	else					//开启
	{
		//保持偏置SDS0 闭合
		_sds0 = 1;			//开一级OPA 偏置
		_sda0en = 1;        //一级OPA 使能
		GCC_DELAY(2000);
		_sda1en = 1;        //二级OPA 使能
		GCC_DELAY(2000);
	}	
}
#endif


//------------------------------
//PWM输出-STM定时器[5542]-PTM0定时器[5541]
//输入=0：停止输出
//输入=1：连续PWM
//------------------------------
#ifdef CODE_SEL_SMOKE
#ifdef CODE_SEL_SMOKE_BUZZLE
uint8 SmokeBuzzlePwm(uint8 flag)
{
	if(0 == flag)
	{
		//引脚输出恢复成IO
		_pas11 = 0;
		_pas10 = 0;
		_stmc0 = 0B00000000;		//关定时器
		_stmc1 = 0B00000000;
		return _stmdl;				//定时值
	}
	else if(1 == flag)   //正常发声
	{
		//PA4输出 PWM
		#if 1
		//分档调节占空比
	    if(FuncData.LoopVolt > 68)  //68:5542 19.92V总线电压
		{
			//256个clk
			_stmc0 = 0B00010010;   //主频4M  256CLK
			_stmc1 = 0B10100011;   //工作模式PWM  PWM输出 CCRP控制占空比 CCRA周期 A匹配清零
		}
		else
		{
			_stmc0 = 0B00010100;   //主频4M  512CLK
			_stmc1 = 0B10100011;   //工作模式PWM  PWM输出 CCRP控制占空比 CCRA周期 A匹配清零	
		}

		//4M主频 1024个计数  3.9K  最小频率
		_stmal = 0xFF;  
		_stmah = 0x03;
		#endif

		
		#if 0
		//占空比无法实现的!!!
		//分档调节占空比
		//主频4M/4  
		//128个clk	
		_stmc0 = 0B00000001;   //主频1M  128CLK
		_stmc1 = 0B10100011;   //工作模式PWM  PWM输出 CCRP控制占空比 CCRA周期 A匹配清零 
		//262个clk  262us 3.82K声音比较OK
		//但是没办法实现: 128clk相当于 50%占空比
		_stmal = 0x06;  
		_stmah = 0x01;
		#endif
		
		_ston  = 1;  //启动 PWM

		_pas11 = 0;
		_pas10 = 1;
		
		return flag;
	}
	else if(2 == flag)   //工厂测试音
	{
		//PA4输出 PWM
		_stmc0 = 0B00010001;   //主频4M  128CLK
		_stmc1 = 0B10100011;   //工作模式PWM  PWM输出 CCRP控制占空比 CCRA周期 A匹配清零

		_stmal = 0xFF;  //1023个clk  255.75 3.9K声音比较OK
		_stmah = 0x03;

		_pas11 = 0;
		_pas10 = 1;
		
		_ston  = 1;  //启动 PWM
	}

	return flag;
}
#endif
#endif

//------------------------------
//ADC转换程序，输入通道号，返回8位数据
//0=A0,1=A1,2=A2,3=A3,4=VREF,5=OPA0,6=OPA1,7-LINEV
//------------------------------
//感温使用12位AD
#ifdef CODE_SEL_HEAT
uint16 AdcSample (uint8 Ch)
{
	uint8 c0,c1;
	uint16 Sadoh = 0;
	uint16 rtn;
	uint16 i =0;

	//通道设置
	switch(Ch)
	{ 
		//外部通道c0=SADC0[3:0]=0-3,c1=SADC1[7:5]=000
		case 0:{c0=0x00; c1=0x00;}break;//AN0
		case 1:{c0=0x01; c1=0x00;}break;//AN1
		case 2:{c0=0x02; c1=0x00;}break;//AN2
		case 3:{c0=0x03; c1=0x00;}break;//AN3
		//内部通道c0=SADC0[3:0]=0x0f,c1=SADC1[7:5]=001-100
		case 4:{c0=0x08; c1=0x20;}break;//VREF
		case 5:{c0=0x08; c1=0x40;}break;//PLT LINEV
		case 6:{c0=0x08; c1=0x60;}break;//内部输入  AVSS
		default:{return 0;}break;
	}
	//_vbgrc = 0x01;//Bandgap开启，测试用
	//设定时钟
	//AD转换周期14个AD周期
	//模块修改为fsys/16   4us
	_sadc1 = (0B00001100  | c1);	//[4:3]SAVRS=01内部VDD,[2:0]SACKS=000时钟分频1
	//使能外设
	_sadc0 = c0;	//[5]ADCEN=1使能,[4]ADRFS=0高对齐
	_adcen = 1;
	//转换	
	GCC_DELAY(8);			// 延时40US @ 4MHz

	//单次采样

	_start = 0;
	_start = 1;
	_start = 0;
	while(_adbz && (i<1000))
	{
		i++;
	}
	Sadoh = _sadoh;
	//10位AD，返回针对感温统一算法的12位AD的位数
    rtn = (((Sadoh << 8) | _sadol)>>4) ;    		//返回结果  
    _adcen = 0;				//关闭ADC

    return rtn;   //感温统一算法目前是针对12bit 的AD。后期
}
#else
uint16 AdcSample (uint8 Ch)
{
	uint8 c0,c1;
	uint16 rtn;
	uint16 i =0;
	uint16 Sadoh = 0;

	//4平均去极值
    #if 0   
	uint8 MaxVolt = 0;
	uint8 MinVolt = 255;
	uint16 SumVolt = 0;
	uint8 i = 0;
    #endif

	//通道设置
	switch(Ch)
	{ 
		//外部通道c0=SADC0[3:0]=0-3,c1=SADC1[7:5]=000
		case 0:{c0=0x00; c1=0x00;}break;//AN0
		case 1:{c0=0x01; c1=0x00;}break;//AN1
		case 2:{c0=0x02; c1=0x00;}break;//AN2
		case 3:{c0=0x03; c1=0x00;}break;//AN3
		//内部通道c0=SADC0[3:0]=0x0f,c1=SADC1[7:5]=001-100
		case 4:{c0=0x08; c1=0x20;}break;//VREF

		//Smoke使用的5542芯片的AD和其他模块使用的5541不一样
	#ifdef CODE_SEL_SMOKE
		case 5:{c0=0x08; c1=0x40;}break;//放大器OPA0
		case 6:{c0=0x08; c1=0x60;}break;//放大器OPA1
		case 7:{c0=0x08; c1=0x80;}break;//收发器LINEV
	#else
		case 5:{c0=0x08; c1=0x40;}break;//PLT LINEV
		case 6:{c0=0x08; c1=0x60;}break;//内部输入  AVSS
	#endif
		default:{return 0;}break;
	}
	//_vbgrc = 0x01;//Bandgap开启，测试用
	//设定时钟
	//AD转换周期14个AD周期
	#ifdef CODE_SEL_SMOKE
	//AD转换的Tack 0.5~10us 。所以 4M主频，必须分频 010 fsys/4  1us
	_sadc1 = (0B00001010  | c1);	//[4:3]SAVRS=01内部VDD,[2:0]SACKS=000时钟分频1
	#else
	//模块修改为fsys/16   4us
	_sadc1 = (0B00001100  | c1);	//[4:3]SAVRS=01内部VDD,[2:0]SACKS=000时钟分频1
	#endif
	//使能外设
	_sadc0 = c0;	//[5]ADCEN=1使能,[4]ADRFS=0高对齐
	_adcen = 1;
	//转换	
	GCC_DELAY(8);			// 延时8US @ 4MHz

	//单次采样
	#if 1
	_start = 0;
	_start = 1;
	_start = 0;
	while(_adbz && (i<1000))
	{
		i++;
	}
	Sadoh = _sadoh;
    //rtn = _sadoh;    		//返回结果
	rtn = (((Sadoh << 8) | _sadol)>>4) ;    		//返回结果  
    _adcen = 0;				//关闭ADC

	#endif

    return rtn;
}
#endif

// void configPreciseTimer(uint16 duration,uint8 flag)
// {
// 	uint16 tempDuration = 0;
// 	#ifdef CODE_SEL_SMOKE
// 		switch (flag)
// 		{
// 		case TIMER_RISING_BIT_TIMEOUT/* constant-expression */:
// 			/* code */
// 			//1ms
// 			_ptmrpl = 0xFA;
// 			_ptmrph = 0x00;
// 			timeLoopCounter = 0;
// 			break;
// 		case TIMER_FALLING_HEAD_TIMEOUT:
// 			_ptmrpl = 0xF4;
// 			_ptmrph = 0x01;
// 			timeLoopCounter = 0;
// 			break;
// 		case TIMER_FALLING_TIMEOUT:
// 			//6ms
// 			timeLoopCounter = 1;
// 			_ptmrpl = 0xFF;
// 			_ptmrph = 0x03;//  1DD remainning
// 			break;
// 		case TIMER_FALLING_PW_BIT_TIMEOUT:
// 			timeLoopCounter = 4;
// 			_ptmrpl = 0xFF;
// 			_ptmrph = 0x03; 
// 			break;
// 		case TIMER_DRAW_CURRENT:
// 			_ptmrpl = (uint8)(tempDuration&0x00FF);
// 			_ptmrph = (uint8)(tempDuration>>8); 
// 			break;
// 		default:
// 			break;
// 		}
// 		_pton = 0;
// 		_pton = 1;
// 	#else
// 		switch (flag)
// 		{
// 		case TIMER_RISING_BIT_TIMEOUT/* constant-expression */:
// 			/* code */
// 			//1ms
// 			_ptm0rpl = 0xFA;
// 			_ptm0rph = 0x00;
// 			timeLoopCounter = 0;
// 			break;
// 		case TIMER_FALLING_HEAD_TIMEOUT:
// 			_ptm0rpl = 0xF4;
// 			_ptm0rph = 0x01;
// 			timeLoopCounter = 0;
// 			break;
// 		case TIMER_FALLING_TIMEOUT:
// 			//6ms
// 			timeLoopCounter = 1;
// 			_ptm0rpl = 0xFF;
// 			_ptm0rph = 0x03;//  1DD remainning
// 			break;
// 		case TIMER_FALLING_PW_BIT_TIMEOUT:
// 			timeLoopCounter = 4;
// 			_ptm0rpl = 0xFF;
// 			_ptm0rph = 0x03; 
// 			break;
// 		case TIMER_DRAW_CURRENT:
// 			_ptm0rpl = (uint8)(tempDuration&0x00FF);
// 			_ptm0rph = (uint8)(tempDuration>>8); 
// 			break;
// 		default:
// 			break;
// 		}
// 		_pt0on = 0;
// 		_pt0on = 1;
// 	#endif
// }


/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
