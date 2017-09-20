#include "zwaitingdialog.h"

ZWaitingDialog::ZWaitingDialog(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setObjectName("ZWaitingDialogLabel");
    this->m_llTitle->setText(title);
    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setObjectName("ZWaitingDialogButton");
    this->m_tbOkay->setText(tr("确认"));
    this->m_tbOkay->setVisible(false);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_llTitle);
    this->m_vLayout->addWidget(this->m_tbOkay);
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(accept()));
}
ZWaitingDialog::~ZWaitingDialog()
{
    delete this->m_llTitle;
    delete this->m_tbOkay;
    delete this->m_vLayout;
}
void ZWaitingDialog::ZSloClose(qint32 retCode,QString errMsg)
{
    if(0==retCode)
    {
        this->accept();
        return;
    }
    this->m_llTitle->setText(errMsg);
    this->m_tbOkay->setVisible(true);
}
