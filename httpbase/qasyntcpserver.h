#ifndef QASYNTCPSERVER_H
#define QASYNTCPSERVER_H

#include <QTcpServer>
#include <QHash>
#include "qasyntcpsocket.h"


//继承QTCPSERVER以实现多线程TCPscoket的服务器。
class QAsynTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit QAsynTcpServer(QObject *parent = 0,int numConnections = 10000);
    ~QAsynTcpServer();

    void setMaxPendingConnections(int numConnections);//重写设置最大连接数函数
protected slots:
    void sockDisConnectSlot(int handle,const QString & ip, quint16 prot, QThread *th);//断开连接的用户信息
public:
    void clear(); //断开所有连接，线程计数器请0
protected:
    void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程

    QHash<int,QTcpSocket *> * m_ClientList;//管理连接的map
private:
    int maxConnections;

};

#endif // QASYNTCPSERVER_H
