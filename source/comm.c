/******************************************************************************
 * (c) Copyright 1995-2020, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      comm.c
 * @author    S
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     通信程序
 ******************************************************************************/
 #include "include.h"
 #include "clip.h"

extern uint8 CLIPDLIPFlag; // 0校验失败， 1CLIP 2 DLIP
extern uint8 m_ptmPfunc;
extern uint8 m_ptm0Pfunc;
// void CommTimeoutConfig(int duration);
// static uint8 remoteTestIndicator = 0; // 0: disable, 1: wait for second remote test cmd. 2: start remote test
#ifdef CODE_SEL_SMOKE
extern uint8 PW4ErrorFlag;
#endif
uint8 CLIPWriteFlag = 0; //0: default; 1: previous two cmd checked,waiting for new Address input
// void TimeoutConfig(int duration,int type);

//------------------------------
//通信协议 Clip Dlip
//------------------------------
void CommProtocol(void)
{
    uint8 Tmp;//  Dat1, Dat2;
    uint16 Tmp16;
    switch (CommData.Step)
    {
		case COMM_STEP_HEAD:
		{
			switch (CommData.BitCnt)
			{
				#if 1
					#ifndef CODE_SEL_SMOKE_BUZZLE 
					case 3:
					{
						Tmp16 = (uint8)(CommData.AddrCLIP >> 6) ;
						CommData.DataAddr = CommData.DataRx & 0x7;
						if (CommData.DataAddr == Tmp16)
						{
							break;
						}
						// TmpAdd = (CommData.AddrDLIP & 0xF0) >> 4;
						Tmp16 = CommData.AddrDLIP >> 6;
						Tmp = CommData.DataAddr & 0x3;
						// screen DLIP indicator
						if (Tmp == Tmp16 || Tmp == 0)
						{
							Tmp16 = 0;
							Tmp = 0;
							break;
						}
						Tmp16 = CommData.DataAddr & 0x3;
						if(Tmp16 == 0x2 || CLIPWriteFlag == CLIP_WRITE_ADD_CHECKED)
						{
							Tmp16 = 0;
							break;
						}
						//Test : temporary test code
						Tmp16 = 0;
						Tmp = 0;
						CLIPDLIPFlag = CLIP_PROTOCOL_INVALID_FRAME;
						CommData.Step = COMM_STEP_OVER;
					}
					break;
					#endif
				#endif
				//TODO:
				//5 bits
			#if 1
				case 5:
				{
					Tmp16 = (CommData.AddrCLIP & 0x1FF) >> 4 ;
					CommData.DataAddr = CommData.DataRx & 0x1F;
					if (CommData.DataAddr == Tmp16)
					{
						// CLIPDLIPFlag = CLIP_PROTOCOL_CLIP_FLAG;// Address confirmed
						Tmp16 = 0;
						break;
					}
					Tmp16 = (CommData.AddrDLIP & 0xF0) >> 4;
					Tmp = CommData.DataAddr & 0x0F;
					// screen DLIP indicator
					if (Tmp == Tmp16 || Tmp == 0)
					{
						// CLIPDLIPFlag = CLIP_PROTOCOL_DLIP_FLAG;
						Tmp16 = 0;
						Tmp = 0;
						break;
					}
					Tmp16 = CommData.DataAddr & 0x0F;
					if(Tmp16 == 0xA || CLIPWriteFlag == CLIP_WRITE_ADD_CHECKED)
					{
						// CLIPDLIPFlag = CLIP_PROTOCOL_ADDRESS_INVALID;
						Tmp16 = 0;
						break;
					}
					//Test : temporary test code
					Tmp16 = 0;
					Tmp = 0;
					CLIPDLIPFlag = CLIP_PROTOCOL_INVALID_FRAME;
					CommData.Step = COMM_STEP_OVER;
				}
				break;
			#endif
				case COMM_HEAD_ADDRESS:
				{
					CommData.DataAddr = CommData.DataRx & 0x1FF;
					if (CommData.DataAddr == CommData.AddrCLIP)
					{
						CLIPDLIPFlag = CLIP_PROTOCOL_CLIP_FLAG;// Address confirmed
						break;
					}
					
					// screen DLIP indicator
					if ((CommData.DataAddr & 0x00F) == COMM_DLIP_INDICATOR)
					{
						CLIPDLIPFlag = CLIP_PROTOCOL_DLIP_FLAG;
						break;
					}
					Tmp16 = CommData.DataAddr & 0xF0;
					if(Tmp16 == 0xA0 || CLIPWriteFlag == CLIP_WRITE_ADD_CHECKED)
					{
						CLIPDLIPFlag = CLIP_PROTOCOL_ADDRESS_INVALID;
						Tmp16 = 0;
						break;
					}
					//Test : temporary test code
					Tmp16 = 0;
					CLIPDLIPFlag = CLIP_PROTOCOL_INVALID_FRAME;
					CommData.Step = COMM_STEP_OVER;
					  
				}
				break; // Address
				
				case 12:
				{
					if(CLIPDLIPFlag == CLIP_PROTOCOL_ADDRESS_INVALID)
					{
						CommData.DataCmd = CommData.DataRx & 0x0007;
					}	
				}
				break;
				case COMM_HEAD_CLIP_VERIFY:
				{
					if (CLIPDLIPFlag == CLIP_PROTOCOL_ADDRESS_INVALID)
					{
						if (CommData.DataVB % 2 > 0) //偶校验失败
						{
							break;
						}
						CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
						CommData.Step = COMM_STEP_OVER;
						break;
					}
					//CheckPoint COM6
					// _pa4 = 1;
					if(CLIPDLIPFlag == CLIP_PROTOCOL_DLIP_FLAG)
					{
						CommData.Step = COMM_STEP_OVER;
						// _pa3 = 1;
						//地址解析:A8+ADDH+0x0D+ADDL
						CommData.DataAddr = ((CommData.DataRx >> 4) & 0x00F0) +
										(CommData.DataRx & 0x000F); //地址高位 + 地址低位
						CommData.Step = COMM_STEP_OVER;					//默认关闭通信
						// 000-240:广播地址或地址匹配
						if ((CommData.DataAddr == 0) ||
							(CommData.DataAddr == CommData.AddrDLIP))
						{
							CommData.Step = COMM_STEP_DLIP;//帧型3							
						}
						CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
						break;
					}
					//CheckPoint COM7  6-7 10us
					// _pa4 = 0;
					CommData.DataCmd = (CommData.DataRx >> 1) & 0x0007;
					CommData.Step = COMM_STEP_OVER;
					if (CommData.DataVB % 2 > 0) //偶校验失败
					{
						break;
					}
					//CheckPoint COM7.1
					// _pa4 = 1;
					if (CLIPDLIPFlag == CLIP_PROTOCOL_CLIP_FLAG)
					{
						CommData.Step = COMM_STEP_CLIP;
						CommData.DataTx = 0xF800; //发送5PW
						// CommData.DataPW = ClipGetPW(1); //返回PW1 //CheckPoint Duration 25us//todo pw = pw1 data here
						Tmp16 = 300; //返回PW1
						CommData.DataPW = Tmp16 >> 2;
						// Checkpoint COM7.1.1  COM7.1-7.1 40us aka CommCLIPCmd() duration 40us
						//  CommCLIPCmd();//TODO Move into PW1 period//TODO move into frame reset period
						CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
#ifdef CODE_SEL_SMOKE

						if ((CommData.DataCmd & 0x5) == 0x4)
						{
							GPO_SMOKE_RED_OFF();
#ifdef CODE_SEL_SMOKE_BUZZLE
							GPO_SMOKE_LED_OFF(); //门灯
							FuncData.CtrlOut = FUNC_CTRL_AV_A_OFF;
#endif
						}
						else if ((CommData.DataCmd & 0x4) == 0x0)
						{
							GPO_SMOKE_RED_ON();
#ifdef CODE_SEL_SMOKE_BUZZLE
							GPO_SMOKE_LED_ON(); //门灯
							FuncData.CtrlOut = FUNC_CTRL_AV_A_ON;
#endif											// GPO_SMOKE_RED_ON
						}

						if ((CommData.DataCmd & 0x5) == 0x5)
						{
							GPO_SMOKE_RED_ON();
#ifdef CODE_SEL_SMOKE_BUZZLE
							GPO_SMOKE_LED_ON(); //门灯
							FuncData.CtrlOut = FUNC_CTRL_AV_A_OFF;
#endif // GPO_SMOKE_RED_ON
	   //退出远程测试
							if (CommData.CLIPCmdLast == 0x5)
							{

								FuncData.RemoteTestInd = FUNC_SD_ST_REMOTTEST_OFF;
							}
						}
						else if ((CommData.DataCmd & 0x5) == 0x0)
						{
							if (CommData.CLIPCmdLast == 0x0)
							{
								FuncData.RemoteTestInd = FUNC_SD_ST_REMOTTEST_ON;
							}
						}

						if ((CommData.DataCmd & 0x2) == 0x2)
						{
							//禁止PW5
							FuncData.State |= FUNC_SD_ST_NO_PW5;
							CommData.DataTx = 0xF000; //发送5PW
						}
						else
						{
							//使能PW5
							FuncData.State &= ~FUNC_SD_ST_NO_PW5;
						}

						CommData.CLIPCmdLast = CommData.DataCmd & 0x5;
#else
						if ((CommData.DataCmd & 0x5) == 0x4)
						{
							GPO_HT_LED_0();
						}
						else if ((CommData.DataCmd & 0x4) == 0x0)
						{
							GPO_HT_LED_1();
						}

						// 1 1 ??        0 X  ON       1 0  OFF     CLIP?
						if ((CommData.DataCmd & 0x5) == 0x5)
						{
							GPO_HT_LED_1();
							if (CommData.CLIPCmdLast == 0x5)
							{
								FuncData.RemoteTestInd = FUNC_SD_ST_REMOTTEST_OFF;
							}
							//退出远程测试
						}
						else if ((CommData.DataCmd & 0x5) == 0x0)
						{
							if (CommData.CLIPCmdLast == 0x0)
							{
								FuncData.RemoteTestInd = FUNC_SD_ST_REMOTTEST_ON;
							}
						}
						
						if ((CommData.DataCmd & 0x2) == 0x2)
						{
							//禁止PW5
							FuncData.State |= FUNC_SD_ST_NO_PW5;
						}
						else
						{
							//使能PW5
							FuncData.State &= ~FUNC_SD_ST_NO_PW5;
						}
						CommData.CLIPCmdLast = CommData.DataCmd & 0x5;
#endif // CODE_SEL_SMOKE
						// CheckPoint COM7.2  7.1-7.2 83us!!!
						//  _pa4 = 0;

						break;
					}
					// CheckPoint COM9		COM8-COM9 38us
				}
				break;
			}
	
		}
		break;

    	case COMM_STEP_CLIP:
    	{
			switch (CommData.BitCnt)
			{
			case CLIP_PW2:
			{
#ifdef CODE_SEL_SMOKE
				if (FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)
				{
					//启动远程测试
					Tmp16 = 600;
				}
				else
				{
					//退出远程测试
					Tmp16 = 300;
				}
// CommData.DataPW = Tmp16 >>2;
#else
				if (FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)
				{
					//启动远程测试
					Tmp16 = 600;
				}
				else
				{
					//退出远程测试
					Tmp16 = 300;
					// remoteTestIndicator = 0;
				}
#endif
			// CommData.DataPW = Tmp16 >>2;
		}
		break;
		case CLIP_PW3:
		{
#ifdef CODE_SEL_SMOKE
			Tmp16 = 300; // notifier OEM
			if (2 == MemData.Rom[EEPROM_ID_SENSITIVITY])
			{
				//低灵敏度
				if (1 == MemData.Rom[EEPROM_ID_OEM_CODE])
				{
					Tmp16 = 600; // systemsensor OEM
				}
			}
#else
				Tmp16 = FuncData.ValuePW3;
#endif

			// CommData.DataPW = Tmp16 >> 2;
		}
		break;
		case CLIP_PW4:
		{
#ifdef CODE_SEL_SMOKE
			if (FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)
			{
				//远程测试
				Tmp16 = 3000;
				PW4ErrorFlag = PW4_NORMAL;
			}
			else if (FuncData.State & FUNC_SD_ST_DUST_ALARM)
			{
				//灰尘报警,脏污
				Tmp16 = 300;
				PW4ErrorFlag = PW4_NORMAL;
			}
			else if (FuncData.State & FUNC_SD_ST_FAULT)
			{
				//有故障
				Tmp16 = 150;
				PW4ErrorFlag = PW4_Error_WAIT_DRAWING;
			}
			else
			{
				Tmp16 = FuncData.ValuePW4;
				PW4ErrorFlag = PW4_NORMAL;
			}
#else
				if (FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)
				{
					//远程测试
					Tmp16 = 2500;
				}
				else if ((FuncData.State & FUNC_HT_ST_NTCSHORT) || (FuncData.State & FUNC_HT_ST_NTCOPEN))
				{
					//有故障
					Tmp16 = 0;
				}
				else
				{
					Tmp16 = FuncData.ValuePW4;
				}
#endif
// CommData.DataPW = Tmp16 >> 2;
#ifdef CODE_SEL_SMOKE
			if (PW4ErrorFlag == PW4_Error_WAIT_DRAWING)
			{
				PW4ErrorFlag = PW4_ERROR_DRAWING;
				break;
			}
			PW4ErrorFlag = 0;
#endif
		}
		break;
		case CLIP_PW5:
		{
#ifdef CODE_SEL_SMOKE
			if (FuncData.State & FUNC_SD_ST_NO_PW5)
			{
				//发送0就不需要拉码了
				Tmp16 = 0;
			}
			else
			{
				Tmp16 = 900;
			}
				#else
			if (FuncData.State & FUNC_SD_ST_NO_PW5)
			{
				//发送0就不需要拉码了
				// CommData.DataTx = CommData.DataTx << 1; //发送数据移位
				Tmp16 = 0;
			}
			else
			{
				Tmp16 = FuncData.ValuePW5;
			}
				#endif
				

			}
			break;
			default:
			{
				// CommData.Step = COMM_STEP_OVER;
			}
			}
			CommData.DataPW = Tmp16 >> 2;
		}
    	break;

		case COMM_STEP_DLIP:
		{

			switch (CommData.BitCnt)
			{
				case COMM_DLIP_CMD:
					CommData.DataCmd = CommData.DataRx & 0x000F;
					break;

				case COMM_DLIP_DATA1: // Data1接收或发送完成
				{
					Tmp = CommData.DataRx & 0x00FF; //提取即将操作的寄存器和EEPROM地址
					       
					//命令应答-应答1字节
					switch (CommData.DataCmd)
					{
						case DLIP_READ_REG: // 0-读寄存器
						{
							CommData.DataDat1 = Tmp; //地址
							CommData.DataDat2 = GetReg(Tmp); //数据
							Tmp16 = CommData.DataDat2;
							CommData.DataTx = Tmp16 << 8; //发送字节
						}
						break;

						case DLIP_READ_EEPROM: // 1-读EEPROM
						{
							CommData.DataDat1 = Tmp;
							CommData.DataDat2 = MemData.Rom[Tmp]; //数据
							Tmp16 = CommData.DataDat2;
							CommData.DataTx = Tmp16 << 8; //发送字节
						}
						break;
						case 0x8:
						case 0x9:
							CommData.DataDat1 = Tmp; //地址
						break;
					}
					
				}
				break;

				case COMM_DLIP_DATA2: // Data2接收或发送完成
				{
					Tmp = CommData.DataRx & 0x00FF;
					switch (CommData.DataCmd)
					{
						case DLIP_WRITE_REG:
						case DLIP_WRITE_EEPROM:
							CommData.DataDat2 = Tmp;
						break;
					}

					//计算校验和
					CommData.DataChk = (CommData.DataAddr >> 4) & 0x0F;
					CommData.DataChk ^= 0x0D;
					CommData.DataChk ^= CommData.DataAddr & 0x0F;
					CommData.DataChk ^= CommData.DataCmd;
					CommData.DataChk ^= CommData.DataDat1;
					CommData.DataChk ^= CommData.DataDat2;
					//应答
					if (CommData.DataCmd < DLIP_READ_CMD_BARRIER) //读命令
					{
						Tmp16 = CommData.DataChk & 0x00FF;
						CommData.DataTx = Tmp16 << 8;
					}
					else //写命令
					{
						CommData.DataTx = 0x0000;
					}
				}
				break;
				//校验和数据接收或发送完成
				case COMM_DLIP_VERIFY:
				{
					Tmp = CommData.DataRx & 0x00FF;
					//应答
					if (CommData.DataCmd < DLIP_READ_CMD_BARRIER) //读指令
					{
						CommData.DataTx = 0x0000; //不应答
					}
					//写命令
					else
					{
						if (CommData.DataChk == Tmp)
						{
							CommData.DataTx = 0x8000; //错误应答

							switch (CommData.DataCmd)
							{

							case DLIP_WRITE_REG: // 8-写寄存器
								SetReg(CommData.DataDat1, CommData.DataDat2);
								break;

							case DLIP_WRITE_EEPROM: // 9-写EEPROM
								PutDataToWriteEEBuff(CommData.DataDat1, CommData.DataDat2);
								break;
							}
						}
						else
						{
							CommData.DataTx = 0x0000; //错误应答
						}
					}
				}
				break;
				// ER接收完成
				case COMM_DLIP_ER:
				{
					CommData.Step = COMM_STEP_OVER;
					CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
				}
					break;
				//完成协议
				default:
				{
					;
				}
			
			}
    	}
    	break;
	
		default:
		{;}

	}
}

//------------------------------
//获取寄存器值
//------------------------------
uint8 GetReg(uint8 Index)
{
	uint8 Rtn = 0;
	//根据地址提取数据
#ifdef CODE_SEL_SMOKE
	switch (Index)
	{ //通用寄存器
		case 0x02:{Rtn = 0x00;}break;
		case 0x03:{Rtn = 0x00;}break;
		//case 0x10:{Rtn = FuncData.SD_IRed_OPA0;}break;
		//case 0x11:{Rtn = FuncData.SD_IRed_OPA1;}break;
		case 0x10:{Rtn = (FuncData.Value & 0xFF);}break;
		case 0x11:{Rtn = ((FuncData.Value >> 0x08)&0xFF);}break;
		case 0x12:
			{
					//Rtn = FuncData.SD_Blue_OPA0;
					Rtn = (FuncData.ValueCh1 & 0xFF);
			}break;
		case 0x13:
			{
					//Rtn = FuncData.SD_Blue_OPA1;
					Rtn = ((FuncData.ValueCh1 >> 0x08)&0xFF);
			}break;
		//case 0x14:{Rtn = FuncData.SD_Dark_OPA0;}break;
		//case 0x15:{Rtn = FuncData.SD_Dark_OPA1;}break;

		case 0x14:{Rtn = (FuncData.ValueCh2 & 0xFF);}break;
		case 0x15:{Rtn = ((FuncData.ValueCh2 >> 0x08)&0xFF);}break;

		case 0x16:{Rtn = FuncData.TempVal >> 4;}break;
		case 0x17:{Rtn = FuncData.EolCmd;}break;
		case 0x18:{Rtn = FuncData.SD_Radio_BlueIR & 0xFF;}break;
		case 0x19:{Rtn = (FuncData.SD_Radio_BlueIR>>0x08)&0xFF;}break;
	}
#else
	switch (Index)
	{ //通用寄存器
		case 0x02:{Rtn = 0x00;}break;
		case 0x03:{Rtn = 0x00;}break;
		case 0x10:{Rtn = HeatAlgorithmRegs.AdCodeFilter >> 0x08;}break;
		case 0x11:{Rtn = HeatAlgorithmRegs.AdCodeFilter & 0xFF;}break;
		case 0x12:{Rtn = HeatAlgorithmRegs.NtcRealTemp >> 0x08;}break;
		case 0x13:{Rtn = HeatAlgorithmRegs.NtcRealTemp & 0xFF;}break;
		case 0x14:{Rtn = HeatAlgorithmRegs.CompTemp >> 0x08;}break;
		case 0x15:{Rtn = HeatAlgorithmRegs.CompTemp & 0xFF;}break;
		case 0x16:{Rtn = 0x00;}break;
		case 0x17:{Rtn = FuncData.EolCmd;}break;
		case 0x18:{Rtn = 0x00;}break;
		case 0x19:{Rtn = 0x00;}break;
	}
#endif //CODE_SEL_SMOKE
	return Rtn;
}

//------------------------------
//写入寄存器值
//------------------------------
uint8 SetReg (uint8 Index, uint8 Data)
{
	uint8 Rtn = 1;
	//根据地址提取数据
	switch(Index)
	{	
		//EOL数据
		case 0x17:{FuncData.EolCmd = Data;}break;	//EOL命令		//EOL测试命令
		//其他
		default:{Rtn = 0;}break;
	}
	#ifdef CODE_SEL_SMOKE
		if(FuncData.EolCmd == 0xD1)
		{
			GPO_SMOKE_RED_ON();
		}
		else
		{
			GPO_SMOKE_RED_OFF();
		}
	#endif
	return Rtn;
}

/*******************************************************************************
 * CLIPAddrEncode	: CLIP地址表根据地址查编码
 * CLIPAddrDecode	: CLIP地址表根据编码查地址
 *******************************************************************************/
const uint8 CLIPAddrEncode[10] = {
	// BCD:	0, 1, 2, 3, 4, 5, 6, 7, 8, 9	//地址
	0, 4, 6, 7, 8, 1, 5, 2, 3, 9 //编码
};
const uint8 CLIPAddrDecode[10] = {
	// Code:	0, 1, 2, 3, 4, 5, 6, 7, 8, 9	//编码
	0, 5, 7, 8, 1, 6, 2, 3, 4, 9 //地址
};
/*******************************************************************************
 * ClipCommand	: CLIP命令数据处理程序
 *******************************************************************************/

/*******************************************************************************
 * CLIPGetPW	: CLIP取得PW值
 *******************************************************************************/
// uint16 ClipGetPW(uint8 PWNumber)
// {
// 	// uint32 Tmp;
// 	uint16 Rtn=0;

// 	//得到基准值
// 	// PWBase = (SysData.EnHProt) ? 150 : 300;				//1:H协议
// 	//PWBase = 150;
// 	//默认回码值
// 	//Rtn = PWBase;
// 	//取得PW值
// #ifdef CODE_SEL_SMOKE
// 	switch (PWNumber)
// 	{
// 	//--------------[PW1]基准值-------------
// 	case 1:
// 	{
// 		Rtn = 300; // 300-125
// 	}
// 	break;
// 	//--------------[PW2]控制状态-----------
// 	case 2:
// 	{
// 		if(FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)
// 		{
// 			//启动远程测试		
// 			Rtn = 600;	
// 		}
// 		else
// 		{
// 			//退出远程测试	
// 			Rtn = 300;		
// 		}
		
// 	}
// 	break;
// 	//--------------[PW3]OEM-----------
// 	case 3:
// 	{
// 		// Rtn = FuncData.ValuePW3 ;
// 		Rtn = 300;//notifier OEM
// 		if(2 == MemData.Rom[EEPROM_ID_SENSITIVITY])
// 		{
// 			//低灵敏度
// 			if(1 == MemData.Rom[EEPROM_ID_OEM_CODE])
// 			{
// 				Rtn = 600;	 //systemsensor OEM
// 			}
// 		}
// 		Rtn += 12; //补偿至500、300
// 	}
// 	break;
// 	//--------------[PW4]报警数值-----------
// 	case 4:
// 	{
// 		// Rtn = 2200; //Done: 实际数据大了700 ？？？？？fixed
// 		// Rtn = FuncData.SD_IRed_OPA1;
// 		if(FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)	
// 		{
// 			//远程测试	
// 			Rtn = 3000;		
// 		}
// 		else if(FuncData.State & FUNC_SD_ST_DUST_ALARM)
// 		{
// 			//灰尘报警,脏污
// 			Rtn = 300;
			
// 		}
// 		else if(FuncData.State & FUNC_SD_ST_FAULT)
// 		{
// 				//有故障
// 				Rtn = 150;	
// 		}
// 		else
// 		{
// 			Rtn = FuncData.ValuePW4;
// 		}
// 		// if(Rtn > 3500) {Rtn = 3500;}
// 		// // Rtn = 3500;
// 		// Rtn = 900 +15;//TODO
// 		Rtn += 15;
// 	}
// 	break;
// 	//--------------[PW5]类型状态-----------
// 	case 5:
// 	{
// 		if(FuncData.State & FUNC_SD_ST_NO_PW5)
// 		{
// 			//发送0就不需要拉码了
// 			//CommData.DataTx = CommData.DataTx << 1; //发送数据移位		
// 			Rtn = 0;
// 		}
// 		else
// 		{
// 			Rtn = 900 + 15;//test
// 		}
		
// 	}
// 	break;

// 	default:;
// 		break;
// 	}
// 	//完成
// 	Rtn = Rtn >> 2 ;
// 	Rtn -= 40 ;
// 	return Rtn;
// #else
// 	switch (PWNumber)
// 	{
// 	//--------------[PW1]基准值-------------
// 	case 1:
// 	{
// 		Rtn = 300; // 300-125
// 	}
// 	break;
// 	//--------------[PW2]控制状态-----------
// 	case 2:
// 	{
// 		if(FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)
// 		{
// 			//启动远程测试		
// 			Rtn = 600;	
// 		}
// 		else
// 		{
// 			//退出远程测试	
// 			Rtn = 300;		
// 		}
		
// 	}
// 	break;
// 	//--------------[PW3]OEM-----------
// 	case 3:
// 	{
// 		Rtn = FuncData.ValuePW3 ;
// 	}
// 	break;
// 	//--------------[PW4]报警数值-----------
// 	case 4:
// 	{
// 		// Rtn = 2200; 
// 		// Rtn = FuncData.SD_IRed_OPA1;
// 		if(FuncData.RemoteTestInd == FUNC_SD_ST_REMOTTEST_ON)	
// 		{
// 			//远程测试	
// 			Rtn = 3000;		 //TOBEDELETED2936
// 		}
// 		else if((FuncData.State & FUNC_HT_ST_NTCSHORT) || (FuncData.State & FUNC_HT_ST_NTCOPEN))
// 		{
// 				//有故障
// 				Rtn = 0;
// 		}
// 		else
// 		{
// 			Rtn = FuncData.ValuePW4;
// 		}

// 	}
// 	break;
// 	//--------------[PW5]类型状态-----------
// 	case 5:
// 	{
// 		if(FuncData.State & FUNC_SD_ST_NO_PW5)
// 		{
// 			//发送0就不需要拉码了
// 			//CommData.DataTx = CommData.DataTx << 1; //发送数据移位		
// 			Rtn = 0;
// 		}
// 		else
// 		{
// 			Rtn = FuncData.ValuePW5;
// 		}
		
// 	}
// 	break;

// 	default:;
// 		break;
// 	}
// 	//完成
// 	Rtn -= 100;
// 	Rtn =  Rtn >> 2;
// 	return Rtn ;
// #endif //
// }






//------------------------------
//通信数据刷新
//------------------------------
void RefreshData(void)
{
#ifdef CODE_SEL_SMOKE
	//设备类型
	SysData.MyType = MemData.Rom[EEPROM_ID_TYPE];
	if(1 == SysData.MyType)	SysData.MyType = CFG_TYPE_SMOKE;		//感烟探测器
	else 					SysData.MyType = CFG_TYPE_SMOKE_BUZZER;	//带蜂鸣器感烟探测器

		FuncData.ValuePW3 = 150;
		if(2 == MemData.Rom[EEPROM_ID_SENSITIVITY])
		{
			//低灵敏度
			if(1 == MemData.Rom[EEPROM_ID_OEM_CODE])
			{
				FuncData.ValuePW3 = 600;	 //systemsensor OEM
			}
			else FuncData.ValuePW3 = 300;//notifier OEM
		}

#else
	//设备类型
	SysData.MyType = MemData.Rom[EEPROM_ID_TYPE];
	if(2 == SysData.MyType)	SysData.MyType = CFG_TYPE_HEAT;		//感温探测器*
	else 					SysData.MyType = CFG_TYPE_HEAT_R;	//感温探测器ROR型

	FuncData.ValuePW3 = 300;
#endif //
	//设备地址
	SysData.MyAddr = MemData.Rom[EEPROM_ID_ADDRESS];
	if (SysData.MyAddr > 199) // 0～199
		SysData.MyAddr = 199;
	#ifdef CODE_SEL_HEAT
		if(SysData.MyAddr>=100)
		{
			FuncData.ValuePW5 = 450;
		}
		else
		{
			FuncData.ValuePW5 = 300;
		}
	#endif 
	// DLIP地址范围
	CommData.AddrDLIP = SysData.MyAddr;
	// CLIP地址转换
	CommData.AddrCLIP = ((SysData.MyAddr / 100) << 8) +
						(CLIPAddrEncode[((SysData.MyAddr % 100) / 10)] << 4) +
						(CLIPAddrEncode[(SysData.MyAddr % 10)]);
	MemData.Rom[EEPROM_ID_FW_VERSION] = SUB_VER2;
}

// void CommTimeoutConfig(int duration)
// {
// 	int tempDuration;
// 	tempDuration = duration >> 4;

// 	#ifdef CODE_SEL_SMOKE
// 		_ptmrpl = (uint8)(tempDuration & 0x00FF);
// 		_ptmrph = (uint8)(tempDuration >> 8);
// 		m_ptmPfunc = COMM_TIMEOUT_FLAG; // Default 0;
// 		_pton = 0;
// 		_pton = 1;
// 	#else
// 		_ptm0rpl = (uint8)(tempDuration & 0xFF);
// 		_ptm0rph = (uint8)(tempDuration >> 8);
// 		m_ptm0Pfunc = COMM_TIMEOUT_FLAG; // Default 0;
// 		_pt0on = 0;
// 		_pt0on = 1;
// 		// m_ptmPfunc = COMM_TIMEOUT_FLAG;//Default 0;
// 	#endif //CODE_SEL_HEAT

// }
// void resetCommFlag(void)
// {
// 	CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
// }

 /******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
