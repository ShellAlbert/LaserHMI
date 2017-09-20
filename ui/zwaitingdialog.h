#ifndef ZWAITINGDIALOG_H
#define ZWAITINGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QToolButton>
class ZWaitingDialog : public QDialog
{
    Q_OBJECT
public:
    ZWaitingDialog(QString title);
    ~ZWaitingDialog();
public slots:
    void ZSloClose(qint32 retCode,QString errMsg);
private:
    QLabel *m_llTitle;
    QToolButton *m_tbOkay;
    QVBoxLayout *m_vLayout;
};

#endif // ZWAITINGDIALOG_H
