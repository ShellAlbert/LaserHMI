#ifndef ZBARUI_H
#define ZBARUI_H

#include "base/zbaseui.h"
#include <QHBoxLayout>
#include <QFrame>
class ZBarUI : public QFrame
{
     Q_OBJECT
public:
    ZBarUI();
    ~ZBarUI();

    void ZDoInitial();
    void ZUnInitial();
protected:
    QHBoxLayout *m_hLayout;
};

#endif // ZBARUI_H
