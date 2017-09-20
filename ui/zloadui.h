#ifndef ZLOADUI_H
#define ZLOADUI_H

#include "base/zbaseui.h"
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
class ZLoadUI : public ZBaseUI
{
    Q_OBJECT
public:
    ZLoadUI();
    ~ZLoadUI();

    void ZSetMsg(QString msg);
    void ZSetProgress(qint32 val);
public slots:
    void ZSloShowMsg(QString msg);
private:
    QLabel *m_llMsg;
    QProgressBar *m_progressBar;
    QVBoxLayout *m_vLayout;
};

#endif // ZLOADUI_H
