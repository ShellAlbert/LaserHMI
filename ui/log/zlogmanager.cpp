#include "zlogmanager.h"
ZLogManager* ZLogManager::m_pInstance=NULL;
ZLogManager::ZLogManager()
{
    this->m_sema=new QSemaphore(1);
}
void ZLogManager::ZPutLog(QString module,ZLogType type,QString logMsg)
{
    ZLogMsg logObj;
    logObj.module=module;
    logObj.type=type;
    logObj.msg=logMsg;
    logObj.bWrFlag=0;
    logObj.timeStamp=QDateTime::currentDateTime();
    this->m_sema->acquire();
    if(this->m_vector.size()>=100)
    {
        this->m_vector.removeFirst();
    }
    this->m_vector.append(logObj);
    this->m_sema->release();
}
void ZLogManager::ZSloFlushLog2DB()
{
    QVector<ZLogMsg>::iterator it;
    for(it=this->m_vector.begin();it!=this->m_vector.end();it++)
    {
        if(it->bWrFlag==0)
        {
            //write to db.
            it->bWrFlag=1;
        }
    }
}
