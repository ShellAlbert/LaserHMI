#include "zsyssetupui.h"
#include <QDateTime>
#include <QMessageBox>
#include "ui/zdigkbd.h"
#include "ui/zfullkbd.h"
#include "ui/zdatetimesetdia.h"
#include <gbl/hmi_const.h>
#include <ui/zupdatewizarddialog.h>
#include <ui/zwaitingdialog.h>

ZSysSetupUI::ZSysSetupUI()
{

}
ZSysSetupUI::~ZSysSetupUI()
{

}

void ZSysSetupUI::ZDoInitial()
{
    this->m_llcCinemaName=new ZLabelLineEditCheckBox(tr("影院名称   "),g_GblHelp.m_cinemaName,tr("标题栏可见"),true);
    this->m_llcDevNo=new ZLabelLineEditCheckBox(tr("设备编号   "),g_GblHelp.m_devNo,tr("标题栏可见"),true);
    this->m_llcDateTime=new ZLabelLineEditCheckBox(tr("本地时间   "),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),tr("标题栏可见"),true);
    this->m_llcPLaserDateTime=new ZLabelLineEditCheckBox(tr("PLaser时间"),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),tr("自动同步  "),true);

    connect(this->m_llcCinemaName,SIGNAL(ZSigLineEditClicked()),this,SLOT(ZSloShowKeyboard()));
    connect(this->m_llcDevNo,SIGNAL(ZSigLineEditClicked()),this,SLOT(ZSloShowKeyboard()));
    connect(this->m_llcDateTime,SIGNAL(ZSigLineEditClicked()),this,SLOT(ZSloShowKeyboard()));
    connect(this->m_llcPLaserDateTime,SIGNAL(ZSigLineEditClicked()),this,SLOT(ZSloShowKeyboard()));

    this->m_tbChangePassword=new QToolButton;
    this->m_tbChangePassword->setText(tr("修改密码"));
    connect(this->m_tbChangePassword,SIGNAL(clicked(bool)),this,SLOT(ZSloChangePassword()));
    this->m_tbRestoreFactory=new QToolButton;
    this->m_tbRestoreFactory->setText(tr("恢复出厂设置"));
    connect(this->m_tbRestoreFactory,SIGNAL(clicked(bool)),this,SLOT(ZSloRestoreFactory()));

    this->m_tbChangePassword->setObjectName("ZSysSetupUIButton");
    this->m_tbRestoreFactory->setObjectName("ZSysSetupUIButton");

    QVector<PieOne> pieData;
    PieOne one1;
    one1.oneColor=QColor(0x00,0x72,0xe3);
    one1.oneTitle=QString("空闲空间");
    one1.onePercent=3;
    PieOne one2;
    one2.oneColor=QColor(0x4d,0xff,0xff);
    one2.oneTitle=QString("已用空间");
    one2.onePercent=5;
    pieData.append(one1);
    pieData.append(one2);
    this->m_diskUsageChart=new ZPieChart(tr("磁盘使用情况"),pieData);

    ////////////////////////////////////////////
    PieOne opLog;
    opLog.oneColor=QColor(0xf7,0x50,0x50);
    opLog.oneTitle=QString("操作日志");
    opLog.onePercent=2;
    PieOne laserLog;
    laserLog.oneColor=QColor(0x7e,0x3d,0x76);
    laserLog.oneTitle=QString("激光日志");
    laserLog.onePercent=4;
    PieOne chillerLog;
    chillerLog.oneColor=QColor(0xe8,0x00,0xe8);
    chillerLog.oneTitle=QString("水冷机日志");
    chillerLog.onePercent=5;
    QVector<PieOne> logData;
    logData.append(opLog);
    logData.append(laserLog);
    logData.append(chillerLog);
    this->m_logUsageChart=new ZPieChart(tr("日志存储情况"),logData);

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llcCinemaName,0,0,1,1);
    this->m_gLayout->addWidget(this->m_llcDevNo,1,0,1,1);
    this->m_gLayout->addWidget(this->m_llcDateTime,2,0,1,1);
    this->m_gLayout->addWidget(this->m_llcPLaserDateTime,3,0,1,1);

    this->m_hLayoutBtn=new QHBoxLayout;
    this->m_hLayoutBtn->addStretch(1);
    this->m_hLayoutBtn->addWidget(this->m_tbChangePassword);
    this->m_hLayoutBtn->addWidget(this->m_tbRestoreFactory);

    this->m_hLayoutChart=new QHBoxLayout;
    this->m_hLayoutChart->addWidget(this->m_diskUsageChart);
    this->m_hLayoutChart->addWidget(this->m_logUsageChart);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addLayout(this->m_gLayout);
    this->m_vLayout->addLayout(this->m_hLayoutBtn);
    this->m_vLayout->addLayout(this->m_hLayoutChart);
    this->m_vLayout->setStretch(0,7);
    this->m_vLayout->setStretch(1,1);
    this->m_vLayout->setStretch(2,3);
    this->setLayout(this->m_vLayout);

    this->m_timer1s=new QTimer;
    connect(this->m_timer1s,SIGNAL(timeout()),this,SLOT(ZSloTimeout()));
    this->m_bProcessEvent=true;
}
void ZSysSetupUI::ZUnInitial()
{
    this->m_timer1s->stop();
    delete this->m_timer1s;

    delete this->m_llcCinemaName;
    delete this->m_llcDevNo;
    delete this->m_llcDateTime;
    delete this->m_llcPLaserDateTime;
    delete this->m_gLayout;

    delete this->m_tbChangePassword;
    delete this->m_tbRestoreFactory;
    delete this->m_hLayoutBtn;

    delete this->m_diskUsageChart;
    delete this->m_logUsageChart;
    delete this->m_hLayoutChart;
    delete this->m_vLayout;
}
bool ZSysSetupUI::event(QEvent *e)
{
    if(this->m_bProcessEvent)
    {
        switch(e->type())
        {
        case QEvent::Show:
            //    case QEvent::Move:
            //        qDebug()<<"ZMdlCtlUI:   Move Event...........";
            //    case QEvent::Paint:
            //        qDebug()<<"ZMdlCtlUI:   Paint Event...........";
            if(!this->m_timer1s->isActive())
            {
                this->m_timer1s->start(1000*g_GblHelp.m_nQueryInterval);
            }
            break;
        case QEvent::Hide:
            this->m_timer1s->stop();
            break;
        default:
            break;
        }
    }
    return ZBaseUI::event(e);
}
void ZSysSetupUI::ZSloShowKeyboard()
{
    this->m_bProcessEvent=false;
    g_GblHelp.beep();
    ZLabelLineEditCheckBox *src=qobject_cast<ZLabelLineEditCheckBox*>(this->sender());
    if(src==this->m_llcCinemaName)
    {
        ZFullKbd kbd(tr("修改影院名称"));
        kbd.ZSetText(this->m_llcCinemaName->ZGetLineEditText());
        if(kbd.exec()==QDialog::Accepted)
        {
            this->m_llcCinemaName->ZSetLineEditText(kbd.ZGetText());
            g_GblHelp.m_cinemaName=kbd.ZGetText();
        }
    }else if(src==this->m_llcDevNo)
    {
        ZDigKbd kbd(tr("修改设备编号"));
        kbd.ZSetDigital(this->m_llcDevNo->ZGetLineEditText());
        if(kbd.exec()==QDialog::Accepted)
        {
            this->m_llcDevNo->ZSetLineEditText(kbd.ZGetDigital());
            g_GblHelp.m_devNo=kbd.ZGetDigital();
        }
    }else if(src==this->m_llcDateTime)
    {
        QDateTime dt=QDateTime::fromString(this->m_llcDateTime->ZGetLineEditText(),"yyyy-MM-dd hh:mm:ss");
        qint32 year=dt.date().year();
        qint32 month=dt.date().month();
        qint32 day=dt.date().day();
        qint32 hour=dt.time().hour();
        qint32 minute=dt.time().minute();
        qint32 second=dt.time().second();
        ZDateTimeSetDia dia(tr("修改本地时间"),year,month,day,hour,minute,second);
        if(dia.exec()==QDialog::Accepted)
        {
            QDateTime newDT=dia.ZGetDateTime();
            char cmdBuffer[128];
            sprintf(cmdBuffer,"/usr/bin/date -s \"%d-%d-%d %d:%d:%d\"",///<
                    newDT.date().year(),///<
                    newDT.date().month(),///<
                    newDT.date().day(),///<
                    newDT.time().hour(),///<
                    newDT.time().minute(),///<
                    newDT.time().second());
            FILE *fp=popen(cmdBuffer,"r");
            if(fp)
            {
                pclose(fp);
            }

            FILE *fp2=popen("/sbin/hwclock -w","r");
            if(fp2)
            {
                pclose(fp2);
            }
            g_GblHelp.beep();
        }
    }else if(src==this->m_llcPLaserDateTime)
    {
        QDateTime dt=QDateTime::fromString(this->m_llcPLaserDateTime->ZGetLineEditText(),"yyyy-MM-dd hh:mm:ss");
        qint32 year=dt.date().year();
        qint32 month=dt.date().month();
        qint32 day=dt.date().day();
        qint32 hour=dt.time().hour();
        qint32 minute=dt.time().minute();
        qint32 second=dt.time().second();
        ZDateTimeSetDia dia(tr("修改PLaser时间"),year,month,day,hour,minute,second);
        if(dia.exec()==QDialog::Accepted)
        {
            QDateTime newDT=dia.ZGetDateTime();
            QString cmd("setRTC");
            QString arguments=newDT.toString("yyyy-MM-dd hh:mm:ss");
            emit this->ZSigRequestCmd(cmd,arguments);
        }
    }
    this->m_bProcessEvent=true;
}
void ZSysSetupUI::ZSloChangePassword()
{
    this->m_bProcessEvent=false;
    g_GblHelp.beep();
    ZChangePasswordDia dia(tr("修改管理员密码"));
    QObject::connect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    if(dia.exec()==QDialog::Accepted)
    {
        QString newPass=dia.ZGetNewPassword();
        if(!gDB.ZUpdatePassword(newPass))
        {
            emit this->ZSigPopupMsg(tr("密码修改成功!"));
            g_GblHelp.m_password=newPass;
        }else{
            emit this->ZSigPopupMsg(tr("密码修改失败!"));
        }
    }
    QObject::disconnect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    this->m_bProcessEvent=true;
}
void ZSysSetupUI::ZSloRestoreFactory()
{
    this->m_bProcessEvent=false;
    g_GblHelp.beep();
    ZRestorFactoryDia dia(tr("恢复出厂默认设置"));
    QObject::connect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    if(dia.exec()==QDialog::Accepted)
    {

    }
    QObject::disconnect(&dia,SIGNAL(ZSigPopupMsg(QString)),this,SIGNAL(ZSigPopupMsg(QString)));
    this->m_bProcessEvent=true;
}
void ZSysSetupUI::ZSloTimeout()
{
    if(this->m_llcPLaserDateTime->ZIsChecked())
    {
        emit this->ZSigRequestCmd("getRTC","");
    }
}
void ZSysSetupUI::ZSloResponseRTCCmd(QString cmd,QString arguments)
{
    if(cmd=="setRTC" || cmd=="getRTC")
    {
        this->m_llcPLaserDateTime->ZSetLineEditText(arguments);
    }
}
ZChangePasswordDia::ZChangePasswordDia(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumWidth(300);
    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(title);
    this->m_llOldPass=new QLabel(tr("旧密码(*)"));
    this->m_llNewPass1=new QLabel(tr("新密码(*)"));
    this->m_llNewPass2=new QLabel(tr("再次输入(*)"));

    this->m_leOldPass=new QLineEdit;
    this->m_leOldPass->setEchoMode(QLineEdit::Password);
    this->m_leOldPass->installEventFilter(this);
    this->m_leNewPass1=new QLineEdit;
    this->m_leNewPass1->setEchoMode(QLineEdit::Password);
    this->m_leNewPass1->installEventFilter(this);
    this->m_leNewPass2=new QLineEdit;
    this->m_leNewPass2->setEchoMode(QLineEdit::Password);
    this->m_leNewPass2->installEventFilter(this);

    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("确认"));
    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("取消"));

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,2);
    this->m_gLayout->addWidget(this->m_llOldPass,1,0,1,1);
    this->m_gLayout->addWidget(this->m_leOldPass,1,1,1,1);
    this->m_gLayout->addWidget(this->m_llNewPass1,2,0,1,1);
    this->m_gLayout->addWidget(this->m_leNewPass1,2,1,1,1);
    this->m_gLayout->addWidget(this->m_llNewPass2,3,0,1,1);
    this->m_gLayout->addWidget(this->m_leNewPass2,3,1,1,1);

    this->m_gLayout->addWidget(this->m_tbOkay,4,0,1,1);
    this->m_gLayout->addWidget(this->m_tbCancel,4,1,1,1);


    this->m_llTitle->setObjectName("ZChangePasswordDiaLabel");
    this->m_llOldPass->setObjectName("ZChangePasswordDiaLabel");
    this->m_llNewPass1->setObjectName("ZChangePasswordDiaLabel");
    this->m_llNewPass2->setObjectName("ZChangePasswordDiaLabel");
    this->m_tbOkay->setObjectName("ZChangePasswordDiaOkayCancel");
    this->m_tbCancel->setObjectName("ZChangePasswordDiaOkayCancel");
    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZChangePasswordDia");
    this->m_frm->setLayout(this->m_gLayout);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_frm);
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(ZSloOkay()));
    connect(this->m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(ZSloCancel()));
}
ZChangePasswordDia::~ZChangePasswordDia()
{
    delete this->m_llTitle;
    delete this->m_llOldPass;
    delete this->m_llNewPass1;
    delete this->m_llNewPass2;
    delete this->m_leOldPass;
    delete this->m_leNewPass1;
    delete this->m_leNewPass2;
    delete this->m_tbOkay;
    delete this->m_tbCancel;
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
QString ZChangePasswordDia::ZGetNewPassword()
{
    return this->m_leNewPass1->text().trimmed();
}
bool ZChangePasswordDia::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);
        if(mouseEvent->buttons()&Qt::LeftButton)
        {
            if(watched==this->m_leOldPass)
            {
                ZDigKbd kbd(tr("输入旧密码"),true);
                if(kbd.exec()==QDialog::Accepted)
                {
                    this->m_leOldPass->setText(kbd.ZGetDigital());
                }
            }else if(watched==this->m_leNewPass1)
            {
                ZDigKbd kbd(tr("输入新密码"),true);
                if(kbd.exec()==QDialog::Accepted)
                {
                    this->m_leNewPass1->setText(kbd.ZGetDigital());
                }
            }else if(watched==this->m_leNewPass2)
            {
                ZDigKbd kbd(tr("输入新密码"),true);
                if(kbd.exec()==QDialog::Accepted)
                {
                    this->m_leNewPass2->setText(kbd.ZGetDigital());
                }
            }
        }
    }
    return QDialog::eventFilter(watched,event);
}
void ZChangePasswordDia::ZSloOkay()
{
    if(this->m_leOldPass->text().isEmpty())
    {
        emit this->ZSigPopupMsg(tr("请输入旧密码!"));
        return;
    }
    if(this->m_leNewPass1->text().isEmpty())
    {
        emit this->ZSigPopupMsg(tr("请输入新密码!"));
        return;
    }
    if(this->m_leNewPass2->text().isEmpty())
    {
        emit this->ZSigPopupMsg(tr("请再次输入新密码!"));
        return;
    }
    if(this->m_leOldPass->text()!=g_GblHelp.m_password)
    {
        emit this->ZSigPopupMsg(tr("输入的旧密码不正确!"));
        return;
    }
    if(this->m_leNewPass1->text().trimmed().length()<8)
    {
        emit this->ZSigPopupMsg(tr("密码长度最低为8位!"));
        return;
    }
    if(this->m_leNewPass1->text()!=this->m_leNewPass2->text())
    {
        emit this->ZSigPopupMsg(tr("两次输入的新密码不相同!"));
        return;
    }
    this->accept();
}
void ZChangePasswordDia::ZSloCancel()
{
    this->reject();
}
ZCountThread::ZCountThread(qint32 sec)
{
    this->m_sec=sec;
}
void ZCountThread::run()
{
    this->sleep(this->m_sec);
    emit this->ZSigThreadDone(0,"");
}
ZRestorFactoryDia::ZRestorFactoryDia(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->m_llTitle=new QLabel;
    this->m_llTitle->setText(title);
    this->m_llWarning=new QLabel;
    this->m_llWarning->setText(tr("恢复出厂默认设置，将清除所有用户设置数据，不可恢复！\n"
                                  "请输入管理员密码执行该操作！"));
    this->m_lecPassword=new ZLabelLineEditCheckBox(tr("管理员密码"),"",tr("可见"),false);
    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("确认"));
    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("取消"));

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,2);
    this->m_gLayout->addWidget(this->m_llWarning,1,0,1,2);
    this->m_gLayout->addWidget(this->m_lecPassword,2,0,1,2);
    this->m_gLayout->addWidget(this->m_tbOkay,3,0,1,1);
    this->m_gLayout->addWidget(this->m_tbCancel,3,1,1,1);

    this->m_llTitle->setObjectName("ZRestorFactoryDiaLabel");
    this->m_llWarning->setObjectName("ZRestorFactoryDiaLabel");
    this->m_tbOkay->setObjectName("ZRestorFactoryDiaToolButton");
    this->m_tbCancel->setObjectName("ZRestorFactoryDiaToolButton");

    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZRestorFactoryDia");
    this->m_frm->setLayout(this->m_gLayout);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_frm);
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(ZSloOkay()));
    connect(this->m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(ZSloCancel()));
}
ZRestorFactoryDia::~ZRestorFactoryDia()
{
    delete this->m_llTitle;
    delete this->m_llWarning;
    delete this->m_lecPassword;
    delete this->m_tbOkay;
    delete this->m_gLayout;
    delete this->m_frm;
    delete this->m_vLayout;
}
void ZRestorFactoryDia::ZSloOkay()
{
    if(this->m_lecPassword->ZGetLineEditText()!=g_GblHelp.m_password)
    {
        emit this->ZSigPopupMsg(tr("输入的管理员密码不正确!"));
        return;
    }
    emit this->ZSigPopupMsg(tr("恢复出厂默认设置操作成功!"));
    ZWaitingDialog waitDia(tr("5秒后系统将重新启动!"));
    ZCountThread thread(5);
    connect(&thread,SIGNAL(ZSigThreadDone(qint32,QString)),&waitDia,SLOT(ZSloClose(qint32,QString)));
    thread.start();
    waitDia.exec();
    thread.wait();
    disconnect(&thread,SIGNAL(ZSigThreadDone(qint32,QString)),&waitDia,SLOT(ZSloClose(qint32,QString)));
    //remove database.
    QFile::remove(QDir::currentPath()+"/LaserHMI.db");

    //execute reboot command.
    FILE *fp=popen("/sbin/reboot","r");
    pclose(fp);
    this->accept();
}
void ZRestorFactoryDia::ZSloCancel()
{
    emit this->ZSigPopupMsg(tr("恢复出厂默认设置操作取消!"));
    this->reject();
}
