#include "zdbgserver.h"

ZDbgServer::ZDbgServer()
{

}
qint32 ZDbgServer::startServer()
{
    return this->listen(QHostAddress::Any,6800);
}
qint32 ZDbgServer::stopServer()
{
    this->close();
    return 0;
}

void ZDbgServer::incomingConnection(qintptr handle)
{
    ZDbgThread *thread=new ZDbgThread(handle);
    connect(this,SIGNAL(ZSigDumpDbgMsg(QString)),thread,SLOT(ZSloDumpDbgMsg(QString)));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();
}
ZDbgThread::ZDbgThread(qintptr handle)
{
    this->m_handle=handle;
    this->m_socket=NULL;
}
void ZDbgThread::ZSloDumpDbgMsg(QString msg)
{
    if(this->m_socket!=NULL && this->m_socket->state()==QAbstractSocket::ConnectedState)
    {
        this->m_socket->write(msg.toLatin1()+"\r\n");
        this->m_socket->waitForBytesWritten(3000);
    }else{
        //exit event-loop.
        this->exit(-1);
    }
}
void ZDbgThread::run()
{
    this->m_socket=new QTcpSocket;
    if(!this->m_socket->setSocketDescriptor(this->m_handle))
    {
        delete this->m_socket;
        return;
    }
    //enter event-loop.
    this->exec();
    this->m_socket->close();
    delete this->m_socket;
}
