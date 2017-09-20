#include "zshutdwndia.h"
#include <QApplication>
#include "gbl/hmi_const.h"
ZShutDwnDia::ZShutDwnDia()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->m_tbLogout=new QToolButton;
    this->m_tbLogout->setText(tr("注销"));
    this->m_tbLogout->setIconSize(QSize(48,48));
    this->m_tbLogout->setIcon(QIcon(":/bottombar/images/bottombar/Logout.png"));
    this->m_tbLogout->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbReboot=new QToolButton;
    this->m_tbReboot->setText(tr("重启"));
    this->m_tbReboot->setIconSize(QSize(48,48));
    this->m_tbReboot->setIcon(QIcon(":/bottombar/images/bottombar/Reboot.png"));
    this->m_tbReboot->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbShutdown=new QToolButton;
    this->m_tbShutdown->setText(tr("关机"));
    this->m_tbShutdown->setIconSize(QSize(48,48));
    this->m_tbShutdown->setIcon(QIcon(":/bottombar/images/bottombar/Shutdown.png"));
    this->m_tbShutdown->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("取消"));
    this->m_tbCancel->setIconSize(QSize(48,48));
    this->m_tbCancel->setIcon(QIcon(":/bottombar/images/bottombar/Cancel.png"));
    this->m_tbCancel->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbLogout->setObjectName("ZShutDwnDiaButton");
    this->m_tbReboot->setObjectName("ZShutDwnDiaButton");
    this->m_tbShutdown->setObjectName("ZShutDwnDiaButton");
    this->m_tbCancel->setObjectName("ZShutDwnDiaButton");
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_tbLogout);
    this->m_hLayout->addWidget(this->m_tbReboot);
    this->m_hLayout->addWidget(this->m_tbShutdown);
    this->m_hLayout->addWidget(this->m_tbCancel);
    this->m_hLayout->setSpacing(10);
    this->m_hLayout->setContentsMargins(20,40,20,40);
    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZShutDwnDiaFrame");
    this->m_frm->setLayout(this->m_hLayout);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_frm);
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbLogout,SIGNAL(clicked(bool)),this,SLOT(ZSloLogout()));
    connect(this->m_tbReboot,SIGNAL(clicked(bool)),this,SLOT(ZSloReboot()));
    connect(this->m_tbShutdown,SIGNAL(clicked(bool)),this,SLOT(ZSloShutdown()));
    connect(this->m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(ZSloCancel()));

    //center me.
     this->setFixedSize(QSize(400,200));
     qint32 tX=(SCREEN_WIDTH-this->width())/2;
     qint32 tY=(SCREEN_HEIGHT-this->height())/2;
     this->move(tX,tY);

     this->m_retCode=0;
}
ZShutDwnDia::~ZShutDwnDia()
{
    delete this->m_tbLogout;
    delete this->m_tbReboot;
    delete this->m_tbShutdown;
    delete this->m_tbCancel;
    delete this->m_hLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
qint32 ZShutDwnDia::ZGetRetCode()
{
    return this->m_retCode;
}
void ZShutDwnDia::ZSloLogout()
{
    this->m_retCode=1;
    this->accept();
}
void ZShutDwnDia::ZSloReboot()
{
    this->m_retCode=2;
    this->accept();
}
void ZShutDwnDia::ZSloShutdown()
{
    this->m_retCode=3;
    this->accept();
}
void ZShutDwnDia::ZSloCancel()
{
    this->m_retCode=4;
    this->accept();
}
