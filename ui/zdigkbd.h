#ifndef ZDIGKBD_H
#define ZDIGKBD_H

#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QToolButton>
class ZDigKbd : public QDialog
{
    Q_OBJECT
public:
    ZDigKbd(QString title,bool passwordMode=false);
    ~ZDigKbd();
    QString ZGetDigital();
    void ZSetDigital(QString digital);
private slots:
    void ZSloButtonClicked();
private:
    QLabel *m_llTitle;
    QLineEdit *m_le;
    QToolButton *m_tb[14];
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;
};

#endif // ZDIGKBD_H
