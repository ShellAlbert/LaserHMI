#ifndef ZSYSSETUPUI_H
#define ZSYSSETUPUI_H

#include "base/zbaseui.h"
#include "base/zpiechart.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QToolButton>
#include <QFrame>
#include <QTimer>
class ZSysSetupUI : public ZBaseUI
{
    Q_OBJECT
public:
    ZSysSetupUI();
    ~ZSysSetupUI();

    void ZDoInitial();
    void ZUnInitial();
protected:
    bool event(QEvent *e);
signals:
    void ZSigDumpDbgMsg(QString msg);
    void ZSigPopupMsg(QString msg);
signals:
    void ZSigRequestCmd(QString cmd,QString arguments);
public slots:
    void ZSloResponseRTCCmd(QString cmd,QString arguments);
private slots:
    void ZSloShowKeyboard();
    void ZSloChangePassword();
    void ZSloRestoreFactory();
    void ZSloTimeout();
private:
    ZLabelLineEditCheckBox *m_llcCinemaName;
    ZLabelLineEditCheckBox *m_llcDevNo;
    ZLabelLineEditCheckBox *m_llcDateTime;
    ZLabelLineEditCheckBox *m_llcPLaserDateTime;
    QToolButton *m_tbChangePassword;
    QToolButton *m_tbRestoreFactory;
    QHBoxLayout *m_hLayoutBtn;

    ZPieChart *m_diskUsageChart;
    ZPieChart *m_logUsageChart;
    QHBoxLayout *m_hLayoutChart;

    QGridLayout *m_gLayout;
    QVBoxLayout *m_vLayout;

    QTimer *m_timer1s;
    //fixup for QWindow::isVisible()const.
    //??????
    //QApplicationPrivate::notify_helper(QObject*,QEvent*);
    //QApplication::notify(QObject*,QEvent*)
    //g_main_context_dispatch.
    bool m_bProcessEvent;
};
class ZChangePasswordDia:public QDialog
{
    Q_OBJECT
public:
    ZChangePasswordDia(QString title);
    ~ZChangePasswordDia();
    QString ZGetNewPassword();
signals:
    void ZSigPopupMsg(QString msg);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void ZSloOkay();
    void ZSloCancel();
private:
    QLabel *m_llTitle;
    QLabel *m_llOldPass;
    QLabel *m_llNewPass1;
    QLabel *m_llNewPass2;
    QLineEdit *m_leOldPass;
    QLineEdit *m_leNewPass1;
    QLineEdit *m_leNewPass2;
    QToolButton *m_tbOkay;
    QToolButton *m_tbCancel;
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
};
class ZCountThread:public QThread
{
    Q_OBJECT
public:
    ZCountThread(qint32 sec);
signals:
    void ZSigThreadDone(qint32 ret,QString errMsg);
protected:
    void run();
private:
    qint32 m_sec;
};
class ZRestorFactoryDia:public QDialog
{
    Q_OBJECT
public:
    ZRestorFactoryDia(QString title);
    ~ZRestorFactoryDia();
signals:
    void ZSigPopupMsg(QString msg);
private slots:
    void ZSloOkay();
    void ZSloCancel();
private:
    QLabel *m_llTitle;
    QLabel *m_llWarning;
    ZLabelLineEditCheckBox *m_lecPassword;
    QToolButton *m_tbOkay;
    QToolButton *m_tbCancel;
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
};
#endif // ZSYSSETUPUI_H
