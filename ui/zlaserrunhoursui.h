#ifndef ZLASERRUNHOURSUI_H
#define ZLASERRUNHOURSUI_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
class ZLaserRunHoursUI : public QFrame
{
public:
    ZLaserRunHoursUI(QString title);
    ~ZLaserRunHoursUI();

    void ZSetRunHours(qint32 hours,qint32 minutes);
private:
    QLabel *m_llTitle;
    QLabel *m_ll[10];
    QVBoxLayout *m_vLayout;
    QString m_title;
};

#endif // ZLASERRUNHOURSUI_H
