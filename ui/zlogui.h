#ifndef ZLOGUI_H
#define ZLOGUI_H

#include "base/zbaseui.h"
#include "ui/log/zoperatelogui.h"
#include "ui/log/zlaserlogui.h"
#include "ui/log/zchillerlogui.h"
#include <QTabWidget>
class ZLogUI : public ZBaseUI
{
    Q_OBJECT
public:
    ZLogUI();
    ~ZLogUI();

    void ZDoInitial();
    void ZUnInitial();
    void ZFetchLogs();
signals:
    void ZSigDumpDbgMsg(QString msg);
private:
    ZOperateLogUI *m_operateLog;
    ZLaserLogUI *m_laserLog;
    ZChillerLogUI *m_chillerLog;
    QTabWidget *m_tabWidget;
    QVBoxLayout *m_vLayout;
};

#endif // ZLOGUI_H
