#include "zdigkbd.h"
#include <QApplication>
#include <QKeyEvent>
#include "gbl/hmi_const.h"
ZDigKbd::ZDigKbd(QString title,bool passwordMode)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->m_llTitle=new QLabel;
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    this->m_llTitle->setText(title);
    this->m_llTitle->setObjectName("ZDigKbTitle");
    this->m_le=new QLineEdit;
    this->m_le->setObjectName("ZDigKbLineEdit");
    if(true==passwordMode)
    {
        this->m_le->setEchoMode(QLineEdit::Password);
    }
    for(qint32 i=0;i<14;i++)
    {
        this->m_tb[i]=new QToolButton;
        this->m_tb[i]->setObjectName("ZDigKbButton");
        connect(this->m_tb[i],SIGNAL(clicked(bool)),this,SLOT(ZSloButtonClicked()));
        switch(i)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            this->m_tb[i]->setText(QString("%1").arg(i));
            break;
        case 10:
            this->m_tb[i]->setText("<-");
            break;
        case 11:
            this->m_tb[i]->setText("C");
            break;
        case 12:
            this->m_tb[i]->setText("Okay");
            break;
        case 13:
            this->m_tb[i]->setText("Cancel");
            break;
        default:
            break;
        }
    }
    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,3);
    this->m_gLayout->addWidget(this->m_le,1,0,1,3);

    this->m_gLayout->addWidget(this->m_tb[1],2,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[2],2,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[3],2,2,1,1);

    this->m_gLayout->addWidget(this->m_tb[4],3,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[5],3,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[6],3,2,1,1);

    this->m_gLayout->addWidget(this->m_tb[7],4,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[8],4,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[9],4,2,1,1);

    this->m_gLayout->addWidget(this->m_tb[0],5,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[10],5,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[11],5,2,1,1);

    this->m_gLayout->addWidget(this->m_tb[12],6,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[13],6,2,1,1);

    this->m_gLayout->setContentsMargins(10,8,10,8);
    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZDigKbd");
    this->m_frm->setLayout(this->m_gLayout);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);
}
ZDigKbd::~ZDigKbd()
{
    delete this->m_llTitle;
    delete this->m_le;
    for(qint32 i=0;i<14;i++)
    {
        delete this->m_tb[i];
    }
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
QString ZDigKbd::ZGetDigital()
{
    return this->m_le->text().trimmed();
}
void ZDigKbd::ZSetDigital(QString digital)
{
    this->m_le->setText(digital);
}
void ZDigKbd::ZSloButtonClicked()
{
    QToolButton *src=qobject_cast<QToolButton*>(this->sender());
    if(src==NULL)
    {
        return;
    }
    if(src->text()=="<-")
    {
        QString oldData=this->m_le->text().trimmed();
        this->m_le->setText(oldData.left(oldData.length()-1));
    }else if(src->text()=="C")
    {
        this->m_le->setText("");
    }else if(src->text()=="Okay"){
        this->accept();
    }else if(src->text()=="Cancel"){
        this->rect();
    }else{
        this->m_le->setText(this->m_le->text()+src->text());
    }
}
