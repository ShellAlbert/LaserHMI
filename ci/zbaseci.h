#ifndef ZBASECI_H
#define ZBASECI_H

#include <QObject>
class ZBaseCI
{
public:
    ZBaseCI();

    virtual qint32 ZPacketCmdToBuffer(char *buffer,const qint32 bufSize);
    virtual qint32 ZParseCmdFromBuffer(const char *buffer,const qint32 bufSize);
};

#endif // ZBASECI_H
