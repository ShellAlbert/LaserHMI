#ifndef ZLOGMANAGER_H
#define ZLOGMANAGER_H

#include "base/zbaseobj.h"
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QTimer>
#include <QSemaphore>
typedef enum {
    Normal=0,
    General_Warning=1,
    Critical_Warning=2,
    General_Error=3,
    Critical_Error=4,
}ZLogType;
class ZLogMsg
{
public:
    QString md5;
    QString module;
    ZLogType type;
    QString msg;
    QDateTime timeStamp;
    bool bWrFlag;
};
class ZLogManager : public ZBaseObj
{
    Q_OBJECT
public:
    static ZLogManager* ZGetInstance()
    {
        if(m_pInstance==NULL)
        {
            m_pInstance=new ZLogManager;
        }
        return m_pInstance;
    }
    void ZPutLog(QString module,ZLogType type,QString logMsg);
private slots:
    void ZSloFlushLog2DB();
private:
    ZLogManager();
    static ZLogManager *m_pInstance;
    QVector<ZLogMsg> m_vector;
    QTimer *m_flushTimer;
    QSemaphore *m_sema;
};

#endif // ZLOGMANAGER_H
