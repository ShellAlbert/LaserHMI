#ifndef ZLITRONICSCI_H
#define ZLITRONICSCI_H

#include "ci/zbaseci.h"
#include <QThread>
#include <QVector>
#include <ringbuffer/zringbuffer.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#define CMD_SELF_CHECK              0x10 //okay.
#define CMD_GET_FIRMWARE_NO         0x11 //okay.
#define CMD_HEART_BEAT              0x20 //okay.
#define CMD_LASER_CTL_ALL           0x60 //okay.
#define CMD_LASER_CTL_SUB           0x61 //okay.
#define CMD_GET_LASER_FAULT         0x62
#define CMD_GET_CHILLER_FAULT       0x64
#define CMD_SET_LASER_PWR_PERCENT   0x66
#define CMD_GET_LASER_PWR_PERCENT   0x68
#define CMD_GET_LASER_PWR_RANGE     0x6A
#define CMD_GET_DRVBRD_INFO         0x6C
#define CMD_SET_CHILLER_TEMP        0x6E
#define CMD_GET_CHILLER_TEMP        0x6F
#define CMD_GET_LOG_NUM             0x70
#define CMD_GET_LOG_DATA            0x71
#define CMD_CLR_LOG_DATA            0x72

#define CMD_SET_COLOR_TEMP_PERCENT  0x73 //okay.
#define CMD_GET_COLOR_TEMP_PERCENT  0x74 //okay.
#define CMD_SET_COLOR_TEMP          0x7C //okay.
#define CMD_GET_GLASER_DATA         0x7B //okay.

#define CMD_SET_RTC_TIME            0x75
#define CMD_GET_RTC_TIME            0x76

#define CMD_CHILLER_CTL             0x7F//okay.
#define CMD_CHILLER_GET_FAULT       0x64
#define CMD_CHILLER_GET_TEMP        0x6F
#define CMD_CHILLER_GET_FLOW        0x7E

#define CMD_SET_FIBER_BREAK_VOLTAGE 0x67
#define CMD_GET_HOME_PAGE_DATA      0x6B

#define CMD_SET_CHILLER_ENTERFLOW_MONITORTHRESHOLD  0x6D

class HeartBeatRxPkt
{
public:
    bool m_logStorageAlarm;
    bool m_fiberFault;
    bool m_laserFault;
    bool m_chillerFault;
    bool m_whiteBalanceOn;
    bool m_bypassChiller;
    bool m_powerOn;
    qint16 m_counter;
};
#define CARE_BIT_RED        (0x1<<0)
#define CARE_BIT_GREEN      (0x1<<1)
#define CARE_BIT_BLUE       (0x1<<2)
#define CARE_BIT_WB         (0x1<<3)
class LaserCtlAllPkt
{
public:
    quint8 m_devId;
    quint8 m_careBits;
    quint8 m_OpCmd;//1:open,0:close.
    bool m_redCS;
    bool m_greenCS;
    bool m_blueCS;
    bool m_whiteBalanceCS;
};
class LaserFaultPkt
{
public:
    qint8 m_moduleNo;
    bool m_bFiberFault;
    bool m_bRedOC;//over current.
    bool m_bRedOV;//over voltage.
    bool m_bRedOT;//over temperature.

    bool m_bGreenOC;//over current.
    bool m_bGreenOV;//over voltage.
    bool m_bGreenOT;//over temperature.

    bool m_bBlueOC;//over current.
    bool m_bBlueOV;//over voltage.
    bool m_bBlueOT;//over temperature.
};
class ChillerFaultPkt
{
public:
    bool m_fanFault;
    bool m_motorFault;
    bool m_temperatureFault;
    bool m_waterLevelFault;
    bool m_compressorFault;
    bool m_selfProtectOn;
    bool m_bypassChiller;
};
class LaserPowerPercentPkt
{
public:
    qint8 m_mdlId;//0xFF:all modules.0x0~0x7: each GLaser.
    qint8 m_RGBChannel;//0x0:RGB,0x1:R,0x2:G,0x3:B.
    qint16 m_percent;
};
class HeartBeatPkt
{
public:
    bool m_bWhiteBalanceOn;
    bool m_bBypassChiller;
    bool m_bPwrOn;
    qint32 m_nHBNum;
};
class ZLitronicsCI : public QThread
{
    Q_OBJECT
public:
    ZLitronicsCI(ZRingBuffer *txBuf,ZRingBuffer *rxBuf);
    ~ZLitronicsCI();

    qint32 ZPacketCmdToBuffer(char *buffer,const qint32 bufSize);
    qint32 ZParseCmdFromBuffer(QVector<quint8> &result);

    QString ZCvtCmd2Hex(char *buffer,const qint32 bufSize);
signals:
    void ZSigErrMsg(QString msg);
    void ZSigTxRxDone(qint32 state);
    void ZSigDecodeDone(qint32 state);
    void ZSigDevLinkDone(qint32 state);
signals:
    void ZSigDumpDbgMsg(QString msg);
public:
    qint32 ZRxCmdTimeout(QString cmd,QString arguments);
    qint32 ZRxCmdError(QString cmd,QString arguments);
    //Heart Beat.
    qint32 ZTxCmdHeartBeat(HeartBeatPkt &pkt);
    qint32 ZRxCmdHeartBeat(quint8 *dataBuf,quint32 dataLen);
    //Self Check.
    qint32 ZTxCmdSelfCheck(quint16 randomNum);
    qint32 ZRxCmdSelfCheck(quint8 *dataBuf,quint32 dataLen);
    //Laser Control.
    qint32 ZTxCmdLaserCtl(LaserCtlAllPkt &pkt);
    qint32 ZRxCmdLaserCtl(quint8 *dataBuf,qint32 dataLen);
    //Set Laser Power Percent.
    qint32 ZTxCmdSetLaserPwrPercent(LaserPowerPercentPkt &pkt);
    qint32 ZRxCmdSetLaserPwrPercent(quint8 *dataBuf,qint32 dataLen);
    //Set ColorTemp.
    qint32 ZTxCmdSetColorTemp(quint8 mdlNo,quint8 colorTempNo);
    qint32 ZRxCmdSetColorTemp(quint8 *dataBuf,qint32 dataLen);

    //set colortemp percent.
    qint32 ZRxCmdGetColorTempPercent(quint8 *dataBuf,qint32 dataLen);
    ////////////////////////////////////////////////////////

    //Get Firmware Product No&Version No.
    qint32 ZTxCmdGetFirmwareNo(void);

    //Get Laser Fault.
    qint32 ZTxCmdGetLaserFault(qint8 moduleNo);
    qint32 ZRxCmdGetLaserFault(LaserFaultPkt &pkt);
    //Get Chiller Fault.
    qint32 ZTxCmdGetChillerFault(void);
    qint32 ZRxCmdGetChillerFault(ChillerFaultPkt &pkt);

    //Get Laser Power Percent.
    qint32 ZTxCmdGetLaserPwrPercent(LaserPowerPercentPkt &pkt);
    qint32 ZRxCmdGetLaserPwrPercent(LaserPowerPercentPkt &pkt);
    //Get Laser Power Range.
    qint32 ZTxCmdGetLaserPwrRange();
    qint32 ZRxCmdGetLaserPwrRange(qint8 *minVal,qint8 *maxVal,qint8 *stepVal);


    //parse out getGLaserModuleData.
    qint32 ZRxCmdGetGLaserData(quint8 *dataBuf,qint32 dataLen);

    //parse out chillerctl.
    qint32 ZRxCmdChillerCtl(quint8 *dataBuf,qint32 dataLen);
    qint32 ZRxCmdChillerFault(quint8 *dataBuf,qint32 dataLen);
    qint32 ZRxCmdChillerFlow(quint8 *dataBuf,qint32 dataLen);
    qint32 ZRxCmdChillerTemp(quint8 *dataBuf,qint32 dataLen);

    //parse out fiber break voltage.
    qint32 ZRxCmdFiberBreakVoltage(quint8 *dataBuf,qint32 dataLen);

    //parse out home page data.
    qint32 ZRxCmdHomePageData(quint8 *dataBuf,qint32 dataLen);

    qint32 ZRxCmdSetChillerEnterFlow(quint8 *dataBuf,qint32 dataLen);

    //set/get PLaser RTC response.
    qint32 ZRxCmdSetRTC(quint8 *dataBuf,qint32 dataLen);
    qint32 ZRxCmdGetRTC(quint8 *dataBuf,qint32 dataLen);
protected:
    void run();
private:
    qint32 ZBuildPkt(quint8 cmd,quint8 *data,quint8 len);

    void ZPopupMsgAddGblErrMsgList(QString errMsg);
private:
    void ZCvtUI2Cmd(QString cmd,QString arguments);
    qint32 ZParseUICmd_heartbeat(QString cmd,QString arguments);
    qint32 ZParseUICmd_pwr(QString cmd,QString arguments);
    qint32 ZParseUICmd_tuning(QString cmd,QString arguments);
    qint32 ZParseUICmd_getGLaserData(QString cmd,QString arguments);
    qint32 ZParseUICmd_colorTemp(QString cmd,QString arguments);
    qint32 ZParseUICmd_setColorTempPercent(QString cmd,QString arguments);
    qint32 ZParseUICmd_getColorTempPercent(QString cmd,QString arguments);
    //chiller ctrl.
    qint32 ZParseUICmd_ChillerCtl(QString cmd,QString arguments);
    //get chiller fault.
    qint32 ZParseUICmd_ChillerFault(QString cmd,QString arguments);
    qint32 ZParseUICmd_ChillerFlow(QString cmd,QString arguments);
    qint32 ZParseUICmd_ChillerTemp(QString cmd,QString arguments);
    //set fiber break voltage.
    qint32 ZParseUICmd_FiberBreakVoltage(QString cmd,QString arguments);
    //get all home page data.
    qint32 ZParseUICmd_GetHomePageData(QString cmd,QString arguments);

    //save temp to chiller.
    qint32 ZParseUICmd_SaveTemp2Chiller(QString cmd,QString arguments);

    //set chiller enter flow & monitor threshold percentage.
    qint32 ZParseUICmd_SetEnterFlowMonitorThreshold(QString cmd,QString arguments);

    //set PLaser RTC.
    qint32 ZParseUICmd_SetRTC(QString cmd,QString arguments);
    //get PLaser RTC.
    qint32 ZParseUICmd_GetRTC(QString cmd,QString arguments);

    //tx and wait rx.
    qint32 ZTxThenRx(quint8 *dataBuf,quint32 dataLen);
private:
    QByteArray *m_txBuffer;
    QByteArray *m_rxBuffer;
    ZRingBuffer *m_txRingBuf;
    ZRingBuffer *m_rxRingBuf;

    quint8 *m_plainBuffer;
    //////////////////////////////
    QSerialPort *m_uart;
    QString m_portName;
    bool m_exitFlag;

    //////////////////////////////////
    bool m_bSelfCheckPassed;

    //device link up or down.
    //here i remember the last timestamp when receive data from device.
    //when current timestamp plus the last timestamp greater than 10s.
    //then the device is link down.
    qint64 m_lastRxTimeStamp;
    qint32 m_devLinkDownTipCnt;
};

#endif // ZLITRONICSCI_H
