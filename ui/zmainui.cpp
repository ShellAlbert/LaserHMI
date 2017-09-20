#include "zmainui.h"
#include <QDebug>
#include <QToolTip>
#include "gbl/hmi_const.h"
ZMainUI::ZMainUI(ZRingBuffer *txRingBuffer,ZRingBuffer *rxRingBuffer)
{
    this->setObjectName("mainUI");
    this->m_txRingBuffer=txRingBuffer;
    this->m_rxRingBuffer=rxRingBuffer;
}
ZMainUI::~ZMainUI()
{

}

void ZMainUI::ZDoInitial()
{
    emit this->ZSigLoadMsg(tr("Creating Top Bar..."));
    this->m_topBar=new ZTopBar;
    this->m_topBar->ZDoInitial();
    connect(this->m_topBar,SIGNAL(ZSigRequestCmd(QString,QString)),this,SLOT(ZSlotRequestCmd(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdHB2TopBar(QString)),this->m_topBar,SLOT(ZSloResponseCmdHB(QString)));
    connect(this->m_topBar,SIGNAL(ZSigPopupMsg(QString)),this,SLOT(ZSloShowPopupMsg(QString)));
    connect(this->m_topBar,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    emit this->ZSigLoadMsg(tr("Creating Bottom Bar..."));
    this->m_bottomBar=new ZBottomBar;
    this->m_bottomBar->ZDoInitial();
    connect(this->m_bottomBar,SIGNAL(ZSigReturn()),this,SLOT(ZSloReturn()));
    connect(this,SIGNAL(ZSigTxRxDone(qint32)),this->m_bottomBar,SLOT(ZSloTxRxDone(qint32)));
    connect(this,SIGNAL(ZSigDecodeDone(qint32)),this->m_bottomBar,SLOT(ZSloDecodeDone(qint32)));
    connect(this,SIGNAL(ZSigDevLinkDone(qint32)),this->m_bottomBar,SLOT(ZSloDevLinkState(qint32)));
    connect(this->m_bottomBar,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    emit this->ZSigLoadMsg(tr("Creating Login UI..."));
    this->m_loginUI=new ZLoginUI;
    this->m_loginUI->ZDoInitial();
    connect(this->m_loginUI,SIGNAL(ZSigLoginSuccess()),this,SLOT(ZSloLoginSuccess()));
    connect(this->m_loginUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    emit this->ZSigLoadMsg(tr("Creating Navigate UI..."));
    this->m_navigate=new ZNavigateUI;
    this->m_navigate->ZDoInitial();
    connect(this->m_navigate,SIGNAL(ZSigModuleClicked(QString)),this,SLOT(ZSloModuleClicked(QString)));
    connect(this->m_navigate,SIGNAL(ZSigModuleClicked(QString)),this->m_topBar,SLOT(ZSloUptPageName(QString)));
    connect(this->m_navigate,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    emit this->ZSigLoadMsg(tr("Creating Summary UI..."));
    this->m_summaryUI=new ZSummaryUI;
    this->m_summaryUI->ZDoInitial();
    connect(this->m_summaryUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    emit this->ZSigLoadMsg(tr("Creating Laser UI..."));
    this->m_laserUI=new ZLaserUI;
    this->m_laserUI->ZDoInitial();
    connect(this->m_laserUI,SIGNAL(ZSigRequestCmd(QString,QString)),this,SLOT(ZSlotRequestCmd(QString,QString)));
    connect(this->m_laserUI,SIGNAL(ZSigPopupMsg(QString)),this,SLOT(ZSloShowPopupMsg(QString)));
    connect(this->m_laserUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    //mainctlUI.
    connect(this,SIGNAL(ZSigResponseCmdPwr2MainCtl(QString)),this->m_laserUI->m_mainCtlUI,SLOT(ZSloResponseCmdPwr(QString)));
    connect(this,SIGNAL(ZSigResponseCmdTuning2MainCtl(QString)),this->m_laserUI->m_mainCtlUI,SLOT(ZSloResponseCmdTuning(QString)));
    connect(this,SIGNAL(ZSigResponseCmdColorTemp2MainCtl(QString)),this->m_laserUI->m_mainCtlUI,SLOT(ZSloResponseCmdColorTemp(QString)));
    connect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MainCtl(QString,QString)),this->m_laserUI->m_mainCtlUI,SIGNAL(ZSigResponseCmdGetColorTempPercent2MainCtl(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdAdjRGBPercent2MainCtl(QString,QString)),this->m_laserUI->m_mainCtlUI,SIGNAL(ZSigAdjRGBPercent(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdFBVthreshold(QString,QString)),this->m_laserUI->m_mainCtlUI,SLOT(ZSloResponseCmdFBVThreshold(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdGetHomePageData(QString,QString)),this->m_laserUI->m_mainCtlUI,SLOT(ZSloResponseCmdGetHomePageData(QString,QString)));
    connect(this->m_laserUI->m_mainCtlUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    //mdlCtlUI.
    for(qint32 i=0;i<8;i++)
    {
        connect(this,SIGNAL(ZSigResponseCmdPwr2MdlCtl(QString,QString)),this->m_laserUI->m_mdlCtlUI[i],SLOT(ZSloResponseCmdPwr2MdlCtl(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdTuning2MdlCtl(QString,QString)),this->m_laserUI->m_mdlCtlUI[i],SLOT(ZSloResponseCmdTuning2MdlCtl(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdColorTemp2MdlCtl(QString,QString)),this->m_laserUI->m_mdlCtlUI[i],SLOT(ZSloResponseCmdColorTemp2MdlCtl(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdGetGLaserData2MdlCtl(QString,QString)),this->m_laserUI->m_mdlCtlUI[i],SLOT(ZSloResponseCmdGLaserData2MdlCtl(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)),this->m_laserUI->m_mdlCtlUI[i],SIGNAL(ZSigResponseCmdGetColorTempPercent2MdlCtl(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdAdjRGBPercent2MdlCtl(QString,QString)),this->m_laserUI->m_mdlCtlUI[i],SIGNAL(ZSigAdjRGBPercent(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdFBVthreshold(QString,QString)),this->m_laserUI->m_mdlCtlUI[i],SLOT(ZSloResponseCmdFBVThreshold(QString,QString)));
        connect(this->m_laserUI->m_mdlCtlUI[i],SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    }

    emit this->ZSigLoadMsg(tr("Creating Chiller UI..."));
    this->m_chillerUI=new ZChillerManager;
    this->m_chillerUI->ZDoInitial();
    connect(this->m_chillerUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    connect(this->m_chillerUI,SIGNAL(ZSigRequestCmd(QString,QString)),this,SLOT(ZSlotRequestCmd(QString,QString)));
    connect(this->m_chillerUI,SIGNAL(ZSigPopMessage(QString)),this,SLOT(ZSloShowPopupMsg(QString)));
    connect(this,SIGNAL(ZSigResponseCmdChillerCtl(QString,QString)),this->m_chillerUI,SIGNAL(ZSigResponseCmdChillerCtl(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdChillerFault(QString,QString)),this->m_chillerUI,SIGNAL(ZSigResponseCmdChillerFault(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdChillerFlow(QString,QString)),this->m_chillerUI,SIGNAL(ZSigResponseCmdChillerFlow(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdChillerTemp(QString,QString)),this->m_chillerUI,SIGNAL(ZSigResponseCmdChillerTemp(QString,QString)));

    emit this->ZSigLoadMsg(tr("Creating System Setup UI..."));
    this->m_sysSetupUI=new  ZSysSetupUI;
    this->m_sysSetupUI->ZDoInitial();
    connect(this->m_sysSetupUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
    connect(this->m_sysSetupUI,SIGNAL(ZSigPopupMsg(QString)),this,SLOT(ZSloShowPopupMsg(QString)));
    connect(this->m_sysSetupUI,SIGNAL(ZSigRequestCmd(QString,QString)),this,SLOT(ZSlotRequestCmd(QString,QString)));
    connect(this,SIGNAL(ZSigResponseCmdRTC(QString,QString)),this->m_sysSetupUI,SLOT(ZSloResponseRTCCmd(QString,QString)));

    emit this->ZSigLoadMsg(tr("Creating Log UI..."));
    this->m_logUI=new ZLogUI;
    this->m_logUI->ZDoInitial();
    connect(this->m_logUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    emit this->ZSigLoadMsg(tr("Creating Debug/Test UI..."));
    this->m_dbgTestUI=new ZDbgTestUI;
    this->m_dbgTestUI->ZDoInitial();
    connect(this->m_dbgTestUI,SIGNAL(ZSigMsg(QString)),this,SLOT(ZSloShowPopupMsg(QString)));
    connect(this->m_dbgTestUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    emit this->ZSigLoadMsg(tr("Creating Network UI..."));
    this->m_networkUI=new ZNetworkUI;
    this->m_networkUI->ZDoInitial();

    emit this->ZSigLoadMsg(tr("Creating About UI..."));
    this->m_aboutUI=new ZAboutUI;
    this->m_aboutUI->ZDoInitial();
    connect(this->m_aboutUI,SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));

    this->m_stackedWidget=new QStackedWidget;
    this->m_stackedWidget->addWidget(this->m_summaryUI);
    this->m_stackedWidget->addWidget(this->m_laserUI);
    this->m_stackedWidget->addWidget(this->m_chillerUI);
    this->m_stackedWidget->addWidget(this->m_sysSetupUI);
    this->m_stackedWidget->addWidget(this->m_logUI);
    this->m_stackedWidget->addWidget(this->m_dbgTestUI);
    this->m_stackedWidget->addWidget(this->m_networkUI);
    this->m_stackedWidget->addWidget(this->m_aboutUI);

    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->setContentsMargins(0,0,0,0);
    this->m_hLayout->addWidget(this->m_navigate);
    this->m_hLayout->addWidget(this->m_stackedWidget);
    this->m_baseUI=new ZBaseUI;
    this->m_baseUI->setLayout(this->m_hLayout);

    this->m_stackedWidget1=new QStackedWidget;
    this->m_stackedWidget1->addWidget(this->m_loginUI);
    this->m_stackedWidget1->addWidget(this->m_baseUI);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setSpacing(0);
    this->m_vLayout->setMargin(0);
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_topBar);
    this->m_vLayout->addWidget(this->m_stackedWidget1);
    this->m_vLayout->addWidget(this->m_bottomBar);

    this->setLayout(this->m_vLayout);
    this->m_topBar->setVisible(false);
    this->m_bottomBar->setVisible(false);


    this->m_tmGetRxRingBuffer=new QTimer;
    connect(this->m_tmGetRxRingBuffer,SIGNAL(timeout()),this,SLOT(ZSloGetRxRingBuffer()));
    this->m_tmGetRxRingBuffer->start(200);
    emit this->ZSigLoadFinish();
}
void ZMainUI::ZUnInitial()
{
    this->m_tmGetRxRingBuffer->stop();
    delete this->m_tmGetRxRingBuffer;

    this->m_topBar->ZUnInitial();
    delete this->m_topBar;
    this->m_bottomBar->ZUnInitial();
    delete this->m_bottomBar;
    this->m_loginUI->ZUnInitial();
    delete this->m_loginUI;
    this->m_navigate->ZUnInitial();
    delete this->m_navigate;
    this->m_summaryUI->ZUnInitial();
    delete this->m_summaryUI;
    this->m_laserUI->ZUnInitial();
    delete this->m_laserUI;
    this->m_chillerUI->ZUnInitial();
    delete this->m_chillerUI;
    this->m_sysSetupUI->ZUnInitial();
    delete this->m_sysSetupUI;
    this->m_logUI->ZUnInitial();
    delete this->m_logUI;
    this->m_dbgTestUI->ZUnInitial();
    delete this->m_dbgTestUI;
    this->m_networkUI->ZUnInitial();
    delete this->m_networkUI;
    this->m_aboutUI->ZUnInitial();
    delete this->m_aboutUI;
    delete this->m_stackedWidget;
    delete this->m_hLayout;
    delete this->m_vLayout;
}
void ZMainUI::ZSloLoginSuccess()
{
    g_GblHelp.m_bLoginSuccess=true;
    this->m_topBar->setVisible(true);
    this->m_bottomBar->setVisible(true);
    this->m_bottomBar->ZSetReturnButtonVisible(true);
    this->m_stackedWidget1->setCurrentWidget(this->m_baseUI);
}
void ZMainUI::ZSloModuleClicked(QString moduleName)
{
    if(moduleName==QString("Summary"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_summaryUI);
    }else if(moduleName==QString("Laser"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_laserUI);
    }else if(moduleName==QString("Chiller"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_chillerUI);
    }else if(moduleName==QString("System"))
    {

    }else if(moduleName==QString("Setup"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_sysSetupUI);
    }else if(moduleName==QString("Log"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_logUI);
        this->m_logUI->ZFetchLogs();
    }else if(moduleName==QString("Debug"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_dbgTestUI);
    }else if(moduleName==QString("Network"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_networkUI);
    }else if(moduleName==QString("About"))
    {
        this->m_stackedWidget->setCurrentWidget(this->m_aboutUI);
    }
}
void ZMainUI::ZSloReturn()
{
    g_GblHelp.m_bLoginSuccess=false;
    this->m_topBar->setVisible(false);
    this->m_bottomBar->setVisible(false);
    this->m_bottomBar->ZSetReturnButtonVisible(false);
    this->m_loginUI->ZResetLogin();
    this->m_stackedWidget1->setCurrentWidget(this->m_loginUI);
}
void ZMainUI::ZSloGetRxRingBuffer()
{
    //try lock.
    //get a element from rx ringbuffer.
    //unlock.
    //parse it.
    //emit signals.
    QString cmd;
    QString arguments;
    if(this->m_rxRingBuffer->ZTryLock())
    {
        if(!this->m_rxRingBuffer->ZIsEmpty())
        {

            this->m_rxRingBuffer->ZGetElement(cmd,arguments);
            qDebug()<<"ZMainUI:Get:"<<cmd<<arguments;
            emit this->ZSigDumpDbgMsg(tr("ZMainUI:Get:cmd:%1,argument:%2").arg(cmd).arg(arguments));
        }
        this->m_rxRingBuffer->ZUnLock();
    }
    if(!cmd.isEmpty() && !arguments.isEmpty())
    {
        if(cmd.right(3)=="pwr")
        {
            if(cmd.left(4)=="main")
            {
                emit this->ZSigResponseCmdPwr2MainCtl(arguments);
            }else{
                emit this->ZSigResponseCmdPwr2MdlCtl(cmd,arguments);
            }
        }else if(cmd.right(6)=="tuning")
        {
            if(cmd.left(4)=="main")
            {
                emit this->ZSigResponseCmdTuning2MainCtl(arguments);
                emit this->ZSigResponseCmdAdjRGBPercent2MainCtl(cmd,arguments);
            }else{
                emit this->ZSigResponseCmdTuning2MdlCtl(cmd,arguments);
                emit this->ZSigResponseCmdAdjRGBPercent2MdlCtl(cmd,arguments);
            }
        }else if(cmd.right(9)=="colorTemp")
        {
            if(cmd.left(4)=="main")
            {
                emit this->ZSigResponseCmdColorTemp2MainCtl(arguments);
            }else{
                emit this->ZSigResponseCmdColorTemp2MdlCtl(cmd,arguments);
            }
        }else if(cmd=="topbar")
        {
            emit this->ZSigResponseCmdHB2TopBar(arguments);
        }else if(cmd.endsWith("getGLaserData"))
        {
            //1_getGLaserData,2_getGLaserData.
            emit this->ZSigResponseCmdGetGLaserData2MdlCtl(cmd,arguments);
        }else if(cmd.endsWith("GetColorTempPercent"))
        {
            if(cmd.startsWith("main"))
            {
                emit this->ZSigResponseCmdGetColorTempPercent2MainCtl(cmd,arguments);
            }else{
                emit this->ZSigResponseCmdGetColorTempPercent2MdlCtl(cmd,arguments);
            }
        }else if(cmd=="ChillerCtl")
        {
            emit this->ZSigResponseCmdChillerCtl(cmd,arguments);
        }else if(cmd=="getChillerFault")
        {
            emit this->ZSigResponseCmdChillerFault(cmd,arguments);
        }else if(cmd=="getChillerFlow")
        {
            emit this->ZSigResponseCmdChillerFlow(cmd,arguments);

        }else if(cmd=="getChillerTemp")
        {
            emit this->ZSigResponseCmdChillerTemp(cmd,arguments);
        }else if(cmd.endsWith("FBVThreshold"))
        {
            emit this->ZSigResponseCmdFBVthreshold(cmd,arguments);
        }else if(cmd=="GetHomePageData")
        {
            emit this->ZSigResponseCmdGetHomePageData(cmd,arguments);
        }else if(cmd=="setRTC")
        {
            emit this->ZSigResponseCmdRTC(cmd,arguments);
        }else if(cmd=="getRTC")
        {
            emit this->ZSigResponseCmdRTC(cmd,arguments);
        }
    }
}
void ZMainUI::ZSlotRequestCmd(QString cmd,QString arguments)
{
    if(this->m_txRingBuffer->ZTryLock())
    {
        if(!this->m_txRingBuffer->ZIsFull())
        {
            this->m_txRingBuffer->ZPutElement(cmd,arguments);
            qDebug()<<"ZMainUI:Put:"<<cmd<<arguments;
            emit this->ZSigDumpDbgMsg(tr("ZMainUI:Put:<%1,%2>").arg(cmd).arg(arguments));
        }
        this->m_txRingBuffer->ZUnLock();
    }else{
        this->ZSloShowPopupMsg(tr("TxFIFO busy!"));
    }
}
void ZMainUI::ZSloShowPopupMsg(QString msg)
{
    if(g_GblHelp.m_bLoginSuccess && g_GblHelp.m_bEnPopupErrMsg)
    {
        qint32 tWidth=this->fontMetrics().width(msg);
        qint32 tHeight=this->fontMetrics().height();
        qint32 tX=(this->width()-tWidth)/2;
        qint32 tY=(this->height()-tHeight)/2;
        QToolTip::showText(QPoint(tX,tY),msg,this,QRect(tX,tY,tWidth*2,tHeight*2),3000);
    }
}
