#ifndef ZCHECKBOX_H
#define ZCHECKBOX_H

#include <QFrame>
#include <QHBoxLayout>
#include <QCheckBox>
class ZCheckBox : public QFrame
{
    Q_OBJECT
public:
    ZCheckBox(QString tips);
    ~ZCheckBox();
    bool ZIsChecked();
    void ZSetChecked(bool checked);
    QString text();
signals:
    void ZSigStateChanged(qint32 state);
private:
    QHBoxLayout *m_hLayout;
    QCheckBox *m_cb;
};

#endif // ZCHECKBOX_H
