#ifndef ZMAINUI_H
#define ZMAINUI_H

#include <QWidget>
#include "ui/ztopbar.h"
#include "ui/zbottombar.h"
#include "ui/znavigateui.h"
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ui/zloginui.h"
#include "ui/zsummaryui.h"
#include "ui/zlaserui.h"
#include "ui/zchillerui.h"
#include "ui/zsyssetupui.h"
#include "ui/zlogui.h"
#include "ui/zdbgtestui.h"
#include "ui/znetworkui.h"
#include "ui/zaboutui.h"
#include "ringbuffer/zringbuffer.h"
#include <QTimer>
class ZMainUI:public QFrame
{
    Q_OBJECT
public:
    ZMainUI(ZRingBuffer *txRingBuffer,ZRingBuffer *rxRingBuffer);
    ~ZMainUI();

    void ZDoInitial();
    void ZUnInitial();
private slots:
    void ZSloLoginSuccess();
    void ZSloModuleClicked(QString moduleName);
    void ZSloGetRxRingBuffer();
    void ZSloReturn();

    void ZSlotRequestCmd(QString cmd,QString arguments);
    void ZSloShowPopupMsg(QString msg);
signals:
    void ZSigDumpDbgMsg(QString msg);
    //////////////////
    void ZSigLoadMsg(QString msg);
    void ZSigLoadFinish();

    void ZSigTxRxDone(qint32 state);
    void ZSigDecodeDone(qint32 state);
    void ZSigDevLinkDone(qint32 state);

    ////////////////////////////////////////
    void ZSigResponseCmdHB2TopBar(QString arguments);
    /////////////////////////////
    void ZSigResponseCmdPwr2MainCtl(QString arguments);
    void ZSigResponseCmdTuning2MainCtl(QString arguments);
    void ZSigResponseCmdColorTemp2MainCtl(QString arguments);
    void ZSigResponseCmdColorTemp2MdlCtl(QString cmd,QString arguments);


    void ZSigResponseCmdAdjRGBPercent2MainCtl(QString cmd,QString arguments);
    void ZSigResponseCmdAdjRGBPercent2MdlCtl(QString cmd,QString arguments);
    ///////////////////////////////////////
    void ZSigResponseCmdPwr2MdlCtl(QString cmd,QString arguments);
    void ZSigResponseCmdTuning2MdlCtl(QString cmd,QString arguments);
    ////////////////////////////////////////////////////////////
    void ZSigResponseCmdGetGLaserData2MdlCtl(QString cmd,QString arguments);

    //GetColorTempPercent.
    void ZSigResponseCmdGetColorTempPercent2MainCtl(QString cmd,QString arguments);
    void ZSigResponseCmdGetColorTempPercent2MdlCtl(QString cmd,QString arguments);

    //chiller ctl.
    void ZSigResponseCmdChillerCtl(QString cmd,QString arguments);
    void ZSigResponseCmdChillerFault(QString cmd,QString arguments);
    void ZSigResponseCmdChillerFlow(QString cmd,QString arguments);
    void ZSigResponseCmdChillerTemp(QString cmd,QString arguments);

    //set fiber break voltage thresold.
    void ZSigResponseCmdFBVthreshold(QString cmd,QString arguments);

    //get home page data.
    void ZSigResponseCmdGetHomePageData(QString cmd,QString arguments);

    //PLaser RTC data.
    void ZSigResponseCmdRTC(QString cmd,QString arguments);
private:
    ZTopBar *m_topBar;
    ZBottomBar *m_bottomBar;
    QStackedWidget *m_stackedWidget1;
    ZLoginUI *m_loginUI;

    ZBaseUI *m_baseUI;
    ZNavigateUI *m_navigate;
    QStackedWidget *m_stackedWidget;
    ZSummaryUI *m_summaryUI;
    ZLaserUI *m_laserUI;
    ZChillerManager *m_chillerUI;
    ZSysSetupUI *m_sysSetupUI;
    ZLogUI *m_logUI;
    ZDbgTestUI *m_dbgTestUI;
    ZNetworkUI *m_networkUI;
    ZAboutUI *m_aboutUI;

    QVBoxLayout *m_vLayout;
    QHBoxLayout *m_hLayout;

    QTimer *m_tmGetRxRingBuffer;

private:
    ZRingBuffer *m_txRingBuffer;
    ZRingBuffer *m_rxRingBuffer;
};

#endif // ZMAINUI_H
