#ifndef ZTEXTCOMBOBOX_H
#define ZTEXTCOMBOBOX_H

#include <QFrame>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
class ZTextComboBox : public QFrame
{
public:
    ZTextComboBox(QString text,QStringList itemList);
    ~ZTextComboBox();

private:
    QLabel *m_llText;
    QComboBox *m_cb;
    QHBoxLayout *m_hLayout;
};

#endif // ZTEXTCOMBOBOX_H
