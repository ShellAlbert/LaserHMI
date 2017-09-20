#include "ztextcombobox.h"

ZTextComboBox::ZTextComboBox(QString text,QStringList itemList)
{
    this->setObjectName("ZTextComboBox");
    this->m_llText=new QLabel(text);
    this->m_llText->setObjectName("ZTextComboBoxLabel");
    this->m_llText->setAlignment(Qt::AlignCenter);
    this->m_cb=new QComboBox;
    this->m_cb->setObjectName("ZTextComboBoxComboBox");
    this->m_cb->addItems(itemList);
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_llText);
    this->m_hLayout->addWidget(this->m_cb);
    this->setLayout(this->m_hLayout);
}
ZTextComboBox::~ZTextComboBox()
{
    delete this->m_llText;
    delete this->m_cb;
    delete this->m_hLayout;
}
