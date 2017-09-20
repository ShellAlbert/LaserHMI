#include "zpwrprecisionsetdialog.h"
#include <gbl/hmi_const.h>
ZPwrPrecisionSetDialog::ZPwrPrecisionSetDialog(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(title);
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    this->m_llTitle->setObjectName("ZPwrPrecisionSetDialog_Title");
    this->m_le=new QLineEdit;
    this->m_le->setMaxLength(5);
    this->m_le->setObjectName("ZPwrPrecisionSetDialog_LineEdit");
    for(qint32 i=0;i<14;i++)
    {
        this->m_tb[i]=new QToolButton;
        this->m_tb[i]->setObjectName("ZPwrPrecisionSetDialog_ToolButton");
        connect(this->m_tb[i],SIGNAL(clicked(bool)),this,SLOT(ZSlotBtnPressed()));
        switch(i)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            this->m_tb[i]->setText(tr("%1").arg(i));
            break;
        case 10:
            this->m_tb[i]->setText(tr("."));
            break;
        case 11:
            this->m_tb[i]->setText(tr("<-"));
            break;
        case 12:
            this->m_tb[i]->setText(tr("确定"));
            break;
        case 13:
            this->m_tb[i]->setText(tr("取消"));
            break;
        default:
            break;
        }
    }
    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,3);
    this->m_gLayout->addWidget(this->m_le,1,0,1,3);
    this->m_gLayout->addWidget(this->m_tb[1],2,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[2],2,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[3],2,2,1,1);
    this->m_gLayout->addWidget(this->m_tb[4],3,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[5],3,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[6],3,2,1,1);
    this->m_gLayout->addWidget(this->m_tb[7],4,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[8],4,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[9],4,2,1,1);
    this->m_gLayout->addWidget(this->m_tb[0],5,0,1,1);
    this->m_gLayout->addWidget(this->m_tb[10],5,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[11],5,2,1,1);
    this->m_gLayout->addWidget(this->m_tb[12],6,1,1,1);
    this->m_gLayout->addWidget(this->m_tb[13],6,2,1,1);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZPwrPrecisionSetDialog_Frame");
    this->m_frm->setLayout(this->m_gLayout);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);
}

ZPwrPrecisionSetDialog::~ZPwrPrecisionSetDialog()
{
    delete this->m_llTitle;
    delete this->m_le;
    for(qint32 i=0;i<14;i++)
    {
        delete this->m_tb[i];
    }
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}

float ZPwrPrecisionSetDialog::ZGetPrecisionPwrValue()
{
    return this->m_le->text().toFloat();
}

void ZPwrPrecisionSetDialog::ZSlotBtnPressed()
{
    g_GblHelp.beep();
    QToolButton *tbSrc=qobject_cast<QToolButton*>(this->sender());
    if(tbSrc==NULL)
    {
        return;
    }
    QString tbText=tbSrc->text();
    if(tbText==tr("确定"))
    {
        //check empty.
        if(this->m_le->text().trimmed().isEmpty())
        {
            emit this->ZSigPopupMsg(tr("请输入精确的功率百分比值!"));
            return;
        }
        //check if it can be convert to float.
        bool bOkay;
        float precisVal=this->m_le->text().trimmed().toFloat(&bOkay);
        if(!bOkay)
        {
            emit this->ZSigPopupMsg(tr("输入无效!"));
            return;
        }
        if(precisVal>100.0 || precisVal<0.0f)
        {
            emit this->ZSigPopupMsg(tr("输入无效!有效范围0.0~100.0!"));
            return;
        }
        this->accept();
    }else if(tbText==tr("取消"))
    {
        this->reject();
    }else if(tbText=="<-")
    {
        QString oldText=this->m_le->text().trimmed();
        this->m_le->setText(oldText.left(oldText.length()-1));
    }else{
        this->m_le->setText(this->m_le->text()+tbText);
    }
}
ZChillerSetDialog::ZChillerSetDialog(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(title);
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    this->m_llTitle->setObjectName("ZChillerSetDialog_Title");

    this->m_cbEnFangJieLu=new ZCheckBox(tr("防结露"));
    this->m_leChillerTemp=new ZTextLineEditLabel(tr("恒温值"),"0");
    connect(this->m_leChillerTemp,SIGNAL(ZSigClicked()),this,SLOT(ZSlotSetChillerTemp()));

    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("确认"));
    this->m_tbOkay->setObjectName("ZChillerSetDialog_ToolButton");
    connect(m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(ZSlotBtnPressed()));

    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("取消"));
    this->m_tbCancel->setObjectName("ZChillerSetDialog_ToolButton");
    connect(m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,2);
    this->m_gLayout->addWidget(this->m_cbEnFangJieLu,1,0,1,1);
    this->m_gLayout->addWidget(this->m_leChillerTemp,2,0,1,1);
    this->m_gLayout->addWidget(this->m_tbOkay,3,0,1,1);
    this->m_gLayout->addWidget(this->m_tbCancel,3,1,1,1);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZChillerSetDialog_Frame");
    this->m_frm->setLayout(this->m_gLayout);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);
}
ZChillerSetDialog::~ZChillerSetDialog()
{
    delete this->m_llTitle;
    delete this->m_cbEnFangJieLu;
    delete this->m_leChillerTemp;
    delete this->m_tbOkay;
    delete this->m_tbCancel;
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
void ZChillerSetDialog::ZSetValue(QString tempValue,bool bEnJieLu)
{
    this->m_leChillerTemp->ZSetLineEdit(tempValue);
    this->m_cbEnFangJieLu->ZSetChecked(bEnJieLu);
}
float ZChillerSetDialog::ZGetValue(void)
{
    return this->m_leChillerTemp->ZGetLineEdit().toFloat();
}
bool ZChillerSetDialog::ZIsEnJieLu(void)
{
    return this->m_cbEnFangJieLu->ZIsChecked();
}
void ZChillerSetDialog::ZSlotBtnPressed()
{
    //check empty.
    if(this->m_leChillerTemp->ZGetLineEdit().trimmed().isEmpty())
    {
        emit this->ZSigPopupMsg(tr("请输入恒温值!"));
        return;
    }
    //check if it can be convert to float.
    bool bOkay;
    float precisVal=this->m_leChillerTemp->ZGetLineEdit().trimmed().toFloat(&bOkay);
    if(!bOkay)
    {
        emit this->ZSigPopupMsg(tr("输入无效!"));
        return;
    }
    if(precisVal<18.0f || precisVal>30.0f)
    {
        emit this->ZSigPopupMsg(tr("错误，恒温值越界，有效范围[18.0~30.0]。"));
        return;
    }
    this->accept();
}
void ZChillerSetDialog::ZSlotSetChillerTemp()
{
    ZPwrPrecisionSetDialog setDia("设置恒温值");
    connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    if(setDia.exec()==QDialog::Accepted)
    {
        float precisVal=setDia.ZGetPrecisionPwrValue();
        if(precisVal<18.0f || precisVal>30.0f)
        {
            emit this->ZSigPopupMsg(tr("错误，恒温值越界，有效范围[18.0~30.0]。"));
        }else{
            this->m_leChillerTemp->ZSetLineEdit(QString::number(precisVal,'f',1));
        }
    }
    disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
}
ZChillerSetEnterFlowDialog::ZChillerSetEnterFlowDialog(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(title);
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    this->m_llTitle->setObjectName("ZChillerSetEnterFlowDialog_Title");

    this->m_leFlow=new ZTextLineEditLabel(tr("回水流量(L/min)"),"0");
    connect(this->m_leFlow,SIGNAL(ZSigClicked()),this,SLOT(ZSlotPopupKeyboard()));
    this->m_lePercent=new ZTextLineEditLabel(tr("报警阈值(%)"),"0");
    connect(this->m_lePercent,SIGNAL(ZSigClicked()),this,SLOT(ZSlotPopupKeyboard()));

    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("确认"));
    this->m_tbOkay->setObjectName("ZChillerSetEnterFlowDialog_ToolButton");
    connect(m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(ZSlotBtnPressed()));

    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("取消"));
    this->m_tbCancel->setObjectName("ZChillerSetEnterFlowDialog_ToolButton");
    connect(m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,2);
    this->m_gLayout->addWidget(this->m_leFlow,1,0,1,2);
    this->m_gLayout->addWidget(this->m_lePercent,2,0,1,2);
    this->m_gLayout->addWidget(this->m_tbOkay,3,0,1,1);
    this->m_gLayout->addWidget(this->m_tbCancel,3,1,1,1);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZChillerSetEnterFlowDialog_Frame");
    this->m_frm->setLayout(this->m_gLayout);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);
}
ZChillerSetEnterFlowDialog::~ZChillerSetEnterFlowDialog()
{
    delete this->m_llTitle;
    delete this->m_leFlow;
    delete this->m_lePercent;
    delete this->m_tbOkay;
    delete this->m_tbCancel;
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
void ZChillerSetEnterFlowDialog::ZSlotBtnPressed()
{
    //check empty.
    if(this->m_leFlow->ZGetLineEdit().trimmed().isEmpty())
    {
        emit this->ZSigPopupMsg(tr("请输入回水流量!"));
        return;
    }
    if(this->m_lePercent->ZGetLineEdit().trimmed().isEmpty())
    {
        emit this->ZSigPopupMsg(tr("请输入监控阈值百分比!"));
        return;
    }

    //check if it can be convert to float.
    bool bOkay;
    float flowVal=this->m_leFlow->ZGetLineEdit().trimmed().toFloat(&bOkay);
    if(!bOkay)
    {
        emit this->ZSigPopupMsg(tr("输入回水流量无效!"));
        return;
    }
    float percentVal=this->m_leFlow->ZGetLineEdit().trimmed().toFloat(&bOkay);
    if(!bOkay)
    {
        emit this->ZSigPopupMsg(tr("输入监控阈值百分比无效!"));
        return;
    }
    this->accept();
}
void ZChillerSetEnterFlowDialog::ZSlotPopupKeyboard()
{
    ZTextLineEditLabel *sender=qobject_cast<ZTextLineEditLabel*>(this->sender());
    if(sender==this->m_leFlow)
    {
        ZPwrPrecisionSetDialog setDia("设置回水流量");
        connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        if(setDia.exec()==QDialog::Accepted)
        {
            float precisVal=setDia.ZGetPrecisionPwrValue();
            if(precisVal<0.0f || precisVal>100.0f)
            {
                emit this->ZSigPopupMsg(tr("错误，回水流量越界，有效范围[0.0~100.0]。"));
            }else{
                this->m_leFlow->ZSetLineEdit(QString::number(precisVal,'f',1));
            }
        }
        disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    }else if(sender==this->m_lePercent)
    {
        ZPwrPrecisionSetDialog setDia("设置报警域值百分比");
        connect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
        if(setDia.exec()==QDialog::Accepted)
        {
            float precisVal=setDia.ZGetPrecisionPwrValue();
            if(precisVal<0.0f || precisVal>100.0f)
            {
                emit this->ZSigPopupMsg(tr("错误，报警域值越界，有效范围[0.0~100.0]。"));
            }else{
                this->m_lePercent->ZSetLineEdit(QString::number(precisVal,'f',1));
            }
        }
        disconnect(&setDia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    }
}
QString ZChillerSetEnterFlowDialog::ZGetFlowValue()
{
    return this->m_leFlow->ZGetLineEdit();
}
QString ZChillerSetEnterFlowDialog::ZGetMonitorPercent()
{
    return this->m_lePercent->ZGetLineEdit();
}
