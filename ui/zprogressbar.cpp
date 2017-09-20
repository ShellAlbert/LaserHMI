#include "zprogressbar.h"
#include <QPainter>
#include <QDebug>
ZProgressBar::ZProgressBar(QColor bgColor,QColor fgColor)
{
    this->m_fgColor=fgColor;
    this->m_bgColor=bgColor;
    this->m_cur=0.0f;
}
ZProgressBar::~ZProgressBar()
{

}
void ZProgressBar::ZSetValue(float val)
{
    this->m_cur=val;
    this->update();
}
float ZProgressBar::ZGetValue()
{
    return this->m_cur;
}
void ZProgressBar::ZSetText(QString text,qint32 fontSize)
{
    this->m_text=text;
    this->m_fontSize=fontSize;
}
void ZProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(this->m_bgColor));

    qint32 winWidth=this->rect().width();
    qint32 winHeight=this->rect().height();

    qint32 eachBlock=winWidth/10;
    qint32 drawBlock=(qint32)this->m_cur/10*eachBlock;
    qDebug()<<"winWidth:"<<winWidth<<",eachBlock:"<<eachBlock;
    qDebug()<<"drawBlock:"<<drawBlock;
    if(drawBlock>0)
    {
        painter.setPen(this->m_fgColor);
        QRect drawRect(this->rect().x(),this->rect().y(),this->rect().x()+drawBlock,winHeight);
        painter.fillRect(drawRect,QBrush(this->m_fgColor));
    }

    if(!this->m_text.isEmpty())
    {
        painter.setPen(QColor(255,0,255));
        QFont font=painter.font();
        font.setPixelSize(this->m_fontSize);
        painter.setFont(font);
        QString drawText=this->m_text+QString::number(this->m_cur,'f',1)+"%";
        qint32 textWidth=painter.fontMetrics().width(drawText);
        qint32 textHeight=painter.fontMetrics().height();
        QRect rect;
        rect.setX(this->rect().x()+(this->rect().width()-textWidth)/2);
        rect.setY(this->rect().y()+(this->rect().height()-textHeight)/2);
        rect.setWidth(textWidth);
        rect.setHeight(textHeight);
        painter.drawText(rect,drawText);
    }
}
