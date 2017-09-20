#ifndef ZLINELABEL_H
#define ZLINELABEL_H

#include <QLabel>
#include <QPaintEvent>
class ZLineLabel:public QLabel
{
    Q_OBJECT
public:
    ZLineLabel();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // ZLINELABEL_H
