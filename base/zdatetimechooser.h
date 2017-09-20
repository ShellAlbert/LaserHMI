#ifndef ZDATETIMECHOOSER_H
#define ZDATETIMECHOOSER_H

#include "base/zbaseui.h"
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
class ZDateTimeChooser : public ZBaseUI
{
    Q_OBJECT
public:
    ZDateTimeChooser(QString tips);
    ~ZDateTimeChooser();

private:
    QLabel *m_llTips;
    QLineEdit *m_le;
    QLabel *m_ll;
    QHBoxLayout *m_hLayout;
};

#endif // ZDATETIMECHOOSER_H
