#include "zverticalindicator.h"
#include <QPainter>
ZVerticalIndicator::ZVerticalIndicator(QWidget *parent) :
    QWidget(parent)
{
    this->initVariables();
}

void ZVerticalIndicator::initVariables()
{
    m_markClr=Qt::white;
    graphcolor=Qt::darkGreen;
    m_value=0;
    m_currentValue=0;
    m_bucketWidth=width()/3;
    m_markSpace=(float)width()/4;
    m_topSpace=(float)height()/15;
    updateTimer=new QTimer(this);
    updateTimer->setInterval(10);
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(UpdateGraph()));
    m_bReverse=false;
}

void ZVerticalIndicator::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHints(QPainter::TextAntialiasing|QPainter::Antialiasing);
    resetDrawingVariables(&painter);
    drawBackground(&painter);
    drawMark(&painter);
    drawBucket(&painter);
    drawGraph(&painter);

    painter.end();
}

void ZVerticalIndicator::drawBackground(QPainter *painter)
{
    painter->save();

    QLinearGradient bgGradient(QPointF(0,0),QPointF(0,height()));
    bgGradient.setColorAt(0.0,QColor(200,200,200));
    bgGradient.setColorAt(0.2,QColor(100,100,100));
    bgGradient.setColorAt(0.9,QColor(100,100,100));
    bgGradient.setColorAt(1.0,QColor(200,200,200));

    painter->setBrush(bgGradient);
    painter->drawRect(rect());
    painter->restore();
}
void ZVerticalIndicator::drawMark(QPainter *painter)
{
    painter->save();

    float initY=m_topSpace;
    float initX=m_markSpace;
    float length=height()-2*m_topSpace;
    float increment=length/100;
    painter->setPen(m_markClr);

    // draw vertical line
    QPointF topPot(m_markSpace,m_topSpace);
    QPointF bottomPot(m_markSpace,height()-m_topSpace);
    painter->drawLine(topPot,bottomPot);

    int vvalue=100;
    QString strValue;
    for(int i=0;i<=100;i++)
    {
        if(i%10==0)
        {
            QPointF leftPot(initX+LONG3,initY);
            QPointF rightPot(initX,initY);
            painter->drawLine(leftPot,rightPot);

            strValue=tr("%1").arg(vvalue);
            float fontWidth=painter->fontMetrics().width(strValue);
            float fontHeight=painter->fontMetrics().height();

            QPointF textPot(initX-fontWidth-5,initY+fontHeight/2);
            painter->drawText(textPot,strValue);

            vvalue-=10;

        }
        else if(i%5==0 && i%10!=0)
        {
            QPointF leftPot(initX+OK3,initY);
            QPointF rightPot(initX,initY);
            painter->drawLine(leftPot,rightPot);
        }
        else
        {
            QPointF leftPot(initX+SHORT3,initY);
            QPointF rightPot(initX,initY);
            painter->drawLine(leftPot,rightPot);
        }
        initY+=increment;
    }
    painter->restore();
}

void ZVerticalIndicator::SetGraphColor(QColor c)
{
    this->graphcolor=c;
}


void ZVerticalIndicator::drawGraph(QPainter *painter)
{
    painter->save();
    qreal bucketHeight=m_bucketRect.height();
    qreal increment=(float)bucketHeight/100;

    QPointF bottomRightPot(m_bucketRect.bottomRight());
    qreal currentY=m_currentValue*increment;

    QPointF topLeftPot(m_bucketRect.topLeft().x(),m_bucketRect.bottomLeft().y()-currentY);
    QRectF graphRect(topLeftPot,bottomRightPot);
    painter->setPen(Qt::NoPen);

    QLinearGradient graphGradient(graphRect.topLeft(),graphRect.topRight());
    painter->setOpacity(0.7);

    graphGradient.setColorAt(0.0,this->graphcolor);

    painter->setBrush(graphGradient);
    painter->drawRect(graphRect);
    painter->restore();

    painter->save();
    painter->setPen(Qt::black);
    QString text=QString::number(this->m_value,'f',2)+QString("%");
    qint32 textWidth=painter->fontMetrics().width(text);
    qint32 textHeight=painter->fontMetrics().height();
    QPoint textLeftTop(topLeftPot.x()+(graphRect.width()-textWidth)/2,topLeftPot.y());
    QPoint textRightBottom(topLeftPot.x()+graphRect.width()/2+textWidth/2,topLeftPot.y()+textHeight);
    QRectF textRect(textLeftTop,textRightBottom);
    painter->drawText(textRect,text);
    painter->restore();
}

void ZVerticalIndicator::drawBucket(QPainter *painter)
{
    painter->save();

    QPointF bucketTopLeftPot(m_markSpace+MARK_BUCKET_SPACE3,m_topSpace);
    QPointF bucketBottomRightPot(m_markSpace+MARK_BUCKET_SPACE3+m_bucketWidth,height()-m_topSpace);
    QRectF bucketRect(bucketTopLeftPot,bucketBottomRightPot);
    m_bucketRect=bucketRect;

    QLinearGradient bucketGradient(bucketRect.topLeft(),bucketRect.topRight());
    bucketGradient.setColorAt(0.0,QColor(100,100,100));
    bucketGradient.setColorAt(0.6,QColor(200,200,200));
    bucketGradient.setColorAt(1.0,QColor(150,150,150));

    painter->setPen(Qt::NoPen);
    painter->setBrush(bucketGradient);
    painter->drawRect(bucketRect);
    painter->restore();
}

void ZVerticalIndicator::resetDrawingVariables(QPainter *painter)
{
    m_markSpace=(float)width()/4;
    m_bucketWidth=(float)width()/3;
    m_topSpace=(float)height()/15;
}

float ZVerticalIndicator::getValue()
{
    return this->m_value;
}
void ZVerticalIndicator::setValue(float value)
{
    if(value>m_value)
    {
        m_bReverse=false;
    }
    else if(value<m_value)
    {
        m_bReverse=true;
    }
    else
    {
        return ;
    }
    m_value=value;
    if(this->updateTimer->isActive())
    {
        this->updateTimer->stop();
    }
    updateTimer->start();
}

void ZVerticalIndicator::setMarkColor(const QColor &clr)
{
    m_markClr=clr;
    update();
}


void ZVerticalIndicator::UpdateGraph()
{
    if(!m_bReverse)
    {
        m_currentValue+=0.5;
        if(m_currentValue>=m_value)
        {
            updateTimer->stop();
        }
    }
    else
    {
        m_currentValue-=0.5;
        if(m_currentValue<=m_value)
        {
            updateTimer->stop();
        }
    }
    update();
}
