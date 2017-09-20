#ifndef ZRINGBUFFER_H
#define ZRINGBUFFER_H

#include <QVector>
#include <QMutex>
class ZRingElement
{
public:
    QString cmd;
    QString arguments;
};
class ZRingBuffer
{
public:
    ZRingBuffer();
    ~ZRingBuffer();

    qint32 ZDOInit(qint32 max=6);
    qint32 ZUnInit();

    bool ZIsEmpty(void);
    bool ZIsFull(void);
    qint32 ZPutElement(QString cmd,QString arguments);
    qint32 ZGetElement(QString &cmd,QString &arguments);
    void ZDoLock();
    void ZUnLock();
    bool ZTryLock();

    void ZDumpElements();
private:
    qint32 m_rdPos;
    qint32 m_wrPos;
    qint32 m_maxNum;
    ZRingElement *m_buffer;
    QMutex *m_mutex;
};

#endif // ZRINGBUFFER_H
