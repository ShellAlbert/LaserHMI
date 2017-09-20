#include "zlogui.h"

ZLogUI::ZLogUI()
{

}
ZLogUI::~ZLogUI()
{

}
void ZLogUI::ZDoInitial()
{
    this->m_operateLog=new ZOperateLogUI;
    this->m_operateLog->ZDoInitial();
    this->m_laserLog=new ZLaserLogUI;
    this->m_laserLog->ZDoInitial();
    this->m_chillerLog=new ZChillerLogUI;
    this->m_chillerLog->ZDoInitial();
    this->m_tabWidget=new QTabWidget;
    this->m_tabWidget->addTab(this->m_operateLog,QIcon(":/log/images/log/Operate.png"),tr("操作日志"));
    this->m_tabWidget->addTab(this->m_laserLog,QIcon(":/log/images/log/Laser.png"),tr("激光日志"));
    this->m_tabWidget->addTab(this->m_chillerLog,QIcon(":/log/images/log/Chiller.png"),tr("水冷机日志"));
    this->m_tabWidget->setIconSize(QSize(24,24));
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_tabWidget);
    this->setLayout(this->m_vLayout);
}
void ZLogUI::ZUnInitial()
{
    this->m_operateLog->ZUnInitial();
    delete this->m_operateLog;
    this->m_laserLog->ZUnInitial();
    delete this->m_laserLog;
    this->m_chillerLog->ZUnInitial();
    delete this->m_chillerLog;
    delete this->m_tabWidget;
    delete this->m_vLayout;
}
void ZLogUI::ZFetchLogs()
{
    this->m_operateLog->ZFetchLogs();
}
