#ifndef ZCURVEUI_H
#define ZCURVEUI_H

#include <QFrame>
#include <QTimer>
#include <QPainterPath>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>
using namespace QtCharts;
class ZCurveUI : public QFrame
{
    Q_OBJECT
public:
    ZCurveUI(QString title);
    ~ZCurveUI();
public slots:
    void ZSloDataReceived(qint32 which,qint32 val);
private slots:
    void ZSloGenerateRandomValue();
private:
    qint32 m_maxSize;
    qint32 m_maxValue;
    QList<qint32> m_valList[6];

    QChart *m_chart;
    QChartView *m_chartView;
    QSplineSeries *m_splineSeries[6];
    QVBoxLayout *m_vLayout;

    QTimer *m_timer;
};

#endif // ZCURVEUI_H
