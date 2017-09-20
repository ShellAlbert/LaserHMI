#ifndef ZBASEOBJ_H
#define ZBASEOBJ_H

#include <QFrame>

class ZBaseObj : public QFrame
{
    Q_OBJECT
public:
    ZBaseObj(QWidget *parent = 0);
    ~ZBaseObj();

    virtual void ZDoInitial(void);
    virtual void ZUnInitial(void);
};

#endif // ZBASEOBJ_H
