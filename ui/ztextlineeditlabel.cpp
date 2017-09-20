#include "ztextlineeditlabel.h"
#include <QEvent>
ZTextLineEditLabel::ZTextLineEditLabel(QString tipMsg,QString data,Qt::Orientation orien)
{
    this->m_orien=orien;

    this->setObjectName("TextLineEditFrame");
    this->m_llTips=new QLabel;
    this->m_llTips->setObjectName("TextLineEditFrameLabel");
    this->m_llTips->setAlignment(Qt::AlignCenter);
    this->m_llTips->setText(tipMsg);
    this->m_leData=new QLineEdit;
    this->m_leData->setAlignment(Qt::AlignCenter);
    this->m_leData->setFocusPolicy(Qt::NoFocus);
    this->m_leData->setText(data);
    this->m_leData->installEventFilter(this);
    this->m_leData2=NULL;
    this->m_llImg=NULL;

    if(this->m_orien==Qt::Horizontal)
    {
        this->m_hLayout=new QHBoxLayout;
        this->m_hLayout->addWidget(this->m_llTips);
        this->m_hLayout->addWidget(this->m_leData);
        this->setLayout(this->m_hLayout);
    }else if(this->m_orien==Qt::Vertical)
    {
        this->m_vLayout=new QVBoxLayout;
        this->m_vLayout->addWidget(this->m_llTips);
        this->m_vLayout->addWidget(this->m_leData);
        this->setLayout(this->m_vLayout);
    }
}

ZTextLineEditLabel::ZTextLineEditLabel(QString tipMsg,QString data1,QString data2,QPixmap img,Qt::Orientation orien)
{
    this->setObjectName("TextLineEditFrame");
    this->m_llTips=new QLabel;
    this->m_llTips->setObjectName("TextLineEditFrameLabel");
    this->m_llTips->setAlignment(Qt::AlignCenter);
    this->m_llTips->setText(tipMsg);
    this->m_leData=new QLineEdit;
    this->m_leData->setAlignment(Qt::AlignCenter);
    this->m_leData->setFocusPolicy(Qt::NoFocus);
    this->m_leData->setText(data1);
    this->m_leData->installEventFilter(this);

    this->m_leData2=new QLineEdit;
    this->m_leData2->setAlignment(Qt::AlignCenter);
    this->m_leData2->setFocusPolicy(Qt::NoFocus);
    this->m_leData2->setText(data2);

    this->m_llImg=new QLabel;
    this->m_llImg->setPixmap(img.scaled(16,16));

    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_llTips);
    this->m_hLayout->addWidget(this->m_leData);
    this->m_hLayout->addWidget(this->m_leData2);
    this->m_hLayout->addWidget(this->m_llImg);
    this->setLayout(this->m_hLayout);
}
ZTextLineEditLabel::~ZTextLineEditLabel()
{
    if(this->m_llTips)
    {
        delete this->m_llTips;
    }
    if(this->m_leData)
    {
        delete this->m_leData;
    }
    if(this->m_leData2)
    {
        delete this->m_leData2;
    }
    if(this->m_llImg)
    {
        delete this->m_llImg;
    }
    if(this->m_orien==Qt::Horizontal)
    {
        delete this->m_hLayout;
    }else if(this->m_orien==Qt::Vertical)
    {
        delete this->m_vLayout;
    }
}

void ZTextLineEditLabel::ZSetTips(QString tipMsg)
{
    this->m_llTips->setText(tipMsg);
}
void ZTextLineEditLabel::ZSetLineEdit(QString data)
{
    this->m_leData->setText(data);
}
void ZTextLineEditLabel::ZSetLineEdit2(QString data)
{
    this->m_leData2->setText(data);
}
QString ZTextLineEditLabel::ZGetLineEdit()
{
    return this->m_leData->text();
}
QString ZTextLineEditLabel::ZGetLineEdit2()
{
    return this->m_leData2->text();
}

void ZTextLineEditLabel::ZSetImg(QPixmap img)
{
    this->m_llImg->setPixmap(img.scaled(16,16));
}
void ZTextLineEditLabel::ZSetBackgroundColor2Error()
{
    QString qss("QLineEdit{"
                    "border-radius: 4px;"
                    "height: 25px;"
                    "border: 1px solid rgb(100, 100, 100);"
                    "background: rgb(200,0,0);"
                    "color:white;"
                 "}");
    if(this->m_leData!=NULL)
    {
        this->m_leData->setStyleSheet(qss);
    }
    if(this->m_leData2!=NULL)
    {
        this->m_leData2->setStyleSheet(qss);
    }
}
void ZTextLineEditLabel::ZSetBackgroundColor2Normal()
{
    QString qss("QLineEdit{"
                    "border-radius: 4px;"
                    "height: 25px;"
                    "border: 1px solid rgb(100, 100, 100);"
                    "background: rgb(102,138,115);"
                    "color:white;"
                 "}");
    if(this->m_leData!=NULL)
    {
        this->m_leData->setStyleSheet(qss);
    }
    if(this->m_leData2!=NULL)
    {
        this->m_leData2->setStyleSheet(qss);
    }
}
bool ZTextLineEditLabel::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
    {
        if(watched==this->m_leData)
        {
            emit this->ZSigClicked();
        }
    }
    return QFrame::eventFilter(watched,event);
}
