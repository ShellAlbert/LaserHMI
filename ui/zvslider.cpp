#include "zvslider.h"
#include <QPainter>
ZVSlider::ZVSlider(QString title)
{
//    this->setObjectName("ZVSlider");
//    this->m_llTitle=new QLabel(title);
//    this->m_llVal=new QLabel(tr("0℃"));
//    this->m_llVal->setObjectName("ZVSliderValLabel");
//    this->m_llVal->setAlignment(Qt::AlignCenter);
//    this->m_llMax=new QLabel(tr("+150℃"));
//    this->m_llMax->setAlignment(Qt::AlignCenter);
//    this->m_llMin=new QLabel(tr("-20℃"));
//    this->m_llMin->setAlignment(Qt::AlignCenter);
//    this->m_slider=new QSlider(Qt::Vertical);
//    this->m_slider->setObjectName("ZVSliderQSlider");
//    this->m_slider->setRange(-20,150);
//    this->m_slider->setValue(0);
//    this->m_slider->setDisabled(true);

//    this->m_llTitle->setObjectName("ZVSliderTipsLabel");
//    this->m_llMax->setObjectName("ZVSliderTipsLabel");
//    this->m_llMin->setObjectName("ZVSliderTipsLabel");
//    this->m_gLayout=new QGridLayout;
//    this->m_gLayout->addWidget(this->m_llTitle,0,0,1,1);
//    this->m_gLayout->addWidget(this->m_llMax,0,1,1,1);
//    this->m_gLayout->addWidget(this->m_slider,1,1,1,1);
//    this->m_gLayout->addWidget(this->m_llMin,2,1,1,1);
//    this->m_gLayout->addWidget(this->m_llVal,1,0,2,1);
//    this->m_gLayout->setColumnStretch(0,7);
//    this->m_gLayout->setColumnStretch(1,3);
//    this->setLayout(this->m_gLayout);
}
ZVSlider::~ZVSlider()
{
//    delete this->m_llTitle;
//    delete this->m_llVal;
//    delete this->m_llMin;
//    delete this->m_llMax;
//    delete this->m_slider;
//    delete this->m_gLayout;
}
void ZVSlider::ZSetValue(qreal val)
{
//    this->m_llVal->setText(QString("%1").arg(val));
//    this->m_slider->setValue((qint32)val);
}
void ZVSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    qint32 tSubWidth=(this->width()-10)/3;
    qint32 tSubHeight=(this->height()-50)/2;
    QRect rect1;
    rect1.setX(this->x()+tSubWidth);
    rect1.setY(10);
    rect1.setWidth(tSubWidth);
    rect1.setHeight(tSubHeight);
    painter.fillRect(rect1,Qt::blue);

    QRect rect2;
    rect2.setX(10);
    rect2.setY(this->y()+this->height()/2);
    rect2.setWidth(this->width()-10);
    rect2.setHeight(tSubHeight);
    painter.drawEllipse(rect2);

//    QPainterPath path1;
//    path1.addPolygon();
}
