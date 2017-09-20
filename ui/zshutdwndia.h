#ifndef ZSHUTDWNDIA_H
#define ZSHUTDWNDIA_H

#include <QDialog>
#include <QToolButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
class ZShutDwnDia : public QDialog
{
    Q_OBJECT
public:
    ZShutDwnDia();
    ~ZShutDwnDia();

    qint32 ZGetRetCode();
private slots:
    void ZSloLogout();
    void ZSloReboot();
    void ZSloShutdown();
    void ZSloCancel();
private:
    QToolButton *m_tbLogout;
    QToolButton *m_tbReboot;
    QToolButton *m_tbShutdown;
    QToolButton *m_tbCancel;
    QHBoxLayout *m_hLayout;

    QFrame *m_frm;
    QVBoxLayout *m_vLayout;

    qint32 m_retCode;
};

#endif // ZSHUTDWNDIA_H
