#ifndef ZPIECHART_H
#define ZPIECHART_H

#include <QFrame>
#include <QtCharts>
#include <QVBoxLayout>
#include <QVector>
class PieOne
{
public:
    QString oneTitle;
    qint32 onePercent;
    QColor oneColor;
};
class ZPieChart : public QFrame
{
public:
    ZPieChart(QString chartTitle,QVector<PieOne> pieVector);
    ~ZPieChart();
private:
    QPieSeries *m_series;
    QChart *m_chart;
    QChartView *m_chartView;
    QVBoxLayout *m_vLayout;
};

#endif // ZPIECHART_H
