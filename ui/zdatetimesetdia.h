#ifndef ZDATETIMESETDIA_H
#define ZDATETIMESETDIA_H

#include <QDialog>
#include <QToolButton>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFrame>
#include <QDateTime>
#include <QVBoxLayout>
class ZDateTimeSetDia : public QDialog
{
    Q_OBJECT
public:
    ZDateTimeSetDia(QString title,qint32 year,qint32 month,qint32 day,qint32 hour,qint32 minute,qint32 second);
    ~ZDateTimeSetDia();

    QDateTime ZGetDateTime();
private slots:
    void ZSlotButtonClicked();

private:
    void ZFunUpdateWidgets();
private:
    QLabel *m_llTitle;
    QLineEdit *m_le[6];
    QToolButton *m_tb[14];
    QGridLayout *m_gLayout;
    QFrame *m_frm;
    QVBoxLayout *m_vLayout;

    qint32 m_year;
    qint32 m_month;
    qint32 m_day;
    qint32 m_hour;
    qint32 m_minute;
    qint32 m_second;
};

#endif // ZDATETIMESETDIA_H
