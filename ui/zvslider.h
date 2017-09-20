#ifndef ZVSLIDER_H
#define ZVSLIDER_H

#include <QLabel>
#include <QSlider>
#include <QGridLayout>
class ZVSlider : public QFrame
{
public:
    ZVSlider(QString title);
    ~ZVSlider();
    void ZSetValue(qreal val);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QLabel *m_llTitle;
    QLabel *m_llVal;
    QLabel *m_llMin;
    QLabel *m_llMax;
    QSlider *m_slider;
    QGridLayout *m_gLayout;
};

#endif // ZVSLIDER_H
