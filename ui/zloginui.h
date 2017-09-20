#ifndef ZLOGINUI_H
#define ZLOGINUI_H

#include "ui/zbarui.h"
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QGridLayout>
class ZLoginUI : public ZBaseUI
{
    Q_OBJECT
public:
    ZLoginUI();
    ~ZLoginUI();

    void ZDoInitial();
    void ZUnInitial();

    void ZResetLogin();
signals:
    void ZSigLoginSuccess();
signals:
    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSloCheckUserInput();
    void ZSloCheckPassword();
private:
    QLabel *m_llImg;
    QLabel *m_llPassTips;
    QLineEdit *m_lePass;
    QToolButton *m_tbInput[12];
    QGridLayout *m_gLayoutTb;
    QVBoxLayout *m_vLayout;
    QHBoxLayout *m_hLayout;
};

#endif // ZLOGINUI_H
