/**
 * @file    interrupt.c
 * @brief  HT45F5541 mcu中断服务函数集
 * @details 
 * @mainpage 部分操作只适用于HOLTEK H45F5541支持位操作指令的单片机平台
 * @author 
 * @version 1.0
 * @date 2021-11-09
 * @copyright HBT xi'an
 */
#include "include.h"
#include "clip.h"

static uint8 sucTmpDin = 0;
uint8 m_ptmPfunc = 0; //< P中断功能选项 0：帧超时  1：回码   2：bit超时
uint8 timeoutDurationFlag = 0; //0 bit1-13 duration, 1 13-18 bit
uint8 CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;// 0校验失败， 1CLIP 2 DLIP
uint8 PW4ErrorFlag = 0;  // 0: None Error, 1: PW4 = 150, wait for Draw, 2: staring draw 150 pw4
extern uint8 CLIPWriteFlag;
// uint8 IrrelevantFrameFlag = 1; // 1: relevant Frame; 0: Irrelevant Frame;
//TODO
//TODO
//TODO
//Test:
uint8 resetInd = 0;
// PLT0中断入口
void __attribute((interrupt(0x04))) PLTCMP0(void)
{
    _scc=0B00000011;

    #if 1
    if(_pltc0pol) // Falling Edge
    {
        _pltc0pol = 0;
        _ston = 0;

        _scc = 0B00000010; 
        SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
    }
    else //Rising
    {

        _pltc0pol = 1;
        _ston = 0;
        _ston = 1;

        _scc = 0B00000011; 
        SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
    }

    #endif
    _pltc0f = 0;  
}

// INT0外部中断入口
void __attribute((interrupt(0x08))) INT0(void)
{
    return;
}
// INT1外部中断入口
void __attribute((interrupt(0x0C))) INT1(void)
{
    return;
}
// 通用串行接口中断入口
void __attribute((interrupt(0x10))) USIM(void)
{
    return;
}

// LVD低电量检测中断入口
void __attribute((interrupt(0x14))) LVD(void)
{
    return;
}

// AD转换中断入口
void __attribute((interrupt(0x18))) ADCONVERTER(void)
{
    return;
}
// EEPROM中断入口
void __attribute((interrupt(0x1C))) EEPROM(void)
{
    return;
}

// PTM.COMP.P中断入口
void __attribute((interrupt(0x20))) PTMCOMP_P(void)
{   
 
    uint8 Tmp, Dat1, Dat2;
    /*-------------------------- 中断入口操作 -----------------------------------------------------*/
    _pton = 0;
    _pton = 1;
    _ptpau = 1;

    /*-------------------------- 通讯超时和回码结束处理 --------------------------------------------*/
    
    switch(timeLoopCounter)
    {
        case 0:
        {
            switch (m_ptmPfunc)
            {
            case COMM_DRAW_TIMER_FLAG:
            {
                m_ptmPfunc = COMM_TIMEOUT_FLAG;
                
                PltOpaSwitchOff();
                timeLoopCounter = 4;
                _ptmrpl = 0xFF;
                _ptmrph = 0x03;
                _ptpau = 0;
            }
            break;
            case COMM_BIT_TIMEOUT_FLAG:
            {
                // greater than 5V
                if (_pltc0out > 0)
                {
                    // CommData.Step = COMM_STEP_OVER;
                    m_ptmPfunc = COMM_TIMEOUT_FLAG;
                    _ptmrpl = 0x71;
                    _ptmrph = 0x02;
                    timeLoopCounter = 0;
                    _pton = 0;
                    _pton = 1;
                    _ptpau = 0;
                    // CheckPoint FR  ER-FR 4.2us    bit13 6us
                    timeoutDurationFlag = 0;
                }
                else
                {
                    CommData.Step = COMM_STEP_ERROR;
                }
            }
            break;
            case COMM_TIMEOUT_FLAG:
            {
                if (CommData.DataCmd == 0x5) //命令匹配
                {
                    CommData.CLIPKey1 = CommData.CLIPKey2; //存储历次密码
                    CommData.CLIPKey2 = CommData.CLIPKey3;
                    CommData.CLIPKey3 = CommData.DataAddr & 0xFF;
                    if((CommData.CLIPKey2 == CLIP_KEY1)&&(CommData.CLIPKey3 == CLIP_KEY2))
                    {
                        CLIPWriteFlag = CLIP_WRITE_ADD_CHECKED;
                    }
                    else
                    {
                        CLIPWriteFlag = 0;
                    }
                    //编址密码开
                    if ((CommData.CLIPKey1 == CLIP_KEY1) &&
                        (CommData.CLIPKey2 == CLIP_KEY2))
                    {
                        {
                            Dat1 = (CommData.DataAddr >> 4) & 0x000F;
                            Dat2 = (CommData.DataAddr) & 0x000F;
                            if ((Dat1 <= 9) && (Dat2 <= 9))
                            {
                                Tmp = (CommData.DataAddr & 0x0100) ? 100 : 0;
                                Tmp += (CLIPAddrDecode[Dat1] * 10);
                                Tmp += (CLIPAddrDecode[Dat2]);
                                PutDataToWriteEEBuff(EEPROM_ID_ADDRESS, Tmp); //写入地址
                            }
                        }
                        CommData.CLIPKey3 = 0; //清除命令序列
                        CLIPWriteFlag = 0;
                    }

                }
                else
                {
                    CommData.CLIPKey3 = 0; //清除命令序列
                }

                if(timeoutDurationFlag)
                {
                    _ptmrpl = 0xFF;
                    _ptmrph = 0x03;
                    timeLoopCounter = 4;
                    break;
                }
                // _pa4 = 1;
                // GCC_DELAY(20);
                // _pa4 = 0;
                CommData.Step = COMM_STEP_IDLE; //通信空闲
                CommData.DataTx = 0;            //发送数据位
                CommData.DataRx = 0;            //接收数据位
                CommData.BitCnt = 0;            //数据位计数
                CommData.DataPW = 0;
                CommData.DataVB = 0;
                CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
                CommData.ClkNext = FALLING_EDGE;
                _ptpau = 1; // Shut down timer
                timeoutDurationFlag = 0;
               
                // _pltc0pol = 0;

                _scc = 0B00000001;                      //休眠模式重新修改为低速时钟使能
                SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
                FuncData.FlagBreak = 0;
                //闪灯
                if (((CommData.DataCmd & 0x5) == 0x5))
                {
                    GPO_SMOKE_RED_OFF();
                    #ifdef CODE_SEL_SMOKE_BUZZLE
                    GPO_SMOKE_LED_OFF(); //门灯
                    #endif                                   // GPO_SMOKE_RED_ON
                    FuncData.State &= ~FUNC_SD_ST_DLIP_LED;
                }
                // GPO_SMOKE_YELLOW_OFF();
                //_pa4 = 0;
            }
            break;
            }
        }
        break;
        case 1:
        {
            _ptmrpl = 0xDD;
            _ptmrph = 0x01;
            timeLoopCounter -= 1;
            _ptpau = 0;
            _pton = 0;
            _pton = 1;
        }
        break;

        case 2:
        case 3:
        case 4:
        case 5:
        {
            _ptmrpl = 0xFF;
            _ptmrph = 0x03;
            timeLoopCounter -= 1;
            _ptpau = 0;
            _pton = 0;
            _pton = 1;
        }
            break;
    }
   
  
    
    /*-------------------------- 中断出口操作 -----------------------------------------------------*/
    _ptmpf = 0;             //清除中断标志
}

// PTM.COMP.A中断入口
void __attribute((interrupt(0x24))) PTMCOMP_A(void)
{

    //   _pa4 = 1;
    //     GCC_DELAY(20);
    //     _pa4 = 0;
    //CheckPoint A  Start-A F29.6us     R31.4us         pw 27
    uint8 TmpClk = 0;
    uint16 pwSetVal = 0;
    // _pltc0e = 0;
    /*-------------------------- 中断入口操作 -----------------------------------------------------*/
    _scc = 0B00000011;
    _pton = 0;
    _pton = 1;
    _ptpau = 0;
    FuncData.FlagBreak = 1;
    _ston = 0;
    /*-------------------------- 通讯数据收发处理 --------------------------------------------------*/
    if(0 == _ptvlf)                //< 下降沿锁存标志
    {
        /*--------------------------------- 提前拉码动作处理 ------------------------------------------*/
        if ((CommData.DataTx & 0x8000) && (CommData.ClkNext == FALLING_EDGE))   //立即启动拉码
        {
            _pltsw |= 0B00000001;
            _pltda2l = 26;
            m_ptmPfunc = 1;
            if (CommData.Step == COMM_STEP_CLIP)
            {
                if (CommData.DataPW != 0)
                {
                    //CheckPoint pwB  A-pwB 30.4us
                    GCC_DELAY(15);
                    //CheckPoint pwC  pwB-pwC 16.4us
                    _pltsw |= 0B00000001;
                    _pltda2l = 8;
                    //CheckPoint pwD  pwC-pwD 4.2u
                    timeLoopCounter = 0;
                    pwSetVal = CommData.DataPW -19;
                    _ptmrpl = (uint8)(pwSetVal & 0x00FF);
                    _ptmrph = (uint8)(pwSetVal >> 8 );      
                }
                else
                {
                    PltOpaSwitchOff();
                    m_ptmPfunc = 0;
                }
            }
            else
            {
                _pltsw |= 0B00000001;
                _pltda2l = 8;
            }
            //CheckPoint pwF  pwE-pwF 3.2us
            _pton = 0;
            _pton = 1;
            //CheckPoint pwG  pwF-pwG 3.4us
        }
    }
    //关闭感烟发光管
    #ifdef CODE_SEL_SMOKE
    IRED_OFF(); //关闭红外
    BLUE_OFF(); //关闭蓝光
    #endif
    //CheckPoint B A-B  13.6us          pwG-B 3us
    TmpClk = (_pltc0out)? 1 : 0;
    TmpClk += (_pltc0out)? 1 : 0;
    TmpClk += (_pltc0out)? 1 : 0;
    TmpClk += (_pltc0out)? 1 : 0;
    TmpClk += (_pltc0out)? 1 : 0;
    TmpClk += (_pltc0out)? 1 : 0;
    //CheckPoint C  B-C 31.6us
    if ((TmpClk > 3) && (CommData.ClkNext == RISING_EDGE))
    {
        PltOpaSwitchOff();
        CommData.ClkNext = FALLING_EDGE;
        //CheckPoint DR  C-DR 17.9us
        //处理协议
        if (CommData.BitCnt < 100) //数据位计数
        {
            CommData.BitCnt++;
        }
        CommProtocol(); 
        //test: once Address invalid and not CLIP write PROTOCOL
        if (CLIPDLIPFlag == CLIP_PROTOCOL_INVALID_FRAME)
        {
             _tb0on = 0;
            // IrrelevantFrameFlag = 0;

            // FuncData.FlagBreak = 0;
            m_ptmPfunc = 0;
            timeoutDurationFlag = 0;
            CLIPDLIPFlag = 0;
            

            _stmc0 = 0B00110000;
            _stmc1 = 0B11000001;
            // _stmal = 0xC8;  //3200us
            _stmal = 0x64;  //1600us
            _stmah = 0x00;
            // _ston = 1;
            _pton = 0;
            _stmaf = 0;
            _stmae = 1;
            _pltc0f = 0;
            _pltc0e = 1;

            _scc = 0B00000011; //低速模式
            SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
            // _scc = 0B11100001; //低速模式
            
            return;
        }
        //CheckPoint ER  DR-ER  45.1us      Bit13 150us
        if ((CommData.DataTx & 0x8000))
        {
            //准备回码
            _pltda2l = 0; //初始值0
            _pltdac2en = 1;
            _pltaen = 1;
        }
        // m_ptmPfunc = COMM_TIMEOUT_FLAG; 
        m_ptmPfunc = COMM_TIMEOUT_FLAG;
        _ptmrpl = 0xFA;
		_ptmrph = 0x00;
		timeLoopCounter = 0;
        _pton = 0;
		_pton = 1;
        //CheckPoint FR  ER-FR 4.2us    bit13 6us
        timeoutDurationFlag = 0;
    }
    else if ((TmpClk < 2) && (CommData.ClkNext == FALLING_EDGE))
    {
        timeoutDurationFlag = 1;
        //CheckPoint D C-D 12.4us
        if (CommData.DataTx == 0)
        {
            _ptmrpl = 0xF4; //2ms 
            _ptmrph = 0x01;
            timeLoopCounter = 0;
            _pton = 0;
            _pton = 1;
            m_ptmPfunc = COMM_BIT_TIMEOUT_FLAG;
            // CheckPoint E  D-E 40.8us
        }

            if (CommData.Step == COMM_STEP_IDLE)
            {
                CommData.Step = COMM_STEP_HEAD; //接收帧头
   
            }

            // CheckPoint F  E-F 13.8us

            sucTmpDin = (_pltc1out) ? 1 : 0; // TEST
            TmpClk += (_pltc0out) ? 1 : 0;   //继续读CLK//TEST

            sucTmpDin += (_pltc1out) ? 1 : 0; // TEST
            TmpClk += (_pltc0out) ? 1 : 0;    //继续读CLK//TEST

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0; //继续读CLK//TEST

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0; //继续读CLK//TEST

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0; //继续读CLK
            //End Drawing incase PW4 = 150, due to PW interrupt processing duration.
            if (PW4ErrorFlag == PW4_ERROR_DRAWING)
            {
                _pltda2l = 0;
                _pltsw &= ~0B00000001; // S2[2]=0-DA,S1[1]=1-LV,S0[0]=0-G
                _pltdac2en = 0;        //关DAC
                _pltaen = 0;
                PW4ErrorFlag = PW4_NORMAL;
            }

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0; //继续读CLK

          
            // CheckPoint G  F-G 45us // TODO redo
            // _pa4 = 1;
            if (TmpClk < 4)
            {
                //读取数据电平
                sucTmpDin = ((sucTmpDin > 3) ? 1 : 0);
                
                //数据接收处理
                CommData.DataRx <<= 1;        //接收数据移位
                CommData.DataRx |= sucTmpDin; //数据保存
                CommData.DataVB += sucTmpDin; //校验计算
                // CheckPoint DLIP draw end here at 184us duration
                CommData.DataTx = CommData.DataTx << 1; //发送数据移位
                CommData.ClkNext = RISING_EDGE;

                // CheckPoint H   G-H  71.2us
            }
            else //< 提前回码有误
            {
                PltOpaSwitchOff();
                // Test
                CommData.Step = COMM_STEP_OVER; //通信空闲
                CommData.DataTx = 0;            //发送数据位
                CommData.DataRx = 0;            //接收数据位
                CommData.BitCnt = 0;            //数据位计数
            }
            // CheckPoint I   H-I 6.4us
            // CheckPoint B
            if(CommData.Step == COMM_STEP_DLIP)
            {
                PltOpaSwitchOff();//Test
            }
    }
    else
    {
        PltOpaSwitchOff();
    }
    
    /*-------------------------- 中断出口操作 -----------------------------------------------------*/
    SysData.CmdActive |= ACTIVE_FLAG_COMM;
    _ptmaf = 0;
    //CheckPoint J  I-J  5.2us

}

// STM.COMP.P中断入口
void __attribute((interrupt(0x28))) STMCOMP_P(void)
{
    return;
}

// STM.COMP.A中断入口
void __attribute((interrupt(0x2C))) STMCOMP_A(void)
{
    //TEST:
    _pa4 = 1;
    GCC_DELAY(20);
    _pa4 = 0;

#if 1

        _ston = 0;
        _pton = 1;
        // stmLoopCtrl = 0;
        CommData.Step = COMM_STEP_IDLE; //通信空闲
        CommData.DataTx = 0;            //发送数据位
        CommData.DataRx = 0;            //接收数据位
        CommData.BitCnt = 0;            //数据位计数
        CommData.DataPW = 0;
        CommData.DataVB = 0;
        CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
        CommData.ClkNext = FALLING_EDGE;
        _ptpau = 1; // Shut down timer
        timeoutDurationFlag = 0;
        // _pltc0f = 0;
        _pltc0e = 0;
        FuncData.FlagBreak = 0;
        // _pa4 = 0;
        _tb0on = 1;

        _pltc0pol = 0;
    _stmaf = 0;
    _scc = 0B00000001;                      //休眠模式重新修改为低速时钟使能
    SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
    #endif
}

// TIMEBASE0时基0中断入口
void __attribute((interrupt(0x30))) TIMEBASE0(void)
{

    //timbase0 当前设定为31.25ms用来进行动态跟踪调整总线通讯波形的比较电平
    //感烟采样过程中不可能进入该中断
    //时基中断0
    _emi = 1;  						//允许嵌套通信等其他中断
    //------------------------------
    //全局主定时中断，for 5542
    //功能函数定时间隔
    if(FuncData.Interval)					//功能函数定时间隔
    {
        FuncData.Interval --;
    }

    
//蜂鸣器感烟首次通讯闪烁3次LED
//间隔500ms
#ifdef CODE_SEL_SMOKE_BUZZLE
    if(SdkeePotData.BuzzleSmokeFstComInterval)
    {
        SdkeePotData.BuzzleSmokeFstComInterval--;
    }		
#endif

    
#ifdef CODE_SEL_SMOKE		
    //上电延时进行AMP的校准
    if(FuncData.AmpCaliInterval)
    {
        FuncData.AmpCaliInterval--;
    }
#endif
    
#ifdef CODE_SEL_SMOKE
    //蜂鸣器感烟的蜂鸣器部分
    if(FuncData.IntervalAV > 0 )
    {
        FuncData.IntervalAV--;
    }
    
#endif
    //------------------------------
    SysData.CmdActive |= ACTIVE_FLAG_RUN;//中断开启单次激活	
    
    _tb0f = 0;					//清除中断标志
   
    return;
}

// TIMEBASE1时基1中断入口
void __attribute((interrupt(0x34))) TIMEBASE1(void)
{
    return;
}

// PLT1中断入口
void __attribute((interrupt(0x38))) PLTCMP1(void)
{
    return;
}
