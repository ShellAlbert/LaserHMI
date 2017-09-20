#ifndef ZPWRPRECISIONSETDIALOG_H
#define ZPWRPRECISIONSETDIALOG_H

#include <QDialog>
#include <QToolButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFrame>
#include <ui/zcheckbox.h>
#include <ui/ztextlineeditlabel.h>
class ZPwrPrecisionSetDialog : public QDialog
{
    Q_OBJECT
public:
    ZPwrPrecisionSetDialog(QString title);
    ~ZPwrPrecisionSetDialog();

    float ZGetPrecisionPwrValue();
signals:
    void ZSigPopupMsg(QString msg);
private slots:
    void ZSlotBtnPressed();
private:
    QLabel *m_llTitle;
    QLineEdit *m_le;
    QToolButton *m_tb[14];
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
};
class ZChillerSetDialog:public QDialog
{
    Q_OBJECT
public:
    ZChillerSetDialog(QString title);
    ~ZChillerSetDialog();

    void ZSetValue(QString tempValue,bool bEnJieLu);
    float ZGetValue(void);
    bool ZIsEnJieLu(void);

signals:
    void ZSigPopupMsg(QString msg);
private slots:
    void ZSlotBtnPressed();
    void ZSlotSetChillerTemp();
private:
    QLabel *m_llTitle;
    ZCheckBox *m_cbEnFangJieLu;
    ZTextLineEditLabel *m_leChillerTemp;
    QToolButton *m_tbOkay;
    QToolButton *m_tbCancel;
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
};
#endif // ZPWRPRECISIONSETDIALOG_H

class ZChillerSetEnterFlowDialog:public QDialog
{
    Q_OBJECT
public:
    ZChillerSetEnterFlowDialog(QString title);
    ~ZChillerSetEnterFlowDialog();

    QString ZGetFlowValue();
    QString ZGetMonitorPercent();
signals:
    void ZSigPopupMsg(QString msg);
private slots:
    void ZSlotBtnPressed();
    void ZSlotPopupKeyboard();
private:
    QLabel *m_llTitle;
    ZTextLineEditLabel *m_leFlow;
    ZTextLineEditLabel *m_lePercent;
    QToolButton *m_tbOkay;
    QToolButton *m_tbCancel;
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
};
