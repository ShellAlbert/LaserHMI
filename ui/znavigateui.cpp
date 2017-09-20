#include "znavigateui.h"
#include <gbl/hmi_const.h>
ZNavigateUI::ZNavigateUI()
{
    this->setObjectName("navigateUI");
}
ZNavigateUI::~ZNavigateUI()
{

}

void ZNavigateUI::ZDoInitial()
{
    this->m_tbSummary=new QToolButton;
    this->m_tbSummary->setText(tr("运行状态"));
    this->m_tbSummary->setIcon(QIcon(":/navigate/images/navigate/status.png"));
    this->m_tbSummary->setIconSize(QSize(32,32));
    this->m_tbSummary->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbLaser=new QToolButton;
    this->m_tbLaser->setText(tr("激光控制"));
    this->m_tbLaser->setIconSize(QSize(32,32));
    this->m_tbLaser->setIcon(QIcon(":/navigate/images/navigate/laser.png"));
    this->m_tbLaser->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbChiller=new QToolButton;
    this->m_tbChiller->setText(tr(" 水冷机"));
    this->m_tbChiller->setIconSize(QSize(32,32));
    this->m_tbChiller->setIcon(QIcon(":/navigate/images/navigate/chiller.png"));
    this->m_tbChiller->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbSystem=new QToolButton;
    this->m_tbSystem->setText(tr("系统信息"));
    this->m_tbSystem->setIconSize(QSize(32,32));
    this->m_tbSystem->setIcon(QIcon(":/navigate/images/navigate/sysinfo.png"));
    this->m_tbSystem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbSetup=new QToolButton;
    this->m_tbSetup->setText(tr("系统设置"));
    this->m_tbSetup->setIconSize(QSize(32,32));
    this->m_tbSetup->setIcon(QIcon(":/navigate/images/navigate/setup.png"));
    this->m_tbSetup->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbLog=new QToolButton;
    this->m_tbLog->setText(tr("报警日志"));
    this->m_tbLog->setIconSize(QSize(32,32));
    this->m_tbLog->setIcon(QIcon(":/navigate/images/navigate/log.png"));
    this->m_tbLog->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbDebug=new QToolButton;
    this->m_tbDebug->setText(tr("调试测试"));
    this->m_tbDebug->setIconSize(QSize(32,32));
    this->m_tbDebug->setIcon(QIcon(":/navigate/images/navigate/debug.png"));
    this->m_tbDebug->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbNetwork=new QToolButton;
    this->m_tbNetwork->setText(tr("网络管理"));
    this->m_tbNetwork->setIconSize(QSize(32,32));
    this->m_tbNetwork->setIcon(QIcon(":/navigate/images/navigate/network.png"));
    this->m_tbNetwork->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbAbout=new QToolButton;
    this->m_tbAbout->setText(tr("关于系统"));
    this->m_tbAbout->setIconSize(QSize(32,32));
    this->m_tbAbout->setIcon(QIcon(":/navigate/images/navigate/about.png"));
    this->m_tbAbout->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    this->m_tbSummary->setObjectName("navigateUIButton");
    this->m_tbLaser->setObjectName("navigateUIButton");
    this->m_tbChiller->setObjectName("navigateUIButton");
    this->m_tbSystem->setObjectName("navigateUIButton");
    this->m_tbSetup->setObjectName("navigateUIButton");
    this->m_tbLog->setObjectName("navigateUIButton");
    this->m_tbDebug->setObjectName("navigateUIButton");
    this->m_tbNetwork->setObjectName("navigateUIButton");
    this->m_tbAbout->setObjectName("navigateUIButton");

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_tbSummary);
    this->m_vLayout->addWidget(this->m_tbLaser);
    this->m_vLayout->addWidget(this->m_tbChiller);
    this->m_vLayout->addWidget(this->m_tbSystem);
    this->m_vLayout->addWidget(this->m_tbSetup);
    this->m_vLayout->addWidget(this->m_tbLog);
    this->m_vLayout->addWidget(this->m_tbDebug);
    this->m_vLayout->addWidget(this->m_tbNetwork);
    this->m_vLayout->addWidget(this->m_tbAbout);
    this->m_vLayout->addStretch();

    this->setLayout(this->m_vLayout);

    connect(this->m_tbSummary,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbLaser,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbChiller,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbSystem,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbSetup,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbLog,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbDebug,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbNetwork,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
    connect(this->m_tbAbout,SIGNAL(clicked(bool)),this,SLOT(ZSloModuleClicked()));
}
void ZNavigateUI::ZUnInitial()
{
    delete this->m_tbSummary;
    delete this->m_tbLaser;
    delete this->m_tbChiller;
    delete this->m_tbSystem;
    delete this->m_tbSetup;
    delete this->m_tbLog;
    delete this->m_tbDebug;
    delete this->m_tbNetwork;
    delete this->m_tbAbout;

    delete this->m_vLayout;
}
void ZNavigateUI::ZSloModuleClicked(void)
{
    QString moduleName("Unknown");
    QToolButton *sigSrc=qobject_cast<QToolButton*>(this->sender());
    g_GblHelp.beep();
    if(sigSrc==this->m_tbSummary)
    {
        moduleName=QString("Summary");
    }else if(sigSrc==this->m_tbLaser)
    {
        moduleName=QString("Laser");
    }else if(sigSrc==this->m_tbChiller)
    {
        moduleName=QString("Chiller");
    }else if(sigSrc==this->m_tbSystem)
    {
        moduleName=QString("System");
    }else if(sigSrc==this->m_tbSetup)
    {
        moduleName=QString("Setup");
    }else if(sigSrc==this->m_tbLog)
    {
        moduleName=QString("Log");
    }else if(sigSrc==this->m_tbDebug)
    {
        moduleName=QString("Debug");
    }else if(sigSrc==this->m_tbNetwork)
    {
        moduleName=QString("Network");
    }else if(sigSrc==this->m_tbAbout)
    {
        moduleName=QString("About");
    }
    emit this->ZSigModuleClicked(moduleName);
}
