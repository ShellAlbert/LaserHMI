#include "zlitronicsci.h"
#include "ci/crc16.h"
#include <QDebug>
#include <QDateTime>
#include "gbl/hmi_const.h"
ZLitronicsCI::ZLitronicsCI(ZRingBuffer *txBuf,ZRingBuffer *rxBuf)
{
    this->m_txRingBuf=txBuf;
    this->m_rxRingBuf=rxBuf;
    this->m_txBuffer=new QByteArray;
    this->m_rxBuffer=new QByteArray;
    this->m_exitFlag=false;
    this->m_bSelfCheckPassed=false;
}
ZLitronicsCI::~ZLitronicsCI()
{
    delete this->m_txBuffer;
    delete this->m_rxBuffer;
}
qint32 ZLitronicsCI::ZPacketCmdToBuffer(char *buffer,const qint32 bufSize)
{
    if(this->m_txBuffer->size()>bufSize)
    {
        return -1;
    }
    memcpy(buffer,this->m_txBuffer->data(),this->m_txBuffer->size());
    return this->m_txBuffer->size();
}
qint32 ZLitronicsCI::ZParseCmdFromBuffer(QVector<quint8> &result)
{
    //SyncHead:4 bytes.
    //PktLen: 1 byte.
    //Address: 1 byte.
    //Command: 1 byte.
    //Data: n byte.
    //CRC16: 2 bytes.
    //check length.
    qint32 ret=0;
    do{
        if(this->m_rxBuffer->size()<9)
        {
            emit this->ZSigErrMsg(tr("Not complete response command!\nParse failed!"));
            ret=-1;
            break;
        }
        //Check SyncHead.
        QByteArray head;
        head.append(0x55);
        head.append(0xAA);
        head.append(0x55);
        head.append(0xAA);
        if(this->m_rxBuffer->mid(0,4)!=head)
        {
            emit this->ZSigErrMsg(tr("Invalid sync head bytes!\nData check failed!"));
            emit this->ZSigDecodeDone(-1);
            ret=-1;
            //set error flags.[3]:error sync head.
            g_GblHelp.m_CommunicateErrFlag|=(0x1<<3);
            break;
        }else{
            g_GblHelp.m_CommunicateErrFlag&=~(0x1<<3);
        }
        //check PktLen.
        quint8 pktLen=this->m_rxBuffer->at(4);
        if(pktLen>this->m_rxBuffer->size()-5)
        {
            emit this->ZSigErrMsg(tr("Packet length error!"));
            emit this->ZSigDecodeDone(-1);
            ret=-1;
            //set error flags.[4]:error packet length.
            g_GblHelp.m_CommunicateErrFlag|=(0x1<<4);
            break;
        }else{
            g_GblHelp.m_CommunicateErrFlag&=~(0x1<<4);
        }
        //Check CRC16.
        if(g_GblHelp.m_bEnCRC16)
        {
            quint16 crc16=crc16_ccitt((unsigned char*)this->m_rxBuffer->data(),this->m_rxBuffer->size()-2);
            //qDebug("%0x",crc16);
            quint8 crc16_high=(quint8)((crc16&0xFF00)>>8);
            quint8 crc16_low=(quint8)(crc16&0x00FF);
            //qDebug("high,low:%0x,%0x",crc16_high,crc16_low);
            //-2: thus CRC16 are two bytes.
            qint8 indexCRC16=4+1+pktLen-2;
            quint8 cmdCRC16High=this->m_rxBuffer->at(indexCRC16);
            quint8 cmdCRC16Low=this->m_rxBuffer->at(indexCRC16+1);
            //qDebug("cmdcrc16:%0x,%0x",cmdCRC16High,cmdCRC16Low);
            if(cmdCRC16High!=crc16_high || cmdCRC16Low!=crc16_low)
            {
                emit this->ZSigErrMsg("CRC16 error!");
                emit this->ZSigDecodeDone(-1);
                ret=-1;
                //set error flags.[5]:crc16 error.
                g_GblHelp.m_CommunicateErrFlag|=(0x1<<5);
                break;
            }else{
                g_GblHelp.m_CommunicateErrFlag&=~(0x1<<5);
            }
        }
        //check Address.
        quint8 address=this->m_rxBuffer->at(5);
        if(address!=0x0)
        {
            emit this->ZSigErrMsg(tr("ID error!"));
            emit this->ZSigDecodeDone(-1);
            ret=-1;
            //set error flags.[6]:address error.
            g_GblHelp.m_CommunicateErrFlag|=(0x1<<6);
            break;
        }else{
            g_GblHelp.m_CommunicateErrFlag&=~(0x1<<6);
        }
        //Check Command.
        quint8 command=this->m_rxBuffer->at(6);
        //dataOffset=syncHead (4)+pktLen(1)+pktLen-Address(1)-Command(1)-CRC16(2)
        //dataLen=pktLen-Address(1)-Command(1)-CRC16(2)
        quint8 dataOffset=4+1+1+1;
        quint8 dataLen=pktLen-1-1-2;
        //qDebug("dataOffset=%d,dataLen=%d",dataOffset,dataLen);
        //for(qint32 i=0;i<dataLen;i++)
        //{
        //   qDebug("%02x ",this->m_rxBuffer->at(dataOffset+i));
        //}
        //clear unknown command flag.
        g_GblHelp.m_CommunicateErrFlag&=~(0x1<<7);
        switch(command)
        {
        case CMD_SELF_CHECK:
            if(2==dataLen)
            {
                result.append(CMD_SELF_CHECK);
                result.append(this->m_rxBuffer->at(dataOffset+0));
                result.append(this->m_rxBuffer->at(dataOffset+1));
                //receive length is right.
                g_GblHelp.m_CommunicateErrFlag&=~(0x1<<8);
            }else{
                //receive length is not right.
                g_GblHelp.m_CommunicateErrFlag|=(0x1<<8);
                ret=-1;
            }
            break;
        case CMD_GET_FIRMWARE_NO:
            if(14==dataLen)
            {
                result.append(CMD_GET_FIRMWARE_NO);
                for(qint32 i=0;i<14;i++)
                {
                    result.append(this->m_rxBuffer->at(dataOffset+i));
                }
                //receive length is right.
                g_GblHelp.m_CommunicateErrFlag&=~(0x1<<8);
            }else{
                //receive length is not right.
                g_GblHelp.m_CommunicateErrFlag|=(0x1<<8);
                ret=-1;
            }
            break;
        case CMD_HEART_BEAT:
            result.append(CMD_HEART_BEAT);
            this->ZRxCmdHeartBeat((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_LASER_CTL_ALL:
            result.append(CMD_LASER_CTL_ALL);
            this->ZRxCmdLaserCtl((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_LASER_CTL_SUB:
            result.append(CMD_LASER_CTL_SUB);
            this->ZRxCmdLaserCtl((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_SET_LASER_PWR_PERCENT:
            result.append(CMD_SET_LASER_PWR_PERCENT);
            this->ZRxCmdSetLaserPwrPercent((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_SET_COLOR_TEMP:
            result.append(CMD_SET_COLOR_TEMP);
            this->ZRxCmdSetColorTemp((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_SET_COLOR_TEMP_PERCENT:
            break;
        case CMD_GET_COLOR_TEMP_PERCENT:
            result.append(CMD_GET_COLOR_TEMP_PERCENT);
            this->ZRxCmdGetColorTempPercent((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_SET_RTC_TIME:
            result.append(CMD_SET_RTC_TIME);
            this->ZRxCmdSetRTC((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_GET_RTC_TIME:
            result.append(CMD_GET_RTC_TIME);
            this->ZRxCmdGetRTC((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_GET_GLASER_DATA:
            //get glaser all data.
            result.append(CMD_GET_GLASER_DATA);
            this->ZRxCmdGetGLaserData((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_CHILLER_CTL:
            result.append(CMD_CHILLER_CTL);
            this->ZRxCmdChillerCtl((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_CHILLER_GET_FAULT:
            result.append(CMD_CHILLER_GET_FAULT);
            this->ZRxCmdChillerFault((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_CHILLER_GET_TEMP:
            result.append(CMD_CHILLER_GET_TEMP);
            this->ZRxCmdChillerTemp((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_CHILLER_GET_FLOW:
            result.append(CMD_CHILLER_GET_FLOW);
            this->ZRxCmdChillerFlow((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_SET_FIBER_BREAK_VOLTAGE:
            result.append(CMD_SET_FIBER_BREAK_VOLTAGE);
            this->ZRxCmdFiberBreakVoltage((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_GET_HOME_PAGE_DATA:
            result.append(CMD_GET_HOME_PAGE_DATA);
            this->ZRxCmdHomePageData((quint8*)this->m_rxBuffer->data()+dataOffset,dataLen);
            break;
        case CMD_SET_CHILLER_TEMP:
            break;
        case CMD_SET_CHILLER_ENTERFLOW_MONITORTHRESHOLD:
            break;
        default:
            emit this->ZSigErrMsg(tr("Unknown response command!"));
            //set error flags.[7]:unknown command.
            g_GblHelp.m_CommunicateErrFlag|=(0x1<<7);
            break;
        }
        this->m_rxBuffer->resize(0);

    }while(0);
    emit this->ZSigTxRxDone(1);//yellow.
    return ret;
}
QString ZLitronicsCI::ZCvtCmd2Hex(char *buffer,const qint32 bufSize)
{
    QString hex;
    for(qint32 i=0;i<bufSize;i++)
    {
        char fmtBuf[3];
        sprintf(fmtBuf,"%02x ",buffer[i]&0xFF);
        hex.append(QString("%1").arg(fmtBuf));
    }
    return hex;
}
qint32 ZLitronicsCI::ZRxCmdTimeout(QString cmd,QString arguments)
{
    qDebug()<<"rxcmdtimeout:"<<cmd<<arguments;
    QString postArguments;
    QStringList argList=arguments.split(",");
    for(qint32 i=0;i<argList.size();i++)
    {
        QString objVal=argList.at(i);
        QStringList objValList=objVal.split("=");
        if(2!=objValList.size())
        {
            continue;
        }
        postArguments+=objValList.at(0)+"=timeout,";
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,postArguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdError(QString cmd,QString arguments)
{
    qDebug()<<"rxcmderror:"<<cmd<<arguments;
    QString postArguments;
    QStringList argList=arguments.split(",");
    for(qint32 i=0;i<argList.size();i++)
    {
        QString objVal=argList.at(i);
        QStringList objValList=objVal.split("=");
        if(2==objValList.size())
        {
            if(objValList.at(0)=="rgbw")
            {
                postArguments+="red=timeout,green=timeout,blue=timeout,wb=timeout,";
            }else{
                postArguments+=objValList.at(0)+"=timeout";
            }
        }
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,postArguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
//MD:55 AA 55 AA 05 01 19 87 xx xx.
//SD:55 AA 55 AA 07 00 19 88 xx xx.
qint32 ZLitronicsCI::ZTxCmdHeartBeat(HeartBeatPkt &pkt)
{
    quint8 dataBody[2];
    dataBody[0]=(quint8)((pkt.m_nHBNum&0xFF00)>>8);
    dataBody[1]=(quint8)(pkt.m_nHBNum&0xFF);
    this->ZBuildPkt(CMD_HEART_BEAT,dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdHeartBeat(quint8 *dataBuf,quint32 dataLen)
{
    if(dataLen!=5)
    {
        QString errMsg(tr("HeartBeat response length is error!"));
        this->ZPopupMsgAddGblErrMsgList(errMsg);
        //receive length is not right.
        g_GblHelp.m_CommunicateErrFlag|=(0x1<<8);
        return -1;
    }else{
        //receive length is right.
        g_GblHelp.m_CommunicateErrFlag&=~(0x1<<8);
    }
    QString cmd("topbar");
    QString arguments;
    qint32 stateFlags=0;
    if(dataBuf[0]&(0x1<<0))
    {
        QString errMsg(tr("PLaser报告GLaser-11发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<10);
        stateFlags|=(0x1<<16);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<10);
        stateFlags&=~(0x1<<16);
    }

    if(dataBuf[0]&(0x1<<1))
    {
        QString errMsg(tr("PLaser报告GLaser-12发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<11);
        stateFlags|=(0x1<<17);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<11);
        stateFlags&=~(0x1<<17);
    }

    if(dataBuf[0]&(0x1<<2))
    {
        QString errMsg(tr("PLaser报告GLaser-13发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<12);
        stateFlags|=(0x1<<18);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<12);
        stateFlags&=~(0x1<<18);
    }

    if(dataBuf[0]&(0x1<<3))
    {
        QString errMsg(tr("PLaser报告GLaser-14发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<13);
        stateFlags|=(0x1<<19);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<13);
        stateFlags&=~(0x1<<19);
    }

    if(dataBuf[0]&(0x1<<4))
    {
        QString errMsg(tr("PLaser报告GLaser-15发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<14);
        stateFlags|=(0x1<<20);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<14);
        stateFlags&=~(0x1<<20);
    }

    if(dataBuf[0]&(0x1<<5))
    {
        QString errMsg(tr("PLaser报告GLaser-16发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<15);
        stateFlags|=(0x1<<21);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<15);
        stateFlags&=~(0x1<<21);
    }
    ///////////////////////////////////////////////////////
    if(dataBuf[1]&(0x1<<0))
    {
        QString errMsg(tr("PLaser报告GLaser-3发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<2);
        stateFlags|=(0x1<<8);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<2);
        stateFlags&=~(0x1<<8);
    }
    if(dataBuf[1]&(0x1<<1))
    {
        QString errMsg(tr("PLaser报告GLaser-4发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<3);
        stateFlags|=(0x1<<9);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<3);
        stateFlags&=~(0x1<<9);
    }

    if(dataBuf[1]&(0x1<<2))
    {
        QString errMsg(tr("PLaser报告GLaser-5发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<4);
        stateFlags|=(0x1<<10);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<4);
        stateFlags&=~(0x1<<10);
    }
    if(dataBuf[1]&(0x1<<3))
    {
        QString errMsg(tr("PLaser报告GLaser-6发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<5);
        stateFlags|=(0x1<<11);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<5);
        stateFlags&=~(0x1<<11);
    }
    if(dataBuf[1]&(0x1<<4))
    {
        QString errMsg(tr("PLaser报告GLaser-7发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<6);
        stateFlags|=(0x1<<12);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<6);
        stateFlags&=~(0x1<<12);
    }

    if(dataBuf[1]&(0x1<<5))
    {
        QString errMsg(tr("PLaser报告GLaser-8发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<7);
        stateFlags|=(0x1<<13);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<7);
        stateFlags&=~(0x1<<13);
    }
    if(dataBuf[1]&(0x1<<6))
    {
        QString errMsg(tr("PLaser报告GLaser-9发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<8);
        stateFlags|=(0x1<<14);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<8);
        stateFlags&=~(0x1<<14);
    }
    if(dataBuf[1]&(0x1<<7))
    {
        QString errMsg(tr("PLaser报告GLaser-10发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<9);
        stateFlags|=(0x1<<15);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<9);
        stateFlags&=~(0x1<<15);
    }
    /////////////////////////////////////////////////////////////
    if(dataBuf[2]&(0x1<<0))
    {
        g_GblHelp.m_projectPwrOn=1;//the project power on.
    }else{
        g_GblHelp.m_projectPwrOn=0;//the project power off.
    }

    if(dataBuf[2]&(0x1<<1))
    {
        QString errMsg(tr("PLaser报告1号水冷机故障!"));
        g_GblHelp.m_chillerFault|=(0x1<<0);//1# chiller fault.

        stateFlags|=(0x1<<1);
    }else{
        g_GblHelp.m_chillerFault&=~(0x1<<0);//1# chiller okay.
        stateFlags&=~(0x1<<1);
    }

    if(dataBuf[2]&(0x1<<2))
    {
        g_GblHelp.m_chillerPwr|=(0x1<<0);//1# chiller on.
    }else{
        g_GblHelp.m_chillerPwr&=~(0x1<<0);//1# chiller off.
    }
    if(dataBuf[2]&(0x1<<3))
    {
        QString errMsg(tr("PLaser报告2号水冷机故障!"));
        g_GblHelp.m_chillerFault|=(0x1<<1);//2# chiller fault.

        stateFlags|=(0x1<<3);
    }else{
        g_GblHelp.m_chillerFault&=~(0x1<<1);//2# chiller okay.
        stateFlags&=~(0x1<<3);
    }
    if(dataBuf[2]&(0x1<<4))
    {
        g_GblHelp.m_chillerPwr|=(0x1<<1);//2# chiller on.
    }else{
        g_GblHelp.m_chillerPwr&=~(0x1<<1);//2# chiller off.
    }
    if(dataBuf[2]&(0x1<<5))
    {
        QString errMsg(tr("PLaser日志空间报警!"));
        g_GblHelp.m_logFault|=(0x1<<0);
    }else{
        g_GblHelp.m_logFault&=~(0x1<<0);
    }
    if(dataBuf[2]&(0x1<<6))
    {
        QString errMsg(tr("PLaser报告GLaser-1发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<0);
        stateFlags|=(0x1<<6);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<0);
    }
    if(dataBuf[2]&(0x1<<7))
    {
        QString errMsg(tr("PLaser报告GLaser-2发生异常!"));
        g_GblHelp.m_laserFault|=(0x1<<1);
        stateFlags|=(0x1<<7);
    }else{
        g_GblHelp.m_laserFault&=~(0x1<<1);
    }
    /////////////////////////////////////////////////////////////////

    qint16 nHBNum=0;
    nHBNum|=(qint16)(dataBuf[3])<<8;
    nHBNum|=(qint16)(dataBuf[4]);
    arguments+=QString("hbNum=%1,").arg(QString::number(nHBNum,10));
    arguments+=QString("stateFlag=%2,").arg(QString::number(stateFlags,10));
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
//Master:55 AA 55 AA 06 01 10 19 87 xx xx.
//Slave:55 AA 55 AA 06 00 10 19 87 xx xx.
qint32 ZLitronicsCI::ZTxCmdSelfCheck(quint16 randomNum)
{
    quint8 chkNumber[2];
    chkNumber[0]=(quint8)((randomNum&0xFFFF)>>8);
    chkNumber[1]=(quint8)(randomNum&0xFF);
    this->ZBuildPkt(CMD_SELF_CHECK,chkNumber,sizeof(chkNumber));
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdSelfCheck(quint8 *dataBuf,quint32 dataLen)
{
    qint32 retNum=0;
    quint8 numHigh=this->m_rxBuffer->at(7);
    quint8 numLow=this->m_rxBuffer->at(8);
    retNum|=(qint32)(numHigh<<8);
    retNum|=(qint32)(numLow);
    return retNum;
}
//MD:55 AA 55 AA 04 01 11 xx xx.
//SD:55 AA 55 AA 0A 00 11 00 00 00 01 02 01 xx xx.
qint32 ZLitronicsCI::ZTxCmdGetFirmwareNo(void)
{
    this->ZBuildPkt(CMD_GET_FIRMWARE_NO,NULL,0);
    return 0;
}

//MD:55 AA 55 AA 05 01 60 3F XX XX
//SD:55 AA 55 AA 05 00 60 3F XX XX
qint32 ZLitronicsCI::ZTxCmdLaserCtl(LaserCtlAllPkt &pkt)
{
    if(pkt.m_devId==0xFF)//main control.
    {
        quint8 dataBody=0;
        if(pkt.m_OpCmd)
        {
            dataBody|=(0x1<<0);//open or close.
        }
        if(pkt.m_careBits&CARE_BIT_RED)
        {
            dataBody|=(0x1<<1);//red channel.
        }
        if(pkt.m_careBits&CARE_BIT_GREEN)
        {
            dataBody|=(0x1<<2);//green channel.
        }
        if(pkt.m_careBits&CARE_BIT_BLUE)
        {
            dataBody|=(0x1<<3);//blue channel.
        }
        if(pkt.m_careBits&CARE_BIT_WB)
        {
            dataBody|=(0x1<<4);//white balance.
        }
        this->ZBuildPkt(CMD_LASER_CTL_ALL,(quint8*)&dataBody,sizeof(dataBody));
    }else{//module control.
        quint8 dataBody[2];
        dataBody[0]=0;
        dataBody[1]=0;
        //////////////////////
        if(pkt.m_OpCmd)
        {
            dataBody[0]|=(0x1<<0);//open or close.
        }
        if(pkt.m_careBits&CARE_BIT_RED)
        {
            dataBody[0]|=(0x1<<1);//red channel.
        }
        if(pkt.m_careBits&CARE_BIT_GREEN)
        {
            dataBody[0]|=(0x1<<2);//green channel.
        }
        if(pkt.m_careBits&CARE_BIT_BLUE)
        {
            dataBody[0]|=(0x1<<3);//blue channel.
        }
        if(pkt.m_careBits&CARE_BIT_WB)
        {
            dataBody[0]|=(0x1<<4);//white balance.
        }
        dataBody[1]=pkt.m_devId;
        this->ZBuildPkt(CMD_LASER_CTL_SUB,(quint8*)&dataBody,sizeof(dataBody));
    }
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdLaserCtl(quint8 *dataBuf,qint32 dataLen)
{
    QString cmd;
    QString arguments;
    if(3==dataLen)
    {
        cmd=QString("main_pwr");
        if(dataBuf[0]&(0x1<<4))
        {
            arguments+=QString("wb=on,");
        }else{
            arguments+=QString("wb=off,");
        }
        if(dataBuf[0]&(0x1<<1))
        {
            arguments+=QString("red=on,");
        }else{
            arguments+=QString("red=off,");
        }
        if(dataBuf[0]&(0x1<<2))
        {
            arguments+=QString("green=on,");
        }else{
            arguments+=QString("green=off,");
        }
        if(dataBuf[0]&(0x1<<3))
        {
            arguments+=QString("blue=on,");
        }else{
            arguments+=QString("blue=off,");
        }
        //dataBuf[1],dataBuf[2].
        if(dataBuf[1]!=0 || dataBuf[2]!=0)
        {
            //16 GLaser modules.
            g_GblHelp.m_glaserPwrFault=0xFFFF;
        }
    }else if(2==dataLen)
    {
        cmd=QString("%1_pwr").arg(dataBuf[1]);
        if(dataBuf[0]&(0x1<<1))
        {
            arguments+=QString("red=on,");
        }else{
            arguments+=QString("red=off,");
        }
        if(dataBuf[0]&(0x1<<2))
        {
            arguments+=QString("green=on,");
        }else{
            arguments+=QString("green=off,");
        }
        if(dataBuf[0]&(0x1<<3))
        {
            arguments+=QString("blue=on,");
        }else{
            arguments+=QString("blue=off,");
        }
        if(dataBuf[0]&(0x1<<4))
        {
            arguments+=QString("wb=on,");
        }else{
            arguments+=QString("wb=off,");
        }
        //clear error flag.
        qint8 nGLaserAddr=dataBuf[1];
        g_GblHelp.m_glaserPwrFault&=~(0x1<<(nGLaserAddr-1));
    }else{
        //something wrong.
        QString errMsg(tr("Response command of LaserCtl length is incorrect!"));
        emit this->ZSigErrMsg(errMsg);
        //set error flag.
        qint8 nGLaserAddr=dataBuf[1];
        g_GblHelp.m_glaserPwrFault|=(0x1<<(nGLaserAddr-1));
    }

    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}

//MD:55 AA 55 AA 05 01 62 01 XX XX.
//SD:55 AA 55 AA 07 00 62 00 0E 01 XX XX.
qint32 ZLitronicsCI::ZTxCmdGetLaserFault(qint8 moduleNo)
{
    this->ZBuildPkt(CMD_GET_LASER_FAULT,(quint8*)&moduleNo,sizeof(moduleNo));
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdGetLaserFault(LaserFaultPkt &pkt)
{
    qint8 tStatus1=this->m_rxBuffer->at(7);
    qint8 tStatus2=this->m_rxBuffer->at(8);
    qint8 moduleNo=this->m_rxBuffer->at(9);

    if(tStatus1&(0x1<<0))
    {
        pkt.m_bBlueOV=1;
    }else{
        pkt.m_bBlueOV=0;
    }
    if(tStatus1&(0x1<<1))
    {
        pkt.m_bBlueOT=1;
    }else{
        pkt.m_bBlueOT=0;
    }

    if(tStatus2&(0x1<<0))
    {
        pkt.m_bFiberFault=1;
    }else{
        pkt.m_bFiberFault=0;
    }
    if(tStatus2&(0x1<<1))
    {
        pkt.m_bRedOC=1;
    }else{
        pkt.m_bRedOC=0;
    }
    if(tStatus2&(0x1<<2))
    {
        pkt.m_bRedOV=1;
    }else{
        pkt.m_bRedOV=0;
    }
    if(tStatus2&(0x1<<3))
    {
        pkt.m_bRedOT=1;
    }else{
        pkt.m_bRedOT=0;
    }
    if(tStatus2&(0x1<<4))
    {
        pkt.m_bGreenOC=1;
    }else{
        pkt.m_bGreenOC=0;
    }
    if(tStatus2&(0x1<<5))
    {
        pkt.m_bGreenOV=1;
    }else{
        pkt.m_bGreenOV=0;
    }
    if(tStatus2&(0x1<<6))
    {
        pkt.m_bGreenOT=1;
    }else{
        pkt.m_bGreenOT=0;
    }
    if(tStatus2&(0x1<<7))
    {
        pkt.m_bBlueOC=1;
    }else{
        pkt.m_bBlueOC=0;
    }
    pkt.m_moduleNo=moduleNo;
    return 0;
}

//MD:55 AA 55 AA 04 01 64 XX XX.
//SD:55 AA 55 AA 06 00 64 00 00 XX XX.
qint32 ZLitronicsCI::ZTxCmdGetChillerFault(void)
{
    this->ZBuildPkt(CMD_GET_CHILLER_FAULT,NULL,0);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdGetChillerFault(ChillerFaultPkt &pkt)
{
    qint32 tStatus1=this->m_rxBuffer->at(7);
    qint32 tStatus2=this->m_rxBuffer->at(8);
    if(tStatus2&(0x1<<0))
    {
        pkt.m_fanFault=1;
    }else{
        pkt.m_fanFault=0;
    }
    if(tStatus2&(0x1<<1))
    {
        pkt.m_motorFault=1;
    }else{
        pkt.m_motorFault=0;
    }
    if(tStatus2&(0x1<<2))
    {
        pkt.m_temperatureFault=1;
    }else{
        pkt.m_temperatureFault=0;
    }
    if(tStatus2&(0x1<<3))
    {
        pkt.m_waterLevelFault=1;
    }else{
        pkt.m_waterLevelFault=0;
    }
    if(tStatus2&(0x1<<4))
    {
        pkt.m_compressorFault=1;
    }else{
        pkt.m_compressorFault=0;
    }
    if(tStatus2&(0x1<<5))
    {
        pkt.m_selfProtectOn=1;
    }else{
        pkt.m_selfProtectOn=0;
    }
    if(tStatus2&(0x1<<6))
    {
        pkt.m_bypassChiller=1;
    }else{
        pkt.m_bypassChiller=0;
    }
    return 0;
}
//MD:55 AA 55 AA 07 01 66 02 00 32 XX XX.
//SD:55 AA 55 AA 07 00 66 02 00 32 XX XX.
qint32 ZLitronicsCI::ZTxCmdSetLaserPwrPercent(LaserPowerPercentPkt &pkt)
{
    quint8 dataBody[4];
    dataBody[0]=pkt.m_mdlId;
    dataBody[1]=pkt.m_RGBChannel;
    dataBody[2]=(quint8)((pkt.m_percent&0xFF00)>>8);
    dataBody[3]=(quint8)((pkt.m_percent&0x00FF)>>0);
    this->ZBuildPkt(CMD_SET_LASER_PWR_PERCENT,dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdSetLaserPwrPercent(quint8 *dataBuf,qint32 dataLen)
{
    QString cmd;
    QString arguments;
    if(4!=dataLen)
    {
        //something wrong.
        QString errMsg(tr("GLaser功率调节命令返回不正确!"));
        this->ZPopupMsgAddGblErrMsgList(errMsg);
    }else{
        quint8 mdlNo=dataBuf[0];
        quint8 channelNo=dataBuf[1];
        quint16 percent=(quint16)(dataBuf[2])<<8|dataBuf[3];
        if(mdlNo==0xFF)
        {
            cmd=QString("main_tuning");
        }else{
            cmd=QString("%1_tuning").arg(mdlNo);
        }
        switch(channelNo)
        {
        case 0x0:
            arguments+=QString("rgb=%1,").arg(percent);
            break;
        case 0x1:
            arguments+=QString("red=%1,").arg(percent);
            break;
        case 0x2:
            arguments+=QString("green=%1,").arg(percent);
            break;
        case 0x3:
            arguments+=QString("blue=%1,").arg(percent);
            break;
        default:
            break;
        }
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdGetColorTempPercent(quint8 *dataBuf,qint32 dataLen)
{
    QString cmd;
    QString arguments;
    if(8!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("获取色温模式命令返回不正确！"));
    }else{
        quint8 mdlNo=dataBuf[0];
        quint8 colorTemp=dataBuf[1];
        if(mdlNo==0xFF)
        {
            cmd=QString("main_GetColorTempPercent");
        }else{
            cmd=QString("%1_GetColorTempPercent").arg(mdlNo);
        }
        arguments+="CT="+ZMapColorTemp2String(colorTemp)+",";
        quint16 redPer=(quint16)dataBuf[2]<<8|(quint16)dataBuf[3];
        quint16 greenPer=(quint16)dataBuf[4]<<8|(quint16)dataBuf[5];
        quint16 bluePer=(quint16)dataBuf[6]<<8|(quint16)dataBuf[7];
        arguments+="R="+QString::number(redPer,10)+",";
        arguments+="G="+QString::number(greenPer,10)+",";
        arguments+="B="+QString::number(bluePer,10);
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZTxCmdSetColorTemp(quint8 mdlNo,quint8 colorTempNo)
{
    quint8 dataBody[2];
    dataBody[0]=mdlNo;
    dataBody[1]=colorTempNo;
    this->ZBuildPkt(CMD_SET_COLOR_TEMP,dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdSetColorTemp(quint8 *dataBuf,qint32 dataLen)
{
    QString cmd;
    QString arguments;
    if(2!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("设置色温模式命令返回不正确！"));
    }else{
        quint8 mdlNo=dataBuf[0];
        quint8 colorTemp=dataBuf[1];
        if(mdlNo==0xFF)
        {
            cmd=QString("main_colorTemp");
        }else{
            cmd=QString("%1_colorTemp").arg(mdlNo);
        }
        arguments="CT="+ZMapColorTemp2String(colorTemp);
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
//MD:55 AA 55 AA 06 01 68 02 01 XX XX.
//SD:55 AA 55 AA 07 00 68 02 01 32 XX XX.
qint32 ZLitronicsCI::ZTxCmdGetLaserPwrPercent(LaserPowerPercentPkt &pkt)
{
    qint8 dataBody[2];
    dataBody[0]=pkt.m_mdlId;
    dataBody[1]=pkt.m_RGBChannel;
    this->ZBuildPkt(CMD_GET_LASER_PWR_PERCENT,(quint8*)dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdGetLaserPwrPercent(LaserPowerPercentPkt &pkt)
{
    qint8 moduleNo=this->m_rxBuffer->at(7);
    qint8 channelNo=this->m_rxBuffer->at(8);
    qint8 percent=this->m_rxBuffer->at(9);
    pkt.m_mdlId=moduleNo;
    pkt.m_RGBChannel=channelNo;
    pkt.m_percent=percent;
    return 0;
}
//MD:55 AA 55 AA 04 01 6A XX XX.
//SD:55 AA 55 AA 07 00 6A 64 01 01 XX XX.
qint32 ZLitronicsCI::ZTxCmdGetLaserPwrRange()
{
    this->ZBuildPkt(CMD_GET_LASER_PWR_RANGE,NULL,0);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdGetLaserPwrRange(qint8 *minVal,qint8 *maxVal,qint8 *stepVal)
{
    qint8 tMinVal=this->m_rxBuffer->at(7);
    qint8 tMaxVal=this->m_rxBuffer->at(8);
    qint8 tStepVal=this->m_rxBuffer->at(9);
    *minVal=tMinVal;
    *maxVal=tMaxVal;
    *stepVal=tStepVal;
    return 0;
}


qint32 ZLitronicsCI::ZBuildPkt(quint8 cmd,quint8 *data,quint8 len)
{
    const quint8 syncHead[]={0x55,0xAA,0x55,0xAA};
    quint8 pktLen;
    quint8 address=0x01;
    quint16 crc16;
    pktLen=sizeof(address)+sizeof(cmd)+len+sizeof(crc16);
    this->m_txBuffer->resize(0);
    //qDebug()<<sizeof(syncHead)<<sizeof(pktLen)<<sizeof(address)<<sizeof(cmd);
    this->m_txBuffer->append((char*)syncHead,sizeof(syncHead));
    this->m_txBuffer->append((char*)&pktLen,sizeof(pktLen));
    this->m_txBuffer->append((char*)&address,sizeof(address));
    this->m_txBuffer->append((char*)&cmd,sizeof(cmd));
    //to support no data cmd.
    if(data!=NULL && len>0)
    {
        this->m_txBuffer->append((char*)data,len);
    }
    crc16=crc16_ccitt((unsigned char*)this->m_txBuffer->data(),this->m_txBuffer->size());
    quint8 crc16_high=(quint8)((crc16&0xFF00)>>8);
    quint8 crc16_low=(quint8)(crc16&0x00FF);
    this->m_txBuffer->append(crc16_high);
    this->m_txBuffer->append(crc16_low);
    return 0;
}

void ZLitronicsCI::run()
{
    this->m_portName=QString("/dev/ttyS1");//yitiji.
    //this->m_portName=QString("/dev/ttyUSB0");//windows.
    //this->m_portName=QString("/dev/pts/1");//linux socat.
    this->m_uart=new QSerialPort;
    this->m_uart->setPortName(this->m_portName);
    this->m_uart->setBaudRate(QSerialPort::Baud115200);
    this->m_uart->setDataBits(QSerialPort::Data8);
    this->m_uart->setParity(QSerialPort::NoParity);
    this->m_uart->setFlowControl(QSerialPort::NoFlowControl);
    this->m_uart->setStopBits(QSerialPort::OneStop);
    this->m_uart->setReadBufferSize(256);
    if(!this->m_uart->open(QIODevice::ReadWrite))
    {
        QString errMsg(tr("cannot open %1,error code %2.").arg(this->m_portName).arg(this->m_uart->errorString()));
        this->ZPopupMsgAddGblErrMsgList(errMsg);
        //return;
    }
    qDebug()<<this->m_uart->portName()<<",open okay!";
    this->m_plainBuffer=new quint8[256];

    //at begin,the self-check is not pass.
    //we need run self-check at first.
    this->m_bSelfCheckPassed=false;

    //used to check the device is link up or down.
    this->m_lastRxTimeStamp=0;
    this->m_devLinkDownTipCnt=0;

    //the thread loop.
    while(!this->m_exitFlag)
    {
        //use this to reduce heavy cpu load.
        QThread::msleep(100);

#if 1
        //update the device is link up or down.
        qint64 nowSec=QDateTime::currentDateTime().toSecsSinceEpoch();
        if(nowSec-this->m_lastRxTimeStamp>g_GblHelp.m_nOfflineTimeout)
        {
            //popup tips message every 10s.
            //10s=10000ms/100ms=100.
            if(this->m_devLinkDownTipCnt++>=100/*999999*/)
            {
                //reset count,we use it to avoid frequently tips.
                this->m_devLinkDownTipCnt=0;
                QString errMsg(tr("PLaser连接已断开!\n请检查物理连接!"));
                this->ZPopupMsgAddGblErrMsgList(errMsg);
                //set error flags.[0]:lost connection to PLaser.
                g_GblHelp.m_CommunicateErrFlag|=(0x1<<0);

                //pause 2s to show warning message on screen.
                QThread::msleep(2000);
                //set self-check flag to false.
                //to cause run self-check again.
                this->m_bSelfCheckPassed=false;
            }
            emit this->ZSigDevLinkDone(-1);//red.
        }else{
            this->m_devLinkDownTipCnt=0;
            emit this->ZSigDevLinkDone(0);//green.
            //reset error flags.[0]:lost connection to PLaser.
            g_GblHelp.m_CommunicateErrFlag&=~(0x1<<0);
        }

        //run self-check command.
        if(g_GblHelp.m_bEnSelfCheck)
        {
            if(false==this->m_bSelfCheckPassed)
            {
                emit this->ZSigErrMsg(tr("HMI正在尝试与PLaser通信自检..."));
                quint16 txRandNum=100;//qrand()%32767;
                this->ZTxCmdSelfCheck(txRandNum);
                qint32 txBytes=this->ZPacketCmdToBuffer((char*)this->m_plainBuffer,256);
                if(!this->ZTxThenRx(this->m_plainBuffer,txBytes))
                {
                    QVector<quint8> result;
                    if(this->ZParseCmdFromBuffer(result)<0)
                    {
                        QString errMsg(tr("HMI与PLaser通信自检失败!"));
                        this->ZPopupMsgAddGblErrMsgList(errMsg);

                        //set error flags.[2]:self-checking failed.
                        g_GblHelp.m_CommunicateErrFlag|=(0x1<<2);

                        //wait for next schedule
                        //we do nothing until self-checking command passed.
                        continue;
                    }
                    if(result.size()!=3)
                    {
                        QString errMsg(tr("通信自检错误:PLaser返回不完整的命令!"));
                        this->ZPopupMsgAddGblErrMsgList(errMsg);
                        //set error flags.[2]:self-checking failed.
                        g_GblHelp.m_CommunicateErrFlag|=(0x1<<2);
                        //wait for next schedule
                        //we do nothing until self-checking command passed.
                        continue;
                    }
                    if(result.at(0)!=CMD_SELF_CHECK)
                    {
                        QString errMsg(tr("通信自检错误:PLaser返回不正确的命令字!"));
                        this->ZPopupMsgAddGblErrMsgList(errMsg);
                        //set error flags.[2]:self-checking failed.
                        g_GblHelp.m_CommunicateErrFlag|=(0x1<<2);
                        //wait for next schedule
                        //we do nothing until self-checking command passed.
                        continue;
                    }
                    quint16 rxHigh=(quint16)(result.at(1))<<8;
                    quint16 rxLow=(quint16)(result.at(2));
                    quint16 rxRandNum=rxHigh|rxLow;
                    qDebug("TX:%x,Rx:%x",txRandNum,rxRandNum);
                    if(rxRandNum!=txRandNum)
                    {
                        QString errMsg(tr("通信自检错误:PLaser返回不正确的随机数!"));
                        this->ZPopupMsgAddGblErrMsgList(errMsg);
                        //set error flags.[2]:self-checking failed.
                        g_GblHelp.m_CommunicateErrFlag|=(0x1<<2);
                        //wait for next schedule
                        //we do nothing until self-checking command passed.
                        continue;
                    }
                    //we get the right code,so set success flag.
                    emit this->ZSigErrMsg(tr("通信自检成功,已建立与PLaser的连接!"));
                    this->m_bSelfCheckPassed=true;
                    //reset error flags.[2]:self-checking failed.
                    g_GblHelp.m_CommunicateErrFlag&=~(0x1<<2);
                }

                //if self-check okay.
                //we request the version number.
                do{
                    this->ZTxCmdGetFirmwareNo();
                    qint32 txBytes=this->ZPacketCmdToBuffer((char*)this->m_plainBuffer,256);
                    if(this->ZTxThenRx(this->m_plainBuffer,txBytes)<0)
                    {
                        this->m_bSelfCheckPassed=false;
                        break;
                    }
                    QVector<quint8> result;
                    if(this->ZParseCmdFromBuffer(result)<0)
                    {
                        QString errMsg(tr("获取PLaser固件版本信息失败!"));
                        this->ZPopupMsgAddGblErrMsgList(errMsg);
                        this->m_bSelfCheckPassed=false;
                        break;
                    }
                    if(result.size()!=15)
                    {
                        QString errMsg(tr("获取PLaser固件版本信息失败,返回长度不对!"));
                        this->ZPopupMsgAddGblErrMsgList(errMsg);
                        this->m_bSelfCheckPassed=false;
                        break;
                    }
                    if(result.at(0)!=CMD_GET_FIRMWARE_NO)
                    {
                        QString errMsg(tr("获取PLaser固件版本信息失败,返回不正确的命令字!"));
                        this->ZPopupMsgAddGblErrMsgList(errMsg);
                        this->m_bSelfCheckPassed=false;
                        break;
                    }
                    char productNo[12];
                    for(qint32 i=0;i<12;i++)
                    {
                        productNo[i]=result.at(i+1);
                    }
                    char versionNo[2];
                    for(qint32 i=0;i<2;i++)
                    {
                        versionNo[i]=result.at(i+13);
                    }
                    g_GblHelp.m_glaserProductNo=QByteArray(productNo,sizeof(productNo)).toHex();
                    g_GblHelp.m_glaserVerNo=QByteArray(versionNo,sizeof(versionNo)).toHex();
                }while(0);
            }//false==this->m_bSelfCheckPassed.

            //if self-check is not pass,then re-schedult self-check command.
            //until it pass.
            if(false==this->m_bSelfCheckPassed)
            {
                continue;
            }
        }//g_GblHelp.m_bEnSelfCheck.
#endif
        /////////////////////////////////////////////////////////////////////
        //we try to fetch normal packets from TxRingBuffer and send them out.
        if(!this->m_txRingBuf->ZTryLock())
        {
            continue;
        }
        QString cmd;
        QString argument;
        if(!this->m_txRingBuf->ZIsEmpty())
        {
            this->m_txRingBuf->ZGetElement(cmd,argument);
        }
        this->m_txRingBuf->ZUnLock();
        if(cmd.isEmpty() && argument.isEmpty())
        {
            continue;
        }

        emit this->ZSigDumpDbgMsg(tr("ZLitronicsCI:get:<%1:%2>").arg(cmd).arg(argument));
        this->ZCvtUI2Cmd(cmd,argument);
        qint32 txBytes=this->ZPacketCmdToBuffer((char*)this->m_plainBuffer,256);
        if(!this->ZTxThenRx(this->m_plainBuffer,txBytes))
        {
            //here we process the commands in ZParseCmdFromBuffer() except SelfCheck.
            //so here we don't care result.
            QVector<quint8> result;
            if(this->ZParseCmdFromBuffer(result)<0)
            {
                this->ZRxCmdError(cmd,argument);
            }
        }else{
            //Tx or Rx failed timeout.
            this->ZRxCmdTimeout(cmd,argument);
        }

        //update the global error flag to enable/disable error flag icon on the right-top corner.
        bool tErrorFlag=false;
        for(qint32 i=0;i<8;i++)
        {
            if(g_GblHelp.m_GLaserErrFlag[i])
            {
                tErrorFlag=true;
                break;
            }
        }
        for(qint32 i=0;i<2;i++)
        {
            if(g_GblHelp.m_ChillerErrFlag[i])
            {
                tErrorFlag=true;
                break;
            }
        }
        if(g_GblHelp.m_CommunicateErrFlag)
        {
            tErrorFlag=true;
        }
        g_GblHelp.m_bGblErr=tErrorFlag;
    }//while.
    //when the exitFlag was set,it will reach here.free resources.
    this->m_uart->close();
    delete this->m_uart;
    delete [] this->m_plainBuffer;
    return;
}
qint32 ZLitronicsCI::ZTxThenRx(quint8 *dataBuf,quint32 dataLen)
{
    bool bTxState=true;
    qint32 ret=0;
    if(dataBuf==NULL || dataLen<=0)
    {
        return -1;
    }
    //set communicate led to green.
    emit this->ZSigTxRxDone(0);//green.

    //clear rx buffer data before tx.
    (void)this->m_uart->readAll();
    this->m_rxBuffer->clear();

    emit this->ZSigDumpDbgMsg("Tx:"+this->ZCvtCmd2Hex((char*)dataBuf,dataLen));
    this->m_uart->write((char*)dataBuf,dataLen);
    if(this->m_uart->waitForBytesWritten(1000))
    {
        if(this->m_uart->waitForReadyRead(g_GblHelp.m_nTimeoutVal*1000))
        {
            this->m_rxBuffer->append(this->m_uart->readAll());
            for(qint32 i=0;i<10;i++)
            {
                if(this->m_uart->waitForReadyRead(20))
                {
                    this->m_rxBuffer->append(this->m_uart->readAll());
                }
            }
            emit this->ZSigDumpDbgMsg("Rx:"+this->ZCvtCmd2Hex(this->m_rxBuffer->data(),this->m_rxBuffer->size()));
            //remember the last rx timestamp.
            this->m_lastRxTimeStamp=QDateTime::currentDateTime().toSecsSinceEpoch();
            emit this->ZSigDevLinkDone(0);//green.
            //reset error flags.[1]:connection timeout.
            g_GblHelp.m_CommunicateErrFlag&=~(0x1<<1);
        }else{
            QString errMsg(tr("HMI与PLaser通信超时!"));
            this->ZPopupMsgAddGblErrMsgList(errMsg);
            bTxState=false;
            ret=-1;
            //set error flags.[1]:connection timeout.
            g_GblHelp.m_CommunicateErrFlag|=(0x1<<1);
        }
    }else{
        QString errMsg(tr("发送命令失败!"));
        g_GblHelp.m_bGblErr=true;
        emit this->ZSigErrMsg(errMsg);
        bTxState=false;
        ret=-1;
    }
    if(bTxState)
    {
        //set communicate led to yellow.
        emit this->ZSigTxRxDone(1);
    }else{
        //set communicate led to red.
        emit this->ZSigTxRxDone(-1);
    }
    return ret;
}
void ZLitronicsCI::ZCvtUI2Cmd(QString cmd,QString arguments)
{
    if(cmd.right(3)=="pwr")
    {
        this->ZParseUICmd_pwr(cmd,arguments);
    }else if(cmd.right(6)==QString("tuning"))
    {
        this->ZParseUICmd_tuning(cmd,arguments);
    }else if(cmd.right(9)==QString("colorTemp"))
    {
        this->ZParseUICmd_colorTemp(cmd,arguments);
    }else if(cmd=="topbar")
    {
        this->ZParseUICmd_heartbeat(cmd,arguments);
    }else if(cmd=="getGLaserData")
    {
        this->ZParseUICmd_getGLaserData(cmd,arguments);
    }else if(cmd.endsWith("SetColorTempPercent"))
    {
        this->ZParseUICmd_setColorTempPercent(cmd,arguments);
    }else if(cmd.endsWith("GetColorTempPercent"))
    {
        this->ZParseUICmd_getColorTempPercent(cmd,arguments);
    }else if(cmd=="ChillerCtl")
    {
        //ChillerCtl,no=1/2,pwr=0/1,bypass=0/1.
        this->ZParseUICmd_ChillerCtl(cmd,arguments);
    }else if(cmd=="getChillerFault")
    {
        //getChillerFault,chillerNo.
        this->ZParseUICmd_ChillerFault(cmd,arguments);
    }else if(cmd=="getChillerFlow")
    {
        this->ZParseUICmd_ChillerFlow(cmd,arguments);
    }else if(cmd=="getChillerTemp")
    {
        this->ZParseUICmd_ChillerTemp(cmd,arguments);
    }else if(cmd.right(3)=="fbv")
    {
        this->ZParseUICmd_FiberBreakVoltage(cmd,arguments);
    }else if(cmd=="getHomePageData")
    {
        this->ZParseUICmd_GetHomePageData(cmd,arguments);
    }else if(cmd=="saveChillerTemp")
    {
        //saveChillerTemp,no=%1,temp=%2,jielu=%3.
        this->ZParseUICmd_SaveTemp2Chiller(cmd,arguments);
    }else if(cmd=="setEnterFlow")
    {
        //setEnterFlow,no=%1,flow=%2,percent=%3.
        this->ZParseUICmd_SetEnterFlowMonitorThreshold(cmd,arguments);
    }else if(cmd=="setRTC")
    {
        //setRTC,2017-09-11 12:00:23.
        this->ZParseUICmd_SetRTC(cmd,arguments);
    }else if(cmd=="getRTC")
    {
        //getRTC
        this->ZParseUICmd_GetRTC(cmd,arguments);
    }
}
qint32 ZLitronicsCI::ZParseUICmd_heartbeat(QString cmd,QString arguments)
{
    HeartBeatPkt pkt;
    if(cmd=="topbar")
    {
        pkt.m_bPwrOn=g_GblHelp.m_bPwrOn;
        pkt.m_bBypassChiller=g_GblHelp.m_bBypassChiller;
        pkt.m_bWhiteBalanceOn=g_GblHelp.m_bWhiteBalanceOn;

        QStringList argList=arguments.split(",");
        for(qint32 i=0;i<argList.size();i++)
        {
            QString objVal=argList.at(i);
            QStringList objValList=objVal.split("=");
            if(objValList.at(0)=="hbNum")
            {
                pkt.m_nHBNum=objValList.at(1).toInt();
            }
        }
        this->ZTxCmdHeartBeat(pkt);
    }
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_pwr(QString cmd,QString arguments)
{
    LaserCtlAllPkt pkt;
    if(cmd.left(4)=="main")
    {
        pkt.m_devId=0xFF;
    }else{
        pkt.m_devId=cmd.left(1).toInt();
    }

    QStringList argList=arguments.split("=");
    if(argList.size()!=2)
    {
        return -1;
    }
    QString obj=argList.at(0);
    QString val=argList.at(1);
    if(obj=="rgbw")
    {
        pkt.m_careBits=CARE_BIT_RED|CARE_BIT_GREEN|CARE_BIT_BLUE|CARE_BIT_WB;
        if(val=="on")
        {
            pkt.m_OpCmd=1;
            pkt.m_redCS=true;
            pkt.m_greenCS=true;
            pkt.m_blueCS=true;
            pkt.m_whiteBalanceCS=true;
        }else if(val=="off")
        {
            pkt.m_OpCmd=0;
            pkt.m_redCS=false;
            pkt.m_greenCS=false;
            pkt.m_blueCS=false;
            pkt.m_whiteBalanceCS=false;
        }
    }else if(obj=="red")
    {
        pkt.m_careBits=CARE_BIT_RED;
        if(val=="on")
        {
            pkt.m_OpCmd=1;
            pkt.m_redCS=true;
        }else if(val=="off")
        {
            pkt.m_OpCmd=0;
            pkt.m_redCS=false;
        }
    }else if(obj=="green")
    {
        pkt.m_careBits=CARE_BIT_GREEN;
        if(val=="on")
        {
            pkt.m_OpCmd=1;
            pkt.m_greenCS=true;
        }else if(val=="off")
        {
            pkt.m_OpCmd=0;
            pkt.m_greenCS=false;
        }
    }else if(obj=="blue")
    {
        pkt.m_careBits=CARE_BIT_BLUE;
        if(val=="on")
        {
            pkt.m_OpCmd=1;
            pkt.m_blueCS=true;
        }else if(val=="off")
        {
            pkt.m_OpCmd=0;
            pkt.m_blueCS=false;
        }
    }else if(obj=="wb")
    {
        pkt.m_careBits=CARE_BIT_WB;
        if(val=="on")
        {
            pkt.m_OpCmd=1;
            pkt.m_whiteBalanceCS=true;
        }else if(val=="off")
        {
            pkt.m_OpCmd=0;
            pkt.m_whiteBalanceCS=false;
        }
    }
    this->ZTxCmdLaserCtl(pkt);
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_tuning(QString cmd,QString arguments)
{
    LaserPowerPercentPkt pkt;
    //glaser module address.
    //0xff,all.
    //0x1,0x2,0x3,0x4....0x8.
    if(cmd.left(4)=="main")
    {
        pkt.m_mdlId=0xFF;
    }else{
        pkt.m_mdlId=cmd.left(1).toInt();
    }
    //rgb=x,red=x,green=x,blue=x.
    QStringList argList=arguments.split("=");
    if(argList.size()!=2)
    {
        return -1;
    }
    QString obj=argList.at(0);
    QString val=argList.at(1);
    if(obj=="rgb")
    {
        pkt.m_RGBChannel=0;
    }else if(obj=="red")
    {
        pkt.m_RGBChannel=1;
    }else if(obj=="green")
    {
        pkt.m_RGBChannel=2;
    }else if(obj=="blue")
    {
        pkt.m_RGBChannel=3;
    }
    pkt.m_percent=val.toInt();
    this->ZTxCmdSetLaserPwrPercent(pkt);
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_getGLaserData(QString cmd,QString arguments)
{
    quint8 dataBody=arguments.toInt();
    this->ZBuildPkt(CMD_GET_GLASER_DATA,&dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_colorTemp(QString cmd,QString arguments)
{
    quint8 mdlNo;
    quint8 colorTempNo;
    if(cmd=="main_colorTemp")
    {
        mdlNo=0xFF;
        QStringList argList=arguments.split("=");
        if(2!=argList.size())
        {
            return -1;
        }
        QString obj=argList.at(0);
        QString val=argList.at(1);
        //qDebug()<<"obj="<<obj<<",val="<<val;
        if(obj=="CT")
        {
            colorTempNo=ZMapColorTemp2Hex(val);
        }else{
            //error.
            return -1;
        }
    }else{
        mdlNo=cmd.left(1).toInt();
        QStringList argList=arguments.split("=");
        if(2!=argList.size())
        {
            return -1;
        }
        QString obj=argList.at(0);
        QString val=argList.at(1);
        //qDebug()<<"obj="<<obj<<",val="<<val;
        if(obj=="CT")
        {
            colorTempNo=ZMapColorTemp2Hex(val);
        }else{
            //error.
            return -1;
        }
    }
    this->ZTxCmdSetColorTemp(mdlNo,colorTempNo);
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_setColorTempPercent(QString cmd,QString arguments)
{
    quint8 addr;
    if(cmd.left(4)=="main")
    {
        addr=0xff;
    }else{
        addr=cmd.left(1).toInt();
    }
    quint8 ct=0;
    quint16 red=0;
    quint16 green=0;
    quint16 blue=0;
    QStringList valList=arguments.split(",");
    for(qint32 i=0;i<valList.size();i++)
    {
        QStringList keyValList=valList.at(i).split("=");
        if(2!=keyValList.size())
        {
            continue;
        }
        if(keyValList.at(0)=="CT")
        {
            ct=ZMapColorTemp2Hex(keyValList.at(1));
        }else if(keyValList.at(0)=="R")
        {
            red=keyValList.at(1).toInt();
        }else if(keyValList.at(0)=="G")
        {
            green=keyValList.at(1).toInt();
        }else if(keyValList.at(0)=="B")
        {
            blue=keyValList.at(1).toInt();
        }
    }
    quint8 dataBody[8];
    dataBody[0]=addr;//glaser address.all.
    dataBody[1]=ct;
    dataBody[2]=(quint8)((red&0xFF00)>>8);//red.
    dataBody[3]=(quint8)(red&0xFF);
    dataBody[4]=(quint8)((green&0xFF00)>>8);//green.
    dataBody[5]=(quint8)(green&0xFF);
    dataBody[6]=(quint8)((blue&0xFF00)>>8);//blue.
    dataBody[7]=(quint8)(blue&0xFF);
    this->ZBuildPkt(CMD_SET_COLOR_TEMP_PERCENT,dataBody,sizeof(dataBody));
    qDebug()<<cmd<<arguments;
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_getColorTempPercent(QString cmd,QString arguments)
{
    quint8 addr;
    if(cmd.left(4)=="main")
    {
        addr=0xff;
    }else{
        addr=cmd.left(1).toInt();
    }

    quint8 dataBody=addr;
    this->ZBuildPkt(CMD_GET_COLOR_TEMP_PERCENT,&dataBody,sizeof(dataBody));
    return 0;
}
void ZLitronicsCI::ZPopupMsgAddGblErrMsgList(QString errMsg)
{
    g_GblHelp.m_bGblErr=true;
    emit this->ZSigErrMsg(errMsg);
}
qint32 ZLitronicsCI::ZRxCmdChillerCtl(quint8 *dataBuf,qint32 dataLen)
{
    if(1!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("水冷机控制数据返回不正确!"));
        return -1;
    }
    quint8 dataBody=dataBuf[0];
    QString cmd("ChillerCtl");
    QString arguments;
    if(dataBody&(0x1<<7))
    {
        arguments+="ret=-1,";
    }else{
        arguments+="ret=0,";
    }
    if(dataBody&(0x1<<2))
    {
        arguments+="no=2,";
    }else{
        arguments+="no=1,";
    }
    if(dataBody&(0x1<<1))
    {
        arguments+="bypass=1,";
    }else{
        arguments+="bypass=0,";
    }
    if(dataBody&(0x1<<0))
    {
        arguments+="pwr=1";
    }else{
        arguments+="pwr=0";
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdChillerFault(quint8 *dataBuf,qint32 dataLen)
{
    if(4!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("水冷机故障状态数据返回不正确!"));
        return -1;
    }
    QString cmd("getChillerFault");
    //getChillerFault,no=1/2,bypass=0/1,pwr=0/1,ice=0/1,flag1=xx,flag2=xx.
    QString arguments;
    if(dataBuf[0]&(0x1<<7))
    {
        arguments+="bypass=1,";
    }else{
        arguments+="bypass=0,";
    }
    if(dataBuf[0]&(0x1<<6))
    {
        arguments+="pwr=1,";
    }else{
        arguments+="pwr=0,";
    }
    if(dataBuf[0]&(0x1<<5))
    {
        arguments+="ice=1,";
    }else{
        arguments+="ice=0,";
    }
    arguments+=QString("flag1=%1,").arg(QString::number(dataBuf[1],10));
    arguments+=QString("flag2=%1,").arg(QString::number(dataBuf[2],10));
    //chiller address.
    qint8 chillerNo=dataBuf[3];
    arguments+=QString("no=%1,").arg(QString::number(chillerNo+1,10));

    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdChillerFlow(quint8 *dataBuf,qint32 dataLen)
{
    if(7!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("水冷机流量数据返回不正确!"));
        return -1;
    }
    QString cmd("getChillerFlow");
    QString arguments;
    //getChillerFlow,no=1/2,leaveFlow=xx.x,enterFlow=xx.xx.
    float leaveFlow;
    float enterFlow;
    qint16 leaveFlowHigh=((qint16)dataBuf[0]<<8)|(qint16)dataBuf[1];
    qint8 leaveFlowLow=dataBuf[2];
    leaveFlow=leaveFlowHigh+leaveFlowLow/100.0f;

    qint16 enterFlowHigh=((qint16)dataBuf[3]<<8)|(qint16)dataBuf[4];
    qint8 enterFlowLow=dataBuf[5];
    enterFlow=enterFlowHigh+enterFlowLow/100.0f;
    //chiller address.
    qint8 chillerNo=dataBuf[6];

    arguments+=QString("no=%1,").arg(QString::number(chillerNo+1,10));
    arguments+=QString("leaveFlow=%1,").arg(QString::number(leaveFlow,'f',2));
    arguments+=QString("enterFlow=%1,").arg(QString::number(enterFlow,'f',2));

    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdChillerTemp(quint8 *dataBuf,qint32 dataLen)
{
    if(9!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("水冷机温度数据返回不正确!"));
        return -1;
    }
    QString cmd("getChillerTemp");
    QString arguments;
    //getChillerTemp,no=0/1,AT=xx.x,LT=xx.xx,ET=xx.xx,CT=xx.xx.
    float ambientTemp=dataBuf[0]+dataBuf[1]/100.0f;
    float leaveTemp=dataBuf[2]+dataBuf[3]/100.0f;
    float enterTemp=dataBuf[4]+dataBuf[5]/100.0f;
    float currentTemp=dataBuf[6]+dataBuf[7]/100.0f;

    //chiller address.
    qint8 chillerNo=dataBuf[8];

    arguments+=QString("no=%1,").arg(QString::number(chillerNo+1,10));
    arguments+=QString("AT=%1,").arg(QString::number(ambientTemp,'f',2));
    arguments+=QString("LT=%1,").arg(QString::number(leaveTemp,'f',2));
    arguments+=QString("ET=%1,").arg(QString::number(enterTemp,'f',2));
    arguments+=QString("CT=%1,").arg(QString::number(currentTemp,'f',2));

    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}

qint32 ZLitronicsCI::ZRxCmdGetGLaserData(quint8 *dataBuf,qint32 dataLen)
{
    if(50!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("获取GLaser状态数据长度不正确!"));
        g_GblHelp.m_CommunicateErrFlag|=(0x1<<8);
        return -1;
    }else{
        g_GblHelp.m_CommunicateErrFlag&=~(0x1<<8);
    }
    quint8 thresHold=dataBuf[0];
    quint16 totalPwrPercent=(quint16)dataBuf[1]<<8|dataBuf[2];
    quint8 colorTemp=dataBuf[3];
    quint16 fiberBreakVoltage=(quint16)dataBuf[4]<<8|dataBuf[5];
    quint16 stateFlag=(quint16)dataBuf[6]<<8|dataBuf[7];
    quint16 RPwrPercent=(quint16)dataBuf[8]<<8|dataBuf[9];
    quint16 GPwrPercent=(quint16)dataBuf[10]<<8|dataBuf[11];
    quint16 BPwrPercent=(quint16)dataBuf[12]<<8|dataBuf[13];
    quint32 runHours=(quint32)dataBuf[14]<<24|(quint32)dataBuf[15]<<16|(quint32)dataBuf[16]<<8|(quint32)dataBuf[17]<<0;
    quint8 runMinute=dataBuf[18];
    quint16 tAmbientTemp=(quint16)dataBuf[19]<<8|dataBuf[20];
    quint16 tG2Temp=(quint16)dataBuf[21]<<8|dataBuf[22];
    quint16 tG1Temp=(quint16)dataBuf[23]<<8|dataBuf[24];
    quint16 tBTemp=(quint16)dataBuf[25]<<8|dataBuf[26];
    quint16 tR2Temp=(quint16)dataBuf[27]<<8|dataBuf[28];
    quint16 tR1Temp=(quint16)dataBuf[29]<<8|dataBuf[30];
    quint16 RWB=(quint16)dataBuf[31]<<8|dataBuf[32];
    quint16 GWB=(quint16)dataBuf[33]<<8|dataBuf[34];
    quint16 BWB=(quint16)dataBuf[35]<<8|dataBuf[36];
    quint16 BVoltage=(quint16)dataBuf[37]<<8|dataBuf[38];
    quint16 BCurrent=(quint16)dataBuf[39]<<8|dataBuf[40];
    quint16 GVoltage=(quint16)dataBuf[41]<<8|dataBuf[42];
    quint16 GCurrent=(quint16)dataBuf[43]<<8|dataBuf[44];
    quint16 RVoltage=(quint16)dataBuf[45]<<8|dataBuf[46];
    quint16 RCurrent=(quint16)dataBuf[47]<<8|dataBuf[48];

    QString cmd=QString("%1_getGLaserData").arg(dataBuf[49]);
    QString arguments;
    arguments+="TH="+QString::number(thresHold,10)+",";
    arguments+="PP="+QString::number(totalPwrPercent,10)+",";
    arguments+="SF="+QString::number(stateFlag,10)+",";
    arguments+="CT="+QString::number(colorTemp,10)+",";
    arguments+="FBV="+QString::number(fiberBreakVoltage,10)+",";
    arguments+="RPP="+QString::number(RPwrPercent,10)+",";
    arguments+="GPP="+QString::number(GPwrPercent,10)+",";
    arguments+="BPP="+QString::number(BPwrPercent,10)+",";
    arguments+="HOUR="+QString::number(runHours,10)+"h"+QString::number(runMinute,10)+"m"+",";
    arguments+="AT="+QString::number(tAmbientTemp,10)+",";
    arguments+="G2="+QString::number(tG2Temp,10)+",";
    arguments+="G1="+QString::number(tG1Temp,10)+",";
    arguments+="B="+QString::number(tBTemp,10)+",";
    arguments+="R2="+QString::number(tR2Temp,10)+",";
    arguments+="R1="+QString::number(tR1Temp,10)+",";
    arguments+="RWB="+QString::number(RWB,10)+",";
    arguments+="GWB="+QString::number(GWB,10)+",";
    arguments+="BWB="+QString::number(BWB,10)+",";
    arguments+="BV="+QString::number(BVoltage,10)+",";
    arguments+="BC="+QString::number(BCurrent,10)+",";
    arguments+="GV="+QString::number(GVoltage,10)+",";
    arguments+="GC="+QString::number(GCurrent,10)+",";
    arguments+="RV="+QString::number(RVoltage,10)+",";
    arguments+="RC="+QString::number(RCurrent,10);
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}

qint32 ZLitronicsCI::ZParseUICmd_ChillerCtl(QString cmd,QString arguments)
{
    //ChillerCtl,no=1/2,pwr=0/1,bypass=0/1.
    quint8 chillerNo;
    quint8 pwrBit;
    quint8 bypassBit;
    QStringList argList=arguments.split(",");
    for(qint32 i=0;i<argList.size();i++)
    {
        QStringList keyValList=argList.at(i).split("=");
        if(keyValList.size()!=2)
        {
            continue;
        }
        if(keyValList.at(0)=="no")
        {
            chillerNo=keyValList.at(1).toInt();
        }else if(keyValList.at(0)=="pwr")
        {
            pwrBit=keyValList.at(1).toInt();
        }else if(keyValList.at(0)=="bypass")
        {
            bypassBit=keyValList.at(1).toInt();
        }
    }
    quint8 dataBody=0;
    //[2]=0,No.1 Chiller.
    //[2]=1,No.2 Chiller.
    if(chillerNo==2)
    {
        dataBody&=~(0x1<<2);
    }
    //[1]=0,enable Chiller.
    //[1]=1,bypass Chiller.
    if(bypassBit==1)
    {
        dataBody|=(0x1<<1);
    }
    //[0]=0,close Chiller.
    //[0]=1,open Chiller.
    if(pwrBit==1)
    {
        dataBody|=(0x1<<0);
    }
    this->ZBuildPkt(CMD_CHILLER_CTL,&dataBody,sizeof(dataBody));
    return 0;
}

//get chiller data.
qint32 ZLitronicsCI::ZParseUICmd_ChillerFault(QString cmd,QString arguments)
{
    //chillerNo.
    quint8 dataBody=arguments.toInt()-1;
    //get chiller Fault.
    this->ZBuildPkt(CMD_CHILLER_GET_FAULT,&dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_ChillerFlow(QString cmd,QString arguments)
{
    //chillerNo.
    quint8 dataBody=arguments.toInt()-1;
    //get chiller Fault.
    this->ZBuildPkt(CMD_CHILLER_GET_FLOW,&dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_ChillerTemp(QString cmd,QString arguments)
{
    //chillerNo.
    quint8 dataBody=arguments.toInt()-1;
    //get chiller Fault.
    this->ZBuildPkt(CMD_CHILLER_GET_TEMP,&dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_FiberBreakVoltage(QString cmd,QString arguments)
{
    //fiber break voltage threshold.
    quint8 dataBody[2];
    dataBody[0]=cmd.left(1).toInt();
    dataBody[1]=arguments.toInt();
    //build packet.
    this->ZBuildPkt(CMD_SET_FIBER_BREAK_VOLTAGE,dataBody,sizeof(dataBody));
    return 0;
}
//parse out fiber break voltage.
qint32 ZLitronicsCI::ZRxCmdFiberBreakVoltage(quint8 *dataBuf,qint32 dataLen)
{
    QString cmd;
    QString arguments;
    if(2!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("设置<光纤折断电压百分比>返回不正确!"));
        return -1;
    }else{
        quint8 mdlNo=dataBuf[0];
        quint8 fbvPercent=dataBuf[1];
        if(fbvPercent==0xFF)
        {
            emit this->ZSigErrMsg(tr("设置<光纤折断电压百分比>失败!"));
            return -1;
        }
        if(mdlNo==0xFF)
        {
            cmd=QString("main_FBVThreshold");
        }else{
            cmd=QString("%1_FBVThreshold").arg(mdlNo);
        }
        arguments=QString::number(fbvPercent,10);
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_GetHomePageData(QString cmd,QString arguments)
{
    //build packet.
    this->ZBuildPkt(CMD_GET_HOME_PAGE_DATA,NULL,0);
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_SaveTemp2Chiller(QString cmd,QString arguments)
{
    quint8 dataBody[4];
    QStringList argList=arguments.split(",");
    for(qint32 i=0;i<argList.size();i++)
    {
        QStringList objValList=argList.at(i).split("=");
        if(objValList.size()!=2)
        {
            continue;//bypass invalid key=value.
        }
        if(objValList.at(0)=="no")
        {
            quint8 chillerNo=objValList.at(1).toInt();
            dataBody[3]=chillerNo-1;
        }else if(objValList.at(0)=="temp")
        {
            float fTemp=objValList.at(1).toFloat();
            int  nZheng=(qint32)fTemp;
            dataBody[1]=(qint8)(nZheng);
            int nXiao=(qint32)((fTemp-nZheng)*100.0f);
            dataBody[2]=(qint8)(nXiao);
        }else if(objValList.at(0)=="jielu")
        {
            if("1"==objValList.at(1))
            {
                dataBody[0]=0x1;
            }else{
                dataBody[0]=0x0;
            }
        }
    }
    this->ZBuildPkt(CMD_SET_CHILLER_TEMP,dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_SetEnterFlowMonitorThreshold(QString cmd,QString arguments)
{
    qDebug()<<"***************************** setEnterFlowMonitorThreshold**********";
    //setEnterFlow,no=%1,flow=%2,percent=%3.
    quint8 dataBody[5];
    QStringList argList=arguments.split(",");
    for(qint32 i=0;i<argList.size();i++)
    {
        QStringList objValList=argList.at(i).split("=");
        if(objValList.size()!=2)
        {
            //error key=value.
            continue;
        }
        if(objValList.at(0)=="no")
        {
            quint8 chillerNo=objValList.at(1).toInt();
            dataBody[4]=chillerNo-1;
        }else if(objValList.at(0)=="flow")
        {
            float fTemp=objValList.at(1).toFloat();
            quint16 nZheng=(quint16)fTemp;
            quint8 nXiao=(quint8)((fTemp-nZheng)*10.0f);

            dataBody[0]=(quint8)((nZheng&0xFF00)>>8);
            dataBody[1]=(quint8)((nZheng&0x00FF)>>0);
            dataBody[2]=(quint8)(nXiao);
        }else if(objValList.at(0)=="percent")
        {
            float fPercent=objValList.at(1).toFloat();
            quint8 nPercent=(quint8)fPercent;
            dataBody[3]=nPercent;
        }
    }
    this->ZBuildPkt(CMD_SET_CHILLER_ENTERFLOW_MONITORTHRESHOLD,dataBody,sizeof(dataBody));
    return 0;
}
qint32 ZLitronicsCI::ZParseUICmd_SetRTC(QString cmd,QString arguments)
{
    QDateTime dtRTC=QDateTime::fromString(arguments,"yyyy-MM-dd hh:mm:ss");
    quint8 dataBody[6];
    dataBody[0]=dtRTC.date().year()-2000;
    dataBody[1]=dtRTC.date().month();
    dataBody[2]=dtRTC.date().day();
    dataBody[3]=dtRTC.time().hour();
    dataBody[4]=dtRTC.time().minute();
    dataBody[5]=dtRTC.time().second();

    this->ZBuildPkt(CMD_SET_RTC_TIME,dataBody,sizeof(dataBody));
    return 0;
}
//get PLaser RTC.
qint32 ZLitronicsCI::ZParseUICmd_GetRTC(QString cmd,QString arguments)
{
    this->ZBuildPkt(CMD_GET_RTC_TIME,NULL,0);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdHomePageData(quint8 *dataBuf,qint32 dataLen)
{
    QString cmd;
    QString arguments;
    if(8!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("获取首页数据返回不正确!"));
        return -1;
    }else{
        quint16 maxPwrPercent=(quint16)dataBuf[0]<<8|dataBuf[1];
        quint16 pLaserTemperature=(quint16)dataBuf[2]<<8|dataBuf[3];
        quint16 pLaserHumudity=(quint16)dataBuf[4]<<8|dataBuf[5];
        qint8 pLaserHTAlarm=dataBuf[6];
        quint8 nRGBPwr=dataBuf[7];
        //GetHomePageData,MPP=23.9,H=24.5,T=29.2,A=2,RGB=23;
        cmd="GetHomePageData";
        arguments="MPP="+QString::number(maxPwrPercent,10)+",";//max power percent.
        arguments+="H="+QString::number(pLaserHumudity,10)+",";//humidity.
        arguments+="T="+QString::number(pLaserTemperature,10)+",";//temperature.
        arguments+="A="+QString::number(pLaserHTAlarm,10)+",";//alarm.
        arguments+="RGB="+QString::number(nRGBPwr,10)+",";//RGB.
    }
    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdSetChillerEnterFlow(quint8 *dataBuf,qint32 dataLen)
{
    QString cmd;
    QString arguments;
    if(2!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("设置水冷机流量值返回不正确!"));
        return -1;
    }else{
        qint8 nRetCode=dataBuf[0];
        qint8 nChillerNo=dataBuf[1]+1;
        if(!nRetCode)
        {
            emit this->ZSigErrMsg(tr("%1号水冷机回水流量&监控阈值设置成功!").arg(nChillerNo));
        }else{
            emit this->ZSigErrMsg(tr("%1号水冷机回水流量&监控阈值设置失败!").arg(nChillerNo));
        }
    }
}
//set/get PLaser RTC response.
qint32 ZLitronicsCI::ZRxCmdSetRTC(quint8 *dataBuf,qint32 dataLen)
{
    if(6!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("设置RTC时间错误!"));
        return -1;
    }
    QDateTime rtcDT;
    rtcDT.setDate(QDate(2000+dataBuf[0],dataBuf[1],dataBuf[2]));
    rtcDT.setTime(QTime(dataBuf[3],dataBuf[4],dataBuf[5]));

    QString cmd("setRTC");
    QString arguments=rtcDT.toString("yyyy-MM-dd hh:mm:ss");
    //setRTC,2017-09-12 12:00:23.

    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
qint32 ZLitronicsCI::ZRxCmdGetRTC(quint8 *dataBuf,qint32 dataLen)
{
    if(6!=dataLen)
    {
        //something wrong.
        emit this->ZSigErrMsg(tr("获取RTC时间错误!"));
        return -1;
    }
    QDateTime rtcDT;
    rtcDT.setDate(QDate(2000+dataBuf[0],dataBuf[1],dataBuf[2]));
    rtcDT.setTime(QTime(dataBuf[3],dataBuf[4],dataBuf[5]));

    QString cmd("getRTC");
    QString arguments=rtcDT.toString("yyyy-MM-dd hh:mm:ss");
    //setRTC,2017-09-12 12:00:23.

    bool bPutRet=false;
    qint32 nTimeout=0;
    do{
        if(this->m_rxRingBuf->ZTryLock())
        {
            if(!this->m_rxRingBuf->ZIsFull())
            {
                this->m_rxRingBuf->ZPutElement(cmd,arguments);
                bPutRet=true;
            }
            this->m_rxRingBuf->ZUnLock();
        }
        QThread::msleep(100);
        if(nTimeout++>=20)
        {
            break;
        }
    }while(!bPutRet);
    return 0;
}
