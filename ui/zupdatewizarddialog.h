#ifndef ZUPDATEWIZARDDIALOG_H
#define ZUPDATEWIZARDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QToolButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QThread>
#include <QStackedWidget>
#include <QListWidget>
class ZUpdateThread:public QThread
{
    Q_OBJECT
public:
    ZUpdateThread();
    ~ZUpdateThread();
    void ZSetUDiskNodeName(QString udiskNodeName);
signals:
    void ZSigUpdateFinished(qint32 ret,QString msg);
    void ZSigUpdateProgress(qint32 progress);
protected:
    void run();
private:
    QString ZGetFileMd5(QString fileName);
private:
    QString m_udiskNode;
};
class ZUpdateWizardDialog:public QDialog
{
    Q_OBJECT
public:
    ZUpdateWizardDialog();
    ~ZUpdateWizardDialog();
signals:
    void ZSigUpdateFinished(qint32 ret,QString msg);
private slots:
    void ZSlotUpdateFinished(qint32 ret,QString msg);
    void ZSlotUpdateProgress(qint32 progress);
    void ZSlotStart();
    void ZSlotStop();
private:
    QLabel *m_llUpdateNote;
    QLabel *m_llUpdateStep;
    QLabel *m_llUpdateWarning;
    QLabel *m_llCurrent;
    QProgressBar *m_progressBar;
    QToolButton *m_tbStart;
    QToolButton *m_tbStop;
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
    ZUpdateThread *m_thread;
};

class ZUDiskSelectDia:public QDialog
{
    Q_OBJECT
public:
    ZUDiskSelectDia(QString title,QStringList udiskNodeList);
    ~ZUDiskSelectDia();
    QString ZGetUDiskName();
private slots:
    void ZSlotSelectUDisk();
private:
    QLabel *m_llTitle;
    QListWidget *m_listWidet;
    QVBoxLayout *m_vLayoutFrm;
    QFrame *m_fram;
    QVBoxLayout *m_vLayout;
};
#endif // ZUPDATEWIZARDDIALOG_H
