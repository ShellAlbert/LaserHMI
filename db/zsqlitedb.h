#ifndef ZSQLITEDB_H
#define ZSQLITEDB_H
#include <QObject>
#include "db/zbasedb.h"
#include <QtSql/QSqlDatabase>
class ZSQLiteDB : public QObject,public ZBaseDB
{
    Q_OBJECT
public:
    ZSQLiteDB();
    ~ZSQLiteDB();
public:
    qint32 ZDoInit(void);
    qint32 ZUnInit(void);
    qint32 ZOpenDB(QString dbName,bool createFlag=false);
    qint32 ZCloseDB(QString dbName);
    qint32 ZExecuteSql(QString sql);
    qint32 ZIsTableExist(QString tableName);

    //common.
    qint32 ZGetRecordCount(QString tableName);
    qint32 ZGetMaxRecordCount(QString tableName);
    qint32 ZDelRecord(QString tableName,qint32 startIndex,qint32 endIndex);

    //ConfigTable.
    qint32 ZCreateConfigTable(void);
    qint32 ZGetConfigData(QStringList &cfgList);
    qint32 ZUpdatePassword(QString newPass);

    //OpLogTable.
    qint32 ZCreateOPLogTable(void);
    qint32 ZAddOPLogRecord(QString dateTime,qint32 logType,QString logMsg);
    qint32 ZGetOPLogRecord(qint32 startIndex,qint32 endIndex,QStringList &dateTime,QStringList &logType,QStringList &logMsg);

    //PinYinTable.
    qint32 ZCreatePinYinTable(void);
    qint32 ZGetPinYinData(QString pinyin,QStringList &hanziList);

    //ColorTempTable.
    qint32 ZCreateColorTempTable(void);
    qint32 ZGetColorTempData(quint8 glaserAddr,QString colorTempName,QStringList &valList);
    qint32 ZPutColorTempData(quint8 glaserAddr,QString colorTempName,QStringList &valList);
signals:
    void ZSigIMEMsg(QString msg);
private:
    QSqlDatabase m_db;
};

#endif // ZSQLITEDB_H
