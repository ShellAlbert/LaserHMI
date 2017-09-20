#include "zcurveui.h"
#include <QPainter>
#include <QDebug>
#include <QDateTime>
ZCurveUI::ZCurveUI(QString title)
{
    this->setObjectName("ZCurveUI");
    this->m_maxSize=30;
    this->m_maxValue=100;


    this->m_chart=new QChart;
    //this->m_chart->addSeries(this->m_scatterSeries);

    for(qint32 i=0;i<6;i++)
    {
        QColor color;
        this->m_splineSeries[i]=new QSplineSeries;
        switch(i)
        {
        case 0:
            color=QColor(220,20,60);
            break;
        case 1:
            color=QColor(255,20,147);
            break;
        case 2:
            color=QColor(0,0,255);
            break;
        case 3:
            color=QColor(0,255,255);
            break;
        case 4:
            color=QColor(249,249,0);
            break;
        case 5:
            color=QColor(255,165,0);
            break;
        }
        this->m_splineSeries[i]->setPen(QPen(color,1));
        this->m_chart->addSeries(this->m_splineSeries[i]);
    }
    this->m_chart->legend()->hide();
    this->m_chart->setTitle(title);
    this->m_chart->createDefaultAxes();
    this->m_chart->axisX()->setRange(0,100);
    this->m_chart->axisY()->setRange(0,this->m_maxValue);
    this->m_chart->setBackgroundBrush(QBrush(QColor(67,80,86)));
    this->m_chart->setPlotAreaBackgroundBrush(QBrush(QColor(255,0,0)));
    this->m_chart->setTitleBrush(QBrush(QColor(255,255,255)));
    this->m_chart->axisX()->setLabelsColor(QColor(255,255,255));
    this->m_chart->axisY()->setLabelsColor(QColor(255,255,255));
    //fill the initial data for 8-GLasers.
    for(qint32 j=0;j<6;j++)
    {
        for(qint32 i=0;i<this->m_maxSize;i++)
        {

            this->m_splineSeries[j]->append(i*10,5);
        }
    }

    this->m_chartView=new QChartView(this->m_chart);
    this->m_chartView->setRenderHint(QPainter::Antialiasing);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_chartView);
    this->setLayout(this->m_vLayout);

    qsrand(QDateTime::currentDateTime().toTime_t());
    this->m_timer=new QTimer;
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(ZSloGenerateRandomValue()));
    this->m_timer->start(1000);
}
ZCurveUI::~ZCurveUI()
{
    this->m_timer->stop();
    delete this->m_timer;


    for(qint32 i=0;i<6;i++)
    {
        delete this->m_splineSeries[i];
    }

    delete this->m_chart;
    delete this->m_chartView;
}
void ZCurveUI::ZSloDataReceived(qint32 which,qint32 val)
{
    this->m_valList[which].append(val);
    while(this->m_valList[which].size()>this->m_maxSize)
    {
        this->m_valList[which].removeFirst();
    }
    int delta=this->m_maxSize-this->m_valList[which].size();
    for(qint32 i=0;i<this->m_valList[which].size();++i)
    {
        qint32 newX=this->m_splineSeries[which]->at(delta+i).x();
        qint32 newY=this->m_valList[which].at(i);
        this->m_splineSeries[which]->replace(delta+i,newX,newY);
    }
}
void ZCurveUI::ZSloGenerateRandomValue()
{
    qint32 which=qrand()%6;
    qint32 newData=qrand()%(this->m_maxValue+1);
    //qDebug()<<"which="<<which<<",val="<<newData;
    this->ZSloDataReceived(which,newData);
}

