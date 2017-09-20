#ifndef HMI_CONST_H
#define HMI_CONST_H
#include <QString>
#include <QElapsedTimer>
#include <QVector>
#include "db/zsqlitedb.h"
//#define UI_VERSION  "V0.1"
//#define UI_VERSION  "V0.2" //2017/6/27.
//#define UI_VERSION  "V0.2.0" //2017/6/28.
//#define UI_VERSION  "V0.2.1" //2017/6/30.
//#define UI_VERSION  "V0.2.2" //2017/7/1.
//#define UI_VERSION  "V0.3.0" //2017/7/17.
//#define UI_VERSION  "V0.3.1" //2017/7/29.
//#define UI_VERSION  "V0.3.2" //2017/8/7.
//#define UI_VERSION  "V0.3.3" //2017/8/14.
//#define UI_VERSION  "V0.3.4" //2017/8/15.
//#define UI_VERSION  "V0.3.5R1" //2017/8/18.
//#define UI_VERSION  "V0.3.5R2" //2017/8/26.
#define UI_VERSION  "V0.3.5R3" //2017/8/27.

#define DEVICE_MANUFACTURER  QString("数码辰星科技发展(北京)有限公司")
#define TECH_SUPPORT    QString("辰星研究院")

#define PauseMs(ms)     do { \
    QElapsedTimer t; \
    t.start();\
    while(t.elapsed()<(ms)) \
    QCoreApplication::processEvents();\
    }while(0)

#define SCREEN_WIDTH    800//800
#define SCREEN_HEIGHT   600//600

extern ZSQLiteDB gDB;

//we hold the err message list.
#define ERR_MSG_LIST_SIZE   50

class ZGblHelp
{
public:
    bool m_bEnSelfCheck;
    bool m_bEnHeartBeat;
    bool m_bEnPopupErrMsg;
    bool m_bEnCRC16;
    bool m_bEnEncrypt;
    bool m_bOpTips;
    bool m_bQueryStatus;
    bool m_bBeep;
    qint32 m_nHeartBeatInterval;
    qint32 m_nTimeoutVal;
    qint32 m_nQueryInterval;
    qint32 m_nOfflineTimeout;

    //for HeartBeat.
    bool m_bWhiteBalanceOn;
    bool m_bBypassChiller;
    bool m_bPwrOn;

    //for global error indicator.
    bool m_bGblErr;
//    QStringList m_errMsgList;
    //communicate error flag.
    qint32 m_CommunicateErrFlag;
    //0~7:glaser error flags.
    qint32 m_GLaserErrFlag[8];
    qint32 m_ChillerErrFlag[2];

    //config data.
    QString m_cinemaName;
    QString m_devNo;
    QString m_password;

    //two chiller data.
    qint8 m_chillerPwr;
    qint8 m_chillerFault;
    qint8 m_projectPwrOn;

    //16 laser state for heartbeat packet.
    qint16 m_laserFault;
    //plaser log storage fault.
    qint8 m_logFault;
    //glaser power fault.
    qint16 m_glaserPwrFault;

    //glaser version information.
    QString m_glaserVerNo;
    QString m_glaserProductNo;

    //login or not.
    bool m_bLoginSuccess;
public:
    void beep();
};

extern ZGblHelp g_GblHelp;
extern quint8 ZMapColorTemp2Hex(QString colorTemp);
extern QString ZMapColorTemp2String(quint8 colorTemp);
#endif // HMI_CONST_H
