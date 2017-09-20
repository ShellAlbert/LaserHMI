#include "zoperatelogui.h"
#include <QHeaderView>
#include <QDebug>
#include "gbl/hmi_const.h"
#include <QToolTip>
#include "ui/log/zlogfilterdialog.h"
ZOperateLogUI::ZOperateLogUI()
{

}
ZOperateLogUI::~ZOperateLogUI()
{

}

void ZOperateLogUI::ZDoInitial()
{
    this->m_tbFilter=new QToolButton;
    this->m_tbFilter->setText(tr("Filter"));
    this->m_tbFilter->setIcon(QIcon(":/log/images/log/Search.png"));
    this->m_tbFilter->setIconSize(QSize(24,24));
    this->m_tbFilter->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbExport=new QToolButton;
    this->m_tbExport->setText(tr("导出..."));
    this->m_tbExport->setIcon(QIcon(":/log/images/log/Export.png"));
    this->m_tbExport->setIconSize(QSize(24,24));
    this->m_tbExport->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbDelete=new QToolButton;
    this->m_tbDelete->setText(tr("删除"));
    this->m_tbDelete->setIcon(QIcon(":/log/images/log/Trash.png"));
    this->m_tbDelete->setIconSize(QSize(24,24));
    this->m_tbDelete->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    this->m_tbPrevious=new QToolButton;
    this->m_tbPrevious->setObjectName("PreviousButton");
    this->m_tbPrevious->setIcon(QIcon(":/log/images/log/previous.png"));
    this->m_tbPrevious->setIconSize(QSize(20,20));
    this->m_tbNext=new QToolButton;
    this->m_tbNext->setObjectName("NextButton");
    this->m_tbNext->setIcon(QIcon(":/log/images/log/next.png"));
    this->m_tbNext->setIconSize(QSize(20,20));
    this->m_cbPages=new QComboBox;
    this->m_cbPages->addItem(tr("1/10"));

    this->m_hLayoutFilter=new QHBoxLayout;
    this->m_hLayoutFilter->addWidget(this->m_tbFilter);
    this->m_hLayoutFilter->addWidget(this->m_tbExport);
    this->m_hLayoutFilter->addWidget(this->m_tbDelete);
    this->m_hLayoutFilter->addStretch(1);
    this->m_hLayoutFilter->addWidget(this->m_tbPrevious);
    this->m_hLayoutFilter->addWidget(this->m_cbPages);
    this->m_hLayoutFilter->addWidget(this->m_tbNext);

    this->m_treeWidget=new QTreeWidget;
    this->m_treeWidget->setHeaderLabels(QStringList()<<tr("DateTime(时间)")<<tr("LogType(类型)")<<tr("LogMsg(事件)"));
    this->m_treeWidget->setColumnWidth(0,150);
    this->m_treeWidget->setColumnWidth(1,100);
    this->m_treeWidget->setFocusPolicy(Qt::NoFocus);
    this->m_treeWidget->setRootIsDecorated(false);
    this->m_treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);

    this->m_vLayoutLog=new QVBoxLayout;
    this->m_vLayoutLog->addLayout(this->m_hLayoutFilter);
    this->m_vLayoutLog->addWidget(this->m_treeWidget);
    this->m_vLayoutLog->setContentsMargins(5,30,5,2);
    this->m_frmLog=new ZFrame(tr("日志列表"));
    this->m_frmLog->setLayout(this->m_vLayoutLog);


    this->m_llStorage=new QLabel(tr("1/1000"));
    this->m_progressBar=new QProgressBar;
    this->m_progressBar->setObjectName("LogStorageProgressBar");
    this->m_progressBar->setRange(0,1000);
    this->m_progressBar->setValue(0);
    this->m_hLayoutStorage=new QHBoxLayout;
    this->m_hLayoutStorage->addWidget(this->m_llStorage);
    this->m_hLayoutStorage->addWidget(this->m_progressBar);
    this->m_hLayoutStorage->setContentsMargins(5,20,5,2);
    this->m_frmStorage=new ZFrame(tr("Storage(存储)"));
    this->m_frmStorage->setLayout(this->m_hLayoutStorage);


    //main layout.
    this->m_gLayoutMain=new QGridLayout;
    this->m_gLayoutMain->addWidget(this->m_frmLog,0,0,1,1);
    this->m_gLayoutMain->addWidget(this->m_frmStorage,1,0,1,1);
    this->m_gLayoutMain->setRowStretch(0,5);
    this->m_gLayoutMain->setRowStretch(1,1);
    this->setLayout(this->m_gLayoutMain);

    connect(this->m_tbFilter,SIGNAL(clicked(bool)),this,SLOT(ZSloFilter()));
    connect(this->m_tbExport,SIGNAL(clicked(bool)),this,SLOT(ZSloExport()));
    connect(this->m_tbDelete,SIGNAL(clicked(bool)),this,SLOT(ZSloDelete()));
}
void ZOperateLogUI::ZUnInitial()
{
    delete this->m_tbFilter;
    delete this->m_tbExport;
    delete this->m_tbDelete;
    delete this->m_tbPrevious;
    delete this->m_tbNext;
    delete this->m_cbPages;
    delete this->m_hLayoutFilter;
    delete this->m_treeWidget;
    delete this->m_vLayoutLog;
    delete this->m_frmLog;

    delete this->m_llStorage;
    delete this->m_progressBar;
    delete this->m_hLayoutStorage;
    delete this->m_frmStorage;

    delete this->m_gLayoutMain;
}
void ZOperateLogUI::ZSloFilter()
{
    ZLogFilterDialog dia(tr("Log Filter Dialog"));
    if(dia.exec()==QDialog::Accepted)
    {

    }
}
void ZOperateLogUI::ZSloExport()
{

}
void ZOperateLogUI::ZSloDelete()
{

}
void ZOperateLogUI::ZFetchLogs()
{
    //check database log record.
    qint32 tRows=gDB.ZGetRecordCount("OpLogTable");
    qint32 tPages=tRows/10;
    this->m_cbPages->clear();
    for(qint32 i=0;i<tPages;i++)
    {
        this->m_cbPages->addItem(QString("%1/%2").arg(i+1).arg(tPages));
    }
    qint32 maxRows=gDB.ZGetMaxRecordCount("OpLogTable");
    this->m_llStorage->setText(QString("%1/%2").arg(tRows).arg(maxRows));
    this->m_progressBar->setRange(0,maxRows);
    this->m_progressBar->setValue(tRows);

    this->m_treeWidget->clear();
    QStringList dateTime;
    QStringList logType;
    QStringList logMsg;
    if(!gDB.ZGetOPLogRecord(0,10,dateTime,logType,logMsg))
    {
        for(qint32 i=0;i<dateTime.size();i++)
        {
            QTreeWidgetItem *item=new QTreeWidgetItem;
            item->setTextAlignment(0,Qt::AlignCenter);
            item->setTextAlignment(1,Qt::AlignCenter);
            item->setTextAlignment(2,Qt::AlignCenter);
            item->setText(0,dateTime.at(i));
            item->setText(1,logType.at(i));
            item->setText(2,logMsg.at(i));
            this->m_treeWidget->addTopLevelItem(item);
        }
    }
}
