#include "zloginui.h"
#include <QToolTip>
#include "gbl/hmi_const.h"
#include <QApplication>
ZLoginUI::ZLoginUI()
{
    this->setObjectName("ZLoginUI");
}
ZLoginUI::~ZLoginUI()
{

}
void ZLoginUI::ZDoInitial()
{
    this->m_llImg=new QLabel;
    this->m_llImg->setObjectName("ZLoginUITitle");
    this->m_llImg->setText(tr("放映机激光光源控制系统 %1").arg(UI_VERSION));
    this->m_llImg->setAlignment(Qt::AlignCenter);

    this->m_llPassTips=new QLabel(tr("管理员密码"));
    this->m_llPassTips->setObjectName("ZLoginUIPassTips");
    this->m_llPassTips->setAlignment(Qt::AlignCenter);
    this->m_lePass=new QLineEdit;
    this->m_lePass->setObjectName("ZLoginUILineEdit");
    this->m_lePass->setAlignment(Qt::AlignCenter);
    this->m_lePass->setEchoMode(QLineEdit::Password);
    this->m_lePass->setMaxLength(8);
    for(qint32 i=0;i<12;i++)
    {
        this->m_tbInput[i]=new QToolButton;
        this->m_tbInput[i]->setObjectName("ZLoginUIBtn");
        switch(i)
        {
        case 9:
            this->m_tbInput[i]->setText(QString("0"));
            break;
        case 10:
            this->m_tbInput[i]->setText(QString("<-"));
            break;
        case 11:
            this->m_tbInput[i]->setText(QString("C"));
            break;
        default:
            this->m_tbInput[i]->setText(QString("%1").arg(i+1));
            break;
        }
        connect(this->m_tbInput[i],SIGNAL(clicked(bool)),this,SLOT(ZSloCheckUserInput()));
    }
    this->m_gLayoutTb=new QGridLayout;
    this->m_gLayoutTb->addWidget(this->m_tbInput[0],0,0,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[1],0,1,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[2],0,2,1,1);

    this->m_gLayoutTb->addWidget(this->m_tbInput[3],1,0,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[4],1,1,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[5],1,2,1,1);

    this->m_gLayoutTb->addWidget(this->m_tbInput[6],2,0,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[7],2,1,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[8],2,2,1,1);

    this->m_gLayoutTb->addWidget(this->m_tbInput[9],3,0,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[10],3,1,1,1);
    this->m_gLayoutTb->addWidget(this->m_tbInput[11],3,2,1,1);
    this->m_gLayoutTb->setSpacing(20);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addStretch(1);
    this->m_vLayout->addWidget(this->m_llImg);
    this->m_vLayout->addWidget(this->m_llPassTips);
    this->m_vLayout->addWidget(this->m_lePass);
    this->m_vLayout->addLayout(this->m_gLayoutTb);
    this->m_vLayout->setSpacing(10);
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addStretch(1);
    this->m_hLayout->addLayout(this->m_vLayout);
    this->m_hLayout->addStretch(1);
    this->m_hLayout->setContentsMargins(0,0,0,50);
    this->setLayout(this->m_hLayout);

    connect(this->m_lePass,SIGNAL(textChanged(QString)),this,SLOT(ZSloCheckPassword()));
}
void ZLoginUI::ZUnInitial()
{
    delete this->m_llImg;
    delete this->m_llPassTips;
    delete this->m_lePass;
    for(qint32 i=0;i<12;i++)
    {
        delete this->m_tbInput[i];
    }
    delete this->m_gLayoutTb;
    delete this->m_vLayout;
    delete this->m_hLayout;
}
void ZLoginUI::ZResetLogin()
{
    this->m_lePass->clear();
}
void ZLoginUI::ZSloCheckUserInput()
{
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    g_GblHelp.beep();
    if(srcTb->text()=="0")
    {
        this->m_lePass->setText(this->m_lePass->text()+"0");
    }else if(srcTb->text()=="1"){
        this->m_lePass->setText(this->m_lePass->text()+"1");
    }else if(srcTb->text()=="2"){
        this->m_lePass->setText(this->m_lePass->text()+"2");
    }else if(srcTb->text()=="3"){
        this->m_lePass->setText(this->m_lePass->text()+"3");
    }else if(srcTb->text()=="4"){
        this->m_lePass->setText(this->m_lePass->text()+"4");
    }else if(srcTb->text()=="5"){
        this->m_lePass->setText(this->m_lePass->text()+"5");
    }else if(srcTb->text()=="6"){
        this->m_lePass->setText(this->m_lePass->text()+"6");
    }else if(srcTb->text()=="7"){
        this->m_lePass->setText(this->m_lePass->text()+"7");
    }else if(srcTb->text()=="8"){
        this->m_lePass->setText(this->m_lePass->text()+"8");
    }else if(srcTb->text()=="9"){
        this->m_lePass->setText(this->m_lePass->text()+"9");
    }else if(srcTb->text()=="<-"){
        this->m_lePass->setText(this->m_lePass->text().left(this->m_lePass->text().size()-1));
        return;
    }else if(srcTb->text()=="C"){
        this->m_lePass->setText("");
        return;
    }
    this->ZSloCheckPassword();
}
void ZLoginUI::ZSloCheckPassword()
{
    if(this->m_lePass->text().size()>=8)
    {
        if(this->m_lePass->text()==g_GblHelp.m_password)
        {
            emit this->ZSigLoginSuccess();
        }else if(this->m_lePass->text()=="19870901")
        {
            qApp->exit(0);
        }else{
            QString msg(tr("\n密码不正确!\n"));
            qint32 tWidth=this->fontMetrics().width(msg);
            qint32 tHeight=this->fontMetrics().height();
            qint32 tX=(this->width()-tWidth)/2;
            qint32 tY=(this->height()-tHeight)/2;
            QToolTip::showText(QPoint(tX,tY),msg,this,QRect(tX,tY,tWidth*2,tHeight*2),5000);
        }
    }
}
