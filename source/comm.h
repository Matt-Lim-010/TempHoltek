/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      comm.h
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     头文件
 ******************************************************************************/
#ifndef __COMM_H
#define __COMM_H
/*******************************************************************************
 * 宏定义
 ******************************************************************************/
//通信位的计数定义
#define COMM_HEAD_BITCNTS			13			
#define COMM_HEAD_ADDRESS			9
#define COMM_HEAD_CLIP_CMD			12
#define COMM_HEAD_CLIP_VERIFY		13


#define COMM_DLIP_CMD				17
#define COMM_DLIP_DATA1				25
#define COMM_DLIP_DATA2				33
#define COMM_DLIP_VERIFY			41
#define COMM_DLIP_ER				42
#define DLIP_READ_REG				0x0
#define DLIP_READ_EEPROM			0x1
#define DLIP_WRITE_REG				0x08
#define DLIP_WRITE_EEPROM			0x09
#define DLIP_READ_CMD_BARRIER		8

// #define CLIP_PREVIOUS_BIT_NUM		12					
#define CLIP_PW2					14
#define CLIP_PW3					15
#define CLIP_PW4					16
#define CLIP_PW5					17
#define CLIP_END					18
#define PW_ERROR					999

//ͨ�Ų������
#define COMM_STEP_IDLE		(0x00)		//通信空闲
#define COMM_STEP_HEAD		(0x01)		//帧头接收
#define COMM_STEP_CLIP		(0x02)		//CLIP通信
// #define COMM_STEP_CLIP_NA	(0x03)		//CLIP通信不应答
// #define COMM_STEP_DLIP_F1	(0x04)		//DLIP事件帧
// #define COMM_STEP_DLIP_F2	(0x05)		//DLIP分组帧
#define COMM_STEP_DLIP		(0x06)		//DLIP读写帧
// #define COMM_STEP_DLIP_F4	(0x07)		//DLIP编址帧
// #define COMM_STEP_DLIP_F5	(0x08)		//DLIP编程帧
#define COMM_STEP_OVER		(0x09)		//通信完成  //通信校验完成
#define COMM_STEP_ERROR		(0x0A)		//通信错误
#define COMM_CLIP_WRITE		(0x0B)		//CLIP write address flag
//通信状态标志
#define COMM_STATE_IDLE		(0x00)		//通信空闲状态
#define	COMM_STATE_BUSY		(0x01)		//通信忙状态
#define	COMM_STATE_WAIT		(0x03)		//通信等待状态
//工厂密码	
#define CLIP_KEY1			(0xAA)		//CLIP设置密码1 
#define CLIP_KEY2			(0xA9)		//CLIP设置密码2

#define COMM_TIMEOUT_FLAG	0
#define TIMEOUT_DURATION	3500
#define TIMEOUT_AFTER_BIT_TIMEOUT	2500

#define TIMER_RISING_BIT_TIMEOUT		1
#define TIMER_FALLING_HEAD_TIMEOUT		2
#define TIMER_FALLING_TIMEOUT			3
#define TIMER_FALLING_PW_BIT_TIMEOUT	4
#define TIMER_DRAW_CURRENT				5

#define COMM_DRAW_TIMER_FLAG		1
#define COMM_BIT_TIMEOUT_FLAG		2
#define FRAME_TIMEOUT_DURATION	3500
#define BIT_TIMEOUT_DURATION	2000
#define PW_BIT_TIMEOUT_DURATION	20000
#define CLIP_ERROR_TIMEOUT_DURATION	6000

#define PW4_NORMAL					0
#define PW4_Error_WAIT_DRAWING		1
#define PW4_ERROR_DRAWING			2

#define CLIP_PROTOCOL_RESET_FLAG					(0)
#define CLIP_PROTOCOL_CLIP_FLAG						(1)
#define CLIP_PROTOCOL_DLIP_FLAG						(2)
#define CLIP_PROTOCOL_INVALID_FRAME					(4)
#define CLIP_PROTOCOL_ADDRESS_INVALID				(5)
#define CLIP_WRITE_ADD_CHECKED						1
//通信位的计数定义
#define COMM_HEAD_BITCNTS	13			//
//
#define COMM_DLIP_TYPE_INFOR  0x00D0
#define COMM_DLIP_INDICATOR  		0x00D
/*******************************************************************************
 * 变量定义
 ******************************************************************************/
typedef struct {
	//通信
	uint8	Step;			//通信步骤标志
	uint8	Busy;			//通信忙碌状态
	//数据收发
	uint16	AddrCLIP;		//CLIP地址转码后
	uint8	AddrDLIP;		//DLIP地址
	//通信收发数据
	uint8	BitCnt;			//数据位计数
	uint16	DataTx;			//发送数据位
	uint16	DataRx;			//接收数据位
	uint16	DataAddr;		//地址
	uint8 	DataCmd;		//命令
	uint8 	DataDat1;		//数据1
	uint8 	DataDat2;		//数据2
	uint8 	DataVB;			//校验位
	uint8	DataChk;		//校验字节
	uint8 	DataER;			//应答标志
	//CLIP通信功能
	uint16	DataPW;			//PW回码宽度
	uint8	CLIPKey1;		//CLIP工厂设置Key1
	uint8	CLIPKey2;		//CLIP工厂设置Key2
	uint8	CLIPKey3;		//CLIP工厂设置Key3
	uint8	CLIPPW5;		//CLIP输出PW5
	uint8	CLIPCmdLast;	//CLIP上次指令
	uint8	CLIPCtrl;		//CLIP控制信号
	//电平检测
	//uint8	VoltLowCnt;		//电压低计数
	//编址功能
	//uint8	DataSN[4];		//��ַ����
	//扩展地址
	//uint8	ExtCLIPPart;	//CLIP扩展标志
	//uint8	ExtCLIPCmdLast;	//CLIP 扩展部分:上次指令
	//uint8	ExtCLIPCtrl;	//CLIP 扩展部分:控制信号
	//uint16	ExtAddrCLIP;//CLIP地址转码后
	//协议用
	uint8	ClkNext;		//下一时钟状态
}CommDataRegs;

/*******************************************************************************
 * 全局变量引用
 ******************************************************************************/
extern CommDataRegs CommData;
extern uint8 const CLIPAddrEncode[10];
extern uint8 const CLIPAddrDecode[10];
/*******************************************************************************
 * 函数声明
 ******************************************************************************/

extern void CommTimeout (void);
extern void CommBitErr(void);
extern void CommTimeoutConfig(int duration);

void CommProtocol (void);
uint8 GetReg (uint8 Index);
uint8 SetReg (uint8 Index, uint8 Data);

extern void RefreshData (void);
// extern void TimeoutConfig(int duration,int type);
void CommProtocol(void);
uint16 ClipGetPW(uint8 PWNumber);
void CommRxTx(void);
/*******************************************************************************
 * 枚举定义
 ******************************************************************************/
//寄存器坐标定义
typedef enum {
	REG_ID_STATE		= 0U,	//STATE寄存器
	REG_ID_EXTSTATE		= 1U,	//EXTSTATE寄存器
	REG_ID_TYPEID       = 2U,   //TYPEID 寄存器
	REG_ID_LEDCONTROL   = 3U,   //LEDCONTROL 寄存器
	REG_ID_CONTROL		= 4U,	//OUT寄存器
	REG_ID_EXTOUT       = 5U,   //Extended Output Control 寄存器
	REG_ID_COMMAND		= 6U,	//COMMAND寄存器
	REG_ID_EXTCOMMAND   = 7U,   //Reserved
	REG_ID_ZONE         = 8U,   //Zone num
	REG_ID_CONFIG       = 9U,   //配置寄存器
	//-------------------------------
	//0x10~0x19 用来作临时数据的输出
	REG_ID_SVALUEL		= 0x10,	//SVALUEL寄存器	  16
	REG_ID_SVALUEH		= 0x11,	//SVALUEH寄存器   17
	REG_ID_VALUE1L		= 0x12,	//CH1采样值低字节 18
	REG_ID_VALUE1H		= 0x13,	//CH1采样值高字节 19
	REG_ID_VALUE2L		= 0x14,	//CH2采样值低字节 20
	REG_ID_VALUE2H		= 0x15,	//CH2采样值高字节 21
	REG_ID_VALUE3L		= 0x16,	//CH3采样值低字节 22
	REG_ID_VALUE3H		= 0x17,	//CH3采样值高字节 23
	REG_ID_VALUE4L		= 0x18,	//CH4采样值低字节 24
	REG_ID_VALUE4H		= 0x19,	//CH4采样值高字节 25

	//---------------------------------
	//FACUSE  ZONE 空间
	//保留
	REG_ID_EOL_CMD		= 208U,	//D0 EOL命令
	REG_ID_EOL_STATE	= 209U,	//D1 EOL状态
	REG_ID_EOL_VAL1_L	= 210U,	//D2 数值1
	REG_ID_EOL_VAL1_H	= 211U,	//D3
	REG_ID_EOL_VAL2_L	= 212U,	//D4 数值2
	REG_ID_EOL_VAL2_H	= 213U,	//D5
	REG_ID_EOL_VAL3_L	= 214U,	//D6 数值3
	REG_ID_EOL_VAL3_H	= 215U,	//D7
	REG_ID_EOL_TAR1_L	= 216U,	//D8 目标值1
	REG_ID_EOL_TAR1_H	= 217U,	//D9
	REG_ID_EOL_TAR2_L 	= 218U,	//DA 目标值2
	REG_ID_EOL_TAR2_H 	= 219U,	//DB
	REG_ID_EOL_RSVD220	= 220U,	//DC -
	REG_ID_EOL_RSVD221	= 221U,	//DD -
	REG_ID_EOL_RSVD222	= 222U,	//DE -
	REG_ID_EOL_MODE 	= 223U,	//DF EOL模式
	
}DlipRegIndex;

#endif /* __COMM_H */
/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
