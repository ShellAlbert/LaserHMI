#include "zmainctlui.h"
#include <QToolTip>
#include <QDebug>
#include <ui/zpwrprecisionsetdialog.h>
#include "gbl/hmi_const.h"
#include <QDateTime>
#include <ui/zcolortemptuningdia.h>
#include <ui/zwarningtipsdialog.h>
ZMainCtlUI::ZMainCtlUI()
{
    this->setObjectName("mainCtlUI");
    this->m_bCmdRunning=false;
    this->m_nTuningBits=0;
}
ZMainCtlUI::~ZMainCtlUI()
{

}

void ZMainCtlUI::ZDoInitial()
{
    //status.
    this->m_llRedLED=new ZTextImgLabel(tr("红光"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llGreenLED=new ZTextImgLabel(tr("绿光"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llBlueLED=new ZTextImgLabel(tr("蓝光"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llWhiteBalanceLED=new ZTextImgLabel(tr("白平衡"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llColorTemp=new ZTextLineEditLabel(tr("色温模式"),tr("D63"));

    this->m_llPLaserAlarm1LED=new ZTextImgLabel(tr("预警"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llPLaserAlarm2LED=new ZTextImgLabel(tr("报警"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llPLaserH=new ZTextLineEditLabel(tr("PLaser湿度"),tr("0"));
    this->m_llPLaserT=new ZTextLineEditLabel(tr("PLaser温度"),tr("0"));

    this->m_gLayoutStatus=new QGridLayout;
    this->m_gLayoutStatus->addWidget(this->m_llRedLED,0,0,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llGreenLED,0,1,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llBlueLED,0,2,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llWhiteBalanceLED,0,3,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llColorTemp,0,4,1,1);

    this->m_gLayoutStatus->addWidget(this->m_llPLaserAlarm1LED,1,0,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llPLaserAlarm2LED,1,1,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llPLaserH,1,2,1,2);
    this->m_gLayoutStatus->addWidget(this->m_llPLaserT,1,4,1,1);

    this->m_gLayoutStatus->setContentsMargins(5,30,5,5);
    this->m_frmStatus=new ZFrame(tr("Status(状态)"));
    this->m_frmStatus->setLayout(this->m_gLayoutStatus);

    //color temperature.
    this->m_llColorTempIndicate=new QLabel;
    this->m_llColorTempIndicate->setPixmap(QPixmap(":/mainctl/images/mainctl/colortemp.png"));
    this->m_llColorTempIndicate->setScaledContents(true);
    this->m_llColorTempIndicate->setMaximumHeight(30);
    this->m_tbD63=new QToolButton;
    this->m_tbD63->setText(tr("D63"));
    this->m_tbD63->installEventFilter(this);

    this->m_tbD65=new QToolButton;
    this->m_tbD65->setText(tr("D65"));
    this->m_tbD65->installEventFilter(this);

    this->m_tbDCI=new QToolButton;
    this->m_tbDCI->setText(tr("DCI"));
    this->m_tbDCI->installEventFilter(this);

    this->m_tbD80=new QToolButton;
    this->m_tbD80->setText(tr("D80"));
    this->m_tbD80->installEventFilter(this);

    this->m_tbD93=new QToolButton;
    this->m_tbD93->setText(tr("D93"));
    this->m_tbD93->installEventFilter(this);

    this->m_tbD95=new QToolButton;
    this->m_tbD95->setText(tr("D95"));
    this->m_tbD95->installEventFilter(this);


    this->m_tbDMore=new QToolButton;
    this->m_tbDMore->setText(tr("更多"));
    this->m_tbDMore->installEventFilter(this);


    this->m_tbD63->setObjectName("mainCtlBtnColorTemp");
    this->m_tbD65->setObjectName("mainCtlBtnColorTemp");
    this->m_tbDCI->setObjectName("mainCtlBtnColorTemp");
    this->m_tbD80->setObjectName("mainCtlBtnColorTemp");
    this->m_tbD93->setObjectName("mainCtlBtnColorTemp");
    this->m_tbD95->setObjectName("mainCtlBtnColorTemp");
    this->m_tbDMore->setObjectName("mainCtlBtnColorTemp");
    this->m_gLayoutColorTemp=new QGridLayout;
    this->m_gLayoutColorTemp->addWidget(this->m_llColorTempIndicate,0,0,1,7);
    this->m_gLayoutColorTemp->addWidget(this->m_tbD63,1,0,1,1);
    this->m_gLayoutColorTemp->addWidget(this->m_tbD65,1,1,1,1);
    this->m_gLayoutColorTemp->addWidget(this->m_tbDCI,1,2,1,1);
    this->m_gLayoutColorTemp->addWidget(this->m_tbD80,1,3,1,1);
    this->m_gLayoutColorTemp->addWidget(this->m_tbD93,1,4,1,1);
    this->m_gLayoutColorTemp->addWidget(this->m_tbD95,1,5,1,1);
    this->m_gLayoutColorTemp->addWidget(this->m_tbDMore,1,6,1,1);
    this->m_gLayoutColorTemp->setContentsMargins(5,30,5,5);
    this->m_frmColorTemp=new ZFrame(tr("ColorTemp(色温)"));
    this->m_frmColorTemp->setLayout(this->m_gLayoutColorTemp);

    //control.
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

    this->m_tbAllOn->setObjectName("mainCtlUIBtn");
    this->m_tbRedOn->setObjectName("mainCtlUIBtn");
    this->m_tbGreenOn->setObjectName("mainCtlUIBtn");
    this->m_tbBlueOn->setObjectName("mainCtlUIBtn");
    this->m_tbWhiteBalanceOn->setObjectName("mainCtlUIBtn");

    this->m_gLayoutControl=new QGridLayout;
    this->m_gLayoutControl->addWidget(this->m_tbAllOn,0,0,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbRedOn,0,1,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbGreenOn,0,2,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbBlueOn,0,3,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbWhiteBalanceOn,0,4,1,1);
    this->m_gLayoutControl->setContentsMargins(5,30,5,5);
    this->m_frmControl=new ZFrame(tr("Control(控制)"));
    this->m_frmControl->setLayout(this->m_gLayoutControl);

    //tuning.
    //    this->m_llAllOpTips=new QLabel;
    //    this->m_llAllOpTips->setText(tr("+/-按钮实现0.1%精度调节,触按指示条进入精准设置模式。"));
    this->m_tbAllDec=new QToolButton;
    this->m_tbAllDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbAllDec->setIcon(QIcon(":/mainctl/images/mainctl/sub_gray.png"));
    this->m_tbAllDec->setIconSize(QSize(64,64));
    this->m_tbAllInc=new QToolButton;
    this->m_tbAllInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbAllInc->setIcon(QIcon(":/mainctl/images/mainctl/add_gray.png"));
    this->m_tbAllInc->setIconSize(QSize(64,64));

    //this->m_pbAll=new ZProgressBar(QColor(67,80,86),QColor(255,255,255));
    this->m_pbAll=new ZVerticalIndicator;
    this->m_pbAll->SetGraphColor(Qt::white);
    this->m_pbAll->setValue(56.0);
    this->m_pbAll->installEventFilter(this);
    //this->m_pbAll->setMaximumWidth(60);

    this->m_tbAllDec->setObjectName("mainCtlUIBtnTuning");
    this->m_tbAllInc->setObjectName("mainCtlUIBtnTuning");

    this->m_gLayoutTuning=new QGridLayout;
    this->m_gLayoutTuning->addWidget(this->m_pbAll,0,0,1,2,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbAllInc,1,0,1,1,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbAllDec,1,1,1,1,Qt::AlignCenter);

    this->m_gLayoutTuning->setContentsMargins(5,30,5,5);
    this->m_frmTuning=new ZFrame(tr("Tuning(调节)"));
    this->m_frmTuning->setLayout(this->m_gLayoutTuning);

    //main layout.
    this->m_gLayoutMain=new QGridLayout;
    this->m_gLayoutMain->addWidget(this->m_frmStatus,0,0,1,1);
    this->m_gLayoutMain->addWidget(this->m_frmControl,1,0,1,1);
    this->m_gLayoutMain->addWidget(this->m_frmColorTemp,2,0,1,1);
    this->m_gLayoutMain->addWidget(this->m_frmTuning,0,1,3,1);
    this->m_gLayoutMain->setColumnStretch(0,9);
    this->m_gLayoutMain->setColumnStretch(1,1);

    this->m_gLayoutMain->setRowStretch(0,1);
    this->m_gLayoutMain->setRowStretch(1,1);
    this->m_gLayoutMain->setRowStretch(2,2);
    this->m_gLayoutMain->setContentsMargins(2,2,2,2);
    this->setLayout(this->m_gLayoutMain);

    //make connections.
    //color temperature.
    connect(this->m_tbD63,SIGNAL(clicked(bool)),this,SLOT(ZSloSetColorTempInternal()));
    connect(this->m_tbD65,SIGNAL(clicked(bool)),this,SLOT(ZSloSetColorTempInternal()));
    connect(this->m_tbDCI,SIGNAL(clicked(bool)),this,SLOT(ZSloSetColorTempInternal()));
    connect(this->m_tbD80,SIGNAL(clicked(bool)),this,SLOT(ZSloSetColorTempInternal()));
    connect(this->m_tbD93,SIGNAL(clicked(bool)),this,SLOT(ZSloSetColorTempInternal()));
    connect(this->m_tbD95,SIGNAL(clicked(bool)),this,SLOT(ZSloSetColorTempInternal()));

    //control.
    connect(this->m_tbAllOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbRedOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbGreenOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbBlueOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));
    connect(this->m_tbWhiteBalanceOn,SIGNAL(clicked(bool)),this,SLOT(ZSloOnCtlInternal()));

    //tuning.
    connect(this->m_tbAllDec,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));
    connect(this->m_tbAllInc,SIGNAL(clicked(bool)),this,SLOT(ZSloPwrPercentTuningInternal()));

    //long press support.
    this->m_longPressCnt=0;
    this->m_bLongPressed=false;
    connect(this,SIGNAL(ZSigLongPressed(QString)),this,SLOT(ZSloLongPressed(QString)));
    //connect all power precision tuning.
    //to avoid block eventFilter().
    connect(this,SIGNAL(ZSigAllPwrPrecisionTuning()),this,SLOT(ZSloAllPwrPrecisionTuning()));

    this->m_getAllMdlDataCount=0;
    //query status timer.
    this->m_timer1s=new QTimer;
    connect(this->m_timer1s,SIGNAL(timeout()),this,SLOT(ZSloTimeout1s()));
//    this->m_timer1s->start(1000*2);
}
void ZMainCtlUI::ZUnInitial()
{
    this->m_timer1s->stop();
    delete this->m_timer1s;

    //status.
    delete this->m_llRedLED;
    delete this->m_llGreenLED;
    delete this->m_llBlueLED;
    delete this->m_llWhiteBalanceLED;
    delete this->m_llColorTemp;

    delete this->m_llPLaserAlarm1LED;
    delete this->m_llPLaserAlarm2LED;
    delete this->m_llPLaserH;
    delete this->m_llPLaserT;
    delete this->m_gLayoutStatus;
    delete this->m_frmStatus;

    //color temperature.
    delete this->m_llColorTempIndicate;
    delete this->m_tbD63;
    delete this->m_tbD65;
    delete this->m_tbDCI;
    delete this->m_tbD80;
    delete this->m_tbD93;
    delete this->m_tbD95;
    delete this->m_tbDMore;
    delete this->m_gLayoutColorTemp;
    delete this->m_frmColorTemp;

    //control.
    delete this->m_tbAllOn;
    delete this->m_tbRedOn;
    delete this->m_tbGreenOn;
    delete this->m_tbBlueOn;
    delete this->m_tbWhiteBalanceOn;
    delete this->m_gLayoutControl;
    delete this->m_frmControl;


    //tuning.
    delete this->m_tbAllDec;
    delete this->m_tbAllInc;
    delete this->m_pbAll;
    delete this->m_gLayoutTuning;
    delete this->m_frmTuning;

    //main layout.
    delete this->m_gLayoutMain;
}
bool ZMainCtlUI::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Show:
        qDebug()<<"ZMainCtlUI:   Show Event...........";
//    case QEvent::Move:
//        qDebug()<<"ZMainCtlUI:   Move Event...........";
//    case QEvent::Paint:
//        qDebug()<<"ZMainCtlUI:   Paint Event...........";
        if(!this->m_timer1s->isActive())
        {
            this->m_timer1s->start(1000*g_GblHelp.m_nQueryInterval);
        }
        break;
    case QEvent::Hide:
        qDebug()<<"ZMainCtlUI:   Hide Event...........";
        this->m_timer1s->stop();
        break;
    default:
        break;
    }
    return ZBaseUI::event(e);
}

bool ZMainCtlUI::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);
        if(mouseEvent->button()==Qt::LeftButton)
        {
            if(watched==this->m_pbAll)
            {
                emit this->ZSigAllPwrPrecisionTuning();
            }else if(watched==this->m_tbD63)
            {
                this->m_bLongPressed=true;
                this->m_longPressColorTemp="D63";
            }else if(watched==this->m_tbD65)
            {
                this->m_bLongPressed=true;
                this->m_longPressColorTemp="D65";
            }else if(watched==this->m_tbD80)
            {
                this->m_bLongPressed=true;
                this->m_longPressColorTemp="D80";
            }else if(watched==this->m_tbD93)
            {
                this->m_bLongPressed=true;
                this->m_longPressColorTemp="D93";
            }else if(watched==this->m_tbD95)
            {
                this->m_bLongPressed=true;
                this->m_longPressColorTemp="D95";
            }else if(watched==this->m_tbDCI)
            {
                this->m_bLongPressed=true;
                this->m_longPressColorTemp="DCI";
            }
        }
    }else if(event->type()==QEvent::MouseButtonRelease)
    {
        this->m_longPressCnt=0;
        this->m_bLongPressed=false;
    }
    return ZBaseUI::eventFilter(watched,event);
}
//status.
void ZMainCtlUI::ZSloGetStatus(bool redOn,bool greenOn,bool blueOn,bool WhiteBalanceOn,QString colorTemp)
{
    if(redOn)
    {
        this->m_llRedLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
    }else{
        this->m_llRedLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
    }

    if(greenOn)
    {
        this->m_llGreenLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
    }else{
        this->m_llGreenLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
    }

    if(blueOn)
    {
        this->m_llBlueLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
    }else{
        this->m_llBlueLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
    }

    if(WhiteBalanceOn)
    {
        this->m_llWhiteBalanceLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
    }else{
        this->m_llWhiteBalanceLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
    }

    this->m_llColorTemp->ZSetLineEdit(colorTemp);
}
void ZMainCtlUI::ZSloTimeout1s()
{
    //long press support.
    if(this->m_bLongPressed)
    {
        this->m_longPressCnt++;
        if(this->m_longPressCnt++>=3)
        {
            emit this->ZSigLongPressed(this->m_longPressColorTemp);
            this->m_bLongPressed=false;
            this->m_longPressCnt=0;
        }
    }

    //invert flag.
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
    //emit to get all module's data.
    if(this->m_getAllMdlDataCount++>=5)
    {
        emit this->ZSigRequestCmd("getHomePageData","");
        this->m_getAllMdlDataCount=0;
    }
}
//color temperature.
void ZMainCtlUI::ZSloSetColorTempInternal()
{
    g_GblHelp.beep();
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    if(srcTb)
    {
        emit this->ZSigSetColorTemp(srcTb->text());
        QString colorTemp=srcTb->text();
        if(colorTemp==QString("D63"))
        {
            emit this->ZSigRequestCmd("main_colorTemp","CT=D63");
        }else if(colorTemp==QString("D65"))
        {
            emit this->ZSigRequestCmd("main_colorTemp","CT=D65");
        }else if(colorTemp==QString("DCI"))
        {
            emit this->ZSigRequestCmd("main_colorTemp","CT=DCI");
        }else if(colorTemp==QString("D80"))
        {
            emit this->ZSigRequestCmd("main_colorTemp","CT=D80");
        }else if(colorTemp==QString("D93"))
        {
            emit this->ZSigRequestCmd("main_colorTemp","CT=D93");
        }else if(colorTemp==QString("D95"))
        {
            emit this->ZSigRequestCmd("main_colorTemp","CT=D95");
        }
    }
}
void ZMainCtlUI::ZSloSetColorTemp(QString colorTemp)
{
    this->m_llColorTemp->ZSetLineEdit(colorTemp);
}
//control.
void ZMainCtlUI::ZSloOnCtlInternal()
{
    g_GblHelp.beep();
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    if(srcTb==this->m_tbAllOn)
    {
        QString tips=this->m_tbAllOn->text();
        if(tips==QString("全部开启"))
        {
            emit this->ZSigRequestCmd("main_pwr","rgbw=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("全部关闭"))
        {
            emit this->ZSigRequestCmd("main_pwr","rgbw=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("操作正在进行中,请稍后..."));
        }
    }else if(srcTb==this->m_tbRedOn)
    {
        QString tips=this->m_tbRedOn->text();
        if(tips==QString("开启红光"))
        {
            emit this->ZSigRequestCmd("main_pwr","red=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭红光"))
        {
            emit this->ZSigRequestCmd("main_pwr","red=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("操作正在进行中,请稍后..."));
        }
    }else if(srcTb==this->m_tbGreenOn)
    {
        QString tips=this->m_tbGreenOn->text();
        if(tips==QString("开启绿光"))
        {
            emit this->ZSigRequestCmd("main_pwr","green=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭绿光"))
        {
            emit this->ZSigRequestCmd("main_pwr","green=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("操作正在进行中,请稍后..."));
        }
    }else if(srcTb==this->m_tbBlueOn)
    {
        QString tips=this->m_tbBlueOn->text();
        if(tips==QString("开启蓝光"))
        {
            emit this->ZSigRequestCmd("main_pwr","blue=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭蓝光"))
        {
            emit this->ZSigRequestCmd("main_pwr","blue=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("操作正在进行中,请稍后..."));
        }
    }else if(srcTb==this->m_tbWhiteBalanceOn)
    {
        QString tips=this->m_tbWhiteBalanceOn->text();
        if(tips==QString("开启白平衡"))
        {
            emit this->ZSigRequestCmd("main_pwr","wb=on");
            srcTb->setText(tr("开启中..."));
        }else if(tips==QString("关闭白平衡"))
        {
            emit this->ZSigRequestCmd("main_pwr","wb=off");
            srcTb->setText(tr("关闭中..."));
        }else{
            //开启中...
            //关闭中...
            emit this->ZSigPopupMsg(tr("操作正在进行中,请稍后..."));
        }
    }
}

//tuning.
void ZMainCtlUI::ZSloPwrPercentTuningInternal()
{
    g_GblHelp.beep();
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    if(srcTb==this->m_tbAllDec)
    {
        if(this->m_nTuningBits&(0x1<<0))
        {
            emit this->ZSigPopupMsg(tr("整体功率减少命令执行中,请稍候..."));
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
        emit this->ZSigRequestCmd("main_tuning",QString("rgb=%1").arg((qint32)(tAllVal*10.0f)));
        this->m_nTuningBits|=(0x1<<0);
    }else if(srcTb==this->m_tbAllInc)
    {
        if(this->m_nTuningBits&(0x1<<1))
        {
            emit this->ZSigPopupMsg(tr("整体功率增加命令执行中,请稍候..."));
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
        qint32 intVal=(qint32)(tAllVal*10);
        emit this->ZSigRequestCmd("main_tuning",QString("rgb=%1").arg(intVal));
        this->m_nTuningBits|=(0x1<<1);
    }
}
void ZMainCtlUI::ZSloResponseCmdPwr(QString arguments)
{
    bool bRedOn=false;
    bool bGreenOn=false;
    bool bBlueOn=false;
    bool bWhiteBalanceOn=false;
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
                    this->m_llRedLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    this->ZUptPwrCtlBtnText("red",false);
                    bRedOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->m_llRedLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->ZUptPwrCtlBtnText("red",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("red");
                }
            }
            //green.
            if(objVal.at(0)=="green")
            {
                if(objVal.at(1)=="on")
                {
                    this->m_llGreenLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    this->ZUptPwrCtlBtnText("green",false);
                    bGreenOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->m_llGreenLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->ZUptPwrCtlBtnText("green",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("green");
                }
            }
            //blue.
            if(objVal.at(0)=="blue")
            {
                if(objVal.at(1)=="on")
                {
                    this->m_llBlueLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    this->ZUptPwrCtlBtnText("blue",false);
                    bBlueOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->m_llBlueLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->ZUptPwrCtlBtnText("blue",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("blue");
                }
            }
            //white balance.
            if(objVal.at(0)=="wb")
            {
                if(objVal.at(1)=="on")
                {
                    this->m_llWhiteBalanceLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    this->ZUptPwrCtlBtnText("wb",false);
                    bWhiteBalanceOn=true;
                }else if(objVal.at(1)=="off")
                {
                    this->m_llWhiteBalanceLED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->ZUptPwrCtlBtnText("wb",true);
                }else if(objVal.at(1)=="timeout")
                {
                    this->ZFixPwrCtlBtnText("wb");
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
void ZMainCtlUI::ZSloResponseCmdTuning(QString arguments)
{
    qDebug()<<"mainctlui:get:"<<arguments;
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
                this->ZPopupMsgAddGblErrMsgList(tr("调节整体功率超时!"));
            }else{
                quint16 retVal=objVal.at(1).toInt();
                if(0xffff==retVal)
                {
                    this->ZPopupMsgAddGblErrMsgList(tr("调节失败,PLaser无法完成请求!"));
                }else{
                    this->m_pbAll->setValue(retVal/10.0f);
                }
            }
            this->m_nTuningBits&=~(0x1<<0);
            this->m_nTuningBits&=~(0x1<<1);
        }
    }
}
void ZMainCtlUI::ZSloResponseCmdColorTemp(QString arguments)
{
    qDebug()<<"mainctlui:get:"<<arguments;
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
        if(objVal.at(0)=="CT")
        {
            if(objVal.at(1)=="timeout")
            {
                this->ZPopupMsgAddGblErrMsgList(tr("设置当前色温模式超时!"));
            }else{
                this->m_llColorTemp->ZSetLineEdit(objVal.at(1));
            }
        }
    }
}
void ZMainCtlUI::ZSloResponseCmdFBVThreshold(QString cmd,QString arguments)
{
    if(cmd.startsWith("main"))
    {

    }
}
void ZMainCtlUI::ZSloResponseCmdGetHomePageData(QString cmd,QString arguments)
{
    ////GetHomePageData,MPP=23.9,H=24.5,T=29.2,A=2,RGB=23;
    if(cmd=="GetHomePageData")
    {
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
            if(objVal.at(0)=="MPP")
            {
                qint32 maxPwrPercent=objVal.at(1).toInt();
                float tMaxPwrP=maxPwrPercent/10.0f;
                this->m_pbAll->setValue(tMaxPwrP);
            }else if(objVal.at(0)=="H")
            {
                quint16 nHumudity=objVal.at(1).toInt();
                quint8 fHigh=(quint8)((nHumudity&0xFF00)>>8);
                quint8 fLow=(quint8)(nHumudity&0x00FF);
                this->m_llPLaserH->ZSetLineEdit(QString::number(fHigh,10)+"."+QString::number(fLow,10)+"%");

            }else if(objVal.at(0)=="T")
            {
                quint16 nTemp=objVal.at(1).toInt();
                quint8 fHigh=(quint8)((nTemp&0xFF00)>>8);
                quint8 fLow=(quint8)(nTemp&0x00FF);
                this->m_llPLaserT->ZSetLineEdit(QString::number(fHigh,10)+"."+QString::number(fLow,10)+"℃");

            }else if(objVal.at(0)=="A")
            {
                qint32 alarmNo=objVal.at(1).toInt();
                switch(alarmNo)
                {
                case 0:
                    this->m_llPLaserAlarm1LED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    this->m_llPLaserAlarm2LED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    break;
                case 1:
                    this->m_llPLaserAlarm1LED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->m_llPLaserAlarm2LED->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    break;
                case 2:
                    this->m_llPLaserAlarm1LED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->m_llPLaserAlarm2LED->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    break;
                }
            }else if(objVal.at(0)=="RGB")
            {
                quint8 nRGBPwr=objVal.at(1).toInt();
                if(nRGBPwr&(0x1<<0))
                {
                    this->ZUptPwrCtlBtnText("blue",false);
                }else{
                    this->ZUptPwrCtlBtnText("blue",true);
                }

                if(nRGBPwr&(0x1<<1))
                {
                    this->ZUptPwrCtlBtnText("green",false);
                }else{
                    this->ZUptPwrCtlBtnText("green",true);
                }

                if(nRGBPwr&(0x1<<2))
                {
                    this->ZUptPwrCtlBtnText("red",false);
                }else{
                    this->ZUptPwrCtlBtnText("red",true);
                }

                if(nRGBPwr&(0x1<<3))
                {
                    this->ZUptPwrCtlBtnText("wb",false);
                }else{
                    this->ZUptPwrCtlBtnText("wb",true);
                }

                if(nRGBPwr&(0x1<<0)&&nRGBPwr&(0x1<<1)&&nRGBPwr&(0x1<<2)&&nRGBPwr&(0x1<<3))
                {
                    this->ZUptPwrCtlBtnText("all",false);
                }else{
                    this->ZUptPwrCtlBtnText("all",true);
                }
            }
        }
    }
}
void ZMainCtlUI::ZUptPwrCtlBtnText(QString what,bool on)
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
}
void ZMainCtlUI::ZFixPwrCtlBtnText(QString what)
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
void ZMainCtlUI::ZSloLongPressed(QString colorTemp)
{

    if(this->m_tbRedOn->text()!=tr("关闭红光")||this->m_tbGreenOn->text()!=tr("关闭绿光")||this->m_tbBlueOn->text()!=tr("关闭蓝光"))
    {
        ZWarningTipsDialog tipsDia(tr("操作警告"),tr("调整色温前需将<RGB>三基色全部打开!"));
        tipsDia.exec();
        return;
    }
    if(this->m_pbAll->getValue()!=100.0f)
    {
        ZWarningTipsDialog tipsDia(tr("操作警告"),tr("调整色温前需将功率输出设置为100.0%!"));
        tipsDia.exec();
        return;
    }

    ZColorTempTuningDia dia(0xff,colorTemp);
    connect(&dia,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    connect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MainCtl(QString,QString)),&dia,SLOT(ZSloSyncColorTempPercent(QString,QString)));
    connect(this,SIGNAL(ZSigAdjRGBPercent(QString,QString)),&dia,SLOT(ZSloAdjRGBPercent(QString,QString)));
    connect(&dia,SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
    connect(&dia,SIGNAL(ZSigPopMessage(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    dia.exec();
    disconnect(&dia,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    disconnect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MainCtl(QString,QString)),&dia,SLOT(ZSloSyncColorTempPercent(QString,QString)));
    disconnect(this,SIGNAL(ZSigAdjRGBPercent(QString,QString)),&dia,SLOT(ZSloAdjRGBPercent(QString,QString)));
    disconnect(&dia,SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
    disconnect(&dia,SIGNAL(ZSigPopMessage(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
}
void ZMainCtlUI::ZSloAllPwrPrecisionTuning()
{
    ZPwrPrecisionSetDialog setDia(tr("整体功率<Precise>"));
    connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    if(setDia.exec()==QDialog::Accepted)
    {
        float precisVal=setDia.ZGetPrecisionPwrValue();
        qint32 intVal=(qint32)(precisVal*10);
        emit this->ZSigRequestCmd("main_tuning",QString("rgb=%1").arg(intVal));
        g_GblHelp.beep();
    }
    disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
}
void ZMainCtlUI::ZPopupMsgAddGblErrMsgList(QString errMsg)
{
    emit this->ZSigPopupMsg(errMsg);
}
