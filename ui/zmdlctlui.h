#ifndef ZMDLCTLUI_H
#define ZMDLCTLUI_H

#include "base/zbaseui.h"
#include "ui/zframe.h"
#include "ui/ztextimglabel.h"
#include "ui/ztextlineeditlabel.h"
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QTimer>
#include <QDialog>
#include <QTabWidget>
#include <QFrame>
#include <ui/zprogressbar.h>
#include <ui/zverticalindicator.h>
class ZMdlColorTempSetDia:public QDialog
{
    Q_OBJECT
public:
    ZMdlColorTempSetDia(quint8 glaserAddr,float fPowerPercent,QString title);
    ~ZMdlColorTempSetDia();
    QString ZGetColorTemp();
signals:
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigPopupMsg(QString msg);
    void ZSigResponseCmdGetColorTempPercent2MdlCtl(QString cmd,QString arguments);
    void ZSigAdjRGBPercent(QString cmd,QString arguments);

    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSlotTimeout1s();
    void ZSlotShowRGBPercentSetDia();
signals:
    void ZSigShowRGBPercentSetDia();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QLabel *m_llTitle;
    QToolButton *m_tbColorTemp[7];
    QLabel *m_llOpTips;
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;

    QString m_colorTemp;

    QTimer *m_timer;
    qint32 m_longPressCnt;
    bool m_bLongPressed;

    quint8 m_glaserAddr;
    float m_fPowerPercent;
};
class ZMdlFaultDialog:public QDialog
{
    Q_OBJECT
public:
    ZMdlFaultDialog(QString title);
    ~ZMdlFaultDialog();
public slots:
    void ZSloMdlFault(qint32 fiber,qint32 wb,qint32 rc,qint32 rv,qint32 rt,qint32 gc,qint32 gv,qint32 gt,qint32 bc,qint32 bv,qint32 bt);
private:
    QLabel *m_llTitle;
    ZTextImgLabel *m_llFiberLED;
    ZTextImgLabel *m_llWhiteBalanceLED;

    ZTextImgLabel *m_OverIVT[9];
    QHBoxLayout *m_hLayoutOverIVT;

    QToolButton *m_tbClose;
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
};
class ZMdlCtlUI:public ZBaseUI
{
    Q_OBJECT
public:
    ZMdlCtlUI(qint32 moduleNo);
    ~ZMdlCtlUI();

    void ZDoInitial();
    void ZUnInitial();
protected:
    bool event(QEvent *e);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
public slots:
    void ZSloResponseCmdPwr2MdlCtl(QString cmd,QString arguments);
    void ZSloResponseCmdTuning2MdlCtl(QString cmd,QString arguments);
    void ZSloResponseCmdColorTemp2MdlCtl(QString cmd,QString arguments);
    void ZSloResponseCmdGLaserData2MdlCtl(QString cmd,QString arguments);
    void ZSloResponseCmdFBVThreshold(QString cmd,QString arguments);
signals:
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigPopupMsg(QString msg);
    void ZSigMdlFault(qint32 fiber,qint32 wb,qint32 rc,qint32 rv,qint32 rt,qint32 gc,qint32 gv,qint32 gt,qint32 bc,qint32 bv,qint32 bt);
    void ZSigResponseCmdGetColorTempPercent2MdlCtl(QString cmd,QString arguments);

    void ZSigPrecisSetupRequest(QString channel);
    void ZSigAdjRGBPercent(QString cmd,QString arguments);

    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSloTimeout1s();
    void ZSloOnCtlInternal();
    void ZSloPwrPercentTuningInternal();
    void ZSloShowMdlFaultDialog();
    void ZSloShowMdlColorTempDialog();

    void ZSloShowPrecisSetupDialog(QString channel);
    void ZSloMdlFault(qint32 fiber,qint32 wb,qint32 rc,qint32 rv,qint32 rt,qint32 gc,qint32 gv,qint32 gt,qint32 bc,qint32 bv,qint32 bt);

    void ZSlotShowFiberBreakVoltageSetupDialog();

    void ZSloTabChanged(qint32);
private:
    void ZUptPwrCtlBtnText(QString what,bool on);
    void ZFixPwrCtlBtnText(QString what);

    void ZPopupMsgAddGblErrMsgList(QString errMsg);
private:
    //status.
    ZTextLineEditLabel *m_leRedIV;
    ZTextLineEditLabel *m_leGreenIV;
    ZTextLineEditLabel *m_leBlueIV;
    ZTextImgLabel *m_llFaultLED;

    ZTextLineEditLabel *m_leOpticalPower;
    ZTextLineEditLabel *m_leRunTime;
    ZTextLineEditLabel *m_leColorTemp;
    ZTextImgLabel *m_llWBLED;

    QGridLayout *m_gLayoutStatus;
    ZFrame *m_frmStatus;

    //temperature.
    ZTextLineEditLabel *m_leR1Temp;
    ZTextLineEditLabel *m_leR2Temp;
    ZTextLineEditLabel *m_leBTemp;
    ZTextLineEditLabel *m_leG1Temp;
    ZTextLineEditLabel *m_leG2Temp;
    ZTextLineEditLabel *m_leAtTemp;

    QGridLayout *m_gLayoutTemp;
    ZFrame *m_frmTemp;

    //Control.
    QToolButton *m_tbAllOn;
    QToolButton *m_tbRedOn;
    QToolButton *m_tbGreenOn;
    QToolButton *m_tbBlueOn;
    QToolButton *m_tbWhiteBalanceOn;

    //Tuning.
    QToolButton *m_tbAllDec;
    QToolButton *m_tbAllInc;
    ZVerticalIndicator *m_pbAll;

    QToolButton *m_tbRedDec;
    QToolButton *m_tbRedInc;
    ZVerticalIndicator *m_pbRed;

    QToolButton *m_tbGreenDec;
    QToolButton *m_tbGreenInc;
    ZVerticalIndicator *m_pbGreen;

    QToolButton *m_tbBlueDec;
    QToolButton *m_tbBlueInc;
    ZVerticalIndicator *m_pbBlue;

    QTabWidget *m_tabTuning;

    QFrame *m_frmControl;
    QGridLayout *m_gLayoutControl;

    QFrame *m_frmTuningAll;
    QGridLayout *m_gLayoutFrmTuningAll;

    QFrame *m_frmTuningRGB;
    QGridLayout *m_gLayoutFrmTuningRGB;

    QVBoxLayout *m_vLayoutTuning;
    ZFrame *m_frmTuning;


    //fiber.
    ZTextImgLabel *m_llFiberBreakLED;
    ZTextLineEditLabel *m_leFiberBreakThreshold;
    ZTextLineEditLabel *m_leFiberBreakVoltage;
    QGridLayout *m_gLayoutFiber;
    QFrame *m_frmFiber;

    //I&V&T monitor.
    ZTextImgLabel *m_OverIVT[9];
    QGridLayout *m_gLayoutIVT;
    QFrame *m_frmIVT;

    //main layout.
    QGridLayout *m_gLayoutMain;

    qint32 m_moduleNo;

    //update timer.
    QTimer *m_timer1s;
    bool m_bCmdRunning;
    qint32 m_nTuningBits;
    qint32 m_nTime1sCounter;

    //white balance value.
    qint32 m_redWBValue;
    qint32 m_greenWBValue;
    qint32 m_blueWBValue;
    bool m_bHasFault;

    //fixup for QWindow::isVisible()const.
    //??????
    //QApplicationPrivate::notify_helper(QObject*,QEvent*);
    //QApplication::notify(QObject*,QEvent*)
    //g_main_context_dispatch.
    bool m_bProcessEvent;
};

#endif // ZMDLCTLUI_H
