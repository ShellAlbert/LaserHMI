#ifndef ZMAINCTLUI_H
#define ZMAINCTLUI_H

#include "base/zbaseui.h"
#include "ui/zframe.h"
#include "ui/zlinelabel.h"
#include "ui/ztextimglabel.h"
#include "ui/ztextlineeditlabel.h"
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QToolButton>
#include <ui/zprogressbar.h>
#include <ui/zverticalindicator.h>
#include <QDialog>

class ZMainCtlUI:public ZBaseUI
{
    Q_OBJECT
public:
    ZMainCtlUI();
    ~ZMainCtlUI();

    void ZDoInitial();
    void ZUnInitial();
protected:
    bool event(QEvent *e);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigPopupMsg(QString msg);
    void ZSigLongPressed(QString colorTemp);
    void ZSigAllPwrPrecisionTuning();

    void ZSigAdjRGBPercent(QString cmd,QString arguments);

    void ZSigDumpDbgMsg(QString msg);
public slots:
    void ZSloSetColorTemp(QString colorTemp);
    void ZSloGetStatus(bool redOn,bool greenOn,bool blueOn,bool WhiteBalanceOn,QString colorTemp);

    //////////////////////////////////
    void ZSloResponseCmdPwr(QString arguments);
    void ZSloResponseCmdTuning(QString arguments);
    void ZSloResponseCmdColorTemp(QString arguments);
    void ZSloResponseCmdFBVThreshold(QString cmd,QString arguments);
    void ZSloResponseCmdGetHomePageData(QString cmd,QString arguments);
signals:
    void ZSigPwrPercentTuning(QString channel,bool increase,qint32 val);
    void ZSigOnCtl(QString what,bool on);
    void ZSigSetColorTemp(QString colorTemp);
    void ZSigGetStatus();

    //getColorTempPercent.//used to update colorTempSetDialog.
    void ZSigResponseCmdGetColorTempPercent2MainCtl(QString cmd,QString arguments);
private slots:
    void ZSloTimeout1s();
    void ZSloSetColorTempInternal();
    void ZSloOnCtlInternal();
    void ZSloPwrPercentTuningInternal();

    void ZSloLongPressed(QString colorTemp);
    void ZSloAllPwrPrecisionTuning();
private:
    void ZUptPwrCtlBtnText(QString what,bool on);
    void ZFixPwrCtlBtnText(QString what);

    void ZPopupMsgAddGblErrMsgList(QString errMsg);
private:
    ZTextImgLabel *m_llRedLED;
    ZTextImgLabel *m_llGreenLED;
    ZTextImgLabel *m_llBlueLED;
    ZTextImgLabel *m_llWhiteBalanceLED;
    ZTextLineEditLabel *m_llColorTemp;

    ZTextImgLabel *m_llPLaserAlarm1LED;
    ZTextImgLabel *m_llPLaserAlarm2LED;
    ZTextLineEditLabel *m_llPLaserH;
    ZTextLineEditLabel *m_llPLaserT;
    QGridLayout *m_gLayoutStatus;
    ZFrame *m_frmStatus;

    QLabel *m_llColorTempIndicate;
    QToolButton *m_tbD63;
    QToolButton *m_tbD65;
    QToolButton *m_tbDCI;
    QToolButton *m_tbD80;
    QToolButton *m_tbD93;
    QToolButton *m_tbD95;
    QToolButton *m_tbDMore;
    QGridLayout *m_gLayoutColorTemp;
    ZFrame *m_frmColorTemp;

    QToolButton *m_tbAllOn;
    QToolButton *m_tbRedOn;
    QToolButton *m_tbGreenOn;
    QToolButton *m_tbBlueOn;
    QToolButton *m_tbWhiteBalanceOn;
    QGridLayout *m_gLayoutControl;
    ZFrame *m_frmControl;


    QToolButton *m_tbAllDec;
    QToolButton *m_tbAllInc;
    ZVerticalIndicator *m_pbAll;
    QGridLayout *m_gLayoutTuning;
    ZFrame *m_frmTuning;

    QGridLayout *m_gLayoutMain;

    //update timer.
    QTimer *m_timer1s;
    bool m_bCmdRunning;
    qint32 m_nTuningBits;

    //long press button to trigger.
    qint32 m_longPressCnt;
    bool m_bLongPressed;
    QString m_longPressColorTemp;

    //get all modules's data counter.
    qint32 m_getAllMdlDataCount;
};

#endif // ZMAINCTLUI_H
