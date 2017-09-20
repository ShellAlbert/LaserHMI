#include "zlaserrunhoursui.h"

ZLaserRunHoursUI::ZLaserRunHoursUI(QString title)
{
    this->setObjectName("ZLaserRunHoursUI");
    this->setMinimumWidth(10);
    this->m_llTitle=new QLabel;
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    this->m_llTitle->setObjectName("ZLaserRunHoursUILabel");
    this->m_title=title;
    for(qint32 i=0;i<10;i++)
    {
        this->m_ll[i]=new QLabel;
        switch(i)
        {
        case 0:
            this->m_ll[i]->setStyleSheet("QLabel{background:#FF0000;color:white;}");
            this->m_ll[i]->setText("30000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 1:
            this->m_ll[i]->setStyleSheet("QLabel{background:#FF0080;color:white;}");
            this->m_ll[i]->setText("27000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 2:
            this->m_ll[i]->setStyleSheet("QLabel{background:#FF5809;color:white;}");
            this->m_ll[i]->setText("24000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 3:
            this->m_ll[i]->setStyleSheet("QLabel{background:#FFD306;color:white;}");
            this->m_ll[i]->setText("21000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 4:
            this->m_ll[i]->setStyleSheet("QLabel{background:#9AFF02;color:white;}");
            this->m_ll[i]->setText("18000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 5:
            this->m_ll[i]->setStyleSheet("QLabel{background:#00DB00;color:white;}");
            this->m_ll[i]->setText("15000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 6:
            this->m_ll[i]->setStyleSheet("QLabel{background:#02DF82;color:white;}");
            this->m_ll[i]->setText("12000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 7:
            this->m_ll[i]->setStyleSheet("QLabel{background:#00CACA;color:white;}");
            this->m_ll[i]->setText("9000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 8:
            this->m_ll[i]->setStyleSheet("QLabel{background:#0072E3;color:white;}");
            this->m_ll[i]->setText("6000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        case 9:
            this->m_ll[i]->setStyleSheet("QLabel{background:#2828FF;color:white;}");
            this->m_ll[i]->setText("3000");
            this->m_ll[i]->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
            break;
        default:
            break;
        }
    }
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setSpacing(0);
    this->m_vLayout->setContentsMargins(10,10,10,10);
    this->m_vLayout->addWidget(this->m_llTitle);
    for(qint32 i=0,x=1;i<10;i++,x++)
    {
        this->m_vLayout->addWidget(this->m_ll[i]);
    }
    //int left, int top, int right, int bottom
    this->setLayout(this->m_vLayout);
    this->ZSetRunHours(0,0);
}
ZLaserRunHoursUI::~ZLaserRunHoursUI()
{
    delete this->m_llTitle;
    for(qint32 i=0;i<10;i++)
    {
        delete this->m_ll[i];
    }
    delete this->m_vLayout;
}
void ZLaserRunHoursUI::ZSetRunHours(qint32 hours,qint32 minutes)
{
    this->m_llTitle->setText(this->m_title+QString("\n%1h %2m").arg(hours).arg(minutes));
}
