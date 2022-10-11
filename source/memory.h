/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      memory.h
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     
 ******************************************************************************/
#ifndef __MEMORY_H
#define __MEMORY_H
/*******************************************************************************
 * 宏定义
 ******************************************************************************/
#define MEM_EEPROM_LEN		(32)		//EEPROM长度  暂不超过32

//由于多处写EE。而EE操作比较慢，所以建立写EE操作的Buff
//3个BUFF是因为最大情况为:通讯写EE和写入漂移补偿红蓝光值得时刻
#define WR_BUFF_LEN			(3)         
//EE参数
#define MEM_EEPROM_KEY1		(0x55)		//EEPROM Write Operate Open Key 1
#define MEM_EEPROM_KEY2		(0x3A)		//EEPROM Write Operate Open Key 2
#define MEM_EEPROM_OPEN		(0xFF)		//EEPROM Write Operate Key
//EE操作步骤
#define MEM_STEP_IDLE		(0x00)
#define MEM_STEP_START		(0x01)
#define MEM_STEP_START_SELF (0x02)
#define MEM_STEP_WRITE		(0x03)
/*******************************************************************************
 * 变量定义
 ******************************************************************************/
//EEPROM数据
typedef struct
{
	uint8 Step;					//工作步骤
	uint8 Index;				//写入数据地址ַ
	uint8 Data;					//写入数据缓存
	uint8 Key;					//开锁密钥存储
	uint8 Rom[MEM_EEPROM_LEN];
	//写操作buff
	uint8 WrAddrBuff[WR_BUFF_LEN];  //待写入EE的地址BUFF
	uint8 WrDataBuff[WR_BUFF_LEN];	//待写入EE的数据BUFF
	uint8 WrBuffCnt;//待写入数据CNT
	
}MemDataRegs;

/*******************************************************************************
 * 全局变量引用定义
 ******************************************************************************/
extern MemDataRegs	MemData;

/*******************************************************************************
 * 函数声明
 ******************************************************************************/
extern void MemoryInit (void);
extern uint8 MemoryWrite (void);
extern void MemoryProcess (void);
extern void PutDataToWriteEEBuff(uint8 Index,uint8 Data);

/*******************************************************************************
 * 枚举定义
 ******************************************************************************/
//EEPROM参数定义
typedef enum {

		//-------------User Configuration Area---------------
	EEPROM_ID_ADDRESS  		= 0U,
	EEPROM_ID_SENSITIVITY   = 1U,       

	EEPROM_CONFIG1			= 2U,   	
	EEPROM_CONFIG2			= 3U,   	 
	//EEPROM_CONFIG3			= 4U,   //测试必用
	EEPROM_ID_LOCK   		= 4U, 	 

	EEPROM_ID_TYPE			= 5U,   	 
	EEPROM_ID_OEM_CODE		= 6U,		
	EEPROM_CONFIG4			= 7U,   	
	EEPROM_CONFIG5			= 8U,   	
	EEPROM_ID_FAC_TEST_FLAG	= 9U,		 
	EEPROM_ID_DATECODE		= 10U,
	EEPROM_CONFIG6			= 13U,   	
	EEPROM_ID_FW_VERSION	= 14U,	
	//EEPROM_ID_LOCK   		= 15U,      
	EEPROM_ID_CLEARAIR		= 16U,		
	EEPROM_ID_CLEARAIRBL    = 17U,      
	EEPROM_ID_IR_CALI	    = 18U,		
	EEPROM_ID_BL_CALI	    = 19U,		
	EEPROM_ID_IR_COMPEN	    = 20U,	

}EEPROMIndex;

#endif /* __MEMORY_H */
/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
