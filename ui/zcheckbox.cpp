#include "zcheckbox.h"

ZCheckBox::ZCheckBox(QString tips)
{
    this->setObjectName("ZCheckBoxFrame");
    this->m_cb=new QCheckBox;
    this->m_cb->setText(tips);
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_cb);
    this->m_hLayout->setContentsMargins(10,20,10,20);
    this->setLayout(this->m_hLayout);

    connect(this->m_cb,SIGNAL(stateChanged(int)),this,SIGNAL(ZSigStateChanged(qint32)));
}
ZCheckBox::~ZCheckBox()
{
    delete this->m_cb;
    delete this->m_hLayout;
}
bool ZCheckBox::ZIsChecked()
{
    return this->m_cb->isChecked();
}
void ZCheckBox::ZSetChecked(bool checked)
{
    this->m_cb->setChecked(checked);
}
QString ZCheckBox::text()
{
    return this->m_cb->text();
}
