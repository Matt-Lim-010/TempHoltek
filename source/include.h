/******************************************************************************
 * (c) Copyright 1995-2018, System Sensor Xi'an.
 * ALL RIGHTS RESERVED.
 *
 * @file      include.h
 * @version   1.0.0
 * @date      Feb-1-2020
 * @brief     头文件入口
 ******************************************************************************/
#ifndef __INCLUDE_H
#define __INCLUDE_H

//在工程IDE中根据需要定义了_HT45F5542_  _HT45F5541_
//系统头文件
#ifdef _HT45F5542_
#include "..\driver\ht45f5542.h"	//5542：感烟
#endif
#ifdef _HT45F5541_
#include "..\driver\ht45f5541.h"	//5541：感温/模块/手报/声光
#endif

#include "..\driver\build-in.h"

//配置程序头文件
#include "ctype.h"

//程序头文件
#include "config.h"
#include "comm.h"
#include "memory.h"
#include "function.h"
#include "main.h"
#include "ThermalAlgorithm.h"


//MCU操作宏
#define EnableInterrupts()		(_emi = 1)		//全局中断开启
#define DisableInterrupts()   	(_emi = 0)		//全局中断关闭
#define SystemReset()			(_wdtc = 0x07)	//系统复位
#define WatchDogReset()			asm("clr wdt")	//看门狗复位
#define Nop()					asm("nop")		//空指令
#define Halt()					asm("halt")		//停机


#endif /* __INCLUDE_H */
/******************************************************************************
 * End of file                                                                *
 ******************************************************************************/
