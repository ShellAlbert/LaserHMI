#include "zlaserui.h"
#include "gbl/hmi_const.h"
#include <QDebug>
ZLaserUI::ZLaserUI()
{

}
ZLaserUI::~ZLaserUI()
{

}

void ZLaserUI::ZDoInitial()
{
    this->m_tabWidget=new QTabWidget;
    this->m_tabWidget->setIconSize(QSize(16,16));
    connect(this->m_tabWidget,SIGNAL(currentChanged(int)),this,SLOT(ZSloTabChanged(qint32)));

    this->m_mainCtlUI=new ZMainCtlUI;
    this->m_mainCtlUI->ZDoInitial();
    this->m_tabWidget->addTab(this->m_mainCtlUI,QIcon(":/laser/images/laser/mainCtl.png"),tr("全部"));
    connect(this->m_mainCtlUI,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    connect(this->m_mainCtlUI,SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));

    for(qint32 i=0;i<8;i++)
    {
        this->m_mdlCtlUI[i]=new ZMdlCtlUI(i+1);
        this->m_mdlCtlUI[i]->ZDoInitial();
        this->m_tabWidget->addTab(this->m_mdlCtlUI[i],QIcon(":/laser/images/laser/mdlCtl.png"),tr("%1号").arg(i+1));
        connect(this->m_mdlCtlUI[i],SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
        connect(this->m_mdlCtlUI[i],SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    }

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_tabWidget);
    this->setLayout(this->m_vLayout);

    this->m_bFaultIconFlag=false;
    this->m_timer=new QTimer;
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(ZSlotTimeout()));
    this->m_timer->start(1000);
}
void ZLaserUI::ZUnInitial()
{
    this->m_timer->stop();
    delete this->m_timer;
    for(qint32 i=0;i<8;i++)
    {
        this->m_mdlCtlUI[i]->ZUnInitial();
        delete this->m_mdlCtlUI[i];
    }
    this->m_mainCtlUI->ZUnInitial();
    delete this->m_mainCtlUI;
    delete this->m_tabWidget;
    delete this->m_vLayout;
}
void ZLaserUI::ZSloTabChanged(qint32 index)
{
    g_GblHelp.beep();
}
void ZLaserUI::ZSlotTimeout()
{
    if(g_GblHelp.m_laserFault&(0x1<<0))//GLaser-1 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(1,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(1,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(1,QIcon(":/laser/images/laser/mdlCtl.png"));
    }

    if(g_GblHelp.m_laserFault&(0x1<<1))//GLaser-2 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(2,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(2,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(2,QIcon(":/laser/images/laser/mdlCtl.png"));
    }

    if(g_GblHelp.m_laserFault&(0x1<<2))//GLaser-3 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(3,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(3,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(3,QIcon(":/laser/images/laser/mdlCtl.png"));
    }

    if(g_GblHelp.m_laserFault&(0x1<<3))//GLaser-4 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(4,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(4,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(4,QIcon(":/laser/images/laser/mdlCtl.png"));
    }

    if(g_GblHelp.m_laserFault&(0x1<<4))//GLaser-5 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(5,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(5,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(5,QIcon(":/laser/images/laser/mdlCtl.png"));
    }

    if(g_GblHelp.m_laserFault&(0x1<<5))//GLaser-6 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(6,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(6,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(6,QIcon(":/laser/images/laser/mdlCtl.png"));
    }

    if(g_GblHelp.m_laserFault&(0x1<<6))//GLaser-7 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(7,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(7,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(7,QIcon(":/laser/images/laser/mdlCtl.png"));
    }

    if(g_GblHelp.m_laserFault&(0x1<<7))//GLaser-8 fault.
    {
        if(this->m_bFaultIconFlag)
        {
            this->m_tabWidget->setTabIcon(8,QIcon(":/laser/images/laser/mdlCtl.png"));
        }else{
            this->m_tabWidget->setTabIcon(8,QIcon(":/topbar/images/led/led_red.png"));
        }
    }else{
        this->m_tabWidget->setTabIcon(8,QIcon(":/laser/images/laser/mdlCtl.png"));
    }
    //invert.
    this->m_bFaultIconFlag=!this->m_bFaultIconFlag;
}
