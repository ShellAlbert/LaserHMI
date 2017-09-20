#include "zringbuffer.h"
#include <QDebug>
ZRingBuffer::ZRingBuffer()
{

}
ZRingBuffer::~ZRingBuffer()
{

}

qint32 ZRingBuffer::ZDOInit(qint32 max)
{
    this->m_buffer=new ZRingElement[max];
    this->m_maxNum=max;
    this->m_rdPos=0;
    this->m_wrPos=0;
    this->m_mutex=new QMutex;
    return 0;
}
qint32 ZRingBuffer::ZUnInit()
{
    delete []this->m_buffer;
    delete this->m_mutex;
    return 0;
}

bool ZRingBuffer::ZIsEmpty(void)
{
    if(this->m_wrPos==this->m_rdPos)
    {
        return true;
    }
    return false;
}
bool ZRingBuffer::ZIsFull(void)
{
    if((this->m_wrPos+1)%this->m_maxNum==this->m_rdPos)
    {
        return true;
    }
    return false;
}
qint32 ZRingBuffer::ZPutElement(QString cmd,QString arguments)
{
    if(this->ZIsFull())
    {
        return -1;
    }
    ZRingElement ringEle;
    ringEle.cmd=cmd;
    ringEle.arguments=arguments;
    this->m_buffer[this->m_wrPos]=ringEle;
    if(this->m_wrPos+1==this->m_maxNum)
    {
        this->m_wrPos=0;
    }else{
        this->m_wrPos++;
    }
    return 0;
}
qint32 ZRingBuffer::ZGetElement(QString &cmd,QString &arguments)
{
    if(this->ZIsEmpty())
    {
        return -1;
    }
    ZRingElement ringEle=this->m_buffer[this->m_rdPos];
    cmd=ringEle.cmd;
    arguments=ringEle.arguments;
    if(this->m_rdPos+1==this->m_maxNum)
    {
        this->m_rdPos=0;
    }else{
        this->m_rdPos++;
    }
    return 0;
}
void ZRingBuffer::ZDoLock()
{
    this->m_mutex->lock();
}
void ZRingBuffer::ZUnLock()
{
    this->m_mutex->unlock();
}
bool ZRingBuffer::ZTryLock()
{
    return this->m_mutex->tryLock(0);
}
void ZRingBuffer::ZDumpElements()
{
    while(!this->ZIsEmpty())
    {
        QString cmd;
        QString argument;
        this->ZGetElement(cmd,argument);
        qDebug()<<"dump:"<<cmd<<argument;
    }
}
