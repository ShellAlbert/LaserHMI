#include "zloadui.h"
#include <QApplication>
ZLoadUI::ZLoadUI()
{
    this->setObjectName("ZLoadUI");
    this->m_llMsg=new QLabel(tr("系统启动中..."));
    this->m_llMsg->setObjectName("ZLoadUIMsgLabel");
    this->m_llMsg->setAlignment(Qt::AlignCenter);
    this->m_progressBar=new QProgressBar;
    this->m_progressBar->setObjectName("ZLoadUIProgressBar");
    this->m_progressBar->setTextVisible(false);
    this->m_progressBar->setRange(0,100);
    this->m_progressBar->setValue(0);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addStretch(1);
    this->m_vLayout->addWidget(this->m_llMsg);
    this->m_vLayout->addWidget(this->m_progressBar);
    this->m_vLayout->addStretch(1);
    this->setLayout(this->m_vLayout);
    this->setStyleSheet("QFrame{background-image: url(:/load/images/load/background.png);}"
                        "QLabel{color:white;font-size:30px;background: transparent;}"
                        "QProgressBar{min-height:30px;border: 1px;border-radius: 5px;color: black;background: rgb(67,80,86);}"
                        "QProgressBar::chunk {background: rgb(85,161,47);}");
}
ZLoadUI::~ZLoadUI()
{
    delete this->m_llMsg;
    delete this->m_progressBar;
    delete this->m_vLayout;
}
void ZLoadUI::ZSetMsg(QString msg)
{
    this->m_llMsg->setText(msg);
}
void ZLoadUI::ZSetProgress(qint32 val)
{
    this->m_progressBar->setValue(val);
}
void ZLoadUI::ZSloShowMsg(QString msg)
{
    this->m_llMsg->setText(msg);
    qApp->processEvents();
}
