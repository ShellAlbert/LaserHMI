#include "zaboutui.h"
#include "gbl/hmi_const.h"
#include <ui/zupdatewizarddialog.h>
#include <stdio.h>
#include <QDateTime>
#include <QFile>
#include <fstream>
#include "qr.h"
using namespace std;
ZAboutUI::ZAboutUI()
{
    this->setObjectName("aboutUI");
}
ZAboutUI::~ZAboutUI()
{

}
void ZAboutUI::ZDoInitial()
{
    this->m_llManufacturer=new QLabel;
    this->m_llManufacturer->setObjectName("ZAboutUILabel");
    this->m_llManufacturer->setText(QString("设备制造商:%1").arg(DEVICE_MANUFACTURER));

    this->m_llTechSupport=new QLabel;
    this->m_llTechSupport->setObjectName("ZAboutUILabel");
    this->m_llTechSupport->setText(QString("技术支持: %1").arg(TECH_SUPPORT));

    this->m_llUIVerNo=new QLabel;
    this->m_llUIVerNo->setObjectName("ZAboutUILabel");
    this->m_llUIVerNo->setText(QString("UI版本号:%1").arg(UI_VERSION));

    this->m_llPublishDate=new QLabel;
    this->m_llPublishDate->setObjectName("ZAboutUILabel");
    this->m_llPublishDate->setText(QString("发布日期:%1 %2").arg(QString(__DATE__)).arg(QString(__TIME__)));

    this->m_tbUIUpgrade=new QToolButton;
    this->m_tbUIUpgrade->setText(tr("升级..."));
    connect(this->m_tbUIUpgrade,SIGNAL(clicked(bool)),this,SLOT(ZSloUpdateHMI()));

    this->m_llFirmwareNo=new QLabel;
    this->m_llFirmwareNo->setObjectName("ZAboutUILabel");
    this->m_llFirmwareNo->setText(QString("光源版本号:%1").arg("Unknown"));
    this->m_llProductNo=new QLabel;
    this->m_llProductNo->setObjectName("ZAboutUILabel");
    this->m_llProductNo->setText(QString("光源产品编号:%1").arg("Unknown"));
    this->m_tbFirmwareUpgrade=new QToolButton;
    this->m_tbFirmwareUpgrade->setText(tr("升级..."));

    connect(this->m_tbFirmwareUpgrade,SIGNAL(clicked(bool)),this,SLOT(ZSloUpdatePLaser()));



    this->m_gLayoutInfo=new QGridLayout;
    this->m_gLayoutInfo->addWidget(this->m_llManufacturer,0,0,1,3);
    this->m_gLayoutInfo->addWidget(this->m_llTechSupport,1,0,1,3);

    this->m_gLayoutInfo->addWidget(this->m_llUIVerNo,2,0,1,1);
    this->m_gLayoutInfo->addWidget(this->m_llPublishDate,2,1,1,1);
    this->m_gLayoutInfo->addWidget(this->m_tbUIUpgrade,2,2,1,1);

    this->m_gLayoutInfo->addWidget(this->m_llFirmwareNo,3,0,1,1);
    this->m_gLayoutInfo->addWidget(this->m_llProductNo,3,1,1,1);
    this->m_gLayoutInfo->addWidget(this->m_tbFirmwareUpgrade,3,2,1,1);

    this->m_gLayoutInfo->setContentsMargins(20,50,20,50);
    this->m_frmInfo=new ZFrame(tr("系统信息"));
    this->m_frmInfo->setLayout(this->m_gLayoutInfo);

    this->m_llLogo=new QLabel;
    this->m_llLogo->setPixmap(QPixmap(":/about/images/about/logo.png").scaled(400,200));
    this->m_llLogo->setScaledContents(true);

    this->m_frmVLayout=new QVBoxLayout;
    //    this->m_frmVLayout->addStretch(1);
    this->m_frmVLayout->addWidget(this->m_frmInfo,0,Qt::AlignCenter);
    this->m_frmVLayout->addWidget(this->m_llLogo,0,Qt::AlignCenter);
    this->setLayout(this->m_frmVLayout);

    this->m_timer=new QTimer;
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(ZSloTimeout()));
    this->m_timer->start(1000);
}
void ZAboutUI::ZUnInitial()
{
    this->m_timer->stop();
    delete this->m_timer;

    delete this->m_llManufacturer;
    delete this->m_llTechSupport;
    delete this->m_llUIVerNo;
    delete this->m_llPublishDate;
    delete this->m_tbUIUpgrade;
    delete this->m_llFirmwareNo;
    delete this->m_llProductNo;
    delete this->m_tbFirmwareUpgrade;

    delete this->m_gLayoutInfo;
    delete this->m_frmInfo;

    delete this->m_llLogo;
    delete this->m_frmVLayout;
}
void ZAboutUI::ZSloUpdateHMI()
{
    g_GblHelp.beep();
    ZUpdateWizardDialog dia;
    dia.exec();
}
void ZAboutUI::ZSloTimeout()
{
    this->m_llFirmwareNo->setText(QString("光源版本号:%1").arg(g_GblHelp.m_glaserVerNo));
    this->m_llProductNo->setText(QString("光源产品编号:%1").arg(g_GblHelp.m_glaserProductNo));
}
void ZAboutUI::ZSloUpdatePLaser()
{
    ZQRDialog dia;
    dia.exec();
}
ZQRDialog::ZQRDialog()
{
    //generate qr code.
    QString obj(tr("Embedded Electronic Product Development 13522296239"));
    QByteArray baDevID=obj.toLatin1();
    //generate QR code.
    int errcode = QR_ERR_NONE;
    //qrInit的5个参数分别是version,mode,纠错等级和掩码，使用过程中使用除了QR_EM_8BIT以外的mode会生成不正常的二维码，暂时不知道原因。
    QRCode* p = qrInit(10, QR_EM_8BIT, 2, -1, &errcode);
    if (p == NULL)
    {
        printf("error\n");
        return;
    }

    qrAddData(p, (const qr_byte_t* )baDevID.data(), baDevID.length());
    //注意需要调用qrFinalize函数
    if (!qrFinalize(p))
    {
        printf("finalize error\n");
        return;
    }
    int size = 0;
    //两个5分别表示：像素之间的距离和二维码图片的放大倍数，范围都是1-16
    qr_byte_t * buffer = qrSymbolToPNG(p, 1, 1, &size);
    if (buffer == NULL)
    {
        printf("error %s", qrGetErrorInfo(p));
        return;
    }
    ofstream f("/tmp/qrcode.png");
    f.write((const char *)buffer, size);
    f.close();

    this->m_llQRCode=new QLabel;
    QPixmap qrcode;
    qrcode.load("/tmp/qrcode.png","PNG");
    this->m_llQRCode->setPixmap(qrcode.scaled(200,200));

    this->m_tbClose=new QToolButton;
    this->m_tbClose->setText(tr("CLOSE ME"));
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_llQRCode,0,Qt::AlignCenter);
    this->m_vLayout->addWidget(this->m_tbClose,0,Qt::AlignCenter);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbClose,SIGNAL(clicked(bool)),this,SLOT(accept()));
}
ZQRDialog::~ZQRDialog()
{
    delete this->m_llQRCode;
    delete this->m_tbClose;
    delete this->m_vLayout;
}
