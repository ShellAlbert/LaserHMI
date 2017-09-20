#include "zsummaryui.h"
#include <QDebug>
#include <gbl/hmi_const.h>
ZGLaserCurve::ZGLaserCurve()
{
    this->m_runHours=new ZLaserRunHoursUI(tr("累计时长"));
    this->m_curveIVP=new ZCurveUI(tr("IVP动态曲线"));
    for(qint32 i=0;i<6;i++)
    {
        this->m_llIPVColor[i]=new QLabel;
        this->m_llIPVColor[i]->setMinimumWidth(30);
        this->m_llIPVColor[i]->setMaximumHeight(10);

        this->m_llIPV[i]=new QLabel;
        QColor color;
        switch(i)
        {
        case 0:
            color=QColor(220,20,60);
            this->m_llIPV[i]->setText(tr("R-I"));
            break;
        case 1:
            color=QColor(255,20,147);
            this->m_llIPV[i]->setText(tr("R-V"));
            break;
        case 2:
            color=QColor(0,0,255);
            this->m_llIPV[i]->setText(tr("G-I"));
            break;
        case 3:
            color=QColor(0,255,255);
            this->m_llIPV[i]->setText(tr("G-V"));
            break;
        case 4:
            color=QColor(249,249,0);
            this->m_llIPV[i]->setText(tr("B-I"));
            break;
        case 5:
            color=QColor(255,165,0);
            this->m_llIPV[i]->setText(tr("B-V"));
            break;
        }
        QString colorStr=QString("QLabel{background:rgb(%1,%2,%3);}").arg(color.red()).arg(color.green()).arg(color.blue());
        this->m_llIPVColor[i]->setStyleSheet(colorStr);
    }

    this->m_curveTemp=new ZCurveUI(tr("温度动态曲线"));
    for(qint32 i=0;i<6;i++)
    {
        this->m_llTempColor[i]=new QLabel;
        this->m_llTempColor[i]->setMinimumWidth(30);
        this->m_llTempColor[i]->setMaximumHeight(10);

        this->m_llTemp[i]=new QLabel;
        QColor color;
        switch(i)
        {
        case 0:
            color=QColor(220,20,60);
            this->m_llTemp[i]->setText(tr("R1"));
            break;
        case 1:
            color=QColor(255,20,147);
            this->m_llTemp[i]->setText(tr("R2"));
            break;
        case 2:
            color=QColor(0,0,255);
            this->m_llTemp[i]->setText(tr("B1"));
            break;
        case 3:
            color=QColor(0,255,255);
            this->m_llTemp[i]->setText(tr("G1"));
            break;
        case 4:
            color=QColor(249,249,0);
            this->m_llTemp[i]->setText(tr("G2"));
            break;
        case 5:
            color=QColor(255,165,0);
            this->m_llTemp[i]->setText(tr("AT"));
            break;
        }
        QString colorStr=QString("QLabel{background:rgb(%1,%2,%3);}").arg(color.red()).arg(color.green()).arg(color.blue());
        this->m_llTempColor[i]->setStyleSheet(colorStr);
    }

    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_runHours,0,0,12,1);
    this->m_gLayout->addWidget(this->m_curveIVP,0,1,6,1);
    this->m_gLayout->addWidget(this->m_llIPVColor[0],0,2,1,1);
    this->m_gLayout->addWidget(this->m_llIPVColor[1],1,2,1,1);
    this->m_gLayout->addWidget(this->m_llIPVColor[2],2,2,1,1);
    this->m_gLayout->addWidget(this->m_llIPVColor[3],3,2,1,1);
    this->m_gLayout->addWidget(this->m_llIPVColor[4],4,2,1,1);
    this->m_gLayout->addWidget(this->m_llIPVColor[5],5,2,1,1);
    this->m_gLayout->addWidget(this->m_llIPV[0],0,3,1,1);
    this->m_gLayout->addWidget(this->m_llIPV[1],1,3,1,1);
    this->m_gLayout->addWidget(this->m_llIPV[2],2,3,1,1);
    this->m_gLayout->addWidget(this->m_llIPV[3],3,3,1,1);
    this->m_gLayout->addWidget(this->m_llIPV[4],4,3,1,1);
    this->m_gLayout->addWidget(this->m_llIPV[5],5,3,1,1);

    this->m_gLayout->addWidget(this->m_curveTemp,6,1,6,1);
    this->m_gLayout->addWidget(this->m_llTempColor[0],6,2,1,1);
    this->m_gLayout->addWidget(this->m_llTempColor[1],7,2,1,1);
    this->m_gLayout->addWidget(this->m_llTempColor[2],8,2,1,1);
    this->m_gLayout->addWidget(this->m_llTempColor[3],9,2,1,1);
    this->m_gLayout->addWidget(this->m_llTempColor[4],10,2,1,1);
    this->m_gLayout->addWidget(this->m_llTempColor[5],11,2,1,1);
    this->m_gLayout->addWidget(this->m_llTemp[0],6,3,1,1);
    this->m_gLayout->addWidget(this->m_llTemp[1],7,3,1,1);
    this->m_gLayout->addWidget(this->m_llTemp[2],8,3,1,1);
    this->m_gLayout->addWidget(this->m_llTemp[3],9,3,1,1);
    this->m_gLayout->addWidget(this->m_llTemp[4],10,3,1,1);
    this->m_gLayout->addWidget(this->m_llTemp[5],11,3,1,1);

    this->m_gLayout->setContentsMargins(0,2,0,2);
    this->m_gLayout->setColumnStretch(0,2);
    this->m_gLayout->setColumnStretch(1,8);
    this->m_gLayout->setColumnStretch(2,1);
    this->m_gLayout->setColumnStretch(3,1);
    this->setLayout(this->m_gLayout);
}
ZGLaserCurve::~ZGLaserCurve()
{
    delete this->m_runHours;
    delete this->m_curveIVP;
    for(qint32 i=0;i<6;i++)
    {
        delete this->m_llIPVColor[i];
        delete this->m_llIPV[i];
    }
    delete this->m_curveTemp;
    for(qint32 i=0;i<6;i++)
    {
        delete this->m_llTempColor[i];
        delete this->m_llTemp[i];
    }
    delete this->m_gLayout;
}

ZSummaryUI::ZSummaryUI()
{
    this->m_index=0;
}
ZSummaryUI::~ZSummaryUI()
{

}
void ZSummaryUI::ZDoInitial()
{
    this->m_stackedWidget=new QStackedWidget;
    for(qint32 i=0;i<8;i++)
    {
        this->m_glaserCurve[i]=new ZGLaserCurve;
        this->m_stackedWidget->addWidget(this->m_glaserCurve[i]);
    }

    this->m_llGLaserNo=new QLabel;
    this->m_llGLaserNo->setObjectName("ZSummaryUIGLaserNoLabel");
    this->m_llGLaserNo->setText(tr("GLaser 1"));
    this->m_llGLaserNo->setAlignment(Qt::AlignCenter);

    this->m_tbLeft=new QToolButton;
    this->m_tbLeft->setObjectName("ZSummaryUIButton");
    this->m_tbLeft->setIconSize(QSize(96,32));
    this->m_tbLeft->setIcon(QIcon(":/Summary/images/summary/Left.png"));
    this->m_tbLeft->setToolButtonStyle(Qt::ToolButtonIconOnly);

    this->m_tbRight=new QToolButton;
    this->m_tbRight->setObjectName("ZSummaryUIButton");
    this->m_tbRight->setIconSize(QSize(96,32));
    this->m_tbRight->setIcon(QIcon(":/Summary/images/summary/Right.png"));
    this->m_tbRight->setToolButtonStyle(Qt::ToolButtonIconOnly);

    this->m_hLayoutBtn=new QHBoxLayout;
    this->m_hLayoutBtn->addWidget(this->m_tbLeft);
    this->m_hLayoutBtn->addStretch(1);
    this->m_hLayoutBtn->addWidget(this->m_llGLaserNo);
    this->m_hLayoutBtn->addStretch(1);
    this->m_hLayoutBtn->addWidget(this->m_tbRight);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_stackedWidget);
    this->m_vLayout->addLayout(this->m_hLayoutBtn);
    this->setLayout(this->m_vLayout);

    connect(this->m_tbLeft,SIGNAL(clicked(bool)),this,SLOT(ZSloShowAnotherOne()));
    connect(this->m_tbRight,SIGNAL(clicked(bool)),this,SLOT(ZSloShowAnotherOne()));
}
void ZSummaryUI::ZUnInitial()
{
    for(qint32 i=0;i<8;i++)
    {
        this->m_glaserCurve[i];
    }
    delete this->m_stackedWidget;
    delete this->m_llGLaserNo;
    delete this->m_tbLeft;
    delete this->m_tbRight;
    delete this->m_hLayoutBtn;
    delete this->m_vLayout;
}
void ZSummaryUI::ZSloShowAnotherOne()
{
    QToolButton *srcTb=qobject_cast<QToolButton*>(this->sender());
    if(srcTb==this->m_tbLeft)
    {
        if(this->m_index>0)
        {
            this->m_index--;
        }else{
            return;
        }
    }else if(srcTb==this->m_tbRight)
    {
        if(this->m_index<7)
        {
            this->m_index++;
        }else{
            return;
        }
    }
    this->m_stackedWidget->setCurrentWidget(this->m_glaserCurve[this->m_index]);
    this->m_llGLaserNo->setText(tr(" GLaser %1 ").arg(this->m_index+1));
    g_GblHelp.beep();
}
