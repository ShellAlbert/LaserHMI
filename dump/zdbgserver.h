#ifndef ZDBGSERVER_H
#define ZDBGSERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>
class ZDbgServer : public QTcpServer
{
    Q_OBJECT
public:
    ZDbgServer();
    qint32 startServer();
    qint32 stopServer();
signals:
    void ZSigDumpDbgMsg(QString msg);
protected:
    void incomingConnection(qintptr handle);
};
class ZDbgThread:public QThread
{
    Q_OBJECT
public:
    ZDbgThread(qintptr handle);
public slots:
    void ZSloDumpDbgMsg(QString msg);
protected:
    void run();
private:
    QTcpSocket *m_socket;
    qintptr m_handle;
};
#endif // ZDBGSERVER_H
