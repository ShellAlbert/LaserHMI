#ifndef ZSPINBOX_H
#define ZSPINBOX_H

#include <QFrame>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
class ZSpinBox : public QFrame
{
    Q_OBJECT
public:
    ZSpinBox(QString title,qint32 min,qint32 max,qint32 val);
    ~ZSpinBox();
signals:
    void ZSigValueChanged(qint32 val);
    void ZSigMsg(QString msg);
private slots:
    void ZSloValChanged();
private:
    QLabel *m_llTitle;
    QToolButton *m_tbDec;
    QLineEdit *m_leVal;
    QToolButton *m_tbInc;
    QHBoxLayout *m_hLayout;

    qint32 m_valMin;
    qint32 m_valMax;
    qint32 m_valNow;
};

#endif // ZSPINBOX_H
