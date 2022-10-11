/**
 * @file    interrupt.c
 * @brief  HT45F5542 mcu�жϷ�������
 * @details 
 * @mainpage ���ֲ���ֻ������HOLTEK H45F5542֧��λ����ָ��ĵ�Ƭ��ƽ̨
 * @author 
 * @version 1.0
 * @date 2021-11-09
 * @copyright HBT xi'an
 */
#include "include.h"
#include "clip.h"

static uint8 sucTmpDin = 0;
uint8 m_ptm0Pfunc = 0; //< P�жϹ���ѡ�� 0��֡��ʱ  1������   2��bit��ʱ
uint8 timeoutDurationFlag = 0;   //0 bit1-13 duration, 1 13-18 bit
uint8 CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;// 0校验失败， 1CLIP 2 DLIP
extern uint8 CLIPWriteFlag;
//PLT0���ж����  0x04H
void __attribute((interrupt(0x04))) ISR_PLTCMP0(void)
{
    _scc=0B00000011;    
 #if 1
    if(_pltc0pol) // Falling Edge
    {
        _pltc0pol = 0;
        _ct0on = 0;
        _scc = 0B00000010; 
        SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
    }
    else //Rising
    {

        _pltc0pol = 1;
        _ct0on = 0;
        _ct0on = 1;
        _scc = 0B00000011; 
        SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
    }
    // _pa4 = 0;
    #endif
    _pltc0f = 0;
    // return;
}

//INT0���ж���� 0x08H
void __attribute((interrupt(0x08))) ISR_INT0(void)
{
    return;
}

//INT1���ж���� 0x0CH
void __attribute((interrupt(0x0C))) ISR_INT1(void)
{
    return;
}

//UART的中断入口 0x10H
void __attribute((interrupt(0x10))) ISR_UART(void)
{
    return;
}

//LVD的中断入口 0x14H
void __attribute((interrupt(0x14))) ISR_LVD(void)
{
    return;
}

//ADC和EERPRM的中断入口 0x18H
void __attribute((interrupt(0x18))) ISR_ADC_EEPROM(void)
{
    return;
}

//PTM0 P和A 中断入口 0x1c
void __attribute((interrupt(0x1C))) ISR_PTM0CMP_PTM0CMA(void)
{
    _ct0on = 0;

    //USER_DEBUG_TEST1(1);
    if (_ptm0af == 1)           //< 如果是PTM0的 比较器A匹配中断标志置位
    {
        //CheckPoint FA RA
        uint8 TmpClk = 0;
        uint16 pwSetVal = 0;
        FuncData.FlagBreak = 1;
        /*-------------------------- 中断入口操作 -----------------------------------------------------*/
        _scc = 0B00000011;
        _pt0on = 0;
        _pt0on = 1;
        _pt0pau = 0;
        //CheckPoint FB RB   A-B  6.2us
        /*-------------------------- 通讯数据收发处理 --------------------------------------------------*/
        if (0 == _pt0vlf)                //< 下降沿锁存标志
        {
            /*--------------------------------- 提前拉码动作处理 ------------------------------------------*/
            if ((CommData.DataTx & 0x8000) && (CommData.ClkNext == FALLING_EDGE))   //立即启动拉码
            {   
                _pltsw |= 0B00000001;
                _pltda2l = 38;
                m_ptm0Pfunc = 1;
                if (CommData.Step == COMM_STEP_CLIP)
                {
                    if (CommData.DataPW != 0)
                    {
                        GCC_DELAY(15);
                        _pltsw |= 0B00000001;
                        _pltda2l = 13;
                        pwSetVal = CommData.DataPW-21;
                         timeLoopCounter = 0;
                        _ptm0rpl = (uint8)(pwSetVal & 0x00FF);
		                _ptm0rph = (uint8)(pwSetVal >>8); 
                    }
                    else
                    {
                        PltOpaSwitchOff();
                        m_ptm0Pfunc = 0;
                    }
                }
                else
                {
                    _pltsw |= 0B00000001;
                    _pltda2l = 13;
                }
                _pt0on = 0;
                _pt0on = 1;
            }
        }
        //CheckPoint FC RC FB-FC 6.8us    RB-RC 4 us
        TmpClk = (_pltc0out)? 1 : 0;
        TmpClk += (_pltc0out)? 1 : 0;
        TmpClk += (_pltc0out)? 1 : 0;
        TmpClk += (_pltc0out)? 1 : 0;
        TmpClk += (_pltc0out)? 1 : 0;
        TmpClk += (_pltc0out)? 1 : 0;
        //CheckPoint FD RD   FC-FD 30us  RC-RD 30.1us
        if ((TmpClk > 3) && (CommData.ClkNext == RISING_EDGE))
        { 
            //CheckPoint RE  RD-RE 8us
            PltOpaSwitchOff();
            CommData.ClkNext = FALLING_EDGE;
            if (CommData.BitCnt < 100) //数据位计数
            {
                CommData.BitCnt++;
            }
            //CheckPoint RF  RE-RF  22.1us
            //处理协议
            CommProtocol();
            if (CLIPDLIPFlag == CLIP_PROTOCOL_INVALID_FRAME)
            {
                FuncData.FlagBreak = 0;
                m_ptm0Pfunc = 0;
                timeoutDurationFlag = 0;
                CLIPDLIPFlag = 0;

                // _stmc0 = 0B00110000;
                _ctm0c0 = 0B00110000;
                _ctm0c1 = 0B11000001;

                _ctm0al = 0x64;
                // _ctm0al = 0x1F;
                _ctm0ah = 0x00;
                _ctm0af = 0;
                _ctm0ae = 1;
                _mf3e = 1;
                _ct0on = 1;
                _ptm0pf = 0;
                _ptm0af = 0;
                _pt0on = 0;
                _pltc0e = 1;
                // _tb0on = 1;
                _ct1on = 0;
                _scc = 0B00000011; //
                SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭

                return;
            }

            //CheckPoint RG  RF-RG  22.1us  Bit9 49.2us  Bit13 96.4us
            if ((CommData.DataTx & 0x8000))
            {
                //准备回码
                _pltda2l = 0; //初始值0
                _pltdac2en = 1;
                _pltaen = 1;
            }
            //CheckPoint RH  RG-RH  5us
            // m_ptm0Pfunc = COMM_TIMEOUT_FLAG;
            m_ptm0Pfunc = COMM_BIT_TIMEOUT_FLAG;
            _ptm0rpl = 0xFA;
			_ptm0rph = 0x00;
            _pt0on = 0;
            _pt0on = 1;
			timeLoopCounter = 0;
            timeoutDurationFlag = 0;
            //CheckPoint RI  RH-RI  9us
        }
        else if ((TmpClk < 2) && (CommData.ClkNext == FALLING_EDGE))
        {
            //CheckPoint FE   FD-FE 13us
            timeoutDurationFlag = 1;
            if ((CommData.DataTx == 0))
            {
                _ptm0rpl = 0xF4; // 2ms
                _ptm0rph = 0x01;
                timeLoopCounter = 0;
                _pt0on = 0;
                _pt0on = 1;
                // m_ptm0Pfunc = COMM_TIMEOUT_FLAG;
                m_ptm0Pfunc = COMM_BIT_TIMEOUT_FLAG;
                //CheckPoint FF   FE-FF 21.2us

                // Already reset in frame timeout duration!!!
                if (CommData.Step == COMM_STEP_IDLE)  //CHECKPOINT This function duration 2 us
                {
                    CommData.Step = COMM_STEP_HEAD; //接收帧头
                }
                //CheckPoint FG   FF-FG 7us
            }
            //CheckPoint FH   FG-FH 1us
            sucTmpDin = (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0;            //继续读CLK

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0;            //继续读CLK

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0;            //继续读CLK

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0;            //继续读CLK

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0;            //继续读CLK

            sucTmpDin += (_pltc1out) ? 1 : 0;
            TmpClk += (_pltc0out) ? 1 : 0;            //继续读CLK
            //CheckPoint FI   FH-FI 43us
            if (TmpClk < 4)
            {
                //读取数据电平
                sucTmpDin = ((sucTmpDin > 3) ? 1 : 0);
                //数据接收处理
                CommData.DataRx <<= 1;          //接收数据移位
                CommData.DataRx |= sucTmpDin;   //数据保存
                CommData.DataVB += sucTmpDin;   //校验计算
                //CheckPoint FJ   FI-FJ 61.2us
                CommData.DataTx = CommData.DataTx << 1;     //发送数据移位
                CommData.ClkNext = RISING_EDGE;

                //CheckPoint FK   FJ-FK 10.1us
            }
            else                                    //< 提前回码有误
            {
                PltOpaSwitchOff();
                CommData.Step = COMM_STEP_OVER;     //通信空闲COMM_STEP_IDLE
                CommData.DataTx = 0;                //发送数据位
                CommData.DataRx = 0;                //接收数据位
                CommData.BitCnt = 0;                //数据位计数
            }
            if (CommData.Step == COMM_STEP_DLIP)
            {
                _pltda2l = 0;
                _pltsw &= ~0B00000001; // S2[2]=0-DA,S1[1]=1-LV,S0[0]=0-G
                _pltdac2en = 0;        //关DAC
                _pltaen = 0;
            }
        }
        else
        {
            PltOpaSwitchOff();
        }
        /*-------------------------- 中断出口操作 -----------------------------------------------------*/
        SysData.CmdActive |= ACTIVE_FLAG_COMM;
        _ptm0af = 0;         //< 清中断标志
        //CheckPoint FL RJ   FK-FL 11us   FA-FL 210.6US  RI-RJ 5us  RA-RJ 111.5us Bit9 138us Bit13 188us       
    }

    uint8 Tmp, Dat1, Dat2;
    if (1 == _ptm0pf)                    //< 如果是PTM0的比较器P匹配，处理2ms断帧超时中断
    {
        /*-------------------------- 中断入口操作 -----------------------------------------------------*/
        _pt0on = 0;
        _pt0on = 1;
        // _pt0pau = 0;//TEST
        /*-------------------------- 通讯超时和回码结束处理 --------------------------------------------*/
        switch (timeLoopCounter)
        {
        //TODO modify number below in cases
        case 0:
        {
            switch (m_ptm0Pfunc)
            {
            case COMM_DRAW_TIMER_FLAG:
            {
                PltOpaSwitchOff();
                m_ptm0Pfunc = COMM_TIMEOUT_FLAG;
                timeLoopCounter = 4;
                _ptm0rpl = 0xFF; // 16.386ms
                _ptm0rph = 0x03;
                _pt0on = 0;
                _pt0on = 1;
                _pt0pau = 0;
            }
            break;
            case COMM_BIT_TIMEOUT_FLAG:
                if(_pltc0out > 0)
                {
                    m_ptm0Pfunc = COMM_TIMEOUT_FLAG;
                    _ptm0rpl = 0x71;
                    _ptm0rph = 0x02;
                    _pt0on = 0;
                    _pt0on = 1;
                    timeLoopCounter = 0;
                    timeoutDurationFlag = 0;
                }
                else
                {
                    CommData.Step = COMM_STEP_ERROR;
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
                    }
                }
                else
                {
                    CommData.CLIPKey3 = 0; //清除命令序列
                }

                if(timeoutDurationFlag)
                {
                    _ptm0rpl = 0xFF;
                    _ptm0rph = 0x03;
                    timeLoopCounter = 4;
                    break;
                }
                if ((((CommData.DataCmd & 0x5) == 0x5))&&(CommData.Step == COMM_STEP_CLIP))
                {
                    GPO_HT_LED_0();
                    FuncData.State &= ~FUNC_SD_ST_DLIP_LED;
                }
                CommData.Step = COMM_STEP_IDLE; //通信空闲
                CommData.DataTx = 0;            //发送数据位
                CommData.DataRx = 0;            //接收数据位
                CommData.BitCnt = 0;            //数据位计数
                CommData.DataPW = 0;
                CommData.DataVB = 0;
                CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
                CommData.ClkNext = FALLING_EDGE;
                timeoutDurationFlag = 0;
                _pt0pau = 1; // Shut down timer
                _scc = 0B00000001;                      //休眠模式重新修改为低速时钟使能
                SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
                FuncData.FlagBreak = 0;
                //闪灯
               
            }
            break;
            }
        }
        break;

        case 1:
        {
            _ptm0rpl = 0xDD;
            _ptm0rph = 0x01;
            timeLoopCounter -= 1;
            _pt0pau = 0;
            _pt0on = 0;
            _pt0on = 1;
        }
        break;

        case 2:
        case 3:
        case 4:
        case 5:
        {
            _ptm0rpl = 0xFF;
            _ptm0rph = 0x03;
            timeLoopCounter -= 1;
            _pt0pau = 0;
            _pt0on = 0;
            _pt0on = 1;
        }
        break;
        }

        /*-------------------------- 中断出口操作 -----------------------------------------------------*/      
        _ptm0pf = 0;                     //清除中断标志
    }
    //CheckPoint I(finish)  H-I 73.6us
    // _pa6 = 0;
}


//PTM1 P和A 中断入口 0x20
void __attribute((interrupt(0x20))) ISR_PTM1CMP_PTM1CMA(void)
{
    return;
}

//CTM0 P和A 中断入口 0x24
void __attribute((interrupt(0x24))) ISR_CTM0CMP_CTM0CMA(void)
{
    //TODO :
    //TODO:
    //TODO:
    //TOdo: if interrupt triggered properly???? to identify the comm issue
    // _scc=0B00000011;
//  _pa3 = 1;
//  GCC_DELAY(20);
//  _pa3 = 0;
    #if 1

        _ptm0pf = 0;
        _ptm0af = 0;
        _ct0on = 0;
        _pt0on = 1;
        // stmLoopCtrl = 0;
        CommData.Step = COMM_STEP_IDLE; //通信空闲
        CommData.DataTx = 0;            //发送数据位
        CommData.DataRx = 0;            //接收数据位
        CommData.BitCnt = 0;            //数据位计数
        CommData.DataPW = 0;
        CommData.DataVB = 0;
        CLIPDLIPFlag = CLIP_PROTOCOL_RESET_FLAG;
        CommData.ClkNext = FALLING_EDGE;
        _pt0pau = 1; // Shut down timer
        timeoutDurationFlag = 0;
        _pltc0f = 0;
        _pltc0e = 0;
        FuncData.FlagBreak = 0;
        // _pa4 = 0;
        _ctm0af = 0;
        _ct1on = 0;

        _pltc0pol = 0;

        _scc = 0B00000001;                      //休眠模式重新修改为低速时钟使能
        SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
               

   
    #endif
    // _pa4 = 1;
    // _pa4 = 0;

    _ctm0af = 0;
    _scc = 0B00000011;                      //休眠模式重新修改为低速时钟使能
    SysData.CmdActive &= ~ACTIVE_FLAG_COMM; //通信激活关闭
}

//CTM1 P和A 中断入口 0x28
void __attribute((interrupt(0x28))) ISR_CTM1CMP_CTM1CMA(void)
{
    //A匹配中断,30ms中断即使选择32.768k的时钟，最大也就32ms
    if(1 == _ctm1af)
    {
        _emi = 1;                                                           //< 允许嵌套通信等其他中断
        if(FuncData.Interval)					//功能函数定时间隔
        {
            FuncData.Interval --;
        }

        //------------------------------
        SysData.CmdActive |= ACTIVE_FLAG_RUN;	//中断开启单次激活
        _ctm1af = 0;
    }

    if(1 == _ctm1pf)
    {
        _ctm1pf = 0;
    }
}

//TimeBase0 中断入口 0x2c
//TimeBase 在开启和关闭的过程中无法清零CNT
//无法满足AV的总线同步要求
void __attribute((interrupt(0x2C))) ISR_TIMEBASE0(void)
{
    return;
}


//TimeBase1 中断入口 0x30
void __attribute((interrupt(0x30))) ISR_TIMEBASE1(void)
{
    return;
}

//PLT Comp 中断入口 0x34
void __attribute((interrupt(0x34))) ISR_PLTCMP1(void)
{
    return;
}

