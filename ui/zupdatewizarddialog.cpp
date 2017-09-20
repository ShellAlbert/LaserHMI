#include "zupdatewizarddialog.h"
#include <QProcess>
#include <QDebug>
#include <stdio.h>
#include <QFile>
#include <QByteArray>
#include <QCryptographicHash>
#include <gbl/hmi_const.h>
ZUpdateThread::ZUpdateThread()
{

}
ZUpdateThread::~ZUpdateThread()
{

}
void ZUpdateThread::ZSetUDiskNodeName(QString udiskNodeName)
{
    this->m_udiskNode=udiskNodeName;
}
//direcoty:
//LaserHMI/LaserHMI.md5
//LaserHMI/LaserHMI.tar.bz2
void ZUpdateThread::run()
{

    char buffer[1024];
    QByteArray nodeName=this->m_udiskNode.toLatin1();
    memset(buffer,0,sizeof(buffer));
    sprintf(buffer,"mkdir /tmp/udisk ; mount -t vfat %s /tmp/udisk",nodeName.data());
    //mkdir /tmp/udisk && mount -t vfat /dev/sdb4 /tmp/udisk
    emit this->ZSigUpdateProgress(10);
    emit this->ZSigUpdateFinished(1,tr("正在挂载U盘"));
    FILE *fp1=popen(buffer,"r");
    pclose(fp1);
    //check two files is exist or not.
    if(false==QFile::exists("/tmp/udisk/LaserHMI/LaserHMI.md5"))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("未检测到md5文件,无法继续!"));
        return;
    }
    if(false==QFile::exists("/tmp/udisk/LaserHMI/LaserHMI.tar.bz2"))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("未检测到主压缩包文件,无法继续!"));
        return;
    }
    //check md5.
    emit this->ZSigUpdateProgress(20);
    QString md5FileData;
    QFile md5File("/tmp/udisk/LaserHMI/LaserHMI.md5");
    if(!md5File.open(QIODevice::ReadOnly))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("无法读取md5文件!"));
        return;
    }
    md5FileData=md5File.readAll();
    md5File.close();
    QStringList md5List=md5FileData.split("  ");
    if(md5List.size()!=2)
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("md5文件格式不对,无法读取!"));
        return;
    }
    QString md5Val=md5List.at(0);
    //calculate new file md5.
    QString newMd5Val=this->ZGetFileMd5("/tmp/udisk/LaserHMI/LaserHMI.tar.bz2");
    qDebug()<<"old:"<<md5Val;
    qDebug()<<"new:"<<newMd5Val;
    if(newMd5Val!=md5Val)
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("md5校验失败,升级中止!"));
        return;
    }

    //copy file.
    emit this->ZSigUpdateProgress(30);
    emit this->ZSigUpdateFinished(1,tr("正在复制文件..."));
    FILE *fp2=popen("mkdir /LaserHMINew ; cp /tmp/udisk/LaserHMI/LaserHMI.tar.bz2 /LaserHMINew","r");
    pclose(fp2);
    if(false==QFile::exists("/LaserHMINew/LaserHMI.tar.bz2"))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("无法复制文件,升级中止!"));
        return;
    }

    emit this->ZSigUpdateProgress(50);
    emit this->ZSigUpdateFinished(1,tr("正在执行解压缩..."));
    FILE *fp3=popen("/bin/tar jxf /LaserHMINew/LaserHMI.tar.bz2 -C /LaserHMINew","r");
    pclose(fp3);

    if(false==QFile::exists("/LaserHMINew/start.sh"))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("解压缩失败start.sh,升级中止!"));
        return;
    }
    if(false==QFile::exists("/LaserHMINew/LaserHMI"))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("解压缩失败LaserHMI,升级中止!"));
        return;
    }

    emit this->ZSigUpdateProgress(80);
    emit this->ZSigUpdateFinished(1,tr("正在执行安装测试..."));
    FILE *fp4=popen("/LaserHMINew/test.sh 1987","r");
    pclose(fp4);
    QFile runTestFile("/tmp/.hmi.zsy");
    if(!runTestFile.open(QIODevice::ReadOnly))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("执行测试失败,无法读取测试文件,升级中止!"));
        return;
    }
    QString fileData=runTestFile.readAll();
    runTestFile.close();
    if(!fileData.startsWith("1987"))
    {
        FILE *fp=popen("umount /tmp/udisk","r");
        pclose(fp);
        emit this->ZSigUpdateFinished(-1,tr("执行测试失败,数据读写错误,升级中止!"));
        return;
    }

    emit this->ZSigUpdateProgress(90);
    emit this->ZSigUpdateFinished(1,tr("正在执行安装替换..."));
    FILE *fp5=popen("mv /HMI /HMI.old && mv /LaserHMINew /HMI","r");
    pclose(fp5);

    emit this->ZSigUpdateFinished(1,tr("正在删除旧版本文件..."));
    FILE *fp6=popen("rm -rf /HMI.old","r");
    pclose(fp6);

    emit this->ZSigUpdateProgress(95);
    emit this->ZSigUpdateFinished(1,tr("正在将修改写入磁盘..."));
    FILE *fp7=popen("sync","r");
    pclose(fp7);
    //umount before exit.
    FILE *fp8=popen("umount /tmp/udisk","r");
    pclose(fp8);
    emit this->ZSigUpdateProgress(100);
    emit this->ZSigUpdateFinished(0,tr("升级完成!"));
    this->sleep(1);
    for(qint32 i=5;i>0;i--)
    {
        emit this->ZSigUpdateFinished(1,tr("%1秒后将重新启动...").arg(QString::number(i,10)));
    }
    FILE *fp9=popen("/sbin/reboot","r");
    pclose(fp9);
    return;
}
QString ZUpdateThread::ZGetFileMd5(QString fileName)
{
    QFile localFile(fileName);
    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return 0;
    }
    QCryptographicHash ch(QCryptographicHash::Md5);
    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    localFile.close();
    QByteArray md5 = ch.result();
    return md5.toHex();
}
ZUpdateWizardDialog::ZUpdateWizardDialog()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(300,200);

    this->m_llUpdateNote=new QLabel;
    this->m_llUpdateNote->setObjectName("ZUpdateWizardDialogLabelTitle");
    this->m_llUpdateNote->setText("\nOristar Laser HMI  本地升级");
    this->m_llUpdateNote->setAlignment(Qt::AlignCenter);

    this->m_llUpdateStep=new QLabel;
    this->m_llUpdateStep->setObjectName("ZUpdateWizardDialogLabel");
    this->m_llUpdateStep->setText("插入U盘->复制文件->校验&升级->部署测试->设备重启");
    this->m_llUpdateStep->setAlignment(Qt::AlignCenter);


    this->m_llUpdateWarning=new QLabel;
    this->m_llUpdateWarning->setObjectName("ZUpdateWizardDialogLabelWarning");
    this->m_llUpdateWarning->setText("警告: 升级过程中请不要拔下U盘!!! \n");
    this->m_llUpdateWarning->setAlignment(Qt::AlignCenter);

    this->m_llCurrent=new QLabel;
    this->m_llCurrent->setObjectName("ZUpdateWizardDialogLabelProgress");
    this->m_llCurrent->setText(tr("准备开始升级工作"));
    this->m_llCurrent->setAlignment(Qt::AlignCenter);

    this->m_progressBar=new QProgressBar;
    this->m_progressBar->setRange(0,100);
    this->m_progressBar->setValue(0);
    this->m_progressBar->setMinimumSize(200,40);
    this->m_tbStart=new QToolButton;
    this->m_tbStart->setText(tr("开始"));
    this->m_tbStop=new QToolButton;
    this->m_tbStop->setText(tr("取消"));
    this->m_tbStart->setObjectName("ZUpdateWizardDialogOkayCancel");
    this->m_tbStop->setObjectName("ZUpdateWizardDialogOkayCancel");

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llUpdateNote,0,0,1,2);
    this->m_gLayout->addWidget(this->m_llUpdateStep,1,0,1,2);
    this->m_gLayout->addWidget(this->m_llUpdateWarning,2,0,1,2);

    this->m_gLayout->addWidget(this->m_llCurrent,3,0,1,2);
    this->m_gLayout->addWidget(this->m_progressBar,4,0,1,2);
    this->m_gLayout->addWidget(this->m_tbStart,5,0,1,1,Qt::AlignCenter);
    this->m_gLayout->addWidget(this->m_tbStop,5,1,1,1,Qt::AlignCenter);
    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZUpdateWizardDialog");
    this->m_frm->setLayout(this->m_gLayout);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbStart,SIGNAL(clicked(bool)),this,SLOT(ZSlotStart()));
    connect(this->m_tbStop,SIGNAL(clicked(bool)),this,SLOT(ZSlotStop()));

    connect(this,SIGNAL(ZSigUpdateFinished(qint32,QString)),this,SLOT(ZSlotUpdateFinished(qint32,QString)));

    this->m_thread=new ZUpdateThread;
    connect(this->m_thread,SIGNAL(ZSigUpdateFinished(qint32,QString)),this,SLOT(ZSlotUpdateFinished(qint32,QString)),Qt::DirectConnection);
    connect(this->m_thread,SIGNAL(ZSigUpdateProgress(qint32)),this,SLOT(ZSlotUpdateProgress(qint32)),Qt::DirectConnection);
}
ZUpdateWizardDialog::~ZUpdateWizardDialog()
{
    this->m_thread->wait();
    delete this->m_thread;
    delete this->m_llUpdateNote;
    delete this->m_llUpdateStep;
    delete this->m_llCurrent;
    delete this->m_progressBar;
    delete this->m_tbStart;
    delete this->m_tbStop;
    delete this->m_gLayout;
    delete this->m_vLayout;
}
void ZUpdateWizardDialog::ZSlotUpdateFinished(qint32 ret,QString msg)
{
    this->m_llCurrent->setText(msg);
    //ret:0,success end,-1:error,>0:progress indicator.
    if(ret>0)
    {
        return;
    }else if(ret<0)
    {
        this->m_progressBar->setValue(0);
    }
    this->m_tbStart->setEnabled(true);
    this->m_tbStop->setEnabled(true);
}
void ZUpdateWizardDialog::ZSlotUpdateProgress(qint32 progress)
{
    this->m_progressBar->setValue(progress);
}
void ZUpdateWizardDialog::ZSlotStart()
{
    g_GblHelp.beep();
    this->m_tbStart->setEnabled(false);
    this->m_tbStop->setEnabled(false);
    //list u disk.
    FILE *fp=popen("/usr/bin/ls /dev/sd?? >/tmp/udisk.zsy","r");
    pclose(fp);

    QFile udiskFile("/tmp/udisk.zsy");
    if(!udiskFile.open(QIODevice::ReadOnly))
    {
        emit this->ZSigUpdateFinished(-1,tr("未发现U盘设备!\n请确认您已插入!"));
        return;
    }
    QString udiskFileData=udiskFile.readAll();
    udiskFile.close();
    if(udiskFileData.isEmpty())
    {
        emit this->ZSigUpdateFinished(-1,tr("未发现U盘设备!\n请确认您已插入!"));
        return;
    }
    QStringList udiskList=udiskFileData.split("\n");
    QStringList udiskNodeList;
    for(qint32 i=0;i<udiskList.size();i++)
    {
        QString udiskNode=udiskList.at(i);
        if(udiskNode.startsWith("/dev/sda"))
        {
            continue;
        }
        udiskNodeList.append(udiskNode);
    }
    if(udiskNodeList.size()<=0)
    {
        emit this->ZSigUpdateFinished(-1,tr("未发现U盘设备!\n请确认您已插入!"));
        return;
    }

    ZUDiskSelectDia uselectDia(tr("请选择U盘"),udiskNodeList);
    uselectDia.exec();
    QString udiskNode=uselectDia.ZGetUDiskName();


    this->m_thread->ZSetUDiskNodeName(udiskNode);
    this->m_thread->start();
}
void ZUpdateWizardDialog::ZSlotStop()
{
    this->reject();
}
ZUDiskSelectDia::ZUDiskSelectDia(QString title,QStringList udiskNodeList)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setObjectName("ZUDiskSelectDiaLabel");
    this->m_llTitle->setText(title);
    this->m_listWidet=new QListWidget;
    this->m_listWidet->setObjectName("ZUDiskSelectDiaListWidget");
    this->m_vLayoutFrm=new QVBoxLayout;
    this->m_vLayoutFrm->addWidget(this->m_llTitle);
    this->m_vLayoutFrm->addWidget(this->m_listWidet);
    this->m_fram=new QFrame;
    this->m_fram->setObjectName("ZUDiskSelectDia");
    this->m_fram->setLayout(this->m_vLayoutFrm);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_fram);
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->m_vLayout);

    connect(this->m_listWidet,SIGNAL(clicked(QModelIndex)),this,SLOT(ZSlotSelectUDisk()));
    for(qint32 i=0;i<udiskNodeList.size();i++)
    {
        this->m_listWidet->addItem(udiskNodeList.at(i));
    }
}
ZUDiskSelectDia::~ZUDiskSelectDia()
{
    delete this->m_llTitle;
    delete this->m_listWidet;
    delete this->m_vLayoutFrm;
    delete this->m_fram;
    delete this->m_vLayout;
}
QString ZUDiskSelectDia::ZGetUDiskName()
{
    return this->m_listWidet->currentItem()->text();
}
void ZUDiskSelectDia::ZSlotSelectUDisk()
{
    this->accept();
}
