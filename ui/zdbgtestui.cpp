#include "zdbgtestui.h"
#include "gbl/hmi_const.h"
ZDbgTestUI::ZDbgTestUI()
{

}
ZDbgTestUI::~ZDbgTestUI()
{

}
void ZDbgTestUI::ZDoInitial()
{
    this->m_cbEnSelfCheck=new ZCheckBox(tr("通信自检"));
    this->m_cbEnSelfCheck->ZSetChecked(g_GblHelp.m_bEnSelfCheck);
    this->m_cbEnHeartBeat=new ZCheckBox(tr("心跳检测"));
    this->m_cbEnHeartBeat->ZSetChecked(g_GblHelp.m_bEnHeartBeat);
    this->m_cbPopupErrMsg=new ZCheckBox(tr("错误弹窗"));
    this->m_cbPopupErrMsg->ZSetChecked(g_GblHelp.m_bEnPopupErrMsg);
    this->m_cbEnCRC16=new ZCheckBox(tr("通信校验"));
    this->m_cbEnCRC16->ZSetChecked(g_GblHelp.m_bEnCRC16);
    this->m_cbEnEncrypt=new ZCheckBox(tr("通信加密"));
    this->m_cbEnEncrypt->ZSetChecked(g_GblHelp.m_bEnEncrypt);
    this->m_cbOpTips=new ZCheckBox(tr("操作提示"));
    this->m_cbOpTips->ZSetChecked(g_GblHelp.m_bOpTips);
    this->m_cbQueryStatus=new ZCheckBox(tr("状态查询"));
    this->m_cbQueryStatus->ZSetChecked(g_GblHelp.m_bQueryStatus);
    this->m_cbBeep=new ZCheckBox(tr("声音提示"));
    this->m_cbBeep->ZSetChecked(g_GblHelp.m_bBeep);

    this->m_sbHBInterval=new ZSpinBox(tr("心跳频率(s)"),1,120,g_GblHelp.m_nHeartBeatInterval);
    this->m_sbTimeoutVal=new ZSpinBox(tr("超时时长(s)"),1,120,g_GblHelp.m_nTimeoutVal);
    this->m_sbQueryInterval=new ZSpinBox(tr("查询间隔(s)"),1,120,g_GblHelp.m_nQueryInterval);
    this->m_sbOfflineTimeout=new ZSpinBox(tr("掉线超时(s)"),1,120,g_GblHelp.m_nOfflineTimeout);

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_cbEnSelfCheck,0,0,1,1);
    this->m_gLayout->addWidget(this->m_cbEnHeartBeat,0,1,1,1);
    this->m_gLayout->addWidget(this->m_cbPopupErrMsg,0,2,1,1);
    this->m_gLayout->addWidget(this->m_cbEnCRC16,0,3,1,1);
    this->m_gLayout->addWidget(this->m_cbEnEncrypt,0,4,1,1);
    this->m_gLayout->addWidget(this->m_cbOpTips,1,0,1,1);
    this->m_gLayout->addWidget(this->m_cbQueryStatus,1,1,1,1);
    this->m_gLayout->addWidget(this->m_cbBeep,1,2,1,1);

    this->m_gLayout->addWidget(this->m_sbHBInterval,2,0,1,2);
    this->m_gLayout->addWidget(this->m_sbTimeoutVal,2,3,1,2);
    this->m_gLayout->addWidget(this->m_sbQueryInterval,3,0,1,2);
    this->m_gLayout->addWidget(this->m_sbOfflineTimeout,3,3,1,2);

    this->m_gLayout->setContentsMargins(10,30,10,10);
    this->m_vLayoutMain=new QVBoxLayout;
    this->m_vLayoutMain->addLayout(this->m_gLayout);
    this->m_vLayoutMain->addStretch(1);
    this->setLayout(this->m_vLayoutMain);

    connect(this->m_cbEnSelfCheck,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));
    connect(this->m_cbEnHeartBeat,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));
    connect(this->m_cbEnCRC16,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));
    connect(this->m_cbEnEncrypt,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));
    connect(this->m_cbPopupErrMsg,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));
    connect(this->m_cbOpTips,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));
    connect(this->m_cbQueryStatus,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));
    connect(this->m_cbBeep,SIGNAL(ZSigStateChanged(qint32)),this,SLOT(ZSloCheckBoxStateChanged(qint32)));

    connect(this->m_sbHBInterval,SIGNAL(ZSigValueChanged(qint32)),this,SLOT(ZSloHBIntervalValChanged(qint32)));
    connect(this->m_sbHBInterval,SIGNAL(ZSigMsg(QString)),this,SIGNAL(ZSigMsg(QString)));


    connect(this->m_sbTimeoutVal,SIGNAL(ZSigValueChanged(qint32)),this,SLOT(ZSloTimeoutValChanged(qint32)));
    connect(this->m_sbTimeoutVal,SIGNAL(ZSigMsg(QString)),this,SIGNAL(ZSigMsg(QString)));

    connect(this->m_sbQueryInterval,SIGNAL(ZSigValueChanged(qint32)),this,SLOT(ZSloQueryIntervalValChanged(qint32)));
    connect(this->m_sbQueryInterval,SIGNAL(ZSigMsg(QString)),this,SIGNAL(ZSigMsg(QString)));

    connect(this->m_sbOfflineTimeout,SIGNAL(ZSigValueChanged(qint32)),this,SLOT(ZSloOfflineTimeoutValChanged(qint32)));
}
void ZDbgTestUI::ZUnInitial()
{
    delete this->m_cbEnSelfCheck;
    delete this->m_cbEnHeartBeat;
    delete this->m_cbPopupErrMsg;
    delete this->m_cbEnCRC16;
    delete this->m_cbEnEncrypt;
    delete this->m_cbOpTips;
    delete this->m_cbQueryStatus;
    delete this->m_cbBeep;
    delete this->m_sbHBInterval;
    delete this->m_sbTimeoutVal;
    delete this->m_sbQueryInterval;
    delete this->m_sbOfflineTimeout;

    delete this->m_gLayout;
    delete this->m_vLayoutMain;
}
void ZDbgTestUI::ZSloCheckBoxStateChanged(qint32 state)
{
    g_GblHelp.beep();
    ZCheckBox *srcCb=qobject_cast<ZCheckBox*>(this->sender());
    if(srcCb==this->m_cbEnSelfCheck)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bEnSelfCheck=true;
        }else{
            g_GblHelp.m_bEnSelfCheck=false;
        }
    }else if(srcCb==this->m_cbEnHeartBeat)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bEnHeartBeat=true;
        }else{
            g_GblHelp.m_bEnHeartBeat=false;
        }
    }else if(srcCb==this->m_cbPopupErrMsg)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bEnPopupErrMsg=true;
        }else{
            g_GblHelp.m_bEnPopupErrMsg=false;
        }
    }else if(srcCb==this->m_cbEnCRC16)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bEnCRC16=true;
        }else{
            g_GblHelp.m_bEnCRC16=false;
        }
    }else if(srcCb==this->m_cbEnEncrypt)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bEnEncrypt=true;
        }else{
            g_GblHelp.m_bEnEncrypt=false;
        }
    }else if(srcCb==this->m_cbOpTips)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bOpTips=true;
        }else{
            g_GblHelp.m_bOpTips=false;
        }
    }else if(srcCb==this->m_cbQueryStatus)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bQueryStatus=true;
        }else{
            g_GblHelp.m_bQueryStatus=false;
        }
    }else if(srcCb==this->m_cbBeep)
    {
        if(srcCb->ZIsChecked())
        {
            g_GblHelp.m_bBeep=true;
        }else{
            g_GblHelp.m_bBeep=false;
        }
    }
    if(srcCb->ZIsChecked())
    {
        emit this->ZSigMsg(tr("%1已启用!").arg(srcCb->text()));
    }else{
        emit this->ZSigMsg(tr("%1已停用!").arg(srcCb->text()));
    }
}
void ZDbgTestUI::ZSloHBIntervalValChanged(qint32 val)
{
    g_GblHelp.beep();
    g_GblHelp.m_nHeartBeatInterval=val;
}
void ZDbgTestUI::ZSloTimeoutValChanged(qint32 val)
{
    g_GblHelp.beep();
    g_GblHelp.m_nTimeoutVal=val;
}
void ZDbgTestUI::ZSloQueryIntervalValChanged(qint32 val)
{
    g_GblHelp.beep();
    g_GblHelp.m_nQueryInterval=val;
}
void ZDbgTestUI::ZSloOfflineTimeoutValChanged(qint32 val)
{
    g_GblHelp.beep();
    g_GblHelp.m_nOfflineTimeout=val;
}
