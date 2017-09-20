#include "zlogfilterdialog.h"

ZLogFilterDialog::ZLogFilterDialog(QString title)
{
    this->setObjectName("ZLogFilterDialog");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(400,200);
    this->m_llTitle=new QLabel(title);
    this->setObjectName("ZLogFilterDialogTitleLabel");
    this->m_llTitle->setAlignment(Qt::AlignCenter);
    QStringList typeList;
    typeList<<tr("All");
    typeList<<tr("Power");
    typeList<<tr("Setting");
    typeList<<tr("Tuning");
    this->m_cbType=new ZTextComboBox(tr("日志类型"),typeList);
    this->m_beginTime=new ZDateTimeChooser(tr("起始时间"));
    this->m_endTime=new ZDateTimeChooser(tr("终止时间"));
    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("Okay"));
    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("Cancel"));
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addStretch(1);
    this->m_hLayout->addWidget(this->m_tbOkay);
    this->m_hLayout->addWidget(this->m_tbCancel);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_llTitle);
    this->m_vLayout->addWidget(this->m_cbType);
    this->m_vLayout->addWidget(this->m_beginTime);
    this->m_vLayout->addWidget(this->m_endTime);
    this->m_vLayout->addLayout(this->m_hLayout);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(this->m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}
ZLogFilterDialog::~ZLogFilterDialog()
{
    delete this->m_llTitle;
    delete this->m_cbType;
    delete this->m_beginTime;
    delete this->m_endTime;
    delete this->m_tbOkay;
    delete this->m_tbCancel;
    delete this->m_hLayout;
    delete this->m_vLayout;
}
