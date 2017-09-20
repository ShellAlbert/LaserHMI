#ifndef ZCOLORTEMPTUNINGDIA_H
#define ZCOLORTEMPTUNINGDIA_H

#include <QDialog>
#include <QLabel>
#include <QToolButton>
#include <QGridLayout>
#include <ui/zprogressbar.h>
#include <ui/zverticalindicator.h>
class ZColorTempTuningDia:public QDialog
{
    Q_OBJECT
public:
    ZColorTempTuningDia(quint8 glaserAddr,QString colorTempName);
    ~ZColorTempTuningDia();
public slots:
    void ZSloSyncColorTemp(QString arguments);
    void ZSloSyncColorTempPercent(QString cmd,QString arguments);
    void ZSloAdjRGBPercent(QString cmd,QString arguments);

signals:
    void ZSigRequestCmd(QString cmd,QString arguments);
    void ZSigPopMessage(QString msg);
    void ZSigRequestPrecisSetup(QString channel);
    void ZSigDumpDbgMsg(QString msg);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void ZSlotSync();
    void ZSlotSave();
    void ZSlotDefault();
    void ZSlotClose();
    void ZSlotValueChanged();
    void ZSlotReadDB();

    void ZSlotShowPrecisSetupDialog(QString channel);
private:
    QLabel *m_llTitle;
    QToolButton *m_tbRedDec;
    QToolButton *m_tbRedInc;
    //ZProgressBar *m_pbRed;
    ZVerticalIndicator *m_pbRed;
    QToolButton *m_tbGreenDec;
    QToolButton *m_tbGreenInc;
    //ZProgressBar *m_pbGreen;
    ZVerticalIndicator *m_pbGreen;
    QToolButton *m_tbBlueDec;
    QToolButton *m_tbBlueInc;
    //ZProgressBar *m_pbBlue;
    ZVerticalIndicator *m_pbBlue;
    QGridLayout *m_gLayoutTuning;

    QLabel *m_llOpTips;
    QToolButton *m_tbSync;
    QToolButton *m_tbSave;
    QToolButton *m_tbDefault;
    QToolButton *m_tbClose;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayoutFrm;

    QString m_colorTempName;
    float m_redDefault;
    float m_greenDefault;
    float m_blueDefault;

    quint8 m_glaserAddress;

    //fixup for QWindow::isVisible()
    //?? ?? ??
    //QApplicationPrivate::notify_helper(QObject*,QEvent*).
    bool m_bProcessEvent;
};

#endif // ZCOLORTEMPTUNINGDIA_H
