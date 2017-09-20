#include "zframe.h"
#include <QPainter>
ZFrame::ZFrame(QString title)
{
    this->m_title=title;
    this->setFrameStyle(QFrame::Box|QFrame::Sunken);
}
void ZFrame::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);

    painter.drawText(5,20,this->m_title);

    QFrame::paintEvent(e);
}
