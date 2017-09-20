#ifndef ZPROGRESSBAR_H
#define ZPROGRESSBAR_H

#include <QLabel>
class ZProgressBar : public QLabel
{
public:
    ZProgressBar(QColor bgColor,QColor fgColor);
    ~ZProgressBar();

    void ZSetValue(float val);
    float ZGetValue();
    void ZSetText(QString text,qint32 fontSize);
protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor m_fgColor;
    QColor m_bgColor;

    QString m_text;
    qint32 m_fontSize;
    float m_cur;
};

#endif // ZPROGRESSBAR_H
