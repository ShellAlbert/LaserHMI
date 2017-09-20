#include "zsqlitedb.h"
#include <QDir>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QTextCodec>
#include <QApplication>
ZSQLiteDB::ZSQLiteDB()
{

}
ZSQLiteDB::~ZSQLiteDB()
{

}
qint32 ZSQLiteDB::ZDoInit(void)
{
    this->m_db=QSqlDatabase::addDatabase("QSQLITE","LaserConnDB");
    return 0;
}
qint32 ZSQLiteDB::ZUnInit(void)
{
    QSqlDatabase::removeDatabase("LaserConnDB");
    return 0;
}
qint32 ZSQLiteDB::ZOpenDB(QString dbName,bool createFlag)
{
    QString dbFileName(QDir::currentPath()+"/"+dbName);
    if(!QFile::exists(dbFileName) && createFlag)
    {
        QFile dbFile(dbFileName);
        if(!dbFile.open(QIODevice::WriteOnly))
        {
            return -1;
        }
        dbFile.close();
        qDebug()<<dbFileName<<",not exists,create it.";
    }
    qDebug()<<dbFileName;
    this->m_db.setDatabaseName(dbFileName);
    if(this->m_db.open())
    {
        qDebug()<<dbName<<",open okay.";
        return 0;
    }
    qDebug()<<this->m_db.lastError().text();
    return -1;
}
qint32 ZSQLiteDB::ZCloseDB(QString dbName)
{
    this->m_db.close();
    return 0;
}
qint32 ZSQLiteDB::ZExecuteSql(QString sql)
{
    QSqlQuery query(this->m_db);
    if(query.exec(sql))
    {
        return 0;
    }else{
        qDebug()<<query.lastError().text();
        return -1;
    }
}
qint32 ZSQLiteDB::ZIsTableExist(QString tableName)
{
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    QString queryStr=QString("SELECT 1 FROM sqlite_master WHERE type='table' AND tbl_name='%1'").arg(tableName);
    if(!query.exec(queryStr))
    {
        qDebug()<<query.lastError().text();
        return -2;
    }
    if(query.next())
    {
        int nResult=query.value(0).toInt();
        if(nResult)
        {
            return 0;
        }
    }
    qDebug()<<query.lastError().text();
    return -1;
}

//common.
qint32 ZSQLiteDB::ZGetRecordCount(QString tableName)
{
    int nCount=0;
    //SELECT COUNT(*) FROM OpLogTable;
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    QString sqlStr=QString("SELECT COUNT(*) FROM %1").arg(tableName);
    if(query.exec(sqlStr))
    {
        if(query.next())
        {
            nCount=query.value(0).toInt();
        }
    }else{
        qDebug()<<query.lastError().text();
    }
    return nCount;
}
qint32 ZSQLiteDB::ZGetMaxRecordCount(QString tableName)
{
    if(tableName==QString("OpLogTable"))
    {
        return 2000;
    }
    return 0;
}
qint32 ZSQLiteDB::ZDelRecord(QString tableName,qint32 startIndex,qint32 endIndex)
{
    //DELETE FROM OpLogTable WHERE ID IN ( SELECT ID FROM OpLogTable LIMIT 0,2);
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    query.prepare("DELETE FROM :TableName1 WHERE ID IN ( SELECT ID FROM :TableName2 LIMIT :startIndex,:endIndex)");
    query.bindValue(":TableName1",tableName);
    query.bindValue(":TableName2",tableName);
    query.bindValue(":startIndex",startIndex);
    query.bindValue(":endIndex",endIndex);
    if(query.exec())
    {
        return 0;
    }
    qDebug()<<query.lastError().text();
    return -1;
}
//ConfigTable.
qint32 ZSQLiteDB::ZCreateConfigTable(void)
{
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    if(query.exec("CREATE TABLE ConfigTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,CinemaName VARCHAR (256),DevNo VARCHAR(16),Password VARCHAR(8))"))
    {
        query.exec("INSERT INTO ConfigTable (CinemaName,DevNo,Password) VALUES(\"大地数字影院(烟台莱山永旺店)\",\"001\",\"77585210\")");
        return 0;
    }else{
        return -1;
    }
}
qint32 ZSQLiteDB::ZGetConfigData(QStringList &cfgList)
{
    //SELECT CinemaName,DevNo,Password FROM ConfigTable.
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    if(query.exec("SELECT CinemaName,DevNo,Password FROM ConfigTable"))
    {
        while(query.next())
        {
            cfgList.append(query.value(0).toString());
            cfgList.append(query.value(1).toString());
            cfgList.append(query.value(2).toString());
        }
        return 0;
    }
    qDebug()<<query.lastError().text();
    cfgList.append(tr("大地数字影院(烟台莱山永旺店)"));
    cfgList.append(tr("001"));
    cfgList.append(tr("77585210"));
    return -1;
}
qint32 ZSQLiteDB::ZUpdatePassword(QString newPass)
{
    //UPDATE ConfigTable SET Password='123';
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    query.prepare("UPDATE ConfigTable SET Password=:newPassword");
    query.bindValue(":newPassword",newPass);
    if(query.exec())
    {
        return 0;
    }
    return -1;
}
//OpLogTable.
qint32 ZSQLiteDB::ZCreateOPLogTable(void)
{
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    if(query.exec("CREATE TABLE OpLogTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,LogTime DATETIME,LogType INTEGER,LogMsg VARCHAR (256))"))
    {
        return 0;
    }else{
        return -1;
    }
}
//PinYinTable.

qint32 ZSQLiteDB::ZCreatePinYinTable(void)
{
    //CREATE TABLE PinYinTable (ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, PinYin VARCHAR (16), HanZi VARCHAR (512));
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    if(query.exec("CREATE TABLE PinYinTable (ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, PinYin VARCHAR (16), HanZi VARCHAR (512))"))
    {
        QFile file(":/IME/images/hzpy-utf8.txt");
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            return -1;
        }
        QTextStream textStream(&file);
        QTextCodec *codec=QTextCodec::codecForName("utf-8");
        textStream.setCodec(codec);
        while(!textStream.atEnd())
        {
            QString lineData=textStream.readLine();
            if(lineData.isEmpty())
            {
                continue;
            }
            QStringList lineDataList=lineData.split(",");
            if(lineDataList.size()!=6)
            {
                continue;
            }
            QString hanzi=lineDataList.at(0);
            QString pinyi=lineDataList.at(1);
            //1, 'zhang', '张，章，眔');
            QString sqlStr=QString("INSERT INTO PinYinTable (PinYin, HanZi) VALUES (\"%1\",\"%2\")").arg(pinyi).arg(hanzi);
            qDebug()<<sqlStr;
            emit this->ZSigIMEMsg(tr("正在构建拼音输入法数据库[%1-%2]...").arg(pinyi).arg(hanzi));
            qApp->processEvents();
            this->ZExecuteSql(sqlStr);
        }
        file.close();
        return 0;
    }
    return -1;
}
qint32 ZSQLiteDB::ZGetPinYinData(QString pinyin,QStringList &hanziList)
{
    //SELECT HanZi FROM PinYinTable WHERE PinYin='aa'.
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    query.prepare("SELECT HanZi FROM PinYinTable WHERE PinYin=:pinyin LIMIT 6");
    query.bindValue(":pinyin",pinyin);
    if(query.exec())
    {
        while(query.next())
        {
            hanziList.append(query.value(0).toString());
        }
        return 0;
    }else{
        return -1;
    }
}
qint32 ZSQLiteDB::ZAddOPLogRecord(QString dateTime,qint32 logType,QString logMsg)
{
    //if the row count greater than 2000.
    //then delete the top 10 records to generate a ringbuffer.
//    if(this->ZGetRecordCount("OpLogTable")>=this->ZGetMaxRecordCount("OpLogTable"))
//    {
//        this->ZDelRecord("OpLogTable",0,10);
//    }

    //ID:ID INTEGER PRIMARY KEY AUTOINCREMENT
    //INSERT INTO OpLogTable (ID,LogTime, LogType, LogMsg) VALUES (1,'2017-05-05 18:10:12', 1, 'HMI start up okay.');
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    query.prepare("INSERT INTO OpLogTable (LogTime, LogType, LogMsg) VALUES (:LogTime,:LogType,:LogMsg)");
    query.bindValue(":LogTime",dateTime);
    query.bindValue(":LogType",logType);
    query.bindValue(":LogMsg",logMsg);
    if(query.exec())
    {
        return 0;
    }
    qDebug()<<query.lastError().text();
    return -1;
}

qint32 ZSQLiteDB::ZGetOPLogRecord(qint32 startIndex,qint32 endIndex,QStringList &dateTime,QStringList &logType,QStringList &logMsg)
{
    //SELECT LogTime,LogType,LogMsg FROM OpLogTable LIMIT 0,10.
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    query.prepare("SELECT LogTime,LogType,LogMsg FROM OpLogTable LIMIT :startIndex,:endIndex");
    query.bindValue(":startIndex",startIndex);
    query.bindValue(":endIndex",endIndex);
    if(query.exec())
    {
        while(query.next())
        {
            dateTime.append(query.value(0).toString());
            logType.append(query.value(1).toString());
            logMsg.append(query.value(2).toString());
        }
        return 0;
    }
    qDebug()<<query.lastError().text();
    return -1;
}

//ColorTempTable.
qint32 ZSQLiteDB::ZCreateColorTempTable(void)
{
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    if(query.exec("CREATE TABLE ColorTempTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,Addr INTEGER,ColorTemp VARCHAR (12),Red REAL,Green REAL,Blue REAL,RedDefault REAL,GreenDefault REAL,BlueDefault REAL)"))
    {
        QStringList supportColorTemp;
        supportColorTemp<<"D63";
        supportColorTemp<<"D65";
        supportColorTemp<<"DCI";
        supportColorTemp<<"D80";
        supportColorTemp<<"D93";
        supportColorTemp<<"D95";
        for(qint32 i=0;i<supportColorTemp.size();i++)
        {
            //all.
            query.prepare("INSERT INTO ColorTempTable (Addr,ColorTemp,Red,Green,Blue,RedDefault,GreenDefault,BlueDefault) VALUES(255,:colorTemp,100.0,100.0,100.0,100.0,100.0,100.0)");
            query.bindValue(":colorTemp",supportColorTemp.at(i));
            query.exec();
            //1~8 glaser.
            for(qint32 j=1;j<=8;j++)
            {
                query.prepare("INSERT INTO ColorTempTable (Addr,ColorTemp,Red,Green,Blue,RedDefault,GreenDefault,BlueDefault) VALUES(:addr,:colorTemp,100.0,100.0,100.0,100.0,100.0,100.0)");
                query.bindValue(":addr",j);
                query.bindValue(":colorTemp",supportColorTemp.at(i));
                query.exec();
            }
        }
        return 0;
    }else{
        return -1;
    }
}
qint32 ZSQLiteDB::ZGetColorTempData(quint8 glaserAddr,QString colorTempName,QStringList &valList)
{
    //SELECT Red,Green,Blue,RedDefault,GreenDefault,BlueDefault FROM ColorTempTable WHERE Addr=1 AND ColorTemp='a'.
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    query.prepare("SELECT Red,Green,Blue,RedDefault,GreenDefault,BlueDefault FROM ColorTempTable WHERE Addr=:addr AND ColorTemp=:colorTemp");
    query.bindValue(":addr",glaserAddr);
    query.bindValue(":colorTemp",colorTempName);
    if(query.exec())
    {
        while(query.next())
        {
            valList.append(query.value(0).toString());
            valList.append(query.value(1).toString());
            valList.append(query.value(2).toString());
            valList.append(query.value(3).toString());
            valList.append(query.value(4).toString());
            valList.append(query.value(5).toString());
        }
        return 0;
    }
    qDebug()<<query.lastError().text();
    for(qint32 i=0;i<6;i++)
    {
        valList.append("100");
    }
    return -1;
}
qint32 ZSQLiteDB::ZPutColorTempData(quint8 glaserAddr,QString colorTempName,QStringList &valList)
{
    //UPDATE ColorTempTable SET Red=1,Green=2,Blue=3 WHERE Addr=1 AND ColorTemp='a'.
    if(colorTempName.isEmpty()||valList.size()!=3)
    {
        return -1;
    }
    QSqlDatabase db=QSqlDatabase::database("LaserConnDB");
    QSqlQuery query(db);
    if(glaserAddr==0xFF)
    {
        query.prepare("UPDATE ColorTempTable SET Red=:red,Green=:green,Blue=:blue WHERE ColorTemp=:colorTemp");
        query.bindValue(":red",valList.at(0).toFloat());
        query.bindValue(":green",valList.at(1).toFloat());
        query.bindValue(":blue",valList.at(2).toFloat());
        query.bindValue(":colorTemp",colorTempName);
        if(query.exec())
        {
            return 0;
        }
    }else{
        query.prepare("UPDATE ColorTempTable SET Red=:red,Green=:green,Blue=:blue WHERE Addr=:addr AND ColorTemp=:colorTemp");
        query.bindValue(":red",valList.at(0).toFloat());
        query.bindValue(":green",valList.at(1).toFloat());
        query.bindValue(":blue",valList.at(2).toFloat());
        query.bindValue(":addr",glaserAddr);
        query.bindValue(":colorTemp",colorTempName);
        if(query.exec())
        {
            return 0;
        }
    }
    return -1;
}
