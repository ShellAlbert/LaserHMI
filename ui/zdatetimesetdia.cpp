#include "zdatetimesetdia.h"
#include <gbl/hmi_const.h>
ZDateTimeSetDia::ZDateTimeSetDia(QString title,qint32 year,qint32 month,qint32 day,qint32 hour,qint32 minute,qint32 second)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(title);
    this->m_llTitle->setObjectName("ZDateTimeSetDiaTitle");
    for(qint32 i=0;i<6;i++)
    {
        this->m_le[i]=new QLineEdit;
        this->m_le[i]->setAlignment(Qt::AlignCenter);
    }
    this->m_year=year;
    this->m_month=month;
    this->m_day=day;
    this->m_hour=hour;
    this->m_minute=minute;
    this->m_second=second;

    for(qint32 i=0;i<14;i++)
    {
        this->m_tb[i]=new QToolButton;
        this->m_tb[i]->setObjectName("ZDateTimeSetDiaAddSubButton");
        connect(this->m_tb[i],SIGNAL(clicked(bool)),this,SLOT(ZSlotButtonClicked()));
        switch(i)
        {
        case 0://year+.
            this->m_tb[i]->setText("+");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/add.png"));
            break;
        case 1://year-.
            this->m_tb[i]->setText("-");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/sub.png"));
            break;
        case 2://month+.
            this->m_tb[i]->setText("+");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/add.png"));
            break;
        case 3://month-.
            this->m_tb[i]->setText("-");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/sub.png"));
            break;
        case 4://day+.
            this->m_tb[i]->setText("+");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/add.png"));
            break;
        case 5://day-.
            this->m_tb[i]->setText("-");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/sub.png"));
            break;
        case 6://hour+.
            this->m_tb[i]->setText("+");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/add.png"));
            break;
        case 7://hour-.
            this->m_tb[i]->setText("-");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/sub.png"));
            break;
        case 8://minute +.
            this->m_tb[i]->setText("+");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/add.png"));
            break;
        case 9://minute-.
            this->m_tb[i]->setText("-");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/sub.png"));
            break;
        case 10://sec+.
            this->m_tb[i]->setText("+");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/add.png"));
            break;
        case 11://sec-.
            this->m_tb[i]->setText("-");
            this->m_tb[i]->setIconSize(QSize(48,24));
            this->m_tb[i]->setIcon(QIcon(":/common/images/common/sub.png"));
            break;
        case 12://okay.
            this->m_tb[i]->setText(tr("Okay"));
            this->m_tb[i]->setObjectName("ZDateTimeSetDiaOkayCancel");
            break;
        case 13://cancel.
            this->m_tb[i]->setText(tr("Cancel"));
            this->m_tb[i]->setObjectName("ZDateTimeSetDiaOkayCancel");
            break;
        default:
            break;
        }
    }
    this->m_gLayout=new QGridLayout;
    this->m_gLayout->setContentsMargins(0,0,0,0);
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,3);
    this->m_gLayout->addWidget(this->m_tb[0],1,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[2],1,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[4],1,2,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_le[0],2,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_le[1],2,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_le[2],2,2,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[1],3,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[3],3,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[5],3,2,1,1,Qt::AlignCenter);

    this->m_gLayout->addWidget(this->m_tb[6],4,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[8],4,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[10],4,2,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_le[3],5,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_le[4],5,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_le[5],5,2,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[7],6,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[9],6,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[11],6,2,1,1,Qt::AlignCenter);

    this->m_gLayout->addWidget(this->m_tb[12],7,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tb[13],7,2,1,1,Qt::AlignCenter);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZDateTimeSetDia");
    this->m_frm->setLayout(this->m_gLayout);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);

    this->ZFunUpdateWidgets();
}

ZDateTimeSetDia::~ZDateTimeSetDia()
{
    delete this->m_llTitle;
    for(qint32 i=0;i<6;i++)
    {
        delete this->m_le[i];
    }
    for(qint32 i=0;i<14;i++)
    {
        delete this->m_tb[i];
    }
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
void ZDateTimeSetDia::ZSlotButtonClicked()
{
    QToolButton *tbSrc=qobject_cast<QToolButton*>(this->sender());
    if(tbSrc==NULL)
    {
        return;
    }
    if(tbSrc==this->m_tb[0])//year+.
    {
        this->m_year++;
    }else if(tbSrc==this->m_tb[1])//year-.
    {
        this->m_year--;
    }else if(tbSrc==this->m_tb[2])//month+.
    {
        this->m_month++;
    }else if(tbSrc==this->m_tb[3])//month-.
    {
        this->m_month--;
    }else if(tbSrc==this->m_tb[4])//day+.
    {
        this->m_day++;
    }else if(tbSrc==this->m_tb[5])//day-.
    {
        this->m_day--;
    }else if(tbSrc==this->m_tb[6])//hour++.
    {
        this->m_hour++;
    }else if(tbSrc==this->m_tb[7])//hour--.
    {
        this->m_hour--;
    }else if(tbSrc==this->m_tb[8])//minute+.
    {
        this->m_minute++;
    }else if(tbSrc==this->m_tb[9])//minute-.
    {
        this->m_minute--;
    }else if(tbSrc==this->m_tb[10])//second+.
    {
        this->m_second++;
    }else if(tbSrc==this->m_tb[11])//seocnd-.
    {
        this->m_second--;
    }else if(tbSrc==this->m_tb[12])//Okay.
    {
        this->accept();
    }else if(tbSrc==this->m_tb[13])//Cancel.
    {
        this->reject();
    }
    this->ZFunUpdateWidgets();
    g_GblHelp.beep();
}
void ZDateTimeSetDia::ZFunUpdateWidgets()
{
    this->m_le[0]->setText(QString::number(this->m_year,10));
    this->m_le[1]->setText(QString::number(this->m_month,10));
    this->m_le[2]->setText(QString::number(this->m_day,10));
    this->m_le[3]->setText(QString::number(this->m_hour,10));
    this->m_le[4]->setText(QString::number(this->m_minute,10));
    this->m_le[5]->setText(QString::number(this->m_second,10));
}
QDateTime ZDateTimeSetDia::ZGetDateTime()
{
    QDateTime dt;
    dt.setDate(QDate(this->m_year,this->m_month,this->m_day));
    dt.setTime(QTime(this->m_hour,this->m_minute,this->m_second));
    return dt;
}
