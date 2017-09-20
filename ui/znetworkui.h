#ifndef ZNETWORKUI_H
#define ZNETWORKUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include "base/zbaseui.h"
#include <QTextEdit>
#include <QThread>
class ZDHCPThread:public QThread
{
    Q_OBJECT
public:
    ZDHCPThread();
signals:
    void ZSigThreadDone(qint32 ret,QString errMsg);
protected:
    void run();
};
class ZNetworkUI:public ZBaseUI
{
    Q_OBJECT
public:
    ZNetworkUI();
    ~ZNetworkUI();

    void ZDoInitial();
    void ZUnInitial();
private slots:
    void ZSloDHCP();
    void ZSloRefresh();
    void ZSloSetup();
private:
    ZLabelLineEditCheckBox *m_mac;
    ZLabelLineEditCheckBox *m_ip;
    ZLabelLineEditCheckBox *m_mask;
    ZLabelLineEditCheckBox *m_broad;
    ZLabelLineEditCheckBox *m_gw;
    ZLabelLineEditCheckBox *m_dns;
    QGridLayout *m_gLayout;
    QToolButton *m_tbDHCP;
    QToolButton *m_tbRefresh;
    QToolButton *m_tbSetup;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
};
#endif // ZNETWORKUI_H
