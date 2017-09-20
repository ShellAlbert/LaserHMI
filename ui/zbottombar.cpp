#include "zbottombar.h"
#include <QMessageBox>
#include <QApplication>
#include "ui/zshutdwndia.h"
#include <QProcess>
#include <gbl/hmi_const.h>
ZBottomBar::ZBottomBar()
{
    this->setObjectName("bottomBar");
}
ZBottomBar::~ZBottomBar()
{

}
void ZBottomBar::ZDoInitial()
{
    this->m_llIPAddr=new ZTextImgLabel(tr("辰星研究院"));

    this->m_llRunTime=new ZTextImgLabel(tr("已运行:0天0时0分0秒"));

    this->m_llAuthority=new ZTextImgLabel(tr("授权"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llLinkLED=new ZTextImgLabel(tr("设备"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llTxRxLED=new ZTextImgLabel(tr("通信"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llErrLED=new ZTextImgLabel(tr("解码"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llProjectPwr=new ZTextImgLabel(tr("放映机"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_tbReturn=new QToolButton;
    this->m_tbReturn->setObjectName("ZBottomBarReturnBtn");
    this->m_tbReturn->setIcon(QIcon(":/bottombar/images/bottombar/Exit.png"));
    this->m_tbReturn->setIconSize(QSize(40,40));
    this->m_tbReturn->setVisible(false);

    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->setSpacing(15);
    this->m_hLayout->addWidget(this->m_llIPAddr);
    this->m_hLayout->addWidget(this->m_llRunTime);
    this->m_hLayout->addWidget(this->m_llAuthority);
    this->m_hLayout->addWidget(this->m_llLinkLED);
    this->m_hLayout->addWidget(this->m_llTxRxLED);
    this->m_hLayout->addWidget(this->m_llErrLED);
    this->m_hLayout->addWidget(this->m_llProjectPwr);
    this->m_hLayout->addStretch(1);
    this->m_hLayout->addWidget(this->m_tbReturn);
    this->setLayout(this->m_hLayout);

    connect(this->m_tbReturn,SIGNAL(clicked(bool)),this,SLOT(ZSlotReturn()));
    this->m_runSeconds=0;
    this->m_tmTick=new QTimer;
    connect(this->m_tmTick,SIGNAL(timeout()),this,SLOT(ZSloUptRunTime()));
    this->m_tmTick->start(1000);
}
void ZBottomBar::ZUnInitial()
{
    this->m_tmTick->stop();
    delete this->m_tmTick;

    delete this->m_llIPAddr;
    delete this->m_llRunTime;
    delete this->m_llAuthority;
    delete this->m_llLinkLED;
    delete this->m_llTxRxLED;
    delete this->m_llErrLED;
    delete this->m_llProjectPwr;
    delete this->m_tbReturn;

    delete this->m_hLayout;
}
void ZBottomBar::ZSetReturnButtonVisible(bool visible)
{
    this->m_tbReturn->setVisible(visible);
}
void ZBottomBar::ZSloDecodeDone(qint32 state)
{
    switch(state)
    {
    case -1:
        this->m_llErrLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    case 0:
        this->m_llErrLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 1:
        this->m_llErrLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    }
}
void ZBottomBar::ZSloTxRxDone(qint32 state)
{
    switch(state)
    {
    case -1:
        this->m_llTxRxLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    case 0:
        this->m_llTxRxLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 1:
        this->m_llTxRxLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    }
}
void ZBottomBar::ZSloDevLinkState(qint32 state)
{
    switch(state)
    {
    case -1:
        this->m_llLinkLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    case 0:
        this->m_llLinkLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 1:
        this->m_llLinkLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    }
}
void ZBottomBar::ZSloUptRunTime(void)
{
    this->m_runSeconds++;
    qint32 tDays=this->m_runSeconds/SECONDS_ONE_DAY;
    qint32 tDaysRemain=this->m_runSeconds%SECONDS_ONE_DAY;
    qint32 tHour=tDaysRemain/SECONDS_ONE_HOUR;
    qint32 tHourRemain=tDaysRemain%SECONDS_ONE_HOUR;
    qint32 tMinute=tHourRemain/SECONDS_ONE_MINUTE;
    qint32 tMinuteRemain=tHourRemain%SECONDS_ONE_MINUTE;
    qint32 tSecond=tMinuteRemain;

    this->m_llRunTime->ZSetText(QString("已运行:%1天%2时%3分%4秒").arg(tDays).arg(tHour).arg(tMinute).arg(tSecond));
    if(g_GblHelp.m_projectPwrOn)
    {
        this->m_llProjectPwr->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
    }else{
        this->m_llProjectPwr->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
    }
}
void ZBottomBar::ZSlotReturn()
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::FramelessWindowHint);
    ZShutDwnDia dia;
    dia.exec();
    switch(dia.ZGetRetCode())
    {
    case 1://logout.
        msgBox.setText(tr("Logout Confirmation"));
        msgBox.setText(tr("Are you sure to logout?"));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec()==QMessageBox::Yes)
        {
            emit this->ZSigReturn();
        }
        break;
    case 2://reboot.
        msgBox.setText(tr("Reboot Confirmation"));
        msgBox.setText(tr("Are you sure to reboot?"));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec()==QMessageBox::Yes)
        {
            QProcess p;
            p.start("/sbin/reboot");
            p.waitForFinished();
        }
        break;
    case 3://shutdown.
        msgBox.setText(tr("Shutdown Confirmation"));
        msgBox.setText(tr("Are you sure to shutdown?"));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        if(msgBox.exec()==QMessageBox::Yes)
        {
            QProcess p;
            p.start("/sbin/poweroff");
            p.waitForFinished();
        }
        break;
    case 4://cancel.
        break;
    default:
        break;
    }

}
