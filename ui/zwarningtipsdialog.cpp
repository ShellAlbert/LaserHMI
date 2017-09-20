#include "zwarningtipsdialog.h"

ZWarningTipsDialog::ZWarningTipsDialog(QString title,QString tipsMsg)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(300,200);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(title);
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    this->m_llTitle->setObjectName("ZWarningTipsDialogTitle");

    this->m_llTipsMsg=new QLabel;
    this->m_llTipsMsg->setText(tipsMsg);
    this->m_llTipsMsg->setAlignment(Qt::AlignCenter);
    this->m_llTipsMsg->setObjectName("ZWarningTipsDialogTipsMsg");

    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("确认"));
    this->m_tbOkay->setObjectName("ZWarningTipsDialogOkay");
    connect(this->m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(accept()));

    this->m_vLayoutFrm=new QVBoxLayout;
    this->m_vLayoutFrm->addWidget(this->m_llTitle,1,Qt::AlignCenter);
    this->m_vLayoutFrm->addWidget(this->m_llTipsMsg,1,Qt::AlignCenter);
    this->m_vLayoutFrm->addWidget(this->m_tbOkay,0,Qt::AlignRight);

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZWarningTipsDialog");
    this->m_frm->setLayout(this->m_vLayoutFrm);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);
}
ZWarningTipsDialog::~ZWarningTipsDialog()
{
    delete this->m_llTitle;
    delete this->m_llTipsMsg;
    delete this->m_tbOkay;
    delete this->m_vLayoutFrm;
    delete this->m_frm;
    delete this->m_vLayout;
}
