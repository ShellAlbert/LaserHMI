#include "ztextimglabel.h"
#include <QEvent>
ZTextImgLabel::ZTextImgLabel(QString text)
{
    this->setObjectName("TextImgFrame");
    this->m_llText=new QLabel(text);
    this->m_llText->setObjectName("textImgLabel");
    this->m_llText->setAlignment(Qt::AlignCenter);
    this->m_llText->installEventFilter(this);
    this->m_llImg=NULL;
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_llText);
    this->setLayout(this->m_hLayout);
}
ZTextImgLabel::ZTextImgLabel(QString text,QPixmap img)
{
    this->setObjectName("TextImgFrame");
    this->m_llText=new QLabel(text);
    this->m_llText->setObjectName("textImgLabel");
    this->m_llText->setAlignment(Qt::AlignCenter);
    this->m_llText->installEventFilter(this);
    this->m_llImg=new QLabel;
    this->m_llImg->setObjectName("textImgLabel");
    this->m_llImg->setPixmap(img.scaled(16,16));
    this->m_llImg->setAlignment(Qt::AlignCenter);
    this->m_llImg->installEventFilter(this);
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addStretch(1);
    this->m_hLayout->addWidget(this->m_llText);
    this->m_hLayout->addWidget(this->m_llImg);
    this->m_hLayout->addStretch(1);
    this->setLayout(this->m_hLayout);
}
ZTextImgLabel::~ZTextImgLabel()
{
    delete this->m_llText;
    if(this->m_llImg)
    {
        delete this->m_llImg;
    }
    delete this->m_hLayout;
}
void ZTextImgLabel::ZSetText(QString text)
{
    this->m_llText->setText(text);
}
void ZTextImgLabel::ZSetPixmap(QPixmap img)
{
    if(this->m_llImg)
    {
        this->m_llImg->setPixmap(img.scaled(16,16));
    }
}
bool ZTextImgLabel::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
    {
        if(watched==this->m_llText || watched==this->m_llImg)
        {
            emit this->ZSigClicked();
        }
    }
    return QFrame::eventFilter(watched,event);
}
