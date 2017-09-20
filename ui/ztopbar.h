#ifndef ZTOPBAR_H
#define ZTOPBAR_H

#include "ui/zbarui.h"
#include <QLabel>
#include <QTimer>
#include <QTextEdit>
#include <QToolButton>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>
class ZGblErrMsgDia:public QDialog
{
  Q_OBJECT
public:
    ZGblErrMsgDia();
    ~ZGblErrMsgDia();
private slots:
    void ZSlotClose();
    void ZSlotClear();
    void ZSlotUpdateErrList();
    void ZSlotShowDetailErrList(QModelIndex index);
private:
    void ZFunCommunicateErrList();
    void ZFunChillerErrList(qint32 index);
    void ZFunGLaserErrList(qint32 index);
    void ZFunStorageErrList();
    void ZFunNetworkErrList();

private:
    QLabel *m_llTitle;
    QListWidget *m_lstItem;
    QListWidget *m_lstMsg;
    QHBoxLayout *m_hLayoutList;

    QToolButton *m_tbClear;
    QToolButton *m_tbClose;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayoutFrm;

    QTimer *m_timer;
};
class ZTopBar : public ZBarUI
{
    Q_OBJECT
public:

    ZTopBar();
    ~ZTopBar();

    void ZDoInitial();
    void ZUnInitial();
signals:
    void ZSigDumpDbgMsg(QString msg);
    void ZSigShowGblErrDialog();
signals:
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigPopupMsg(QString msg);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
public slots:
    void ZSloUptPageName(QString pageName);
    void ZSloResponseCmdHB(QString arguments);
private slots:
    void ZSloUptTick(void);
    void ZSloShowGblErrDialog();
private:
    void ZResetRandomNum();
    void ZPopupMsgAddGblErrMsgList(QString errMsg);
private:
    QLabel *m_llCinemaName;
    QLabel *m_llDeviceNo;
    QLabel *m_llPageName;
    QLabel *m_llSysTime;
    QLabel *m_llHeartBeat;
    QLabel *m_llGblErr;

    QTimer *m_tmTick;
    qint32 m_nHBIndex;
    qint32 m_nHBTick;
    qint32 m_nHBRandNum;

    //global error indicator flash flag.
    qint8 m_nGblErrFlag;

    //fixup for QWindow::isVisible().
    bool m_bProcessEvent;
};

#endif // ZTOPBAR_H
