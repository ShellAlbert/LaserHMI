#ifndef ZBASEDB_H
#define ZBASEDB_H

#include <QString>
class ZBaseDB
{
public:
    ZBaseDB();

    virtual qint32 ZDoInit(void);
    virtual qint32 ZUnInit(void);
    virtual qint32 ZOpenDB(QString dbName,bool createFlag=false);
    virtual qint32 ZCloseDB(QString dbName);
    virtual qint32 ZExecuteSql(QString sql);
};

#endif // ZBASEDB_H
