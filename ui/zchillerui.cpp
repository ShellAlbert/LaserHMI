#include "zchillerui.h"
#include <QDebug>
#include <gbl/hmi_const.h>
#include <ui/zpwrprecisionsetdialog.h>
ZChillerManager::ZChillerManager()
{

}
ZChillerManager::~ZChillerManager()
{

}
void ZChillerManager::ZDoInitial()
{
    this->m_tabWidget=new QTabWidget;
    this->m_tabWidget->setIconSize(QSize(24,24));
    connect(this->m_tabWidget,SIGNAL(currentChanged(int)),this,SLOT(ZSloTabChanged(qint32)));
    for(qint32 i=0;i<2;i++)
    {
        this->m_chiller[i]=new ZChillerUI(i+1);
        this->m_chiller[i]->ZDoInitial();
        connect(this->m_chiller[i],SIGNAL(ZSigDumpDbgMsg(QString)),this,SIGNAL(ZSigDumpDbgMsg(QString)));
        connect(this->m_chiller[i],SIGNAL(ZSigRequestCmd(QString,QString)),this,SIGNAL(ZSigRequestCmd(QString,QString)));
        connect(this->m_chiller[i],SIGNAL(ZSigPopMessage(QString)),this,SIGNAL(ZSigPopMessage(QString)));
        connect(this,SIGNAL(ZSigResponseCmdChillerCtl(QString,QString)),this->m_chiller[i],SLOT(ZSloChillerCmdResponse(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdChillerFault(QString,QString)),this->m_chiller[i],SLOT(ZSloChillerCmdResponse(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdChillerFlow(QString,QString)),this->m_chiller[i],SLOT(ZSloChillerCmdResponse(QString,QString)));
        connect(this,SIGNAL(ZSigResponseCmdChillerTemp(QString,QString)),this->m_chiller[i],SLOT(ZSloChillerCmdResponse(QString,QString)));
        this->m_tabWidget->addTab(this->m_chiller[i],QIcon(":/navigate/images/navigate/chiller.png"),tr("%1号水冷机").arg(i+1));
    }
    connect(this->m_chiller[0],SIGNAL(ZSigOtherPwrOn(bool)),this->m_chiller[1],SLOT(ZSloOtherPwrOn(bool)));
    connect(this->m_chiller[1],SIGNAL(ZSigOtherPwrOn(bool)),this->m_chiller[0],SLOT(ZSloOtherPwrOn(bool)));
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_tabWidget);
    this->setLayout(this->m_vLayout);
}
void ZChillerManager::ZUnInitial()
{
    for(qint32 i=0;i<2;i++)
    {
        this->m_chiller[i]->ZUnInitial();
        delete this->m_chiller[i];
    }
    delete this->m_tabWidget;
    delete this->m_vLayout;
}
void ZChillerManager::ZSloTabChanged(qint32 index)
{
    g_GblHelp.beep();
}
ZChillerUI::ZChillerUI(quint8 chillerNo)
{
    this->m_chillerNo=chillerNo;
}
ZChillerUI::~ZChillerUI()
{

}

void ZChillerUI::ZDoInitial()
{
    //temp.
    this->m_vsEnterWater=new ZCircleIndicator(tr("出水温度"));
    this->m_vsLeaveWater=new ZCircleIndicator(tr("回水温度"));
    this->m_vsAmbientTemp=new ZCircleIndicator(tr("环境温度"));
    this->m_vsEnterWater->setAnimating(true);
    this->m_vsLeaveWater->setAnimating(true);
    this->m_vsAmbientTemp->setAnimating(true);
    this->m_vsEnterWater->setValue(0.0f);
    this->m_vsLeaveWater->setValue(0.0f);
    this->m_vsAmbientTemp->setValue(0.0f);

    this->m_gLayoutTemp=new QGridLayout;
    this->m_gLayoutTemp->addWidget(this->m_vsEnterWater,0,0,1,1);
    this->m_gLayoutTemp->addWidget(this->m_vsLeaveWater,0,1,1,1);
    this->m_gLayoutTemp->addWidget(this->m_vsAmbientTemp,0,2,1,1);
    this->m_gLayoutTemp->setContentsMargins(2,20,2,2);
    this->m_frmTemp=new ZFrame(tr("Temperature (温度)"));
    this->m_frmTemp->setLayout(this->m_gLayoutTemp);

    //flow speed.
    this->m_leEnterSpeed=new ZTextLineEditLabel(tr("出水流量"),"0 L/min",Qt::Vertical);
    this->m_leLeaveSpeed=new ZTextLineEditLabel(tr("回水流量"),"0 L/min",Qt::Vertical);
    this->m_gLayoutSpeed=new QGridLayout;
    this->m_gLayoutSpeed->addWidget(this->m_leEnterSpeed,0,0,1,1);
    this->m_gLayoutSpeed->addWidget(this->m_leLeaveSpeed,0,1,1,1);
    this->m_frmSpeed=new QFrame;
    this->m_frmSpeed->setLayout(this->m_gLayoutSpeed);

    connect(this->m_leLeaveSpeed,SIGNAL(ZSigClicked()),this,SLOT(ZSloSetEnterFlowParameters()));

    //control.
    this->m_tbBypass=new QToolButton;
    this->m_tbBypass->setText(tr("屏蔽水冷机"));
    this->m_tbBypass->setIcon(QIcon(":/chiller/images/chiller/BypassChiller.png"));
    this->m_tbBypass->setIconSize(QSize(48,48));
    this->m_tbBypass->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->m_tbPwrCtl=new QToolButton;
    this->m_tbPwrCtl->setText(tr("开启水冷机"));
    this->m_tbPwrCtl->setIcon(QIcon(":/chiller/images/chiller/ReEnableChiller.png"));
    this->m_tbPwrCtl->setIconSize(QSize(48,48));
    this->m_tbPwrCtl->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    this->m_tbBypass->setObjectName("ZChillerUIBtn");
    this->m_tbPwrCtl->setObjectName("ZChillerUIBtn");

    this->m_gLayoutControl=new QGridLayout;
    this->m_gLayoutControl->addWidget(this->m_tbBypass,0,0,1,1);
    this->m_gLayoutControl->addWidget(this->m_tbPwrCtl,0,1,1,1);
    this->m_gLayoutControl->setContentsMargins(10,20,5,5);
    this->m_frmControl=new QFrame;
    this->m_frmControl->setLayout(this->m_gLayoutControl);

    //setup.
    this->m_frmSetup=new QFrame;
    this->m_leChillerTemp=new ZTextLineEditLabel(tr("恒温值"),"24");
    this->m_cbEnFangJieLu=new ZCheckBox(tr("防结露"));
    this->m_tbSaveChiller=new QToolButton;
    this->m_tbSaveChiller->setText(tr("默认配置"));
    this->m_tbSaveChiller->setObjectName("ZChillerUIBtn");
    connect(this->m_tbSaveChiller,SIGNAL(clicked(bool)),this,SLOT(ZSloShowSetChillerTempDialog()));
    this->m_gLayoutSetup=new QGridLayout;
    this->m_gLayoutSetup->addWidget(this->m_leChillerTemp,0,0,1,1);
    this->m_gLayoutSetup->addWidget(this->m_cbEnFangJieLu,0,1,1,1);
    this->m_gLayoutSetup->addWidget(this->m_tbSaveChiller,0,2,1,1);
    this->m_gLayoutSetup->setColumnStretch(0,1);
    this->m_gLayoutSetup->setColumnStretch(1,1);
    this->m_gLayoutSetup->setColumnStretch(2,1);
    this->m_frmSetup->setLayout(this->m_gLayoutSetup);

    //alarm&fault.
    this->m_frmFault=new QFrame;
    for(qint32 i=0;i<16;i++)
    {
        this->m_faultLED[i]=new ZTextImgLabel("",QPixmap(":/topbar/images/led/led_yellow.png"));
        switch(i)
        {
        case 0:
            this->m_faultLED[i]->ZSetText(tr("通信"));
            break;
        case 1:
            this->m_faultLED[i]->ZSetText(tr("相序"));
            break;
        case 2:
            this->m_faultLED[i]->ZSetText(tr("马达"));
            break;
        case 3:
            this->m_faultLED[i]->ZSetText(tr("风扇"));
            break;
        case 4:
            this->m_faultLED[i]->ZSetText(tr("自保护"));
            break;
        case 5:
            this->m_faultLED[i]->ZSetText(tr("水位"));
            break;
        case 6:
            this->m_faultLED[i]->ZSetText(tr("冻水流量"));
            break;
        case 7:
            this->m_faultLED[i]->ZSetText(tr("冻水低温"));
            break;
        case 8:
            this->m_faultLED[i]->ZSetText(tr("冻水高温"));
            break;
        case 9:
            this->m_faultLED[i]->ZSetText(tr("低压氟利昂"));
            break;
        case 10:
            this->m_faultLED[i]->ZSetText(tr("高压氟利昂"));
            break;
        case 11:
            this->m_faultLED[i]->ZSetText(tr("压缩机"));
            break;
        case 12:
            this->m_faultLED[i]->ZSetText(tr("低压氟传感"));
            break;
        case 13:
            this->m_faultLED[i]->ZSetText(tr("高压氟传感"));
            break;
        case 14:
            this->m_faultLED[i]->ZSetText(tr("冻水温传感"));
            break;
        case 15:
            this->m_faultLED[i]->ZSetText(tr("环境温传感"));
            break;
        }

    }
    this->m_gLayoutFault=new QGridLayout;
    //相序,马达,风扇,自保护.
    this->m_gLayoutFault->addWidget(this->m_faultLED[1],0,0,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[2],0,1,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[3],0,2,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[4],0,3,1,1);
    //水位,冻水流量,冻水低温,冻水高温.
    this->m_gLayoutFault->addWidget(this->m_faultLED[5],1,0,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[6],1,1,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[7],1,2,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[8],1,3,1,1);
    //低压氟利昂,高压氟利昂,压缩机.
    this->m_gLayoutFault->addWidget(this->m_faultLED[9],2,0,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[10],2,1,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[11],2,2,1,1);
    //低压氟利昂传感,高压氟利昂传感,冻水温度传感,环境温度传感.
    this->m_gLayoutFault->addWidget(this->m_faultLED[12],3,0,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[13],3,1,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[14],3,2,1,1);
    this->m_gLayoutFault->addWidget(this->m_faultLED[15],3,3,1,1);
    this->m_gLayoutFault->setContentsMargins(0,0,0,0);
    this->m_frmFault->setLayout(this->m_gLayoutFault);

    //tab widget.
    this->m_frmTab=new QFrame;
    this->m_tabWidget=new QTabWidget;
    this->m_tabWidget->addTab(this->m_frmControl,QIcon(""),tr("开关控制"));
    this->m_tabWidget->addTab(this->m_frmSpeed,QIcon(""),tr("流体速度"));
    this->m_tabWidget->addTab(this->m_frmSetup,QIcon(""),tr("参数设置"));
    this->m_tabWidget->addTab(this->m_frmFault,QIcon(""),tr("故障报警"));
    connect(this->m_tabWidget,SIGNAL(currentChanged(int)),this,SLOT(ZSloTabChanged(qint32)));
    this->m_vLayoutTab=new QVBoxLayout;
    this->m_vLayoutTab->addWidget(this->m_tabWidget);
    this->m_frmTab->setLayout(this->m_vLayoutTab);

    //status.
    this->m_llCommunicate=new ZTextImgLabel(tr("通信"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llWaterLevel=new ZTextImgLabel(tr("水位"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llSelfProtect=new ZTextImgLabel(tr("自保护"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_llOthers=new ZTextImgLabel(tr("其他"),QPixmap(":/topbar/images/led/led_yellow.png"));
    this->m_gLayoutStatus=new QGridLayout;
    this->m_gLayoutStatus->addWidget(this->m_llCommunicate,0,0,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llWaterLevel,1,0,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llSelfProtect,2,0,1,1);
    this->m_gLayoutStatus->addWidget(this->m_llOthers,3,0,1,1);
    this->m_gLayoutStatus->setContentsMargins(5,30,5,2);
    this->m_frmStatus=new ZFrame(tr("Status (状态)"));
    this->m_frmStatus->setLayout(this->m_gLayoutStatus);

    this->m_gLayoutMain=new QGridLayout;
    this->m_gLayoutMain->addWidget(this->m_frmTemp,0,0,1,2);
    this->m_gLayoutMain->addWidget(this->m_frmTab,1,0,1,1);
    this->m_gLayoutMain->addWidget(this->m_frmStatus,1,1,1,1);
    this->m_gLayoutMain->setColumnStretch(0,8);
    this->m_gLayoutMain->setColumnStretch(1,2);
    this->m_gLayoutMain->setRowStretch(0,4);
    this->m_gLayoutMain->setRowStretch(1,1);
    this->setLayout(this->m_gLayoutMain);

    connect(this->m_tbBypass,SIGNAL(clicked(bool)),this,SLOT(ZSloBypassChiller()));
    connect(this->m_tbPwrCtl,SIGNAL(clicked(bool)),this,SLOT(ZSloPowerOnChiller()));

    this->m_bChillerPwrOn=false;
    this->m_bChillerBypass=false;


    //query status timer.
    this->m_nTime1sCounter=0;
    this->m_bCmdRunning=false;
    this->m_bEnableMe=true;
    this->m_timer1s=new QTimer;
    connect(this->m_timer1s,SIGNAL(timeout()),this,SLOT(ZSloTimeout1s()));

    this->m_bProcessEvent=true;
}
void ZChillerUI::ZSloTabChanged(qint32 index)
{
    g_GblHelp.beep();
}

void ZChillerUI::ZUnInitial()
{
    delete this->m_vsLeaveWater;
    delete this->m_vsEnterWater;
    delete this->m_vsAmbientTemp;
    delete this->m_gLayoutTemp;
    delete this->m_frmTemp;

    delete this->m_leEnterSpeed;
    delete this->m_leLeaveSpeed;
    delete this->m_gLayoutSpeed;
    delete this->m_frmSpeed;

    delete this->m_tbBypass;
    delete this->m_tbPwrCtl;
    delete this->m_gLayoutControl;
    delete this->m_frmControl;

    //setup.
    delete this->m_leChillerTemp;
    delete this->m_cbEnFangJieLu;
    delete this->m_tbSaveChiller;
    delete this->m_gLayoutSetup;
    delete this->m_frmSetup;

    //alarm&fault.
    for(qint32 i=0;i<16;i++)
    {
        delete this->m_faultLED[i];
    }
    delete this->m_gLayoutFault;
    delete this->m_frmFault;

    //tab widget.
    delete this->m_tabWidget;
    delete this->m_vLayoutTab;
    delete this->m_frmTab;

    //status.
    delete this->m_llCommunicate;
    delete this->m_llWaterLevel;
    delete this->m_llSelfProtect;
    delete this->m_llOthers;
    delete this->m_gLayoutStatus;
    delete this->m_frmStatus;

    delete this->m_gLayoutMain;
}
bool ZChillerUI::event(QEvent *e)
{
    if(this->m_bProcessEvent)
    {
        switch(e->type())
        {
        case QEvent::Show:
            //qDebug()<<"ZChillerUI:   Show Event...........";
            //    case QEvent::Move:
            //        qDebug()<<"ZChillerUI:   Move Event...........";
            //    case QEvent::Paint:
            //        qDebug()<<"ZChillerUI:   Paint Event...........";
            if(this->m_bEnableMe)
            {
                if(!this->m_timer1s->isActive())
                {
                    this->m_timer1s->start(1000*g_GblHelp.m_nQueryInterval);
                }
            }
            break;
        case QEvent::Hide:
            //qDebug()<<"ZChillerUI:   Hide Event...........";
            this->m_timer1s->stop();
            break;
        default:
            break;
        }
    }
    return ZBaseUI::event(e);
}
void ZChillerUI::ZSloBypassChiller()
{
    g_GblHelp.beep();
    //ChillerCtl,no=1/2,pwr=0/1,bypass=0/1.
    if(this->m_tbBypass->text()==tr("屏蔽水冷机"))
    {
        emit this->ZSigRequestCmd("ChillerCtl",QString("no=%1,pwr=0,bypass=1").arg(this->m_chillerNo));
        this->m_tbBypass->setText("...");
    }else if(this->m_tbBypass->text()==tr("使能水冷机"))
    {
        emit this->ZSigRequestCmd("ChillerCtl",QString("no=%1,pwr=1,bypass=0").arg(this->m_chillerNo));
        this->m_tbBypass->setText("...");
    }else if(this->m_tbBypass->text().endsWith("..."))
    {
        emit this->ZSigPopMessage(tr("正在等待PLaser响应..."));
        return;
    }
}
void ZChillerUI::ZSloPowerOnChiller()
{
    g_GblHelp.beep();
    //ChillerCtl,no=1/2,pwr=0/1,bypass=0/1.
    if(this->m_tbPwrCtl->text()==tr("开启水冷机"))
    {
        //now chiller is in bypass mode.
        if(this->m_tbBypass->text()==tr("使能水冷机"))
        {
            emit this->ZSigRequestCmd("ChillerCtl",QString("no=%1,pwr=1,bypass=1").arg(this->m_chillerNo));
        }else{
            emit this->ZSigRequestCmd("ChillerCtl",QString("no=%1,pwr=1,bypass=0").arg(this->m_chillerNo));
        }
        this->m_tbPwrCtl->setText("...");
    }else if(this->m_tbPwrCtl->text()==tr("关闭水冷机"))
    {
        if(this->m_tbBypass->text()==tr("使能水冷机"))
        {
            emit this->ZSigRequestCmd("ChillerCtl",QString("no=%1,pwr=0,bypass=1").arg(this->m_chillerNo));
        }else{
            emit this->ZSigRequestCmd("ChillerCtl",QString("no=%1,pwr=0,bypass=0").arg(this->m_chillerNo));
        }
        this->m_tbPwrCtl->setText("...");
    }else if(this->m_tbPwrCtl->text().endsWith("..."))
    {
        emit this->ZSigPopMessage(tr("正在等待PLaser响应..."));
        return;
    }
}
void ZChillerUI::ZSloOtherPwrOn(bool powerOn)
{
    //if other's power is on,then disable me.
    //otherwise,enable me.
    if(powerOn)
    {
        this->m_timer1s->stop();
        this->m_bEnableMe=false;
        this->setEnabled(false);
    }else{
        this->m_bEnableMe=true;
        this->setEnabled(true);
    }
}
void ZChillerUI::ZSloChillerCmdResponse(QString cmd,QString arguments)
{
    if(cmd=="ChillerCtl")
    {
        if(arguments.contains("timeout"))
        {
            if(!this->m_bChillerPwrOn)
            {
                this->m_tbPwrCtl->setText(tr("开启水冷机"));
            }else{
                this->m_tbPwrCtl->setText(tr("关闭水冷机"));
            }

            if(!this->m_bChillerBypass)
            {
                this->m_tbBypass->setText(tr("屏蔽水冷机"));
            }else{
                this->m_tbBypass->setText(tr("使能水冷机"));
            }
            return;
        }
        if(arguments.contains("ret=-1"))
        {
            emit this->ZSigPopMessage(tr("GLaser返回请求失败!"));
            return;
        }
        QStringList argList=arguments.split(",");
        for(qint32 i=0;i<argList.size();i++)
        {
            QStringList keyValList=argList.at(i).split("=");
            if(keyValList.size()!=2)
            {
                continue;
            }
            if(keyValList.at(0)=="no")
            {
                if(this->m_chillerNo!=keyValList.at(1).toInt())
                {
                    return;
                }
            }
            if(keyValList.at(0)=="bypass")
            {
                quint8 bypassBit=keyValList.at(1).toInt();
                if(bypassBit==1)
                {
                    this->m_bChillerBypass=true;
                    this->m_tbBypass->setText(tr("使能水冷机"));
                }else{
                    this->m_bChillerBypass=false;
                    this->m_tbBypass->setText(tr("屏蔽水冷机"));
                }
            }
            if(keyValList.at(0)=="pwr")
            {
                quint8 pwrBit=keyValList.at(1).toInt();
                if(pwrBit==1)
                {
                    this->m_bChillerPwrOn=true;
                    this->m_tbPwrCtl->setText(tr("关闭水冷机"));
                    emit this->ZSigOtherPwrOn(false);
                }else{
                    this->m_bChillerPwrOn=false;
                    this->m_tbPwrCtl->setText(tr("开启水冷机"));
                    emit this->ZSigOtherPwrOn(true);
                }
            }
        }
    }else if(cmd=="getChillerTemp")
    {
        //no=1/2,AT=xx.x,LT=xx.xx,ET=xx.xx,CT=xx.xx.
        if(arguments.contains("timeout"))
        {
            return;
        }
        QStringList argList=arguments.split(",");
        qint8 chillerNo=0;
        float fAT,fLT,fET,fCT;

        for(qint32 i=0;i<argList.size();i++)
        {
            QStringList keyValList=argList.at(i).split("=");
            if(keyValList.size()!=2)
            {
                continue;
            }
            if(keyValList.at(0)=="no")
            {
                chillerNo=keyValList.at(1).toInt();
            }else if(keyValList.at(0)=="AT")
            {
                fAT=keyValList.at(1).toFloat();
            }else if(keyValList.at(0)=="LT")
            {
                fLT=keyValList.at(1).toFloat();
            }else if(keyValList.at(0)=="ET")
            {
                fET=keyValList.at(1).toFloat();
            }else if(keyValList.at(0)=="CT")
            {
                fCT=keyValList.at(1).toFloat();
            }
        }
        if(chillerNo==this->m_chillerNo)
        {
            this->m_vsAmbientTemp->setValue(fAT);
            this->m_vsLeaveWater->setValue(fLT);
            this->m_vsEnterWater->setValue(fET);
            this->m_leChillerTemp->ZSetLineEdit(QString::number(fCT,'f',2)+"℃");
        }
    }else if(cmd=="getChillerFault")
    {
        //getChillerFault,no=1/2,bypass=0/1,pwr=0/1,ice=0/1,flag1=xx,flag2=xx.
        if(arguments.contains("timeout"))
        {
            return;
        }
        QStringList argList=arguments.split(",");
        qint8 chillerNo=0;
        qint8 bypass=0;
        qint8 pwr=0;
        qint8 ice=0;
        qint8 flag1=0;
        qint8 flag2=0;
        for(qint32 i=0;i<argList.size();i++)
        {
            QStringList keyValList=argList.at(i).split("=");
            if(keyValList.size()!=2)
            {
                continue;
            }
            if(keyValList.at(0)=="no")
            {
                chillerNo=keyValList.at(1).toInt();
            }else if(keyValList.at(0)=="bypass")
            {
                bypass=keyValList.at(1).toInt();
            }else if(keyValList.at(0)=="pwr")
            {
                pwr=keyValList.at(1).toInt();
            }else if(keyValList.at(0)=="ice")
            {
                ice=keyValList.at(1).toInt();
            }else if(keyValList.at(0)=="flag1")
            {
                flag1=keyValList.at(1).toInt();
            }else if(keyValList.at(0)=="flag2")
            {
                flag2=keyValList.at(1).toInt();
            }
        }
        if(chillerNo==this->m_chillerNo)
        {
            if(!bypass)
            {
                this->m_tbBypass->setText(tr("屏蔽水冷机"));
            }else{
                this->m_tbBypass->setText(tr("使能水冷机"));
            }
            if(!pwr)
            {
                this->m_tbPwrCtl->setText(tr("开启水冷机"));
                emit this->ZSigOtherPwrOn(false);
            }else{
                this->m_tbPwrCtl->setText(tr("关闭水冷机"));
                emit this->ZSigOtherPwrOn(true);
            }
            if(!ice)
            {
                this->m_cbEnFangJieLu->ZSetChecked(false);
            }else{
                this->m_cbEnFangJieLu->ZSetChecked(true);
            }
            //flag1.
            if(flag1&(0x1<<7))
            {
                //communicate fault.
                this->m_faultLED[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                this->m_llCommunicate->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<15);

                //change all led to yellow.
                //self-protect.
                this->m_faultLED[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                this->m_llSelfProtect->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));

                //dong shui liuliang.
                this->m_faultLED[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //diya fuliyang sensor.
                this->m_faultLED[12]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //gaoya fuliyang sensor.
                this->m_faultLED[13]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //dongsui temp sensor.
                this->m_faultLED[14]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //Ambient temp sensor.
                this->m_faultLED[15]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //diya fuliyang.
                this->m_faultLED[9]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //gaoya fuliyang.
                this->m_faultLED[10]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //dongshui diwen.
                this->m_faultLED[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //dong shui gaowen.
                this->m_faultLED[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //water level.
                this->m_faultLED[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                this->m_llWaterLevel->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //xiang xu.
                this->m_faultLED[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //compressor.
                this->m_faultLED[11]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //motor.
                this->m_faultLED[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
                //fan.
                this->m_faultLED[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));

                //other fault.
                this->m_llOthers->ZSetPixmap(QPixmap(":/topbar/images/led/led_yellow.png"));
            }else{
                //communicate okay.
                this->m_faultLED[0]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                this->m_llCommunicate->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<15);

                if(flag1&(0x1<<6))//self-protect.
                {
                    this->m_faultLED[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->m_llSelfProtect->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    //set error flag
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
                    //[15]:communicate error.
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<0);
                }else{
                    this->m_faultLED[4]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    this->m_llSelfProtect->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    //reset error flag,[0]:self-protect.
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<0);
                }
                if(flag1&(0x1<<5))//dong shui liuliang.
                {
                    this->m_faultLED[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<1);
                }else{
                    this->m_faultLED[6]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<1);
                }
                if(flag1&(0x1<<4))//diya fuliyang sensor.
                {
                    this->m_faultLED[12]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<2);
                }else{
                    this->m_faultLED[12]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<2);
                }
                if(flag1&(0x1<<3))//gaoya fuliyang sensor.
                {
                    this->m_faultLED[13]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<3);
                }else{
                    this->m_faultLED[13]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<3);
                }
                if(flag1&(0x1<<2))//dongsui temp sensor.
                {
                    this->m_faultLED[14]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<4);
                }else{
                    this->m_faultLED[14]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<4);
                }
                if(flag1&(0x1<<1))//Ambient temp sensor.
                {
                    this->m_faultLED[15]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<5);
                }else{
                    this->m_faultLED[15]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<5);
                }
                if(flag1&(0x1<<0))//diya fuliyang.
                {
                    this->m_faultLED[9]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<6);
                }else{
                    this->m_faultLED[9]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<6);
                }
                //flag2.
                if(flag2&(0x1<<7))//gaoya fuliyang.
                {
                    this->m_faultLED[10]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<7);
                }else{
                    this->m_faultLED[10]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<7);
                }
                if(flag2&(0x1<<6))//dongshui diwen.
                {
                    this->m_faultLED[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<8);
                }else{
                    this->m_faultLED[7]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<8);
                }
                if(flag2&(0x1<<5))//dong shui gaowen.
                {
                    this->m_faultLED[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<9);
                }else{
                    this->m_faultLED[8]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<9);
                }
                if(flag2&(0x1<<4))//water level.
                {
                    this->m_faultLED[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    this->m_llWaterLevel->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<10);
                }else{
                    this->m_faultLED[5]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    this->m_llWaterLevel->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<10);
                }
                if(flag2&(0x1<<3))//xiang xu.
                {
                    this->m_faultLED[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<11);
                }else{
                    this->m_faultLED[1]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<11);
                }
                if(flag2&(0x1<<2))//compressor.
                {
                    this->m_faultLED[11]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<12);
                }else{
                    this->m_faultLED[11]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<12);
                }
                if(flag2&(0x1<<1))//motor.
                {
                    this->m_faultLED[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<13);
                }else{
                    this->m_faultLED[2]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<13);
                }
                if(flag2&(0x1<<0))//fan.
                {
                    this->m_faultLED[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]|=(0x1<<14);
                }else{
                    this->m_faultLED[3]->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
                    g_GblHelp.m_ChillerErrFlag[this->m_chillerNo-1]&=~(0x1<<14);
                }
            }

            if(flag1 || flag2)
            {
                this->m_llOthers->ZSetPixmap(QPixmap(":/topbar/images/led/led_red.png"));
            }else{
                this->m_llOthers->ZSetPixmap(QPixmap(":/topbar/images/led/led_green.png"));
            }
        }
    }else if(cmd=="getChillerFlow")
    {
        //getChillerFlow,no=1/2,leaveFlow=xx.x,enterFlow=xx.xx.
        if(arguments.contains("timeout"))
        {
            return;
        }
        QStringList argList=arguments.split(",");
        qint8 chillerNo=0;
        QString leaveFlow;
        QString enterFlow;
        for(qint32 i=0;i<argList.size();i++)
        {
            QStringList keyValList=argList.at(i).split("=");
            if(keyValList.size()!=2)
            {
                continue;
            }
            if(keyValList.at(0)=="no")
            {
                chillerNo=keyValList.at(1).toInt();
            }if(keyValList.at(0)=="leaveFlow")
            {
                leaveFlow=keyValList.at(1);
            }else if(keyValList.at(0)=="enterFlow")
            {
                enterFlow=keyValList.at(1);
            }
        }
        if(chillerNo==this->m_chillerNo)
        {
            this->m_leLeaveSpeed->ZSetLineEdit(leaveFlow+" L/min");
            this->m_leEnterSpeed->ZSetLineEdit(enterFlow+" L/min");
        }
    }
}
void ZChillerUI::ZSloTimeout1s()
{
    //used to generate movie effection.
    this->m_bCmdRunning=!this->m_bCmdRunning;
    //Pwr.
    if(this->m_tbBypass->text().right(3)=="...")
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbBypass->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbBypass->setIcon(QIcon(":/chiller/images/chiller/BypassChiller.png"));
        }
    }else{
        this->m_tbBypass->setIcon(QIcon(":/chiller/images/chiller/BypassChiller.png"));
    }
    if(this->m_tbPwrCtl->text().right(3)=="...")
    {
        if(this->m_bCmdRunning)
        {
            this->m_tbPwrCtl->setIcon(QIcon(":/mainctl/images/mainctl/busy.png"));
        }else{
            this->m_tbPwrCtl->setIcon(QIcon(":/chiller/images/chiller/ReEnableChiller.png"));
        }
    }else{
        this->m_tbPwrCtl->setIcon(QIcon(":/chiller/images/chiller/ReEnableChiller.png"));
    }

    //emit get module data.
    if(g_GblHelp.m_bQueryStatus)
    {
        if(this->m_nTime1sCounter++>=g_GblHelp.m_nQueryInterval)
        {
            this->m_nTime1sCounter=0;
            emit this->ZSigRequestCmd("getChillerFault",QString::number(this->m_chillerNo,10));
            emit this->ZSigRequestCmd("getChillerFlow",QString::number(this->m_chillerNo,10));
            emit this->ZSigRequestCmd("getChillerTemp",QString::number(this->m_chillerNo,10));
        }
    }
}
void ZChillerUI::ZSloShowSetChillerTempDialog()
{
    this->m_bProcessEvent=false;
    ZChillerSetDialog setDia("水冷机配置");
    QString nowValue=this->m_leChillerTemp->ZGetLineEdit();
    QString fTempVal=nowValue.left(nowValue.length()-2);
    setDia.ZSetValue(fTempVal,this->m_cbEnFangJieLu->ZIsChecked());
    connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
    if(setDia.exec()==QDialog::Accepted)
    {
        float precisVal=setDia.ZGetValue();
        //saveChillerTemp,no=%1,temp=%2,jielu=%3.
        if(setDia.ZIsEnJieLu())
        {
            emit this->ZSigRequestCmd("saveChillerTemp",QString("no=%1,temp=%2,jielu=1").arg(QString::number(this->m_chillerNo,10)).arg(QString::number(precisVal,'f',1)));
        }else{
            emit this->ZSigRequestCmd("saveChillerTemp",QString("no=%1,temp=%2,jielu=0").arg(QString::number(this->m_chillerNo,10)).arg(QString::number(precisVal,'f',1)));
        }
    }
    disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
    this->m_bProcessEvent=true;
}
void ZChillerUI::ZSloSetEnterFlowParameters()
{
    this->m_bProcessEvent=false;
    ZChillerSetEnterFlowDialog setDia(tr("回水流量配置"));
    setDia.setMinimumSize(300,200);
    connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
    if(setDia.exec()==QDialog::Accepted)
    {
        QString enterFlow=setDia.ZGetFlowValue();
        QString monitorPercent=setDia.ZGetMonitorPercent();
        //setEnterFlow,no=%1,flow=%2,percent=%3.
        emit this->ZSigRequestCmd("setEnterFlow",QString("no=%1,flow=%2,percent=%3").arg(QString::number(this->m_chillerNo,10)).arg(enterFlow).arg(monitorPercent));
    }
    disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
    this->m_bProcessEvent=true;
}
