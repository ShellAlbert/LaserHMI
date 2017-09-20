#include "zspinbox.h"

ZSpinBox::ZSpinBox(QString title,qint32 min,qint32 max,qint32 val)
{
    this->setObjectName("ZSpinBoxFrame");
    this->m_valMin=min;
    this->m_valMax=max;
    this->m_valNow=val;
    this->m_llTitle=new QLabel(title);
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    this->m_llTitle->setObjectName("ZSpinBoxLabel");
    this->m_tbDec=new QToolButton;
    this->m_tbDec->setIconSize(QSize(24,24));
    this->m_tbDec->setObjectName("ZSpinBoxDecButton");
    this->m_tbInc=new QToolButton;
    this->m_tbInc->setIconSize(QSize(24,24));
    this->m_tbInc->setObjectName("ZSpinBoxIncButton");
    this->m_leVal=new QLineEdit;
    this->m_leVal->setAlignment(Qt::AlignCenter);
    this->m_leVal->setFocusPolicy(Qt::NoFocus);
    this->m_leVal->setText(QString("%1").arg(this->m_valNow));
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->setContentsMargins(10,20,10,20);
    this->m_hLayout->addWidget(this->m_llTitle);
    this->m_hLayout->addWidget(this->m_tbDec);
    this->m_hLayout->addWidget(this->m_leVal);
    this->m_hLayout->addWidget(this->m_tbInc);
    this->setLayout(this->m_hLayout);

    connect(this->m_tbDec,SIGNAL(clicked(bool)),this,SLOT(ZSloValChanged()));
    connect(this->m_tbInc,SIGNAL(clicked(bool)),this,SLOT(ZSloValChanged()));
}
ZSpinBox::~ZSpinBox()
{
    delete this->m_llTitle;
    delete this->m_tbDec;
    delete this->m_leVal;
    delete this->m_tbInc;
    delete this->m_hLayout;
}
void ZSpinBox::ZSloValChanged()
{
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    if(srcTb==this->m_tbDec)
    {
        if(this->m_valNow>this->m_valMin)
        {
            this->m_valNow--;
            this->m_leVal->setText(QString("%1").arg(this->m_valNow));
            emit this->ZSigValueChanged(this->m_valNow);
        }else{
            emit this->ZSigMsg(tr("已达到最小值!"));
        }
    }else if(srcTb==this->m_tbInc)
    {
        if(this->m_valNow<this->m_valMax)
        {
            this->m_valNow++;
            this->m_leVal->setText(QString("%1").arg(this->m_valNow));
            emit this->ZSigValueChanged(this->m_valNow);
        }else{
            emit this->ZSigMsg(tr("已达到最大值!"));
        }
    }
}
