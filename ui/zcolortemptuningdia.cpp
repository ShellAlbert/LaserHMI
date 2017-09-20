#include "zcolortemptuningdia.h"
#include <gbl/hmi_const.h>
#include <QEvent>
#include <ui/zpwrprecisionsetdialog.h>
ZColorTempTuningDia::ZColorTempTuningDia(quint8 glaserAddr,QString colorTempName)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(400,300);
    this->setMaximumSize(SCREEN_WIDTH-200,SCREEN_HEIGHT-200);

    this->m_colorTempName=colorTempName;
    this->m_glaserAddress=glaserAddr;

    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(tr("色温模式:    %1").arg(colorTempName));

    //tuning.
    this->m_tbRedDec=new QToolButton;
    this->m_tbRedDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbRedDec->setIcon(QIcon(":/mainctl/images/mainctl/sub.png"));
    this->m_tbRedDec->setIconSize(QSize(64,64));
    this->m_tbRedInc=new QToolButton;
    this->m_tbRedInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbRedInc->setIcon(QIcon(":/mainctl/images/mainctl/add.png"));
    this->m_tbRedInc->setIconSize(QSize(64,64));

    //this->m_pbRed=new ZProgressBar(QColor(67,80,86),QColor(255,0,0));
    this->m_pbRed=new ZVerticalIndicator;
    this->m_pbRed->SetGraphColor(Qt::red);
    this->m_pbRed->setValue(100.0f);
    //    this->m_pbRed->ZSetValue(100.0f);
    //    this->m_pbRed->ZSetText(tr("Red功率"),20);
    this->m_pbRed->installEventFilter(this);
    //this->m_pbRed->setMinimumSize(200,64);

    this->m_tbGreenDec=new QToolButton;
    this->m_tbGreenDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbGreenDec->setIcon(QIcon(":/mainctl/images/mainctl/sub.png"));
    this->m_tbGreenDec->setIconSize(QSize(64,64));
    this->m_tbGreenInc=new QToolButton;
    this->m_tbGreenInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbGreenInc->setIcon(QIcon(":/mainctl/images/mainctl/add.png"));
    this->m_tbGreenInc->setIconSize(QSize(64,64));

    //this->m_pbGreen=new ZProgressBar(QColor(67,80,86),QColor(0,255,0));
    this->m_pbGreen=new ZVerticalIndicator;
    this->m_pbGreen->SetGraphColor(Qt::green);
    this->m_pbGreen->setValue(100.0f);
    //this->m_pbGreen->ZSetValue(100.0f);
    //this->m_pbGreen->ZSetText(tr("Green功率"),20);
    this->m_pbGreen->installEventFilter(this);
    //this->m_pbGreen->setMinimumSize(200,64);

    this->m_tbBlueDec=new QToolButton;
    this->m_tbBlueDec->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbBlueDec->setIcon(QIcon(":/mainctl/images/mainctl/sub.png"));
    this->m_tbBlueDec->setIconSize(QSize(64,64));
    this->m_tbBlueInc=new QToolButton;
    this->m_tbBlueInc->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->m_tbBlueInc->setIcon(QIcon(":/mainctl/images/mainctl/add.png"));
    this->m_tbBlueInc->setIconSize(QSize(64,64));

    //this->m_pbBlue=new ZProgressBar(QColor(67,80,86),QColor(0,0,255));
    this->m_pbBlue=new ZVerticalIndicator;
    this->m_pbBlue->SetGraphColor(Qt::blue);
    this->m_pbBlue->setValue(100.0f);
    //this->m_pbBlue->ZSetValue(100.0f);
    //this->m_pbBlue->ZSetText(tr("Blue功率"),20);
    this->m_pbBlue->installEventFilter(this);
    //this->m_pbBlue->setMinimumSize(200,64);

    this->m_gLayoutTuning=new QGridLayout;
    this->m_gLayoutTuning->addWidget(this->m_pbRed,0,0,1,2,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbRedInc,1,0,1,1,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbRedDec,1,1,1,1,Qt::AlignCenter);

    this->m_gLayoutTuning->addWidget(this->m_pbGreen,0,2,1,2,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbGreenInc,1,2,1,1,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbGreenDec,1,3,1,1,Qt::AlignCenter);


    this->m_gLayoutTuning->addWidget(this->m_pbBlue,0,4,1,2,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbBlueInc,1,4,1,1,Qt::AlignCenter);
    this->m_gLayoutTuning->addWidget(this->m_tbBlueDec,1,5,1,1,Qt::AlignCenter);

    this->m_llOpTips=new QLabel;
    this->m_llOpTips->setText(tr("<触按进度条进入精准设置模式>"));
    this->m_tbSync=new QToolButton;
    this->m_tbSync->setText(tr("同步"));
    this->m_tbSave=new QToolButton;
    this->m_tbSave->setText(tr("保存"));
    this->m_tbDefault=new QToolButton;
    this->m_tbDefault->setText(tr("默认值"));
    this->m_tbClose=new QToolButton;
    this->m_tbClose->setText(tr("关闭"));

    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_llOpTips);
    this->m_hLayout->addStretch(1);
    this->m_hLayout->addWidget(this->m_tbSync);
    this->m_hLayout->addWidget(this->m_tbSave);
    this->m_hLayout->addWidget(this->m_tbDefault);
    this->m_hLayout->addWidget(this->m_tbClose);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_llTitle);
    this->m_vLayout->addLayout(this->m_gLayoutTuning);
    this->m_vLayout->addLayout(this->m_hLayout);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZColorTempTuningDia");
    this->m_frm->setLayout(this->m_vLayout);
    this->m_vLayoutFrm=new QVBoxLayout;
    this->m_vLayoutFrm->setContentsMargins(0,0,0,0);
    this->m_vLayoutFrm->addWidget(this->m_frm);
    this->setLayout(this->m_vLayoutFrm);

    this->m_llTitle->setObjectName("ZColorTempTuningDiaLabel");
    this->m_llOpTips->setObjectName("ZColorTempTuningDiaLabel");
    this->m_pbRed->setObjectName("mainCtlProgressBarRed");
    this->m_pbGreen->setObjectName("mainCtlProgressBarGreen");
    this->m_pbBlue->setObjectName("mainCtlProgressBarBlue");
    this->m_tbRedDec->setObjectName("ZColorTempTuningDiaButton");
    this->m_tbRedInc->setObjectName("ZColorTempTuningDiaButton");
    this->m_tbGreenDec->setObjectName("ZColorTempTuningDiaButton");
    this->m_tbGreenInc->setObjectName("ZColorTempTuningDiaButton");
    this->m_tbBlueDec->setObjectName("ZColorTempTuningDiaButton");
    this->m_tbBlueInc->setObjectName("ZColorTempTuningDiaButton");


    this->m_tbSync->setObjectName("ZColorTempTuningDiaOkayCancel");
    this->m_tbSave->setObjectName("ZColorTempTuningDiaOkayCancel");
    this->m_tbDefault->setObjectName("ZColorTempTuningDiaOkayCancel");
    this->m_tbClose->setObjectName("ZColorTempTuningDiaOkayCancel");

    connect(this->m_tbSync,SIGNAL(clicked(bool)),this,SLOT(ZSlotSync()));
    connect(this->m_tbSave,SIGNAL(clicked(bool)),this,SLOT(ZSlotSave()));
    connect(this->m_tbDefault,SIGNAL(clicked(bool)),this,SLOT(ZSlotDefault()));
    connect(this->m_tbClose,SIGNAL(clicked(bool)),this,SLOT(ZSlotClose()));

    connect(this->m_tbRedDec,SIGNAL(clicked(bool)),this,SLOT(ZSlotValueChanged()));
    connect(this->m_tbRedInc,SIGNAL(clicked(bool)),this,SLOT(ZSlotValueChanged()));

    connect(this->m_tbGreenDec,SIGNAL(clicked(bool)),this,SLOT(ZSlotValueChanged()));
    connect(this->m_tbGreenInc,SIGNAL(clicked(bool)),this,SLOT(ZSlotValueChanged()));

    connect(this->m_tbBlueDec,SIGNAL(clicked(bool)),this,SLOT(ZSlotValueChanged()));
    connect(this->m_tbBlueInc,SIGNAL(clicked(bool)),this,SLOT(ZSlotValueChanged()));

    connect(this,SIGNAL(ZSigRequestPrecisSetup(QString)),this,SLOT(ZSlotShowPrecisSetupDialog(QString)));

    QTimer::singleShot(500, this, SLOT(ZSlotReadDB()));
    this->m_bProcessEvent=true;
}
void ZColorTempTuningDia::ZSlotReadDB()
{
    //load value from database.
    QStringList valList;
    if(!gDB.ZGetColorTempData(this->m_glaserAddress,this->m_colorTempName,valList))
    {
        //this->m_pbRed->ZSetValue(valList.at(0).toFloat());
        this->m_pbRed->setValue(valList.at(0).toFloat());
        //this->m_pbGreen->ZSetValue(valList.at(1).toFloat());
        this->m_pbGreen->setValue(valList.at(1).toFloat());
        //this->m_pbBlue->ZSetValue(valList.at(2).toFloat());
        this->m_pbBlue->setValue(valList.at(2).toFloat());
        this->m_redDefault=valList.at(3).toFloat();
        this->m_greenDefault=valList.at(4).toFloat();
        this->m_blueDefault=valList.at(5).toFloat();
    }
}
ZColorTempTuningDia::~ZColorTempTuningDia()
{
    delete this->m_llTitle;
    delete this->m_tbRedDec;
    delete this->m_tbRedInc;
    delete this->m_pbRed;

    delete this->m_tbGreenDec;
    delete this->m_tbGreenInc;
    delete this->m_pbGreen;

    delete this->m_tbBlueDec;
    delete this->m_tbBlueInc;
    delete this->m_pbBlue;

    delete this->m_gLayoutTuning;

    delete this->m_llOpTips;
    delete this->m_tbSync;
    delete this->m_tbSave;
    delete this->m_tbDefault;
    delete this->m_tbClose;
    delete this->m_hLayout;

    delete this->m_vLayout;

    delete this->m_frm;
    delete this->m_vLayoutFrm;
}
bool ZColorTempTuningDia::eventFilter(QObject *watched, QEvent *event)
{
    if(this->m_bProcessEvent)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            if(watched==this->m_pbRed)
            {
                emit this->ZSigRequestPrecisSetup("red");
            }else if(watched==this->m_pbGreen)
            {
                emit this->ZSigRequestPrecisSetup("green");
            }else if(watched==this->m_pbBlue)
            {
                emit this->ZSigRequestPrecisSetup("blue");
            }
        }
    }
    return QDialog::eventFilter(watched,event);
}
void ZColorTempTuningDia::ZSloSyncColorTemp(QString arguments)
{
    QStringList argList=arguments.split(",");
    for(qint32 i=0;i<argList.size();i++)
    {
        QStringList keyValList=argList.at(i).split("=");
        if(keyValList.size()!=2)
        {
            continue;
        }
        if(keyValList.at(0)=="R")
        {
            //this->m_pbRed->ZSetValue(keyValList.at(1).toInt()/10.0f);
            this->m_pbRed->setValue(keyValList.at(1).toInt()/10.0f);
        }else if(keyValList.at(0)=="G")
        {
            //this->m_pbGreen->ZSetValue(keyValList.at(1).toInt()/10.0f);
            this->m_pbGreen->setValue(keyValList.at(1).toInt()/10.0f);
        }else if(keyValList.at(0)=="B")
        {
            //this->m_pbBlue->ZSetValue(keyValList.at(1).toInt()/10.0f);
            this->m_pbBlue->setValue(keyValList.at(1).toInt()/10.0f);
        }
    }
    emit this->ZSigPopMessage(tr("成功同步至GLaser!"));
}
void ZColorTempTuningDia::ZSloSyncColorTempPercent(QString cmd,QString arguments)
{
    QStringList argList=arguments.split(",");
    for(qint32 i=0;i<argList.size();i++)
    {
        QStringList keyValList=argList.at(i).split("=");
        if(keyValList.size()!=2)
        {
            continue;
        }
        if(keyValList.at(0)=="CT")
        {
            this->m_colorTempName=keyValList.at(1);
            this->m_llTitle->setText(tr("色温模式:    %1").arg(this->m_colorTempName));
        }else if(keyValList.at(0)=="R")
        {
            //this->m_pbRed->ZSetValue(keyValList.at(1).toInt()/10.0f);
            this->m_pbRed->setValue(keyValList.at(1).toInt()/10.0f);
        }else if(keyValList.at(0)=="G")
        {
            //this->m_pbGreen->ZSetValue(keyValList.at(1).toInt()/10.0f);
            this->m_pbGreen->setValue(keyValList.at(1).toInt()/10.0f);
        }else if(keyValList.at(0)=="B")
        {
            //this->m_pbBlue->ZSetValue(keyValList.at(1).toInt()/10.0f);
            this->m_pbBlue->setValue(keyValList.at(1).toInt()/10.0f);
        }
    }
}
void ZColorTempTuningDia::ZSlotSync()
{
    g_GblHelp.beep();
    if(this->m_glaserAddress==0xFF)
    {
        emit this->ZSigRequestCmd("main_GetColorTempPercent","");
    }else{
        emit this->ZSigRequestCmd(QString("%1_GetColorTempPercent").arg(this->m_glaserAddress),"");
    }
}
void ZColorTempTuningDia::ZSlotSave()
{
    g_GblHelp.beep();
    //0.0~100.0  =>  0~1000.(*10).
    //float redVal=this->m_pbRed->ZGetValue();
    float redVal=this->m_pbRed->getValue();
    //float greenVal=this->m_pbGreen->ZGetValue();
    float greenVal=this->m_pbGreen->getValue();
    //float blueVal=this->m_pbBlue->ZGetValue();
    float blueVal=this->m_pbBlue->getValue();
    quint16 redINT=(quint16)(redVal*10);
    quint16 greenINT=(quint16)(greenVal*10);
    quint16 blueINT=(quint16)(blueVal*10);
    QStringList valList;
    valList.append(QString::number(redVal,'f',1));
    valList.append(QString::number(greenVal,'f',1));
    valList.append(QString::number(blueVal,'f',1));
    //save to local database.
    if(!gDB.ZPutColorTempData(this->m_glaserAddress,this->m_colorTempName,valList))
    {
        emit this->ZSigPopMessage(tr("色温模式%1配比保存成功!\nRGB:<%2%,%3%,%4%>!").arg(this->m_colorTempName).arg(QString::number(redVal,'f',1)).arg(QString::number(greenVal,'f',1)).arg(QString::number(blueVal,'f',1)));
    }else{
        emit this->ZSigPopMessage(tr("色温模式%1配比保存失败!").arg(this->m_colorTempName));
    }
    //save to PLaser.
    if(this->m_glaserAddress==0xFF)
    {
        emit this->ZSigRequestCmd("main_SetColorTempPercent",QString("CT=%1,R=%2,G=%3,B=%4").arg(this->m_colorTempName).arg(QString::number(redINT,10)).arg(QString::number(greenINT,10)).arg(QString::number(blueINT,10)));
    }else{
        emit this->ZSigRequestCmd(QString("%1_SetColorTempPercent").arg(this->m_glaserAddress),QString("CT=%1,R=%2,G=%3,B=%4").arg(this->m_colorTempName).arg(QString::number(redINT,10)).arg(QString::number(greenINT,10)).arg(QString::number(blueINT,10)));
    }
}
void ZColorTempTuningDia::ZSlotDefault()
{
    g_GblHelp.beep();
    this->m_pbRed->setValue(this->m_redDefault);
    this->m_pbGreen->setValue(this->m_greenDefault);
    this->m_pbBlue->setValue(this->m_blueDefault);
}
void ZColorTempTuningDia::ZSlotClose()
{
    this->reject();
}
void ZColorTempTuningDia::ZSlotValueChanged()
{
    QToolButton *src=qobject_cast<QToolButton*>(this->sender());
    if(src==NULL)
    {
        return;
    }
    if(src==this->m_tbRedDec)
    {
        //if(this->m_pbRed->ZGetValue()>=1.0f)
        if(this->m_pbRed->getValue()>=0.1f)
        {
            //this->m_pbRed->ZSetValue(this->m_pbRed->ZGetValue()-1.0f);
            //this->m_pbRed->setValue(this->m_pbRed->getValue()-1.0f);
            float redVal=this->m_pbRed->getValue()-0.1f;
            quint16 redINT=(quint16)(redVal*10.0f);
            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("red=%1").arg(redINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("red=%1").arg(redINT));
            }
        }
    }else if(src==this->m_tbRedInc)
    {
        //if(this->m_pbRed->ZGetValue()<100.0f)
        if(this->m_pbRed->getValue()<100.0f)
        {
            //this->m_pbRed->ZSetValue(this->m_pbRed->ZGetValue()+1.0f);
            //this->m_pbRed->setValue(this->m_pbRed->getValue()+1.0f);
            float redVal=this->m_pbRed->getValue()+0.1f;
            quint16 redINT=(quint16)(redVal*10.0f);
            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("red=%1").arg(redINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("red=%1").arg(redINT));
            }
        }
    }else if(src==this->m_tbGreenDec)
    {
        //if(this->m_pbGreen->ZGetValue()>=1.0f)
        if(this->m_pbGreen->getValue()>=0.1f)
        {
            //this->m_pbGreen->ZSetValue(this->m_pbGreen->ZGetValue()-1.0f);
            //this->m_pbGreen->setValue(this->m_pbGreen->getValue()-0.1f);
            float greenVal=this->m_pbGreen->getValue()-0.1f;
            quint16 greenINT=(quint16)(greenVal*10.0f);
            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("green=%1").arg(greenINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("green=%1").arg(greenINT));
            }
        }
    }else if(src==this->m_tbGreenInc)
    {
        //if(this->m_pbGreen->ZGetValue()<100.0f)
        if(this->m_pbGreen->getValue()<100.0f)
        {
            //this->m_pbGreen->ZSetValue(this->m_pbGreen->ZGetValue()+1.0f);
            //this->m_pbGreen->setValue(this->m_pbGreen->getValue()+1.0f);
            float greenVal=this->m_pbGreen->getValue()+0.1f;
            quint16 greenINT=(quint16)(greenVal*10.0f);
            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("green=%1").arg(greenINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("green=%1").arg(greenINT));
            }
        }
    }else if(src==this->m_tbBlueDec)
    {
        //if(this->m_pbBlue->ZGetValue()>=1.0f)
        if(this->m_pbBlue->getValue()>=1.0f)
        {
            //this->m_pbBlue->ZSetValue(this->m_pbBlue->ZGetValue()-1.0f);
            //this->m_pbBlue->setValue(this->m_pbBlue->getValue()-1.0f);
            float blueVal=this->m_pbBlue->getValue()-0.1f;
            quint16 blueINT=(quint16)(blueVal*10.0f);

            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("blue=%1").arg(blueINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("blue=%1").arg(blueINT));
            }
        }
    }else if(src==this->m_tbBlueInc)
    {
        //if(this->m_pbBlue->ZGetValue()<100.0f)
        if(this->m_pbBlue->getValue()<100.0f)
        {
            //this->m_pbBlue->ZSetValue(this->m_pbBlue->ZGetValue()+1.0f);
            //this->m_pbBlue->setValue(this->m_pbBlue->getValue()+1.0f);
            float blueVal=this->m_pbBlue->getValue()+0.1f;
            quint16 blueINT=(quint16)(blueVal*10.0f);

            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("blue=%1").arg(blueINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("blue=%1").arg(blueINT));
            }
        }
    }
    g_GblHelp.beep();
}
void ZColorTempTuningDia::ZSloAdjRGBPercent(QString cmd,QString arguments)
{

    emit this->ZSigDumpDbgMsg(tr("ZSloAdjRGBPercent:cmd:%1,argument:%2").arg(cmd).arg(arguments));
    if(cmd.right(6)=="tuning")
    {
        if((cmd.startsWith("main") && this->m_glaserAddress==0xFF) || (cmd.left(1).toInt()==this->m_glaserAddress))
        {
            QStringList argList=arguments.split(",");
            for(qint32 i=0;i<argList.size();i++)
            {
                QStringList keyValList=argList.at(i).split("=");
                if(keyValList.size()!=2)
                {
                    continue;
                }
                if(keyValList.at(0)=="red")
                {
                    qint16 percent=keyValList.at(1).toInt();
                    if((qint16)0xFFFF==percent)
                    {
                        emit this->ZSigPopMessage(tr("PLaser返回错误,Red调节失败!"));
                        continue;
                    }
                    this->m_pbRed->setValue(percent/10.0f);
                }else if(keyValList.at(0)=="green")
                {
                    qint16 percent=keyValList.at(1).toInt();
                    if((qint16)0xFFFF==percent)
                    {
                        emit this->ZSigPopMessage(tr("PLaser返回错误,Green调节失败!"));
                        continue;
                    }
                    this->m_pbGreen->setValue(percent/10.0f);
                }else if(keyValList.at(0)=="blue")
                {
                    qint16 percent=keyValList.at(1).toInt();
                    if((qint16)0xFFFF==percent)
                    {
                        emit this->ZSigPopMessage(tr("PLaser返回错误,Blue调节失败!"));
                        continue;
                    }
                    this->m_pbBlue->setValue(percent/10.0f);
                }
            }
        }
    }
}
void ZColorTempTuningDia::ZSlotShowPrecisSetupDialog(QString channel)
{
    g_GblHelp.beep();
    if(channel=="red")
    {
        this->m_bProcessEvent=false;
        ZPwrPrecisionSetDialog dia(tr("Red功率 精准设置"));
        connect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
        if(dia.exec()==QDialog::Accepted)
        {
            float precisVal=dia.ZGetPrecisionPwrValue();
            //this->m_pbRed->ZSetValue(precisVal);
            //this->m_pbRed->setValue(precisVal);
            quint16 redINT=(quint16)(precisVal*10.0f);
            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("red=%1").arg(redINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("red=%1").arg(redINT));
            }
        }
        disconnect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
        this->m_bProcessEvent=true;
    }else if(channel=="green")
    {
        this->m_bProcessEvent=false;
        ZPwrPrecisionSetDialog dia(tr("Green功率 精准设置"));
        connect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
        if(dia.exec()==QDialog::Accepted)
        {
            float precisVal=dia.ZGetPrecisionPwrValue();
            //this->m_pbGreen->ZSetValue(precisVal);
            //this->m_pbGreen->setValue(precisVal);
            quint16 greenINT=(quint16)(precisVal*10.0f);
            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("green=%1").arg(greenINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("green=%1").arg(greenINT));
            }
        }
        disconnect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
        this->m_bProcessEvent=true;
    }else if(channel=="blue")
    {
        this->m_bProcessEvent=false;
        ZPwrPrecisionSetDialog dia(tr("Green功率 精准设置"));
        connect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
        if(dia.exec()==QDialog::Accepted)
        {
            float precisVal=dia.ZGetPrecisionPwrValue();
            //this->m_pbBlue->ZSetValue(precisVal);
            //this->m_pbBlue->setValue(precisVal);
            quint16 blueINT=(quint16)(precisVal*10.0f);
            if(this->m_glaserAddress==0xFF)
            {
                emit this->ZSigRequestCmd(QString("main_tuning"),QString("blue=%1").arg(blueINT));
            }else{
                emit this->ZSigRequestCmd(QString("%1_tuning").arg(QString::number(this->m_glaserAddress,10)),QString("blue=%1").arg(blueINT));
            }
        }
        disconnect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopMessage(QString)));
        this->m_bProcessEvent=true;
    }
}
