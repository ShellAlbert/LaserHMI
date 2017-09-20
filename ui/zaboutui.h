#ifndef ZABOUTUI_H
#define ZABOUTUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include "base/zbaseui.h"
#include "ui/zframe.h"
#include <QDialog>

class ZAboutUI:public ZBaseUI
{
    Q_OBJECT
public:
    ZAboutUI();
    ~ZAboutUI();

    void ZDoInitial();
    void ZUnInitial();
signals:
    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSloUpdateHMI();
    void ZSloUpdatePLaser();
    void ZSloTimeout();
private:
    QLabel *m_llManufacturer;
    QLabel *m_llTechSupport;

    QLabel *m_llUIVerNo;
    QLabel *m_llPublishDate;
    QToolButton *m_tbUIUpgrade;

    QLabel *m_llFirmwareNo;
    QLabel *m_llProductNo;
    QToolButton *m_tbFirmwareUpgrade;

    QGridLayout *m_gLayoutInfo;
    ZFrame *m_frmInfo;

    QLabel *m_llLogo;
    QVBoxLayout *m_frmVLayout;

    //this page update timer.
    QTimer *m_timer;
};
class ZQRDialog:public QDialog
{
public:
    ZQRDialog();
    ~ZQRDialog();
private:
    QLabel *m_llQRCode;
    QToolButton *m_tbClose;
    QVBoxLayout *m_vLayout;

};
#endif // ZABOUTUI_H
