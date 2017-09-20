#ifndef ZFRAME_H
#define ZFRAME_H

#include <QFrame>
#include <QPaintEvent>
class ZFrame:public QFrame
{
public:
    ZFrame(QString title);

protected:
    void paintEvent(QPaintEvent *e);

private:
    QString m_title;
};

#endif // ZFRAME_H
