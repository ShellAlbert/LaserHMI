#ifndef ZLASERUI_H
#define ZLASERUI_H

#include "base/zbaseui.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "ui/zmainctlui.h"
#include "ui/zmdlctlui.h"
#include <QTimer>
class ZLaserUI:public ZBaseUI
{
    Q_OBJECT
public:
    ZLaserUI();
    ~ZLaserUI();

    void ZDoInitial();
    void ZUnInitial();
signals:
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigPopupMsg(QString msg);
signals:
    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSlotTimeout();
    void ZSloTabChanged(qint32 index);
private:
    QTabWidget *m_tabWidget;
    QVBoxLayout *m_vLayout;
public:
    ZMainCtlUI *m_mainCtlUI;
    ZMdlCtlUI *m_mdlCtlUI[8];

    QTimer *m_timer;
    bool m_bFaultIconFlag;
};

#endif // ZLASERUI_H
