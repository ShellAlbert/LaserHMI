#ifndef ZBOTTOMBAR_H
#define ZBOTTOMBAR_H

#include "ui/zbarui.h"
#include "ui/ztextimglabel.h"
#include <QLabel>
#include <QTimer>
#include <QToolButton>
#define SECONDS_ONE_DAY (24*60*60)
#define SECONDS_ONE_HOUR    (60*60)
#define SECONDS_ONE_MINUTE  (60)
class ZBottomBar : public ZBarUI
{
    Q_OBJECT
public:
    ZBottomBar();
    ~ZBottomBar();

    void ZDoInitial();
    void ZUnInitial();
    void ZSetReturnButtonVisible(bool visible);
public slots:
    void ZSloDecodeDone(qint32 state);
    void ZSloTxRxDone(qint32 state);
    void ZSloDevLinkState(qint32 state);
signals:
    void ZSigReturn();
signals:
    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSloUptRunTime(void);
    void ZSlotReturn();
private:
    ZTextImgLabel *m_llIPAddr;
    ZTextImgLabel *m_llRunTime;
    ZTextImgLabel *m_llAuthority;
    ZTextImgLabel *m_llLinkLED;
    ZTextImgLabel *m_llTxRxLED;
    ZTextImgLabel *m_llErrLED;
    ZTextImgLabel *m_llProjectPwr;
    QToolButton *m_tbReturn;

    QTimer *m_tmTick;
    qint64 m_runSeconds;

};

#endif // ZBOTTOMBAR_H
