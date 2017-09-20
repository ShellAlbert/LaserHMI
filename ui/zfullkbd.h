#ifndef ZFULLKBD_H
#define ZFULLKBD_H

#include <QDialog>
#include <QFrame>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
class ZFullKbd : public QDialog
{
    Q_OBJECT
public:
    ZFullKbd(QString title);
    ~ZFullKbd();
    QString ZGetText();
    void ZSetText(QString text);
private slots:
    void ZSloA2ZButtonClicked();
    void ZSloChSelectButtonClicked();
    void ZSloFunButtonClicked();
private:
    qint32 ZGetKeyCode(QString keyChar);
private:
    QLabel *m_llTitle;
    QToolButton *m_tbOkay;
    QToolButton *m_tbCancel;
    QLineEdit *m_le;
    QLabel *m_llInput;
    QToolButton *m_tbTurnPage[2];
    QToolButton *m_tbChSelect[6];
    QToolButton *m_tbA2Z[26];
    QToolButton *m_tbFun[9];
    QHBoxLayout *m_hLayout[7];
    QVBoxLayout *m_vLayoutFrm;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;

    bool m_bChMode;
    QString m_input;
};
#endif // ZFULLKBD_H
