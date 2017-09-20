#ifndef ZBASEUI_H
#define ZBASEUI_H

#include <QWidget>
#include "base/zbaseobj.h"
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
class ZBaseUI : public ZBaseObj
{
     Q_OBJECT
public:
    ZBaseUI();
    ~ZBaseUI();
private slots:
    virtual void ZSloGetDeviceStatus(void);

protected:
    QTimer *m_tmGetDevStatus;
};
class ZLabelLineEditCheckBox:public QFrame
{
    Q_OBJECT
public:
    ZLabelLineEditCheckBox(QString labelText,QString lineEditText,QString checkBoxText,bool checked,bool checkedVisible=true);
    ~ZLabelLineEditCheckBox();

    QString ZGetLineEditText();
    void ZSetLineEditText(QString data);
    bool ZIsChecked();
signals:
    void ZSigLineEditClicked();
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QHBoxLayout *m_hLayout;
    QLabel *m_ll;
    QLineEdit *m_le;
    QCheckBox *m_cb;
};
#endif // ZBASEUI_H
