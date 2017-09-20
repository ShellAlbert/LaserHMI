#include "zdatetimechooser.h"
#include <QDateTime>
ZDateTimeChooser::ZDateTimeChooser(QString tips)
{
    this->setObjectName("ZDateTimeChooser");
    this->m_llTips=new QLabel(tips);
    this->m_llTips->setObjectName("ZDateTimeChooserLabel");
    this->m_le=new QLineEdit;
    this->m_le->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    this->m_ll=new QLabel;
    this->m_ll->setPixmap(QPixmap(":/log/images/log/Keyboard.png").scaled(24,24));
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_llTips);
    this->m_hLayout->addWidget(this->m_le);
    this->m_hLayout->addWidget(this->m_ll);
    this->setLayout(this->m_hLayout);
}
ZDateTimeChooser::~ZDateTimeChooser()
{
    delete this->m_llTips;
    delete this->m_le;
    delete this->m_ll;
    delete this->m_hLayout;
}
