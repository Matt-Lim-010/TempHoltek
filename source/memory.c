	/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      memory.c
 * @author    S
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     存储器程序
 ******************************************************************************/
#include "include.h" 

//------------------------------
//存储初始
//------------------------------
void MemoryInit (void)
{
	uint8 i;
	for(i=0;i<MEM_EEPROM_LEN;i++)
	{
		_eea = i;
		_mp1l = 0x40; //MP1=EEC (Bank1)
		_mp1h = 0x01; //MP1=EEC (Bank1)	
		#asm
			IAR1 EQU [02H]
			SET IAR1.1 ; RDEN=1
			SET IAR1.0 ; RD=1
		LDWAIT:
			SZ  IAR1.0 ; wait
			JMP LDWAIT ;
		#endasm
		_iar1 = 0;    //RESET
		_mp1h = 0x00;
		//保存
		MemData.Rom[i] = _eed;
	}
	MemData.Rom[EEPROM_ID_FW_VERSION] = SUB_VER2;
}


//------------------------------
//存储数据命令, 返回0=正常
//------------------------------
uint8 MemoryWrite (void)
{
	uint8 i = 0;
	//空闲检查
	if(MemData.Step != MEM_STEP_IDLE)
		return 1;

	//数据写入
	MemData.Data	= MemData.WrDataBuff[0];//数据
	MemData.Index	= MemData.WrAddrBuff[0];//地址
	MemData.Step	= MEM_STEP_START;		//写指令

	//
	for(i = 1;i<MemData.WrBuffCnt;i++)
	{
		MemData.WrAddrBuff[i-1] = MemData.WrAddrBuff[i];
		MemData.WrDataBuff[i-1] = MemData.WrDataBuff[i];	
	}	
	
	//成功完成一次写EE操作，待写入数量-1
	if(MemData.WrBuffCnt)
		MemData.WrBuffCnt--;	
	
	return 0;
}

//------------------------------
//PutDataToWriteEEBuff  要发送的数据写入
//------------------------------
void PutDataToWriteEEBuff(uint8 Index,uint8 Data)
{	
	//越界返回
	if(Index > (MEM_EEPROM_LEN-1))
	{
		return ;
	}
	//超出Buff容量不再写入
	if(MemData.WrBuffCnt < WR_BUFF_LEN)
	{
		MemData.WrAddrBuff[MemData.WrBuffCnt] = Index;   //存入最新值
		MemData.WrDataBuff[MemData.WrBuffCnt] = Data;	
		MemData.WrBuffCnt++;
		
	}
	
}

//------------------------------
//存储操作
//------------------------------ 
void MemoryProcess (void)
{
	//空闲检查
	if(MemData.Step == MEM_STEP_IDLE)
	{
		SysData.CmdActive &= ~ACTIVE_FLAG_EEPROM;//去除激活
		return;
	}
	else
	{
		SysData.CmdActive |= ACTIVE_FLAG_EEPROM;//开启激活
	}
	//通信检查
	if(CommData.Step != COMM_STEP_IDLE)
	{
		return;
	}
	//写入数据步骤
	switch(MemData.Step)
	{
		//-----开始入口-----
		case MEM_STEP_START:
		case MEM_STEP_START_SELF:
		{
			//工厂数据区
			//工厂设置区范围时0x04~0x3F
			//0x20之后的内容暂时不用，全部锁定
			if((MemData.Index < 0x3F)
			&&(MemData.Index > 0x03))
			{
				//解锁状态或自写
				if( (MemData.Rom[EEPROM_ID_LOCK] == MEM_EEPROM_OPEN) ||
					(MemData.Step == MEM_STEP_START_SELF))
				{
					MemData.Key  = 0;					//清除解锁密钥
					MemData.Step = MEM_STEP_WRITE;		//进入写过程
				}
				//解锁地址
				else if(MemData.Index == EEPROM_ID_LOCK)
				{
					if( (MemData.Key  == MEM_EEPROM_KEY1) && \
						(MemData.Data == MEM_EEPROM_KEY2) )
					{
						MemData.Key  = 0;				//清除解锁密钥
						MemData.Data = MEM_EEPROM_OPEN;	//设置解锁数据
						MemData.Step = MEM_STEP_WRITE;	//进入写过程
					}
					else
					{
						MemData.Key   = MemData.Data;	//保存当前密钥
						MemData.Index = 0;				//清除地址
						MemData.Data  = 0;				//清除数据
						MemData.Step  = MEM_STEP_IDLE;//退出 并保持激活
					}
				}
				else
				{
					MemData.Key   = 0;					//清除解锁密钥
					MemData.Index = 0;					//清除地址
					MemData.Data  = 0;					//清除数据
					MemData.Step  = MEM_STEP_IDLE;		//退出
				}
			}
			//用户数据
			else
			{
				MemData.Key   = 0;						//清除解锁密钥
				MemData.Step = MEM_STEP_WRITE;			//进入写过程			
			}
		}break;
		//写命令执行
		case MEM_STEP_WRITE:
		{
			_eea = MemData.Index;						//地址
			_eed = MemData.Data; 						//数据
			//访问EEC
			_mp1l = 0x40; 					//MP1=EEC (Bank1)
			_mp1h = 0x01; 					//MP1=EEC (Bank1)	
			#asm
			WR:
				SET IAR1.3 ; WREN=1
				SET IAR1.2 ; WR=1
			WRWAIT:
				SZ  IAR1.2 ; wait
				JMP WRWAIT
				CLR IAR1.3 ; WREN=0
			RD:
				SET IAR1.1 ; RDEN=1
				SET IAR1.0 ; RD=1
			RDWAIT:
				SZ  IAR1.0 ; wait
				JMP RDWAIT
			#endasm
			_iar1 = 0;    								//关闭
			_mp1h = 0x00;								//关闭
			//更新
			MemData.Step = MEM_STEP_IDLE;				//退出			
			MemData.Rom[MemData.Index] = _eed;
			
		}break;
		default:{MemData.Step = MEM_STEP_IDLE;}break;
	}
}

//------------------------------
//EEPROM 初始数据定义:产品参数
//------------------------------ 
//暂无
//EERPOM初始数据，请在HT-IDE3000环境中设置
//设置方法：界面顶部菜单栏->工具->编辑器->Data Editor
//其中数据以十六进制表示，设置完成后点击保存即可。
//注意保存文件的位置，项目文件夹根目录下的MEM文件不一定是保存的默认位置。
 /******************************************************************************
 * End of file                                                                *
 ******************************************************************************/