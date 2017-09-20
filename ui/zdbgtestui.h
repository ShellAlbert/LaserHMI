#ifndef ZDBGTESTUI_H
#define ZDBGTESTUI_H

#include "base/zbaseui.h"
#include "ui/zcheckbox.h"
#include <QGridLayout>
#include "ui/zspinbox.h"
class ZDbgTestUI : public ZBaseUI
{
    Q_OBJECT
public:
    ZDbgTestUI();
    ~ZDbgTestUI();

    void ZDoInitial();
    void ZUnInitial();
signals:
    void ZSigMsg(QString msg);
signals:
    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSloCheckBoxStateChanged(qint32 state);
    void ZSloHBIntervalValChanged(qint32 val);
    void ZSloTimeoutValChanged(qint32 val);
    void ZSloQueryIntervalValChanged(qint32 val);
    void ZSloOfflineTimeoutValChanged(qint32 val);
private:
    ZCheckBox *m_cbEnSelfCheck;
    ZCheckBox *m_cbEnHeartBeat;
    ZCheckBox *m_cbPopupErrMsg;
    ZCheckBox *m_cbEnCRC16;
    ZCheckBox *m_cbEnEncrypt;
    ZCheckBox *m_cbOpTips;
    ZCheckBox *m_cbQueryStatus;
    ZCheckBox *m_cbBeep;
    ZSpinBox *m_sbHBInterval;
    ZSpinBox *m_sbTimeoutVal;
    ZSpinBox *m_sbQueryInterval;
    ZSpinBox *m_sbOfflineTimeout;
    ZSpinBox *m_sbGLaserCount;//dynamic GLaser Count.
    QGridLayout *m_gLayout;
    QVBoxLayout *m_vLayoutMain;
};
#endif // ZDBGTESTUI_H
