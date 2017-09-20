#ifndef ZVERTICALINDICATOR_H
#define ZVERTICALINDICATOR_H

#define LONG3 7
#define OK3 5
#define SHORT3 3
#define MARK_BUCKET_SPACE3 20

#include <QWidget>
#include <QTimer>

class ZVerticalIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit ZVerticalIndicator(QWidget *parent = 0);

    void SetGraphColor(QColor c);

protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const
    {
        return QSize(150,300);
    }

    QSize minimumSizeHint() const
    {
        return QSize(50,100);
    }

private:
    void drawMark(QPainter* painter);
    void drawBucket(QPainter* painter);
    void drawBackground(QPainter* painter);
    void drawGraph(QPainter* painter);
    void resetDrawingVariables(QPainter* painter);

private:
    float m_value;
    float m_currentValue;
    float m_bucketWidth;
    QTimer* updateTimer;
    bool  m_bReverse;
    float m_markSpace;
    float m_topSpace;
    QColor m_markClr;
    QColor graphcolor;
    QRectF m_bucketRect;

private:
    void initVariables();

private slots:
    void UpdateGraph();

public slots:
    void setValue(float value);
    float getValue();
    void setMarkColor(const QColor& clr);

};
#endif // ZVERTICALINDICATOR_H
