#ifndef ZNAVIGATEUI_H
#define ZNAVIGATEUI_H

#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QFrame>
class ZNavigateUI:public QFrame
{
    Q_OBJECT
public:
    ZNavigateUI();
    ~ZNavigateUI();

    void ZDoInitial();
    void ZUnInitial();

signals:
    void ZSigModuleClicked(QString moduleName);
signals:
    void ZSigDumpDbgMsg(QString msg);
private slots:
    void ZSloModuleClicked(void);
private:
    QToolButton *m_tbSummary;
    QToolButton *m_tbLaser;
    QToolButton *m_tbChiller;
    QToolButton *m_tbSystem;
    QToolButton *m_tbSetup;
    QToolButton *m_tbLog;
    QToolButton *m_tbDebug;
    QToolButton *m_tbNetwork;
    QToolButton *m_tbAbout;

    QVBoxLayout *m_vLayout;
};

#endif // ZNAVIGATEUI_H
