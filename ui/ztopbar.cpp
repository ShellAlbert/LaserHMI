#include "ztopbar.h"
#include <QDateTime>
#include "gbl/hmi_const.h"
#include <QEvent>
#include <QDebug>

ZGblErrMsgDia::ZGblErrMsgDia()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(500,450);
    this->m_llTitle=new QLabel;
    this->m_llTitle->setObjectName("ZGblErrMsgDiaLabel");
    this->m_llTitle->setText(tr("警告和错误信息列表"));

    this->m_lstItem=new QListWidget;

    for(qint32 i=0;i<14;i++)
    {
        QListWidgetItem *item=new QListWidgetItem;
        switch(i)
        {
        case 0:
            item->setText(tr("所有警报"));
            break;
        case 1:
            item->setText(tr("PLaser通信"));
            break;
        case 2:
            item->setText(tr("1#水冷机"));
            break;
        case 3:
            item->setText(tr("2#水冷机"));
            break;
        case 4:
            item->setText(tr("GLaser-1"));
            break;
        case 5:
            item->setText(tr("GLaser-2"));
            break;
        case 6:
            item->setText(tr("GLaser-3"));
            break;
        case 7:
            item->setText(tr("GLaser-4"));
            break;
        case 8:
            item->setText(tr("GLaser-5"));
            break;
        case 9:
            item->setText(tr("GLaser-6"));
            break;
        case 10:
            item->setText(tr("GLaser-7"));
            break;
        case 11:
            item->setText(tr("GLaser-8"));
            break;
        case 12:
            item->setText(tr("系统存储"));
            break;
        case 13:
            item->setText(tr("网络"));
            break;
        default:
            break;
        }
        this->m_lstItem->addItem(item);
    }


    this->m_lstMsg=new QListWidget;
    this->m_hLayoutList=new QHBoxLayout;
    this->m_hLayoutList->addWidget(this->m_lstItem);
    this->m_hLayoutList->addWidget(this->m_lstMsg);
    this->m_hLayoutList->setStretch(0,3);
    this->m_hLayoutList->setStretch(1,7);

    this->m_tbClear=new QToolButton;
    this->m_tbClear->setText(tr("清空"));
    this->m_tbClose=new QToolButton;
    this->m_tbClose->setText(tr("关闭"));
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_tbClear);
    this->m_hLayout->addWidget(this->m_tbClose);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_llTitle);
    this->m_vLayout->addLayout(this->m_hLayoutList);
    this->m_vLayout->addLayout(this->m_hLayout);

    this->m_tbClear->setObjectName("ZGblErrMsgDiaBtn");
    this->m_tbClose->setObjectName("ZGblErrMsgDiaBtn");
    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZGblErrMsgDia");
    this->m_frm->setLayout(this->m_vLayout);
    this->m_vLayoutFrm=new QVBoxLayout;
    this->m_vLayoutFrm->addWidget(this->m_frm);
    this->m_vLayoutFrm->setContentsMargins(0,0,0,0);
    this->setLayout(this->m_vLayoutFrm);

    connect(this->m_lstItem,SIGNAL(clicked(QModelIndex)),this,SLOT(ZSlotShowDetailErrList(QModelIndex)));
    connect(this->m_tbClear,SIGNAL(clicked(bool)),this,SLOT(ZSlotClear()));
    connect(this->m_tbClose,SIGNAL(clicked(bool)),this,SLOT(ZSlotClose()));
    this->ZSlotUpdateErrList();

    this->m_timer=new QTimer;
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(ZSlotUpdateErrList()));
    this->m_timer->start(1000);
}
ZGblErrMsgDia::~ZGblErrMsgDia()
{
    this->m_timer->stop();
    delete this->m_timer;

    delete this->m_llTitle;
    this->m_lstItem->clear();
    delete this->m_lstItem;
    this->m_lstMsg->clear();
    delete this->m_lstMsg;
    delete this->m_hLayoutList;
    delete this->m_tbClear;
    delete this->m_tbClose;
    delete this->m_hLayout;
    delete this->m_vLayout;
    delete this->m_frm;
    delete this->m_vLayoutFrm;
}
void ZGblErrMsgDia::ZSlotClose()
{
    this->accept();
}
void ZGblErrMsgDia::ZSlotClear()
{
    g_GblHelp.m_CommunicateErrFlag=0;
    for(qint32 i=0;i<8;i++)
    {
        g_GblHelp.m_GLaserErrFlag[i]=0;
    }
    for(qint32 i=0;i<2;i++)
    {
        g_GblHelp.m_ChillerErrFlag[i]=0;
    }
    g_GblHelp.m_bGblErr=false;
}
void ZGblErrMsgDia::ZSlotUpdateErrList()
{
    bool bGblErr=false;
    if(g_GblHelp.m_CommunicateErrFlag)
    {
        bGblErr=true;
    }
    for(qint32 i=0;i<8;i++)
    {
        if(g_GblHelp.m_GLaserErrFlag[i])
        {
            bGblErr=true;
            break;
        }
    }
    for(qint32 i=0;i<2;i++)
    {
        if(g_GblHelp.m_ChillerErrFlag[i])
        {
            bGblErr=true;
            break;
        }
    }
    if(bGblErr)
    {
        this->m_lstItem->item(0)->setIcon(QIcon(":/topbar/images/led/led_red.png"));
    }else{
        this->m_lstItem->item(0)->setIcon(QIcon(":/topbar/images/led/led_green.png"));
    }
    //communication.
    if(g_GblHelp.m_CommunicateErrFlag)
    {
        this->m_lstItem->item(1)->setIcon(QIcon(":/topbar/images/led/led_red.png"));
    }else{
        this->m_lstItem->item(1)->setIcon(QIcon(":/topbar/images/led/led_green.png"));
    }
    //no.1 chiller.
    if(g_GblHelp.m_ChillerErrFlag[0])
    {
        this->m_lstItem->item(2)->setIcon(QIcon(":/topbar/images/led/led_red.png"));
    }else{
        this->m_lstItem->item(2)->setIcon(QIcon(":/topbar/images/led/led_green.png"));
    }
    //no.2 chiller.
    if(g_GblHelp.m_ChillerErrFlag[1])
    {
        this->m_lstItem->item(3)->setIcon(QIcon(":/topbar/images/led/led_red.png"));
    }else{
        this->m_lstItem->item(3)->setIcon(QIcon(":/topbar/images/led/led_green.png"));
    }
    //GLaser-1.
    for(qint32 i=0;i<8;i++)
    {
        if(g_GblHelp.m_GLaserErrFlag[i])
        {
            this->m_lstItem->item(4+i)->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        }else{
            this->m_lstItem->item(4+i)->setIcon(QIcon(":/topbar/images/led/led_green.png"));
        }
    }
    //storage.
    this->m_lstItem->item(12)->setIcon(QIcon(":/topbar/images/led/led_green.png"));
    //network.
    this->m_lstItem->item(13)->setIcon(QIcon(":/topbar/images/led/led_green.png"));

    //show all alarms.
    this->ZSlotShowDetailErrList(this->m_lstItem->currentIndex());
}
void ZGblErrMsgDia::ZSlotShowDetailErrList(QModelIndex index)
{
    this->m_lstMsg->clear();
    switch(index.row())
    {
    case 0://所有警报
        this->ZFunCommunicateErrList();
        for(qint32 i=0;i<2;i++)
        {
            this->ZFunChillerErrList(i);
        }
        for(qint32 i=0;i<8;i++)
        {
            this->ZFunGLaserErrList(i);
        }
        this->ZFunStorageErrList();
        this->ZFunNetworkErrList();
        break;
    case 1://PLaser通信.
        this->ZFunCommunicateErrList();
        break;
    case 2://1#水冷机
        this->ZFunChillerErrList(0);
        break;
    case 3://2#水冷机.
        this->ZFunChillerErrList(1);
        break;
    case 4://GLaser-1.
        this->ZFunGLaserErrList(0);
        break;
    case 5://GLaser-2.
        this->ZFunGLaserErrList(1);
        break;
    case 6://GLaser-3.
        this->ZFunGLaserErrList(2);
        break;
    case 7://GLaser-4.
        this->ZFunGLaserErrList(3);
        break;
    case 8://GLaser-5.
        this->ZFunGLaserErrList(4);
        break;
    case 9://GLaser-6.
        this->ZFunGLaserErrList(5);
        break;
    case 10://GLaser-7.
        this->ZFunGLaserErrList(6);
        break;
    case 11://GLaser-8.
        this->ZFunGLaserErrList(7);
        break;
    case 12://storage.
        this->ZFunStorageErrList();
        break;
    case 13://network.
        this->ZFunNetworkErrList();
        break;
    }
}
void ZGblErrMsgDia::ZFunCommunicateErrList()
{
    //connection lost.
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<0))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("HMI丢失与PLaser的连接!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<1))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("通信超时,PLaser无应答!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<2))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("通信自检失败!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<3))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("无效同步数据帧头!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<4))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("数据帧长度错误!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<5))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("CRC16校验码错误!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<6))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("通讯地址错误!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<7))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("接收到未定义命令字!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_CommunicateErrFlag&(0x1<<8))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(tr("接收到长度不正确的指令!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
}
void ZGblErrMsgDia::ZFunChillerErrList(qint32 index)
{
    if(index<0 || index>1)
    {
        return;
    }
    //[0]:self-protect.
    //[1]:dong shui liuliang.
    //[2]:diya fuliyang sensor.
    //[3]:gaoya fuliyang sensor.
    //[4]:dongshui temp sensor.
    //[5]:ambient temp sensor.
    //[6]:diya fuliyang.
    //[7]:gaoya fuliyang.
    //[8]:dongshui diwen.
    //[9]:dong shui gaowen.
    //[10]:water level.
    //[11]:xiang xu.
    //[12]:compressor
    //[13]:motor
    //[14]:fan.
    QString chillerNo=QString("%1号:").arg(index+1);
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<0))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("自保护!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<1))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("冻水流量异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<2))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("低压氟利昂传感器异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<3))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("高压氟利昂传感器异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<4))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("冻水温度传感器异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<5))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("环境温度传感器异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<6))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("低压氟利昂异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<7))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("高压氟利昂异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<8))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("冻水低温异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<9))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("冻水高温异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<10))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("水位异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<11))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("相序异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<12))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("压缩机异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<13))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("马达异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<14))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("风扇异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_ChillerErrFlag[index]&(0x1<<15))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("通信故障,PLaser无法连接水冷机!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
}

void ZGblErrMsgDia::ZFunGLaserErrList(qint32 index)
{
    //[15]:communicate fault.
    //[14]:blue over temperature.
    //[13]:blue over voltage.
    //[12]:blue over current.
    //[11]:green over temperature.
    //[10]:green over voltage.
    //[9]:green over current.
    //[8]:red over temperature.
    //[7]:red over voltage.
    //[6]:red over current.
    //[5]:fiber fault.
    //[4]:white balance fault.
    if(index<0 || index>7)
    {
        return;
    }
    QString chillerNo=QString("%1号:").arg(index+1);
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<15))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("PLaser与GLaser通信异常!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
        return;//bypass all other faults.
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<14))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Blue过温!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<13))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Blue过压!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<12))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Blue过流!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<11))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Green过温!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<10))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Green过压!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<9))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Green过流!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<8))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Red过温!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<7))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Red过压!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<6))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("Red过流!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<5))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("光纤故障!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
    if(g_GblHelp.m_GLaserErrFlag[index]&(0x1<<4))
    {
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(chillerNo+tr("白平衡故障!"));
        item->setIcon(QIcon(":/topbar/images/led/led_red.png"));
        this->m_lstMsg->addItem(item);
    }
}
void ZGblErrMsgDia::ZFunStorageErrList()
{

}
void ZGblErrMsgDia::ZFunNetworkErrList()
{

}
ZTopBar::ZTopBar()
{
    this->setObjectName("topBar");
    this->m_nHBIndex=1;
}
ZTopBar::~ZTopBar()
{

}
void ZTopBar::ZDoInitial()
{
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->setMargin(10);
    this->setLayout(this->m_hLayout);

    this->m_llCinemaName=new QLabel;
    this->m_llCinemaName->setObjectName("ZTopBarCinemaNameLabel");
    this->m_llCinemaName->setText(g_GblHelp.m_cinemaName);
    this->m_llCinemaName->setAlignment(Qt::AlignCenter);
    this->m_llDeviceNo=new QLabel;
    this->m_llDeviceNo->setText(g_GblHelp.m_devNo);
    this->m_llDeviceNo->setAlignment(Qt::AlignCenter);
    this->m_llPageName=new QLabel(tr("> Login"));
    this->m_llPageName->setAlignment(Qt::AlignCenter);
    this->m_llSysTime=new QLabel;
    this->m_llSysTime->setAlignment(Qt::AlignCenter);
    this->m_llHeartBeat=new QLabel;
    this->m_llHeartBeat->setPixmap(QPixmap(":/topbar/images/topbar/signal0.png"));
    this->m_llGblErr=new QLabel;
    this->m_llGblErr->setPixmap(QPixmap(":/topbar/images/led/led_green.png").scaled(20,20));
    this->m_llGblErr->installEventFilter(this);

    this->m_llDeviceNo->setObjectName("topBarLabel");
    this->m_llPageName->setObjectName("topBarLabel");
    this->m_llSysTime->setObjectName("topBarLabel");
    this->m_hLayout->addWidget(this->m_llCinemaName);
    this->m_hLayout->addWidget(this->m_llDeviceNo);
    this->m_hLayout->addStretch();
    this->m_hLayout->addWidget(this->m_llPageName);
    this->m_hLayout->addStretch();
    this->m_hLayout->addWidget(this->m_llSysTime);
    this->m_hLayout->addWidget(this->m_llHeartBeat);
    this->m_hLayout->addWidget(this->m_llGblErr);


    connect(this,SIGNAL(ZSigShowGblErrDialog()),this,SLOT(ZSloShowGblErrDialog()));

    this->m_nHBTick=0;
    this->ZResetRandomNum();
    this->m_tmTick=new QTimer;
    QObject::connect(this->m_tmTick,SIGNAL(timeout()),this,SLOT(ZSloUptTick()));
    this->m_tmTick->start(1000);

    this->m_nGblErrFlag=0;

    this->m_bProcessEvent=true;
}
void ZTopBar::ZUnInitial()
{
    this->m_tmTick->stop();
    delete this->m_tmTick;

    delete this->m_llCinemaName;
    delete this->m_llDeviceNo;
    delete this->m_llPageName;
    delete this->m_llSysTime;
    delete this->m_llHeartBeat;
    delete this->m_llGblErr;

    delete this->m_hLayout;
}
void ZTopBar::ZSloUptTick(void)
{
    QString tmStr=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    this->m_llSysTime->setText(tmStr);
    //enable heartbeat.
    if(g_GblHelp.m_bEnHeartBeat)
    {
        //heartbeat movie icon.
        switch(this->m_nHBIndex)
        {
        case 1:
            this->m_llHeartBeat->setPixmap(QPixmap(":/topbar/images/topbar/signal1.png"));
            break;
        case 2:
            this->m_llHeartBeat->setPixmap(QPixmap(":/topbar/images/topbar/signal2.png"));
            break;
        case 3:
            this->m_llHeartBeat->setPixmap(QPixmap(":/topbar/images/topbar/signal3.png"));
            break;
        }
        if(++this->m_nHBIndex>=4)
        {
            this->m_nHBIndex=1;
        }

        //if timer is overflow,then emit heartbeat request cmd to mainUI.
        if(this->m_nHBTick>=g_GblHelp.m_nHeartBeatInterval)
        {
            this->m_nHBTick=0;
            emit this->ZSigRequestCmd("topbar",QString("hbNum=%1,").arg(QString::number(this->m_nHBRandNum,10)));
        }else{
            this->m_nHBTick++;
        }
    }else{
        this->m_llHeartBeat->setPixmap(QPixmap(":/topbar/images/topbar/signal0.png"));
    }

    //update the global error indicator.
    if(g_GblHelp.m_bGblErr)
    {
        if(this->m_nGblErrFlag)
        {
            this->m_nGblErrFlag=0;
            this->m_llGblErr->setPixmap(QPixmap(":/topbar/images/led/led_red.png").scaled(20,20));
        }else{
            this->m_nGblErrFlag=1;
            this->m_llGblErr->setPixmap(QPixmap(":/topbar/images/led/led_green.png").scaled(20,20));
        }
    }else{
        this->m_llGblErr->setPixmap(QPixmap(":/topbar/images/led/led_green.png").scaled(20,20));
    }
}
bool ZTopBar::eventFilter(QObject *watched, QEvent *event)
{
    if(this->m_bProcessEvent)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            if(watched==this->m_llGblErr)
            {
                emit this->ZSigShowGblErrDialog();
            }
        }
    }
    return ZBarUI::eventFilter(watched,event);
}
void ZTopBar::ZSloShowGblErrDialog()
{
    this->m_bProcessEvent=false;
    ZGblErrMsgDia dia;
    dia.exec();
    this->m_bProcessEvent=true;
}
void ZTopBar::ZSloUptPageName(QString pageName)
{
    this->m_llPageName->setText("> "+pageName);
}
void ZTopBar::ZSloResponseCmdHB(QString arguments)
{
    emit this->ZSigDumpDbgMsg(tr("ZTopBar:getHeartBeat:%1").arg(arguments));
    QStringList itemsList=arguments.split(",");
    for(qint32 i=0;i<itemsList.size();i++)
    {
        QString item=itemsList.at(i);
        QStringList objVal=item.split("=");
        if(objVal.size()!=2)
        {
            //bypass error format <key=value>
            continue;
        }
        if(objVal.at(0)=="hbNum")
        {
            if(objVal.at(1)=="timeout")
            {
                QString errMsg(tr("心跳检测超时!"));
                this->ZPopupMsgAddGblErrMsgList(errMsg);
                this->m_llHeartBeat->setPixmap(QPixmap(":/topbar/images/topbar/signal0.png"));
            }else{
                //check heartBeat num.
                qint32 tRxNum=objVal.at(1).toInt();
                qDebug()<<"RxNum:"<<tRxNum<<",TxNum:"<<this->m_nHBRandNum;
                if(this->m_nHBRandNum+1!=tRxNum)
                {
                    QString errMsg(tr("心跳检测计数器错误!"));
                    this->ZPopupMsgAddGblErrMsgList(errMsg);
                    this->m_llHeartBeat->setPixmap(QPixmap(":/topbar/images/topbar/signal0.png"));
                }else{
                    //generate next heart beat num.
                    this->ZResetRandomNum();
                }
            }
        }else if(objVal.at(0)=="stateFlag")
        {
            qint32 stateFlag=objVal.at(1).toInt();
            if(stateFlag&(0x1<<1))
            {
                emit this->ZSigRequestCmd("getChillerFault","1");
            }
            if(stateFlag&(0x1<<3))
            {
                emit this->ZSigRequestCmd("getChillerFault","2");
            }
            if(stateFlag&(0x1<<6))
            {
                emit this->ZSigRequestCmd("getGLaserData","1");
            }
            if(stateFlag&(0x1<<7))
            {
                emit this->ZSigRequestCmd("getGLaserData","2");
            }
            if(stateFlag&(0x1<<8))
            {
                emit this->ZSigRequestCmd("getGLaserData","3");
            }
            if(stateFlag&(0x1<<9))
            {
                emit this->ZSigRequestCmd("getGLaserData","4");
            }
            if(stateFlag&(0x1<<10))
            {
                emit this->ZSigRequestCmd("getGLaserData","5");
            }
            if(stateFlag&(0x1<<11))
            {
                emit this->ZSigRequestCmd("getGLaserData","6");
            }
            if(stateFlag&(0x1<<12))
            {
                emit this->ZSigRequestCmd("getGLaserData","7");
            }
            if(stateFlag&(0x1<<13))
            {
                emit this->ZSigRequestCmd("getGLaserData","8");
            }
            if(stateFlag&(0x1<<14))
            {
                emit this->ZSigRequestCmd("getGLaserData","9");
            }
            if(stateFlag&(0x1<<15))
            {
                emit this->ZSigRequestCmd("getGLaserData","10");
            }
            if(stateFlag&(0x1<<16))
            {
                emit this->ZSigRequestCmd("getGLaserData","11");
            }
            if(stateFlag&(0x1<<17))
            {
                emit this->ZSigRequestCmd("getGLaserData","12");
            }
            if(stateFlag&(0x1<<18))
            {
                emit this->ZSigRequestCmd("getGLaserData","13");
            }
            if(stateFlag&(0x1<<19))
            {
                emit this->ZSigRequestCmd("getGLaserData","14");
            }
            if(stateFlag&(0x1<<20))
            {
                emit this->ZSigRequestCmd("getGLaserData","15");
            }
            if(stateFlag&(0x1<<21))
            {
                emit this->ZSigRequestCmd("getGLaserData","16");
            }
        }
    }
}
void ZTopBar::ZResetRandomNum()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    this->m_nHBRandNum=qrand()%1000;
    QString randomNumStr=QString::number(this->m_nHBRandNum,10);
    emit this->ZSigDumpDbgMsg(tr("HeartBeat:the new Random Number=%1").arg(QString(randomNumStr.toLatin1().toHex())));
}
void ZTopBar::ZPopupMsgAddGblErrMsgList(QString errMsg)
{
    emit this->ZSigPopupMsg(errMsg);
}
