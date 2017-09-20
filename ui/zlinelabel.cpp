#include "zlinelabel.h"
#include <QPainter>
ZLineLabel::ZLineLabel()
{

}
void ZLineLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(10);
    painter.setPen(pen);
    painter.drawLine(QPoint(0,0),QPoint(this->width(),this->height()));
}
