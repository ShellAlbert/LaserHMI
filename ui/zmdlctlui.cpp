#include "zmdlctlui.h"
#include <QToolTip>
#include <QDebug>
#include <gbl/hmi_const.h>
#include <QDateTime>
#include <QEvent>
#include <ui/zcolortemptuningdia.h>
#include <ui/zpwrprecisionsetdialog.h>
#include <ui/zwarningtipsdialog.h>
ZMdlColorTempSetDia::ZMdlColorTempSetDia(quint8 glaserAddr,float fPowerPercent,QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_glaserAddr=glaserAddr;
    this->m_fPowerPercent=fPowerPercent;

    this->m_llTitle=new QLabel;
    this->m_llTitle->setObjectName("ZMdlColorTempSetDiaLabel");
    this->m_llTitle->setText(title);
    for(qint32 i=0;i<7;i++)
    {
        this->m_tbColorTemp[i]=new QToolButton;
        this->m_tbColorTemp[i]->installEventFilter(this);
        this->m_tbColorTemp[i]->setObjectName("ZMdlColorTempSetDiaButton");
        switch(i)
        {
        case 0:
            this->m_tbColorTemp[i]->setText(tr("D63"));
            break;
        case 1:
            this->m_tbColorTemp[i]->setText(tr("D65"));
            break;
        case 2:
            this->m_tbColorTemp[i]->setText(tr("DCI"));
            break;
        case 3:
            this->m_tbColorTemp[i]->setText(tr("D80"));
            break;
        case 4:
            this->m_tbColorTemp[i]->setText(tr("D93"));
            break;
        case 5:
            this->m_tbColorTemp[i]->setText(tr("D95"));
            break;
        case 6:
            this->m_tbColorTemp[i]->setText(tr("取消"));
            break;
        default:
            break;
        }
    }
    this->m_llOpTips=new QLabel(tr("<长按5秒进入编辑状态>"));
    this->m_llOpTips->setObjectName("ZMdlColorTempSetDiaLabelTips");
    this->m_gLayout=new QGridLayout;
    this->m_gLayout->setContentsMargins(6,6,6,6);
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,3,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tbColorTemp[0],1,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tbColorTemp[1],1,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tbColorTemp[2],1,2,1,1,Qt::AlignCenter);

    this->m_gLayout->addWidget(this->m_tbColorTemp[3],2,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tbColorTemp[4],2,1,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tbColorTemp[5],2,2,1,1,Qt::AlignCenter);

    this->m_gLayout->addWidget(this->m_llOpTips,3,0,1,2,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tbColorTemp[6],3,2,1,1,Qt::AlignCenter);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZMdlColorTempSetDia");
    this->m_frm->setLayout(this->m_gLayout);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_frm);
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->m_vLayout);

    this->m_bLongPressed=false;
    this->m_longPressCnt=0;
    this->m_timer=new QTimer;
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(ZSlotTimeout1s()));
    this->m_timer->start(1000);

    connect(this,SIGNAL(ZSigShowRGBPercentSetDia()),this,SLOT(ZSlotShowRGBPercentSetDia()));
}
ZMdlColorTempSetDia::~ZMdlColorTempSetDia()
{
    delete this->m_llTitle;
    delete this->m_llOpTips;
    for(qint32 i=0;i<7;i++)
    {
        delete this->m_tbColorTemp[i];
    }
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
QString ZMdlColorTempSetDia::ZGetColorTemp()
{
    return this->m_colorTemp;
}
bool ZMdlColorTempSetDia::eventFilter(QObject *watched, QEvent *event)
{
    switch(event->type())
    {
    case QEvent::MouseButtonPress:
        if(watched==this->m_tbColorTemp[0])
        {
            this->m_colorTemp="D63";
            this->m_bLongPressed=true;
        }else if(watched==this->m_tbColorTemp[1])
        {
            this->m_colorTemp="D65";
            this->m_bLongPressed=true;
        }else if(watched==this->m_tbColorTemp[2])
        {
            this->m_colorTemp="DCI";
            this->m_bLongPressed=true;
        }else if(watched==this->m_tbColorTemp[3])
        {
            this->m_colorTemp="D80";
            this->m_bLongPressed=true;
        }else if(watched==this->m_tbColorTemp[4])
        {
            this->m_colorTemp="D93";
            this->m_bLongPressed=true;
        }else if(watched==this->m_tbColorTemp[5])
        {
            this->m_colorTemp="D95";
            this->m_bLongPressed=true;
        }else if(watched==this->m_tbColorTemp[6])
        {
            this->reject();
        }
        break;
    case QEvent::MouseButtonRelease:
        if(watched==this->m_tbColorTemp[0]||watched==this->m_tbColorTemp[1] ///<
                || watched==this->m_tbColorTemp[2] || watched==this->m_tbColorTemp[3] //<
                || watched==this->m_tbColorTemp[4] || watched==this->m_tbColorTemp[5])
        {
            this->accept();
        }
        break;
    default:
        break;
    }
    return QDialog::eventFilter(watched,event);
}
void ZMdlColorTempSetDia::ZSlotTimeout1s()
{
    //long press support.
    if(this->m_bLongPressed)
    {
        this->m_longPressCnt++;
        if(this->m_longPressCnt++>=5)
        {
            emit this->ZSigShowRGBPercentSetDia();
            this->m_bLongPressed=false;
            this->m_longPressCnt=0;
        }
    }
}
void ZMdlColorTempSetDia::ZSlotShowRGBPercentSetDia()
{
    if(this->m_fPowerPercent!=100.0f)
    {
        ZWarningTipsDialog tipsDia(tr("操作警告"),tr("调整色温前需将功率输出设置为100.0%!"));
        tipsDia.exec();
        return;
    }

    g_GblHelp.beep();
    ZColorTempTuningDia dia(this->m_glaserAddr,this->m_colorTemp);
    connect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)),&dia,SLOT(ZSloSyncColorTempPercent(QString,QString)));
    connect(this,SIGNAL(ZSigAdjRGBPercent(QString,QString)),&dia,SLOT(ZSloAdjRGBPercent(QString,QString)));
    connect(&dia,SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
    connect(&dia,SIGNAL(ZSigPopMessage(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    connect(&dia,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    dia.exec();
    disconnect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)),&dia,SLOT(ZSloSyncColorTempPercent(QString,QString)));
    disconnect(this,SIGNAL(ZSigAdjRGBPercent(QString,QString)),&dia,SLOT(ZSloAdjRGBPercent(QString,QString)));
    disconnect(&dia,SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
    disconnect(&dia,SIGNAL(ZSigPopMessage(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    disconnect(&dia,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
}
ZMdlFaultDialog::ZMdlFaultDialog(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setObjectName("ZMdlFaultDialogLabel");
    this->m_llTitle->setText(title);

    this->m_llFiberLED=new ZTextImgLabel(tr("光纤"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llWhiteBalanceLED=new ZTextImgLabel(tr("白平衡"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_OverIVT[0]=new ZTextImgLabel(tr("R过流"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[1]=new ZTextImgLabel(tr("R过压"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[2]=new ZTextImgLabel(tr("R过温"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_OverIVT[3]=new ZTextImgLabel(tr("G过流"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[4]=new ZTextImgLabel(tr("G过压"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[5]=new ZTextImgLabel(tr("G过温"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_OverIVT[6]=new ZTextImgLabel(tr("B过流"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[7]=new ZTextImgLabel(tr("B过压"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[8]=new ZTextImgLabel(tr("B过温"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_tbClose=new QToolButton;
    this->m_tbClose->setObjectName("ZMdlFaultDialogButton");
    this->m_tbClose->setText(tr("确定"));
    connect(this->m_tbClose,SIGNAL(clicked(bool)),this,SLOT(accept()));

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,3);
    this->m_gLayout->addWidget(this->m_llFiberLED,1,0,1,1);
    this->m_gLayout->addWidget(this->m_llWhiteBalanceLED,1,1,1,1);

    this->m_gLayout->addWidget(this->m_OverIVT[0],2,0,1,1);
    this->m_gLayout->addWidget(this->m_OverIVT[3],2,1,1,1);
    this->m_gLayout->addWidget(this->m_OverIVT[6],2,2,1,1);

    this->m_gLayout->addWidget(this->m_OverIVT[1],3,0,1,1);
    this->m_gLayout->addWidget(this->m_OverIVT[4],3,1,1,1);
    this->m_gLayout->addWidget(this->m_OverIVT[7],3,2,1,1);

    this->m_gLayout->addWidget(this->m_OverIVT[2],4,0,1,1);
    this->m_gLayout->addWidget(this->m_OverIVT[5],4,1,1,1);
    this->m_gLayout->addWidget(this->m_OverIVT[8],4,2,1,1);

    this->m_gLayout->addWidget(this->m_tbClose,5,2,1,1,Qt::AlignCenter);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZMdlFaultDialog");
    this->m_frm->setLayout(this->m_gLayout);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_frm);
    this->m_vLayout->setContentsMargins(4,4,4,4);
    this->setLayout(this->m_vLayout);
}
ZMdlFaultDialog::~ZMdlFaultDialog()
{
    delete this->m_llTitle;
    delete this->m_llFiberLED;
    delete this->m_llWhiteBalanceLED;
    for(qint32 i=0;i<9;i++)
    {
        delete this->m_OverIVT[i];
    }
    delete this->m_tbClose;
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
void ZMdlFaultDialog::ZSloMdlFault(qint32 fiber,qint32 wb,qint32 rc,qint32 rv,qint32 rt,qint32 gc,qint32 gv,qint32 gt,qint32 bc,qint32 bv,qint32 bt)
{
    //0:yellow,1:green,2:red.
    switch(fiber)
    {
    case 0:
        this->m_llFiberLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_llFiberLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_llFiberLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(wb)
    {
    case 0:
        this->m_llWhiteBalanceLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_llWhiteBalanceLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_llWhiteBalanceLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(rc)
    {
    case 0:
        this->m_OverIVT[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(rv)
    {
    case 0:
        this->m_OverIVT[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(rt)
    {
    case 0:
        this->m_OverIVT[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(gc)
    {
    case 0:
        this->m_OverIVT[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(gv)
    {
    case 0:
        this->m_OverIVT[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(gt)
    {
    case 0:
        this->m_OverIVT[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(bc)
    {
    case 0:
        this->m_OverIVT[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(bv)
    {
    case 0:
        this->m_OverIVT[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(bt)
    {
    case 0:
        this->m_OverIVT[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
}
ZMdlCtlUI::ZMdlCtlUI(qint32 moduleNo)
{
    this->setObjectName("mdlCtlUI");
    this->m_moduleNo=moduleNo;
    this->m_nTuningBits=0;

    //default white balance value to 0.
    this->m_redWBValue=0;
    this->m_greenWBValue=0;
    this->m_blueWBValue=0;
}
ZMdlCtlUI::~ZMdlCtlUI()
{

}

void ZMdlCtlUI::ZDoInitial()
{
    //status.
    this->m_leRedIV=new ZTextLineEditLabel(tr("红光"),"0.0A","0.0V",QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_leGreenIV=new ZTextLineEditLabel(tr("绿光"),"0.0A","0.0V",QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_leBlueIV=new ZTextLineEditLabel(tr("蓝光"),"0.0A","0.0V",QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llFaultLED=new ZTextImgLabel(tr("故障"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_leOpticalPower=new ZTextLineEditLabel(tr("白平衡"),("0/0/0"));
    this->m_leRunTime=new ZTextLineEditLabel(tr("累计时长"),"0h0m");
    this->m_leColorTemp=new ZTextLineEditLabel(tr("色温"),"D63");
    connect(this->m_leColorTemp,SIGNAL(ZSigClicked()),this,SLOT(ZSloShowMdlColorTempDialog()));
    this->m_leColorTemp->installEventFilter(this);
    this->m_llWBLED=new ZTextImgLabel(tr("白平衡"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_gLayoutStatus=new QGridLayout;
    this->m_gLayoutStatus->addWidget(this->m_leRedIV,0,0,1,1);
    this->m_gLayoutStatus->addWidget(this->m_leGreenIV,0,1,1,1);
    this->m_gLayoutStatus->addWidget(this->m_leBlueIV,0,2,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llFaultLED,0,3,1,1);

    this->m_gLayoutStatus->addWidget(this->m_leRunTime,1,0,1,1);
    this->m_gLayoutStatus->addWidget(this->m_leOpticalPower,1,1,1,1);
    this->m_gLayoutStatus->addWidget(this->m_leColorTemp,1,2,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llWBLED,1,3,1,1);
    this->m_gLayoutStatus->setContentsMargins(2,20,2,2);

    this->m_frmStatus=new ZFrame(tr("状态数据"));
    this->m_frmStatus->setLayout(this->m_gLayoutStatus);

    //temperature.
    this->m_leR1Temp=new ZTextLineEditLabel(tr("R1"),"0 ℃");
    this->m_leR2Temp=new ZTextLineEditLabel(tr("R2"),"0 ℃");
    this->m_leBTemp=new ZTextLineEditLabel(tr("B1"),"0 ℃");
    this->m_leG1Temp=new ZTextLineEditLabel(tr("G1"),"0 ℃");
    this->m_leG2Temp=new ZTextLineEditLabel(tr("G2"),"0 ℃");
    this->m_leAtTemp=new ZTextLineEditLabel(tr("A1"),"0 ℃");

    this->m_gLayoutTemp=new QGridLayout;
    this->m_gLayoutTemp->addWidget(this->m_leR1Temp,0,0,1,1);
    this->m_gLayoutTemp->addWidget(this->m_leR2Temp,1,0,1,1);
    this->m_gLayoutTemp->addWidget(this->m_leBTemp,2,0,1,1);
    this->m_gLayoutTemp->addWidget(this->m_leG1Temp,3,0,1,1);
    this->m_gLayoutTemp->addWidget(this->m_leG2Temp,4,0,1,1);
    this->m_gLayoutTemp->addWidget(this->m_leAtTemp,5,0,1,1);
    this->m_gLayoutTemp->setContentsMargins(2,20,2,2);
    this->m_frmTemp=new ZFrame(tr("温度监控"));
    this->m_frmTemp->setLayout(this->m_gLayoutTemp);

    //Control.
    this->m_tbAllOn=new QToolButton;
    this->m_tbAllOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_All.png"));
    this->m_tbAllOn->setIconSize(QSize(48,48));
    this->m_tbAllOn->setText(tr("全部开启"));
    this->m_tbAllOn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbRedOn=new QToolButton;
    this->m_tbRedOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Red.png"));
    this->m_tbRedOn->setIconSize(QSize(48,48));
    this->m_tbRedOn->setText(tr("开启红光"));
    this->m_tbRedOn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbGreenOn=new QToolButton;
    this->m_tbGreenOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Green.png"));
    this->m_tbGreenOn->setIconSize(QSize(48,48));
    this->m_tbGreenOn->setText(tr("开启绿光"));
    this->m_tbGreenOn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbBlueOn=new QToolButton;
    this->m_tbBlueOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Blue.png"));
    this->m_tbBlueOn->setIconSize(QSize(48,48));
    this->m_tbBlueOn->setText(tr("开启蓝光"));
    this->m_tbBlueOn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbWhiteBalanceOn=new QToolButton;
    this->m_tbWhiteBalanceOn->setIcon(QIcon(":/mainctl/images/mainctl/WhiteBalance.png"));
    this->m_tbWhiteBalanceOn->setIconSize(QSize(48,48));
    this->m_tbWhiteBalanceOn->setText(tr("开启白平衡"));
    this->m_tbWhiteBalanceOn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbAllOn->setObjectName("ZMdlCtlUIButton");
    this->m_tbRedOn->setObjectName("ZMdlCtlUIButton");
    this->m_tbGreenOn->setObjectName("ZMdlCtlUIButton");
    this->m_tbBlueOn->setObjectName("ZMdlCtlUIButton");
    this->m_tbWhiteBalanceOn->setObjectName("ZMdlCtlUIButton");

    this->m_gLayoutControl=new QGridLayout;
    this->m_gLayoutControl->addWidget(this->m_tbAllOn,0,0,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbRedOn,0,1,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbGreenOn,0,2,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbBlueOn,0,3,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbWhiteBalanceOn,0,4,1,1);
    //this->m_gLayoutControl->setContentsMargins(5,20,5,2);
    this->m_frmControl=new QFrame;
    this->m_frmControl->setLayout(this->m_gLayoutControl);

    //Tuning.
    this->m_tbAllDec=new QToolButton;
    this->m_tbAllDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbAllDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    this->m_tbAllDec->setIconSize(QSize(48,48));
    this->m_tbAllInc=new QToolButton;
    this->m_tbAllInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbAllInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    this->m_tbAllInc->setIconSize(QSize(48,48));

    this->m_pbAll=new ZVerticalIndicator;
    this->m_pbAll->SetGraphColor(Qt::white);
    this->m_pbAll->setValue(50.0f);
    this->m_pbAll->installEventFilter(this);

    this->m_gLayoutFrmTuningAll=new QGridLayout;
    this->m_gLayoutFrmTuningAll->addWidget(this->m_pbAll,0,0,1,2);
    this->m_gLayoutFrmTuningAll->addWidget(this->m_tbAllInc,1,0,1,1);
    this->m_gLayoutFrmTuningAll->addWidget(this->m_tbAllDec,1,1,1,1);
    this->m_frmTuningAll=new QFrame;
    this->m_frmTuningAll->setLayout(this->m_gLayoutFrmTuningAll);


    this->m_tbRedDec=new QToolButton;
    this->m_tbRedDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbRedDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    this->m_tbRedDec->setIconSize(QSize(48,48));
    this->m_tbRedInc=new QToolButton;
    this->m_tbRedInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbRedInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    this->m_tbRedInc->setIconSize(QSize(48,48));

    this->m_pbRed=new ZVerticalIndicator;
    this->m_pbRed->SetGraphColor(Qt::red);
    this->m_pbRed->setValue(48.0f);
    this->m_pbRed->installEventFilter(this);


    this->m_tbGreenDec=new QToolButton;
    this->m_tbGreenDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbGreenDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    this->m_tbGreenDec->setIconSize(QSize(48,48));
    this->m_tbGreenInc=new QToolButton;
    this->m_tbGreenInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbGreenInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    this->m_tbGreenInc->setIconSize(QSize(48,48));

    this->m_pbGreen=new ZVerticalIndicator;
    this->m_pbGreen->SetGraphColor(Qt::green);
    this->m_pbGreen->setValue(10.8f);
    this->m_pbGreen->installEventFilter(this);

    this->m_tbBlueDec=new QToolButton;
    this->m_tbBlueDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbBlueDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    this->m_tbBlueDec->setIconSize(QSize(48,48));
    this->m_tbBlueInc=new QToolButton;
    this->m_tbBlueInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbBlueInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    this->m_tbBlueInc->setIconSize(QSize(48,48));

    this->m_pbBlue=new ZVerticalIndicator;
    this->m_pbBlue->SetGraphColor(Qt::blue);
    this->m_pbBlue->setValue(89.0f);
    this->m_pbBlue->installEventFilter(this);

    this->m_gLayoutFrmTuningRGB=new QGridLayout;
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_pbRed,0,0,1,2,Qt::AlignCenter);
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_pbGreen,0,2,1,2,Qt::AlignCenter);
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_pbBlue,0,4,1,2,Qt::AlignCenter);

    this->m_gLayoutFrmTuningRGB->addWidget(this->m_tbRedInc,1,0,1,1,Qt::AlignCenter);
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_tbRedDec,1,1,1,1,Qt::AlignCenter);
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_tbGreenInc,1,2,1,1,Qt::AlignCenter);
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_tbGreenDec,1,3,1,1,Qt::AlignCenter);
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_tbBlueInc,1,4,1,1,Qt::AlignCenter);
    this->m_gLayoutFrmTuningRGB->addWidget(this->m_tbBlueDec,1,5,1,1,Qt::AlignCenter);

    this->m_frmTuningRGB=new QFrame;
    this->m_frmTuningRGB->setLayout(this->m_gLayoutFrmTuningRGB);

    this->m_tbAllDec->setObjectName("MdlCtlUIBtnTuning");
    this->m_tbAllInc->setObjectName("MdlCtlUIBtnTuning");
    this->m_tbRedDec->setObjectName("MdlCtlUIBtnTuning");
    this->m_tbRedInc->setObjectName("MdlCtlUIBtnTuning");
    this->m_tbGreenDec->setObjectName("MdlCtlUIBtnTuning");
    this->m_tbGreenInc->setObjectName("MdlCtlUIBtnTuning");
    this->m_tbBlueDec->setObjectName("MdlCtlUIBtnTuning");
    this->m_tbBlueInc->setObjectName("MdlCtlUIBtnTuning");


    //fiber.
    this->m_llFiberBreakLED=new ZTextImgLabel(tr("光纤折断"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_leFiberBreakThreshold=new ZTextLineEditLabel(tr("监控阈值"),"100%");
    connect(this->m_leFiberBreakThreshold,SIGNAL(ZSigClicked()),this,SLOT(ZSlotShowFiberBreakVoltageSetupDialog()));
    this->m_leFiberBreakVoltage=new ZTextLineEditLabel(tr("折断电压"),"0");
    this->m_gLayoutFiber=new QGridLayout;
    this->m_gLayoutFiber->addWidget(this->m_llFiberBreakLED,0,0,1,1);
    this->m_gLayoutFiber->addWidget(this->m_leFiberBreakThreshold,0,1,1,1);
    this->m_gLayoutFiber->addWidget(this->m_leFiberBreakVoltage,1,1,1,1);
    this->m_frmFiber=new QFrame;
    this->m_frmFiber->setLayout(this->m_gLayoutFiber);

    //I&V&T Monitor.
    this->m_OverIVT[0]=new ZTextImgLabel(tr("R过流"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[1]=new ZTextImgLabel(tr("R过压"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[2]=new ZTextImgLabel(tr("R过温"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_OverIVT[3]=new ZTextImgLabel(tr("G过流"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[4]=new ZTextImgLabel(tr("G过压"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[5]=new ZTextImgLabel(tr("G过温"),QPixmap(":/topbar/images/led/led_yellow.png"));

    this->m_OverIVT[6]=new ZTextImgLabel(tr("B过流"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[7]=new ZTextImgLabel(tr("B过压"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_OverIVT[8]=new ZTextImgLabel(tr("B过温"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_gLayoutIVT=new QGridLayout;
    this->m_gLayoutIVT->addWidget(this->m_OverIVT[0],0,0,1,1);
    this->m_gLayoutIVT->addWidget(this->m_OverIVT[1],0,1,1,1);
    this->m_gLayoutIVT->addWidget(this->m_OverIVT[2],0,2,1,1);

    this->m_gLayoutIVT->addWidget(this->m_OverIVT[3],1,0,1,1);
    this->m_gLayoutIVT->addWidget(this->m_OverIVT[4],1,1,1,1);
    this->m_gLayoutIVT->addWidget(this->m_OverIVT[5],1,2,1,1);

    this->m_gLayoutIVT->addWidget(this->m_OverIVT[6],2,0,1,1);
    this->m_gLayoutIVT->addWidget(this->m_OverIVT[7],2,1,1,1);
    this->m_gLayoutIVT->addWidget(this->m_OverIVT[8],2,2,1,1);

    this->m_frmIVT=new QFrame;
    this->m_frmIVT->setLayout(this->m_gLayoutIVT);

    //tab widget.
    this->m_tabTuning=new QTabWidget;
    connect(this->m_tabTuning,SIGNAL(currentChanged(int)),this,SLOT(ZSloTabChanged(qint32)));

    this->m_tabTuning->addTab(this->m_frmControl,QIcon(""),tr("开关控制"));
    this->m_tabTuning->addTab(this->m_frmTuningAll,QIcon(""),tr("整体调节"));
    this->m_tabTuning->addTab(this->m_frmTuningRGB,QIcon(""),tr("RGB调节"));
    this->m_tabTuning->addTab(this->m_frmFiber,QIcon(""),tr("光纤监控"));
    this->m_tabTuning->addTab(this->m_frmIVT,QIcon(""),tr("IVT监控"));
    this->m_vLayoutTuning=new QVBoxLayout;
    this->m_vLayoutTuning->addWidget(this->m_tabTuning);
    this->m_frmTuning=new ZFrame(tr(""));
    this->m_frmTuning->setLayout(this->m_vLayoutTuning);

    //main layout.
    this->m_gLayoutMain=new QGridLayout;
    this->m_gLayoutMain->addWidget(this->m_frmStatus,0,0,1,2);
    this->m_gLayoutMain->addWidget(this->m_frmTuning,1,0,1,1);
    this->m_gLayoutMain->addWidget(this->m_frmTemp,1,1,1,1);
    this->m_gLayoutMain->setColumnStretch(0,4);
    this->m_gLayoutMain->setColumnStretch(1,2);
    this->m_gLayoutMain->setRowStretch(0,1);
    this->m_gLayoutMain->setRowStretch(1,2);
    this->m_gLayoutMain->setContentsMargins(1,1,1,1);
    this->setLayout(this->m_gLayoutMain);

    //make connections.
    //mdl fault.
    connect(this->m_llFaultLED,SIGNAL(ZSigClicked()),this,SLOT(ZSloShowMdlFaultDialog()));

    //control.
    connect(this->m_tbAllOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbRedOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbGreenOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbBlueOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbWhiteBalanceOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));

    //tuning.
    connect(this->m_tbAllDec,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));
    connect(this->m_tbAllInc,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));

    connect(this->m_tbRedDec,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));
    connect(this->m_tbRedInc,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));

    connect(this->m_tbGreenDec,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));
    connect(this->m_tbGreenInc,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));

    connect(this->m_tbBlueDec,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));
    connect(this->m_tbBlueInc,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));


    connect(this,SIGNAL(ZSigPrecisSetupRequest(QString)),this,SLOT(ZSloShowPrecisSetupDialog(QString)));
    connect(this,SIGNAL(ZSigMdlFault(qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32)),this,SLOT(ZSloMdlFault(qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32)));
    //query status timer.
    this->m_nTime1sCounter=0;
    this->m_bHasFault=false;
    this->m_timer1s=new QTimer;
    connect(this->m_timer1s,SIGNAL(timeout()),this,SLOT(ZSloTimeout1s()));
    // this->m_timer1s->start(1000*2);
    this->m_bProcessEvent=true;
}

void ZMdlCtlUI::ZUnInitial()
{
    this->m_timer1s->stop();
    delete this->m_timer1s;

    //status.
    delete this->m_leRedIV;
    delete this->m_leGreenIV;
    delete this->m_leBlueIV;
    delete this->m_llFaultLED;
    delete this->m_leOpticalPower;
    delete this->m_leRunTime;
    delete this->m_leColorTemp;
    delete this->m_llWBLED;

    delete this->m_gLayoutStatus;
    delete this->m_frmStatus;

    //temperature.
    delete this->m_leR1Temp;
    delete this->m_leR2Temp;
    delete this->m_leBTemp;
    delete this->m_leG1Temp;
    delete this->m_leG2Temp;
    delete this->m_leAtTemp;

    delete this->m_gLayoutTemp;
    delete this->m_frmTemp;

    //Control.
    delete this->m_tbAllOn;
    delete this->m_tbRedOn;
    delete this->m_tbGreenOn;
    delete this->m_tbBlueOn;
    delete this->m_tbWhiteBalanceOn;

    delete this->m_gLayoutControl;
    delete this->m_frmControl;

    //Tuning.
    delete this->m_tbAllDec;
    delete this->m_tbAllInc;
    delete this->m_pbAll;

    delete this->m_tbRedDec;
    delete this->m_tbRedInc;
    delete this->m_pbRed;

    delete this->m_tbGreenDec;
    delete this->m_tbGreenInc;
    delete this->m_pbGreen;

    delete this->m_tbBlueDec;
    delete this->m_tbBlueInc;
    delete this->m_pbBlue;

    delete this->m_gLayoutFrmTuningAll;
    delete this->m_frmTuningAll;
    delete this->m_gLayoutFrmTuningRGB;
    delete this->m_frmTuningRGB;
    delete this->m_vLayoutTuning;
    delete this->m_frmTuning;

    //fiber.
    delete this->m_llFiberBreakLED;
    delete this->m_leFiberBreakThreshold;
    delete this->m_leFiberBreakVoltage;
    delete this->m_gLayoutFiber;
    delete this->m_frmFiber;

    //I&V&T monitor.
    for(qint32 i=0;i<9;i++)
    {
        delete this->m_OverIVT[i];
    }
    delete this->m_gLayoutIVT;
    delete this->m_frmIVT;

    //main layout.
    delete this->m_gLayoutMain;
}
bool ZMdlCtlUI::event(QEvent *e)
{
    if(this->m_bProcessEvent)
    {
        switch(e->type())
        {
        case QEvent::Show:
            qDebug()<<"ZMdlCtlUI:   Show Event...........";
            //    case QEvent::Move:
            //        qDebug()<<"ZMdlCtlUI:   Move Event...........";
            //    case QEvent::Paint:
            //        qDebug()<<"ZMdlCtlUI:   Paint Event...........";
            if(!this->m_timer1s->isActive())
            {
                this->m_timer1s->start(1000*g_GblHelp.m_nQueryInterval);
            }
            break;
        case QEvent::Hide:
            qDebug()<<"ZMdlCtlUI:   Hide Event...........";
            this->m_timer1s->stop();
            break;
        default:
            break;
        }
    }
    return ZBaseUI::event(e);
}
void ZMdlCtlUI::ZSloTabChanged(qint32)
{
    g_GblHelp.beep();
}
void ZMdlCtlUI::ZSloOnCtlInternal()
{
    g_GblHelp.beep();
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    if(srcTb==this->m_tbAllOn)
    {
        QString tips=this->m_tbAllOn->text();
        if(tips==QString("全部开启"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"rgbw=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("全部关闭"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"rgbw=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("All command are scheduling,\nplease wait..."));
        }
    }else if(srcTb==this->m_tbRedOn)
    {
        QString tips=this->m_tbRedOn->text();
        if(tips==QString("开启红光"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"red=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭红光"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"red=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("Red command are scheduling,\nplease wait..."));
        }
    }else if(srcTb==this->m_tbGreenOn)
    {
        QString tips=this->m_tbGreenOn->text();
        if(tips==QString("开启绿光"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"green=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭绿光"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"green=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("Green command are scheduling,\nplease wait..."));
        }
    }else if(srcTb==this->m_tbBlueOn)
    {
        QString tips=this->m_tbBlueOn->text();
        if(tips==QString("开启蓝光"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"blue=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭蓝光"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"blue=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("Blue command are scheduling,\nplease wait..."));
        }
    }else if(srcTb==this->m_tbWhiteBalanceOn)
    {
        QString tips=this->m_tbWhiteBalanceOn->text();
        if(tips==QString("开启白平衡"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"wb=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭白平衡"))
        {
            emit this->ZSigRequestCmd(QString("%1_pwr").arg(this->m_moduleNo),"wb=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("WhiteBalance command are scheduling,\nplease wait..."));
        }
    }
}
void ZMdlCtlUI::ZSloPwrPercentTuningInternal()
{
    g_GblHelp.beep();
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    if(srcTb==this->m_tbAllDec)
    {
        if(this->m_nTuningBits&(0x1<<0))
        {
            emit this->ZSigPopupMsg(tr("整体调节命令执行中..."));
            return;
        }
        float tAllVal=this->m_pbAll->getValue();
        if(tAllVal>=0.1f)
        {
            tAllVal-=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n整体功率已达到最小值!\n"));
            return;
        }
        QString argument=QString("rgb=%1").arg((qint32)(tAllVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<0);
    }else if(srcTb==this->m_tbAllInc)
    {
        if(this->m_nTuningBits&(0x1<<1))
        {
            emit this->ZSigPopupMsg(tr("整体调节命令执行中..."));
            return;
        }
        float tAllVal=this->m_pbAll->getValue();
        if(tAllVal<100.0f)
        {
            tAllVal+=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n整体功率已达到最大值!\n"));
            return;
        }
        QString argument=QString("rgb=%1").arg((qint32)(tAllVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<1);
    }else if(srcTb==this->m_tbRedDec)
    {
        if(this->m_nTuningBits&(0x1<<2))
        {
            emit this->ZSigPopupMsg(tr("Red调节命令执行中..."));
            return;
        }
        float tRedVal=this->m_pbRed->getValue();
        if(tRedVal>=0.1f)
        {
            tRedVal-=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n红光功率已达到最小值!\n"));
            return;
        }
        QString argument=QString("red=%1").arg((qint32)(tRedVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<2);
    }else if(srcTb==this->m_tbRedInc)
    {
        if(this->m_nTuningBits&(0x1<<3))
        {
            emit this->ZSigPopupMsg(tr("Red调节命令执行中..."));
            return;
        }
        float tRedVal=this->m_pbRed->getValue();
        if(tRedVal<100.0f)
        {
            tRedVal+=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n红光功率已达到最大值!\n"));
            return;
        }
        QString argument=QString("red=%1").arg((qint32)(tRedVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<3);
    }else if(srcTb==this->m_tbGreenDec)
    {
        if(this->m_nTuningBits&(0x1<<4))
        {
            emit this->ZSigPopupMsg(tr("Green调节命令执行中..."));
            return;
        }
        float tGreenVal=this->m_pbGreen->getValue();
        if(tGreenVal>=0.1f)
        {
            tGreenVal-=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n绿光功率已达到最小值!\n"));
            return;
        }
        QString argument=QString("green=%1").arg((qint32)(tGreenVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<4);
    }else if(srcTb==this->m_tbGreenInc)
    {
        if(this->m_nTuningBits&(0x1<<5))
        {
            emit this->ZSigPopupMsg(tr("Green调节命令执行中..."));
            return;
        }
        float tGreenVal=this->m_pbGreen->getValue();
        if(tGreenVal<100.0f)
        {
            tGreenVal+=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n绿光功率已达到最大值!\n"));
            return;
        }
        QString argument=QString("green=%1").arg((qint32)(tGreenVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<5);
    }else if(srcTb==this->m_tbBlueDec)
    {
        if(this->m_nTuningBits&(0x1<<6))
        {
            emit this->ZSigPopupMsg(tr("Blue调节命令执行中..."));
            return;
        }
        float tBlueVal=this->m_pbBlue->getValue();
        if(tBlueVal>=0.1f)
        {
            tBlueVal-=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n蓝光功率已达到最小值!\n"));
            return;
        }
        QString argument=QString("blue=%1").arg((qint32)(tBlueVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<6);
    }else if(srcTb==this->m_tbBlueInc)
    {
        if(this->m_nTuningBits&(0x1<<7))
        {
            emit this->ZSigPopupMsg(tr("Blue调节命令执行中..."));
            return;
        }
        float tBlueVal=this->m_pbBlue->getValue();
        if(tBlueVal<100.0f)
        {
            tBlueVal+=0.1f;
        }else{
            emit this->ZSigPopupMsg(tr("\n蓝光功率已达到最大值!\n"));
            return;
        }
        QString argument=QString("blue=%1").arg((qint32)(tBlueVal*10.0f));
        emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),argument);
        qDebug()<<argument;
        this->m_nTuningBits|=(0x1<<7);
    }
}
void ZMdlCtlUI::ZSloTimeout1s()
{
    //used to generate movie effection.
    this->m_bCmdRunning=!this->m_bCmdRunning;
    //Pwr.
    if(this->m_tbAllOn->text().right(3)=="...")
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbAllOn->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbAllOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_All.png"));
        }
    }else{
        this->m_tbAllOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_All.png"));
    }
    if(this->m_tbRedOn->text().right(3)=="...")
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbRedOn->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbRedOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Red.png"));
        }
    }else{
        this->m_tbRedOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Red.png"));
    }
    if(this->m_tbGreenOn->text().right(3)=="...")
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbGreenOn->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbGreenOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Green.png"));
        }
    }else{
        this->m_tbGreenOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Green.png"));
    }
    if(this->m_tbBlueOn->text().right(3)=="...")
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbBlueOn->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbBlueOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Blue.png"));
        }
    }else{
        this->m_tbBlueOn->setIcon(QIcon(":/mainctl/images/mainctl/RGB_Blue.png"));
    }
    if(this->m_tbWhiteBalanceOn->text().right(3)=="...")
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbWhiteBalanceOn->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbWhiteBalanceOn->setIcon(QIcon(":/mainctl/images/mainctl/WhiteBalance.png"));
        }
    }else{
        this->m_tbWhiteBalanceOn->setIcon(QIcon(":/mainctl/images/mainctl/WhiteBalance.png"));
    }
    //Tuning.
    if(this->m_nTuningBits&(0x1<<0))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbAllDec->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbAllDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
        }
    }else{
        this->m_tbAllDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    }
    if(this->m_nTuningBits&(0x1<<1))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbAllInc->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbAllInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
        }
    }else{
        this->m_tbAllInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    }
    if(this->m_nTuningBits&(0x1<<2))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbRedDec->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbRedDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
        }
    }else{
        this->m_tbRedDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    }
    if(this->m_nTuningBits&(0x1<<3))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbRedInc->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbRedInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
        }
    }else{
        this->m_tbRedInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    }
    if(this->m_nTuningBits&(0x1<<4))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbGreenDec->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbGreenDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
        }
    }else{
        this->m_tbGreenDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    }
    if(this->m_nTuningBits&(0x1<<5))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbGreenInc->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbGreenInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
        }
    }else{
        this->m_tbGreenInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    }
    if(this->m_nTuningBits&(0x1<<6))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbBlueDec->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbBlueDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
        }
    }else{
        this->m_tbBlueDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    }
    if(this->m_nTuningBits&(0x1<<7))
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbBlueInc->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbBlueInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
        }
    }else{
        this->m_tbBlueInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    }
    //flash the fault led.
    if(this->m_bHasFault)
    {
        if(this->m_bCmdRunning)
        {
            this->m_llFaultLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        }else{
            this->m_llFaultLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        }
    }else{
        //no fault.
        this->m_llFaultLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
    }

    //emit get module data.
    if(g_GblHelp.m_bQueryStatus)
    {
        if(this->m_nTime1sCounter++>=g_GblHelp.m_nQueryInterval)
        {
            this->m_nTime1sCounter=0;
            emit this->ZSigRequestCmd("getGLaserData",QString::number(this->m_moduleNo,10));
        }
    }
}

void ZMdlCtlUI::ZUptPwrCtlBtnText(QString what,bool on)
{
    if(what==QString("all"))
    {
        this->m_tbAllOn->setText(on?tr("全部开启"):tr("全部关闭"));
    }else if(what==QString("red"))
    {
        this->m_tbRedOn->setText(on?tr("开启红光"):tr("关闭红光"));
    }else if(what==QString("green"))
    {
        this->m_tbGreenOn->setText(on?tr("开启绿光"):tr("关闭绿光"));
    }else if(what==QString("blue"))
    {
        this->m_tbBlueOn->setText(on?tr("开启蓝光"):tr("关闭蓝光"));
    }else if(what==QString("wb"))
    {
        this->m_tbWhiteBalanceOn->setText(on?tr("开启白平衡"):tr("关闭白平衡"));
    }

    //fixup for all on / all off.
    if(this->m_tbRedOn->text()==tr("开启红光") && this->m_tbGreenOn->text()==tr("开启绿光") && this->m_tbBlueOn->text()==tr("开启蓝光"))
    {
        this->m_tbAllOn->setText(tr("全部开启"));
    }else if(this->m_tbRedOn->text()==tr("关闭红光") && this->m_tbGreenOn->text()==tr("关闭绿光") && this->m_tbBlueOn->text()==tr("关闭蓝光"))
    {
        this->m_tbAllOn->setText(tr("全部关闭"));
    }
}
void ZMdlCtlUI::ZFixPwrCtlBtnText(QString what)
{
    if(what==QString("all"))
    {
        if(this->m_tbAllOn->text()==QString("开启中..."))
        {
            this->m_tbAllOn->setText(tr("全部开启"));
        }else if(this->m_tbAllOn->text()==QString("关闭中..."))
        {
            this->m_tbAllOn->setText(tr("全部关闭"));
        }
    }else if(what==QString("red"))
    {
        if(this->m_tbRedOn->text()==QString("开启中..."))
        {
            this->m_tbRedOn->setText(tr("开启红光"));
        }else if(this->m_tbRedOn->text()==QString("关闭中..."))
        {
            this->m_tbRedOn->setText(tr("关闭红光"));
        }
    }else if(what==QString("green"))
    {
        if(this->m_tbGreenOn->text()==QString("开启中..."))
        {
            this->m_tbGreenOn->setText(tr("开启绿光"));
        }else if(this->m_tbGreenOn->text()==QString("关闭中..."))
        {
            this->m_tbGreenOn->setText(tr("关闭绿光"));
        }
    }else if(what==QString("blue"))
    {
        if(this->m_tbBlueOn->text()==QString("开启中..."))
        {
            this->m_tbBlueOn->setText(tr("开启蓝光"));
        }else if(this->m_tbBlueOn->text()==QString("关闭中..."))
        {
            this->m_tbBlueOn->setText(tr("关闭蓝光"));
        }
    }else if(what==QString("wb"))
    {
        if(this->m_tbWhiteBalanceOn->text()==QString("开启中..."))
        {
            this->m_tbWhiteBalanceOn->setText(tr("开启白平衡"));
        }else if(this->m_tbWhiteBalanceOn->text()==QString("关闭中..."))
        {
            this->m_tbWhiteBalanceOn->setText(tr("关闭白平衡"));
        }
    }
}
void ZMdlCtlUI::ZSloResponseCmdPwr2MdlCtl(QString cmd,QString arguments)
{
    qDebug()<<"ZMdlCtlUI:"<<cmd<<arguments<<this->m_moduleNo;
    if(cmd.left(1).toInt()!=this->m_moduleNo)
    {
        return;
    }
    bool bRedOn=false;
    bool bGreenOn=false;
    bool bBlueOn=false;
    bool bWhiteBalanceOn=false;
    bool bTimeout=false;
    QStringList itemsList=arguments.split(",");
    for(qint32 i=0;i<itemsList.size();i++)
    {
        QString item=itemsList.at(i);
        QStringList objVal=item.split("=");
        if(2==objVal.size())
        {
            //red.
            if(objVal.at(0)=="red")
            {
                if(objVal.at(1)=="on")
                {
                    this->m_leRedIV->ZSetImg(QPixmap(":/topbar/images/led/led_green.png"));
                    this->ZUptPwrCtlBtnText("red",false);
                    bRedOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->m_leRedIV->ZSetImg(QPixmap(":/topbar/images/led/led_red.png"));
                    this->ZUptPwrCtlBtnText("red",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("red");
                    bTimeout=true;
                }
            }
            //green.
            if(objVal.at(0)=="green")
            {
                if(objVal.at(1)=="on")
                {
                    this->m_leGreenIV->ZSetImg(QPixmap(":/topbar/images/led/led_green.png"));
                    this->ZUptPwrCtlBtnText("green",false);
                    bGreenOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->m_leGreenIV->ZSetImg(QPixmap(":/topbar/images/led/led_red.png"));
                    this->ZUptPwrCtlBtnText("green",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("green");
                    bTimeout=true;
                }
            }
            //blue.
            if(objVal.at(0)=="blue")
            {
                if(objVal.at(1)=="on")
                {
                    this->m_leBlueIV->ZSetImg(QPixmap(":/topbar/images/led/led_green.png"));
                    this->ZUptPwrCtlBtnText("blue",false);
                    bBlueOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->m_leBlueIV->ZSetImg(QPixmap(":/topbar/images/led/led_red.png"));
                    this->ZUptPwrCtlBtnText("blue",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("blue");
                    bTimeout=true;
                }
            }
            //white balance.
            if(objVal.at(0)=="wb")
            {
                if(objVal.at(1)=="on")
                {
                    this->ZUptPwrCtlBtnText("wb",false);
                    bWhiteBalanceOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->ZUptPwrCtlBtnText("wb",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("wb");
                    bTimeout=true;
                }
            }
        }
    }
    if(!bRedOn || !bBlueOn || !bGreenOn || !bWhiteBalanceOn)
    {
        this->ZUptPwrCtlBtnText("all",true);
        this->ZFixPwrCtlBtnText("all");
    }else if(bRedOn || bBlueOn || bGreenOn || bWhiteBalanceOn)
    {
        this->ZUptPwrCtlBtnText("all",false);
        this->ZFixPwrCtlBtnText("all");
    }
}
void ZMdlCtlUI::ZSloResponseCmdTuning2MdlCtl(QString cmd,QString arguments)
{
    qDebug()<<"ZMdlCtlUI:"<<cmd<<arguments;
    if(cmd.left(1).toInt()!=this->m_moduleNo)
    {
        return;
    }
    qDebug()<<"mdlctlui-"<<this->m_moduleNo<<":get:"<<arguments;

    //to help the ColorTempTuning Dialog.
    emit this->ZSigAdjRGBPercent(cmd,arguments);

    QStringList itemsList=arguments.split(",");
    for(qint32 i=0;i<itemsList.size();i++)
    {
        QString item=itemsList.at(i);
        QStringList objVal=item.split("=");
        if(objVal.size()!=2)
        {
            //bypass bad pair key=value.
            continue;
        }

        if(objVal.at(0)=="rgb")
        {
            if(objVal.at(1)=="timeout")
            {

            }else{
                qint32 retVal=objVal.at(1).toInt();
                if(0xffff==retVal)
                {
                    this->ZPopupMsgAddGblErrMsgList(tr("调节失败,PLaser无法完成请求!"));
                }else{
                    this->m_pbAll->setValue(retVal/10.0f);
                }
            }
            this->m_nTuningBits&=~(0x1<<0);
            this->m_nTuningBits&=~(0x1<<1);
        }else if(objVal.at(0)=="red")
        {
            if(objVal.at(1)=="timeout")
            {

            }else{
                quint16 retVal=objVal.at(1).toInt();
                if(0xffff==retVal)
                {
                    this->ZPopupMsgAddGblErrMsgList(tr("调节失败,PLaser无法完成请求!"));
                }else{
                    this->m_pbRed->setValue(retVal/10.0f);
                }
            }
            this->m_nTuningBits&=~(0x1<<2);
            this->m_nTuningBits&=~(0x1<<3);
        }else if(objVal.at(0)=="green")
        {
            if(objVal.at(1)=="timeout")
            {

            }else{
                quint16 retVal=objVal.at(1).toInt();
                if(0xffff==retVal)
                {
                    this->ZPopupMsgAddGblErrMsgList(tr("调节失败,PLaser无法完成请求!"));
                }else{
                    this->m_pbGreen->setValue(retVal/10.0f);
                }
            }
            this->m_nTuningBits&=~(0x1<<4);
            this->m_nTuningBits&=~(0x1<<5);
        }else if(objVal.at(0)=="blue")
        {
            if(objVal.at(1)=="timeout")
            {

            }else{
                quint16 retVal=objVal.at(1).toInt();
                if(0xffff==retVal)
                {
                    this->ZPopupMsgAddGblErrMsgList(tr("调节失败,PLaser无法完成请求!"));
                }else{
                    this->m_pbBlue->setValue(retVal/10.0f);
                }
            }
            this->m_nTuningBits&=~(0x1<<6);
            this->m_nTuningBits&=~(0x1<<7);
        }
    }
}
void ZMdlCtlUI::ZSloResponseCmdColorTemp2MdlCtl(QString cmd,QString arguments)
{
    if(cmd.left(1).toInt()!=this->m_moduleNo)
    {
        return;
    }
    QStringList argList=arguments.split("=");
    if(argList.size()!=2)
    {
        return;
    }
    if(argList.at(0)=="CT")
    {
        this->m_leColorTemp->ZSetLineEdit(argList.at(1));
    }
}
void ZMdlCtlUI::ZSloResponseCmdGLaserData2MdlCtl(QString cmd,QString arguments)
{
    if(cmd.left(1).toInt()!=this->m_moduleNo)
    {
        return;
    }
    qDebug()<<arguments;
    //"TH=50,PP=596,SF=2,CT=x,FBV=1,RPP=3,GPP=4,BPP=5,HOUR=6h9m,AT=9,G2=10,G1=11,B=12,R2=13,R1=14,RWB=15,GWB=1,BWB=2,BV=3,BC=4,GV=5,GC=6,RV=7,RC=8"
    QStringList keyValList=arguments.split(",");
    qDebug()<<keyValList;
    for(qint32 i=0;i<keyValList.size();i++)
    {
        QString keyVal=keyValList.at(i);
        qDebug()<<"keyval:"<<keyVal;
        QStringList objVal=keyVal.split("=");
        if(objVal.size()!=2)
        {
            continue;
        }
        if(objVal.at(0)=="TH")
        {
            this->m_leFiberBreakThreshold->ZSetLineEdit(objVal.at(1)+"%");
        }else if(objVal.at(0)=="PP")
        {
            qint32 nTotalPwrPercent=objVal.at(1).toInt();
            float fTotalPwrPercent=nTotalPwrPercent/10.0f;
            this->m_pbAll->setValue(fTotalPwrPercent);
        }else if(objVal.at(0)=="SF")
        {
            //state flag.
            quint16 stateFlag=objVal.at(1).toInt();
            //0:yellow,1:green,2:red.
            qint32 fiberFault=0;
            qint32 whiteBalanceFault=0;
            qint32 redOC=0;
            qint32 redOV=0;
            qint32 redOT=0;
            qint32 greenOC=0;
            qint32 greenOV=0;
            qint32 greenOT=0;
            qint32 blueOC=0;
            qint32 blueOV=0;
            qint32 blueOT=0;
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
            if(stateFlag&(0x1<<15))
            {
                //plaser cannot connect to glaser.
                //set all led to yellow.
                emit this->ZSigMdlFault(fiberFault,whiteBalanceFault,redOC,redOV,redOT,greenOC,greenOV,greenOT,blueOC,blueOV,blueOT);
                this->m_bHasFault=true;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<15);
            }else{
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<15);
            }

            if(stateFlag&(0x1<<14))
            {
                //blue over temperature.
                blueOT=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<14);
            }else{
                blueOT=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<14);
            }

            if(stateFlag&(0x1<<13))
            {
                //blue over voltage.
                blueOV=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<13);
            }else{
                blueOV=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<13);
            }

            if(stateFlag&(0x1<<12))
            {
                //blue over current.
                blueOC=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<12);
            }else{
                blueOC=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<12);
            }

            if(stateFlag&(0x1<<11))
            {
                //green over temperature.
                greenOT=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<11);
            }else{
                greenOT=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<11);
            }

            if(stateFlag&(0x1<<10))
            {
                //green over voltage.
                greenOV=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<10);
            }else{
                greenOV=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<10);
            }


            if(stateFlag&(0x1<<9))
            {
                //green over current.
                greenOC=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<9);
            }else{
                greenOC=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<9);
            }

            if(stateFlag&(0x1<<8))
            {
                //red over temperature.
                redOT=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<8);
            }else{
                redOT=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<8);
            }

            if(stateFlag&(0x1<<7))
            {
                //red over voltage.
                redOV=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<7);
            }else{
                redOV=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<7);
            }

            if(stateFlag&(0x1<<6))
            {
                //red over current.
                redOC=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<6);
            }else{
                redOC=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<6);
            }
            if(stateFlag&(0x1<<5))
            {
                //fiber fault.
                fiberFault=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<5);
            }else{
                fiberFault=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<5);
            }
            if(stateFlag&(0x1<<4))
            {
                //white balance fault.
                whiteBalanceFault=2;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]|=(0x1<<4);
            }else{
                whiteBalanceFault=1;
                g_GblHelp.m_GLaserErrFlag[this->m_moduleNo-1]&=(0x1<<4);
            }

            //update the fault led.
            emit this->ZSigMdlFault(fiberFault,whiteBalanceFault,redOC,redOV,redOT,greenOC,greenOV,greenOT,blueOC,blueOV,blueOT);
            if(fiberFault!=1 || whiteBalanceFault!=1 || redOC!=1 || redOV!=1 || redOT!=1 || greenOC!=1 || greenOV!=1 || greenOT!=1 || blueOC!=1 || blueOV!=1 || blueOT!=1)
            {
                this->m_bHasFault=true;
            }else{
                this->m_bHasFault=false;
            }

            if(stateFlag&(0x1<<3))
            {
                //white balance on.
                this->ZUptPwrCtlBtnText("wb",false);
                this->m_llWBLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
            }else{
                this->ZUptPwrCtlBtnText("wb",true);
                this->m_llWBLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
            }
            if(stateFlag&(0x1<<2))
            {
                //blue on.
                this->ZUptPwrCtlBtnText("blue",false);
                this->m_leBlueIV->ZSetImg(QPixmap(":/topbar/images/led/led_green.png"));
            }else{
                this->ZUptPwrCtlBtnText("blue",true);
                this->m_leBlueIV->ZSetImg(QPixmap(":/topbar/images/led/led_red.png"));
            }
            if(stateFlag&(0x1<<1))
            {
                //green on.
                this->ZUptPwrCtlBtnText("green",false);
                this->m_leGreenIV->ZSetImg(QPixmap(":/topbar/images/led/led_green.png"));
            }else{
                this->ZUptPwrCtlBtnText("green",true);
                this->m_leGreenIV->ZSetImg(QPixmap(":/topbar/images/led/led_red.png"));
            }
            if(stateFlag&(0x1<<0))
            {
                //red on.
                this->ZUptPwrCtlBtnText("red",false);
                this->m_leRedIV->ZSetImg(QPixmap(":/topbar/images/led/led_green.png"));
            }else{
                this->ZUptPwrCtlBtnText("red",true);
                this->m_leRedIV->ZSetImg(QPixmap(":/topbar/images/led/led_red.png"));
            }
        }else if(objVal.at(0)=="CT")
        {
            //color temp.
            QString colorTemp=ZMapColorTemp2String(objVal.at(1).toInt());
            this->m_leColorTemp->ZSetLineEdit(colorTemp);
        }else if(objVal.at(0)=="FBV")
        {
            //fiber break voltage.
            this->m_leFiberBreakVoltage->ZSetLineEdit(objVal.at(1)+"mV");
        }else if(objVal.at(0)=="RPP")
        {
            //red power percent.
            qint32 redPer=objVal.at(1).toInt();
            float redPerF=(float)redPer/10.0f;
            this->m_pbRed->setValue(redPerF);
        }else if(objVal.at(0)=="GPP")
        {
            //green power percent.
            qint32 greenPer=objVal.at(1).toInt();
            float greenPerF=(float)greenPer/10.0f;
            this->m_pbGreen->setValue(greenPerF);
        }else if(objVal.at(0)=="BPP")
        {
            //blue power percent.
            qint32 bluePer=objVal.at(1).toInt();
            float bluePerF=(float)bluePer/10.0f;
            this->m_pbBlue->setValue(bluePerF);
        }else if(objVal.at(0)=="HOUR")
        {
            //run hour.
            this->m_leRunTime->ZSetLineEdit(objVal.at(1));
        }else if(objVal.at(0)=="AT")
        {
            //ambient temperature.
            quint16 at=objVal.at(1).toInt();
            quint8 atHigh=(quint8)((at&0xFF00)>>8);
            quint8 atLow=(quint8)(at&0x00FF);
            this->m_leAtTemp->ZSetLineEdit(QString::number(atHigh,10)+"."+QString::number(atLow,10)+" ℃");
        }else if(objVal.at(0)=="G2")
        {
            //G2 temperature.
            quint16 g2=objVal.at(1).toInt();
            quint8 g2High=(quint8)((g2&0xFF00)>>8);
            quint8 g2Low=(quint8)(g2&0x00FF);
            this->m_leG2Temp->ZSetLineEdit(QString::number(g2High,10)+"."+QString::number(g2Low,10)+" ℃");
        }else if(objVal.at(0)=="G1")
        {
            //G1 temperature.
            quint16 g1=objVal.at(1).toInt();
            quint8 g1High=(quint8)((g1&0xFF00)>>8);
            quint8 g1Low=(quint8)(g1&0x00FF);
            this->m_leG1Temp->ZSetLineEdit(QString::number(g1High,10)+"."+QString::number(g1Low,10)+" ℃");
        }else if(objVal.at(0)=="B")
        {
            //B temperature.
            quint16 bt=objVal.at(1).toInt();
            quint8 btHigh=(quint8)((bt&0xFF00)>>8);
            quint8 btLow=(quint8)(bt&0x00FF);
            this->m_leBTemp->ZSetLineEdit(QString::number(btHigh,10)+"."+QString::number(btLow,10)+" ℃");
        }else if(objVal.at(0)=="R2")
        {
            //R2 temperature.
            quint16 r2=objVal.at(1).toInt();
            quint8 r2High=(quint8)((r2&0xFF00)>>8);
            quint8 r2Low=(quint8)(r2&0x00FF);
            this->m_leR2Temp->ZSetLineEdit(QString::number(r2High,10)+"."+QString::number(r2Low,10)+" ℃");
        }else if(objVal.at(0)=="R1")
        {
            //R1 temperature.
            quint16 r1=objVal.at(1).toInt();
            quint8 r1High=(quint8)((r1&0xFF00)>>8);
            quint8 r1Low=(quint8)(r1&0x00FF);
            this->m_leR1Temp->ZSetLineEdit(QString::number(r1High,10)+"."+QString::number(r1Low,10)+" ℃");
        }else if(objVal.at(0)=="RWB")
        {
            //red white balance.
            this->m_redWBValue=objVal.at(1).toInt();
            this->m_leOpticalPower->ZSetLineEdit(tr("%1/%2/%3").arg(this->m_redWBValue).arg(this->m_greenWBValue).arg(this->m_blueWBValue));
        }else if(objVal.at(0)=="GWB")
        {
            //green white balance.
            this->m_greenWBValue=objVal.at(1).toInt();
            this->m_leOpticalPower->ZSetLineEdit(tr("%1/%2/%3").arg(this->m_redWBValue).arg(this->m_greenWBValue).arg(this->m_blueWBValue));
        }else if(objVal.at(0)=="BWB")
        {
            //blue white balance.
            this->m_blueWBValue=objVal.at(1).toInt();
            this->m_leOpticalPower->ZSetLineEdit(tr("%1/%2/%3").arg(this->m_redWBValue).arg(this->m_greenWBValue).arg(this->m_blueWBValue));
        }else if(objVal.at(0)=="BV")
        {
            //blue voltage.
            qint32 blueV=objVal.at(1).toInt();
            float blueVF=blueV/1000.0f;
            this->m_leBlueIV->ZSetLineEdit2(QString::number(blueVF,'f',2));
        }else if(objVal.at(0)=="BC")
        {
            //blue current.
            qint32 blueI=objVal.at(1).toInt();
            float blueIF=blueI/1000.0f;
            this->m_leBlueIV->ZSetLineEdit(QString::number(blueIF,'f',2));
        }else if(objVal.at(0)=="GV")
        {
            //green voltage.
            qint32 greenV=objVal.at(1).toInt();
            float greenVF=greenV/1000.0f;
            this->m_leGreenIV->ZSetLineEdit2(QString::number(greenVF,'f',2));
        }else if(objVal.at(0)=="GC")
        {
            //green current.
            qint32 greenI=objVal.at(1).toInt();
            float greenIF=greenI/1000.0f;
            this->m_leGreenIV->ZSetLineEdit(QString::number(greenIF,'f',2));
        }else if(objVal.at(0)=="RV")
        {
            //red voltage.
            qint32 redV=objVal.at(1).toInt();
            float redVF=redV/1000.0f;
            this->m_leRedIV->ZSetLineEdit2(QString::number(redVF,'f',2));
        }else if(objVal.at(0)=="RC")
        {
            //red current.
            qint32 redI=objVal.at(1).toInt();
            float redIF=redI/1000.0f;
            this->m_leRedIV->ZSetLineEdit(QString::number(redIF,'f',2));
        }
    }
}
void ZMdlCtlUI::ZSloResponseCmdFBVThreshold(QString cmd,QString arguments)
{
    if(cmd.left(1).toInt()==this->m_moduleNo)
    {
        this->m_leFiberBreakThreshold->ZSetLineEdit(arguments+"%");
    }
}
void ZMdlCtlUI::ZPopupMsgAddGblErrMsgList(QString errMsg)
{
    emit this->ZSigPopupMsg(errMsg);
}
void ZMdlCtlUI::ZSloShowMdlFaultDialog()
{
    this->m_bProcessEvent=false;
    ZMdlFaultDialog dia(tr("GLaser-%1 模块报警信息").arg(this->m_moduleNo));
    connect(this,SIGNAL(ZSigMdlFault(qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32)),&dia,SLOT(ZSloMdlFault(qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32)));
    dia.exec();
    disconnect(this,SIGNAL(ZSigMdlFault(qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32)),&dia,SLOT(ZSloMdlFault(qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32,qint32)));
    this->m_bProcessEvent=true;
}
void ZMdlCtlUI::ZSloShowMdlColorTempDialog()
{
    g_GblHelp.beep();
    this->m_bProcessEvent=false;
    ZMdlColorTempSetDia dia(this->m_moduleNo,this->m_pbAll->getValue(),tr("GLaser-%1 色温模式设置").arg(this->m_moduleNo));
    connect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)),&dia,SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)));
    connect(this,SIGNAL(ZSigAdjRGBPercent(QString,QString)),&dia,SIGNAL(ZSigAdjRGBPercent(QString,QString)));
    connect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    connect(&dia,SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
    connect(&dia,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    if(dia.exec()==QDialog::Accepted)
    {
        emit this->ZSigRequestCmd(QString("%1_colorTemp").arg(this->m_moduleNo),"CT="+dia.ZGetColorTemp());
    }
    disconnect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)),&dia,SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)));
    disconnect(this,SIGNAL(ZSigAdjRGBPercent(QString,QString)),&dia,SIGNAL(ZSigAdjRGBPercent(QString,QString)));
    disconnect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    disconnect(&dia,SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
    disconnect(&dia,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    this->m_bProcessEvent=true;
}
bool ZMdlCtlUI::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
    {
        if(watched==this->m_pbAll)
        {
            emit this->ZSigPrecisSetupRequest("all");
            return true;
        }else if(watched==this->m_pbRed)
        {
            emit this->ZSigPrecisSetupRequest("red");
            return true;
        }else if(watched==this->m_pbGreen)
        {
            emit this->ZSigPrecisSetupRequest("green");
            return true;
        }else if(watched==this->m_pbBlue)
        {
            emit this->ZSigPrecisSetupRequest("blue");
            return true;
        }
    }
    return ZBaseUI::eventFilter(watched,event);
}
void ZMdlCtlUI::ZSloShowPrecisSetupDialog(QString channel)
{
    g_GblHelp.beep();
    if(channel=="all")
    {
        this->m_bProcessEvent=false;
        ZPwrPrecisionSetDialog setDia(tr("整体功率<Precise>"));
        connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        if(setDia.exec()==QDialog::Accepted)
        {
            float precisVal=setDia.ZGetPrecisionPwrValue();
            qint32 intVal=(qint32)(precisVal*10);
            emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),QString("rgb=%1").arg(intVal));
        }
        disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        this->m_bProcessEvent=true;
    }else if(channel=="red")
    {
        this->m_bProcessEvent=false;
        ZPwrPrecisionSetDialog setDia(tr("Red功率<Precise>"));
        connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        if(setDia.exec()==QDialog::Accepted)
        {
            float precisVal=setDia.ZGetPrecisionPwrValue();
            qint32 intVal=(qint32)(precisVal*10);
            emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),QString("red=%1").arg(intVal));
        }
        disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        this->m_bProcessEvent=true;
    }else if(channel=="green")
    {
        this->m_bProcessEvent=false;
        ZPwrPrecisionSetDialog setDia(tr("Green功率<Precise>"));
        connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        if(setDia.exec()==QDialog::Accepted)
        {
            float precisVal=setDia.ZGetPrecisionPwrValue();
            qint32 intVal=(qint32)(precisVal*10);
            emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),QString("green=%1").arg(intVal));
        }
        disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        this->m_bProcessEvent=true;
    }else if(channel=="blue")
    {
        this->m_bProcessEvent=false;
        ZPwrPrecisionSetDialog setDia(tr("Blue功率<Precise>"));
        connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        if(setDia.exec()==QDialog::Accepted)
        {
            float precisVal=setDia.ZGetPrecisionPwrValue();
            qint32 intVal=(qint32)(precisVal*10);
            emit this->ZSigRequestCmd(QString("%1_tuning").arg(this->m_moduleNo),QString("blue=%1").arg(intVal));
        }
        disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        this->m_bProcessEvent=true;
    }
}
void ZMdlCtlUI::ZSlotShowFiberBreakVoltageSetupDialog()
{
    g_GblHelp.beep();
    this->m_bProcessEvent=false;
    ZPwrPrecisionSetDialog setDia(tr("光纤折断监控电压百分比"));
    connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    if(setDia.exec()==QDialog::Accepted)
    {
        float precisVal=setDia.ZGetPrecisionPwrValue();
        qint32 intVal=(qint32)(precisVal);
        //fbv:fiber break voltage.
        emit this->ZSigRequestCmd(QString("%1_fbv").arg(this->m_moduleNo),QString::number(intVal,10));
        //this->m_leFiberBreakThreshold->ZSetLineEdit(QString::number(intVal,10));
    }
    disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    this->m_bProcessEvent=true;
}
void ZMdlCtlUI::ZSloMdlFault(qint32 fiber,qint32 wb,qint32 rc,qint32 rv,qint32 rt,qint32 gc,qint32 gv,qint32 gt,qint32 bc,qint32 bv,qint32 bt)
{
    //0:yellow,1:green,2:red.
    switch(fiber)
    {
    case 0:
        this->m_llFiberBreakLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_llFiberBreakLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_llFiberBreakLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(wb)
    {
    case 0:
        this->m_llWBLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_llWBLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_llWBLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(rc)
    {
    case 0:
        this->m_OverIVT[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(rv)
    {
    case 0:
        this->m_OverIVT[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(rt)
    {
    case 0:
        this->m_OverIVT[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
    switch(gc)
    {
    case 0:
        this->m_OverIVT[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(gv)
    {
    case 0:
        this->m_OverIVT[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(gt)
    {
    case 0:
        this->m_OverIVT[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(bc)
    {
    case 0:
        this->m_OverIVT[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(bv)
    {
    case 0:
        this->m_OverIVT[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }

    switch(bt)
    {
    case 0:
        this->m_OverIVT[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
        break;
    case 1:
        this->m_OverIVT[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
        break;
    case 2:
        this->m_OverIVT[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
        break;
    }
}
