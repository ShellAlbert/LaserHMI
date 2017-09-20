#include "zpiechart.h"

ZPieChart::ZPieChart(QString chartTitle,QVector<PieOne> pieVector)
{
    this->m_series=new QPieSeries;
    this->m_series->setLabelsVisible(true);
    this->m_series->setLabelsPosition(QPieSlice::LabelInsideTangential);
    for(qint32 i=0;i<pieVector.size();i++)
    {
        PieOne oneData=pieVector.at(i);
       this->m_series->append(oneData.oneTitle,oneData.onePercent);
        qDebug()<<oneData.oneTitle<<oneData.onePercent;
    }
    for(qint32 i=0;i<pieVector.size();i++)
    {
        PieOne oneData=pieVector.at(i);
        QPieSlice *slice=this->m_series->slices().at(i);
        slice->setColor(oneData.oneColor);
    }
    this->m_chart=new QChart;
    this->m_chart->addSeries(this->m_series);
    this->m_chart->setTitle(chartTitle);

    this->m_chartView=new QChartView(this->m_chart);
    this->m_chartView->setRenderHint(QPainter::Antialiasing);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_chartView);
    this->setLayout(this->m_vLayout);
}
ZPieChart::~ZPieChart()
{
    delete this->m_series;
    delete this->m_chart;
    delete this->m_chartView;
    delete this->m_vLayout;
}
