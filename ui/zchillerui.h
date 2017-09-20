#ifndef ZCHILLERUI_H
#define ZCHILLERUI_H

#include "base/zbaseui.h"
#include "ui/ztextimglabel.h"
#include <QToolButton>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QSlider>
#include <QGridLayout>
#include "ui/zframe.h"
#include "ui/ztextlineeditlabel.h"
#include "ui/zvslider.h"
#include <QTabWidget>
#include <QDialog>
#include <ui/zcircleindicator.h>
#include <ui/zcheckbox.h>
class ZChillerUI : public ZBaseUI
{
    Q_OBJECT
public:
    ZChillerUI(quint8 chillerNo);
    ~ZChillerUI();

    void ZDoInitial();
    void ZUnInitial();
protected:
    bool event(QEvent *e);
public slots:
    void ZSloChillerCmdResponse(QString cmd,QString arguments);
    void ZSloOtherPwrOn(bool powerOn);
private slots:
    void ZSloTimeout1s();
    void ZSloBypassChiller();
    void ZSloPowerOnChiller();
    void ZSloShowSetChillerTempDialog();

    void ZSloTabChanged(qint32 index);
    void ZSloSetEnterFlowParameters();
signals:
    void ZSigDumpDbgMsg(QString msg);
    void ZSigPopMessage(QString msg);
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigOtherPwrOn(bool powerOn);
private:
    ZCircleIndicator *m_vsEnterWater;
    ZCircleIndicator *m_vsLeaveWater;
    ZCircleIndicator *m_vsAmbientTemp;

    QGridLayout *m_gLayoutTemp;
    ZFrame *m_frmTemp;

    //flow speed.
    ZTextLineEditLabel *m_leEnterSpeed;
    ZTextLineEditLabel *m_leLeaveSpeed;
    QGridLayout *m_gLayoutSpeed;
    QFrame *m_frmSpeed;

    //control.
    QToolButton *m_tbBypass;
    QToolButton *m_tbPwrCtl;
    QToolButton *m_tbSetup;
    QGridLayout *m_gLayoutControl;
    QFrame *m_frmControl;

    //setup.
    QFrame *m_frmSetup;
    ZTextLineEditLabel *m_leChillerTemp;
    ZCheckBox *m_cbEnFangJieLu;
    QToolButton *m_tbSaveChiller;
    QGridLayout *m_gLayoutSetup;

    //alarm&fault.
    QFrame *m_frmFault;
    ZTextImgLabel *m_faultLED[16];
    QGridLayout *m_gLayoutFault;

    //tab widget.
    QFrame *m_frmTab;
    QTabWidget *m_tabWidget;
    QVBoxLayout *m_vLayoutTab;

    ZTextImgLabel *m_llCommunicate;
    ZTextImgLabel *m_llWaterLevel;
    ZTextImgLabel *m_llSelfProtect;
    ZTextImgLabel *m_llOthers;
    QGridLayout *m_gLayoutStatus;
    ZFrame *m_frmStatus;

    QGridLayout *m_gLayoutMain;

    bool m_bChillerPwrOn;
    bool m_bChillerBypass;

    quint8 m_chillerNo;


    //update timer.
    QTimer *m_timer1s;
    qint32 m_nTime1sCounter;
    bool m_bCmdRunning;

    //2 chillers mutex.
    bool m_bEnableMe;

    //fixup for QWindow::isVisible().
    bool m_bProcessEvent;
};
class ZChillerManager:public ZBaseUI
{
    Q_OBJECT
public:
    ZChillerManager();
    ~ZChillerManager();
    void ZDoInitial();
    void ZUnInitial();
signals:
    void ZSigDumpDbgMsg(QString dbgMsg);
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigResponseCmdChillerCtl(QString cmd,QString arguments);
    void ZSigResponseCmdChillerFault(QString cmd,QString arguments);
    void ZSigResponseCmdChillerFlow(QString cmd,QString arguments);
    void ZSigResponseCmdChillerTemp(QString cmd,QString arguments);

    void ZSigPopMessage(QString msg);
private slots:
    void ZSloTabChanged(qint32 index);
private:
    QTabWidget *m_tabWidget;
    ZChillerUI *m_chiller[2];
    QVBoxLayout *m_vLayout;
};
#endif // ZCHILLERUI_H
