#ifndef ZSUMMARYUI_H
#define ZSUMMARYUI_H

#include "base/zbaseui.h"
#include "ui/zcurveui.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QToolButton>
#include <QStackedWidget>
#include <QFrame>
#include <QLabel>
#include <ui/zlaserrunhoursui.h>
class ZGLaserCurve:public QFrame
{
public:
    ZGLaserCurve();
    ~ZGLaserCurve();
private:
    ZLaserRunHoursUI *m_runHours;
    ZCurveUI *m_curveIVP;
    QLabel *m_llIPV[6];
    QLabel *m_llIPVColor[6];
    ZCurveUI *m_curveTemp;
    QLabel *m_llTemp[6];
    QLabel *m_llTempColor[6];
    QGridLayout *m_gLayout;
};
class ZSummaryUI:public ZBaseUI
{
    Q_OBJECT
public:
    ZSummaryUI();
    ~ZSummaryUI();

    void ZDoInitial();
    void ZUnInitial();
private slots:
    void ZSloShowAnotherOne();
signals:
    void ZSigDumpDbgMsg(QString msg);
private:
    ZGLaserCurve* m_glaserCurve[8];
    QStackedWidget *m_stackedWidget;
    QLabel *m_llGLaserNo;
    QToolButton *m_tbLeft;
    QToolButton *m_tbRight;
    QHBoxLayout *m_hLayoutBtn;
    QVBoxLayout *m_vLayout;
    qint32 m_index;
};

#endif // ZSUMMARYUI_H
