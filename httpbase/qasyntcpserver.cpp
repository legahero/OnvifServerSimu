#include "qasyntcpserver.h"
#include "threadhandle.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"

QAsynTcpServer::QAsynTcpServer(QObject *parent,int numConnections) :
    QTcpServer(parent)
{
     m_ClientList = new  QHash<int,QTcpSocket *>;
     setMaxPendingConnections(numConnections);
}

QAsynTcpServer::~QAsynTcpServer()
{
    this->clear();
    delete m_ClientList;
}

void QAsynTcpServer::setMaxPendingConnections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections);//调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->maxConnections = numConnections;
}

void QAsynTcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    qDebug() << "QAsynTcpServer:incomingConnection" ;
    //继承重写此函数后，QQAsynTcpServer默认的判断最大连接数失效，自己实现
    if (m_ClientList->size() > maxPendingConnections())
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    auto th = ThreadHandle::getClass().getThread();
    auto tcpTemp = new QAsynTcpSocket(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();

    //NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(tcpTemp,&QAsynTcpSocket::sockDisConnect,this,
            &QAsynTcpServer::sockDisConnectSlot);

    //必须在QAsynTcpSocket的线程中执行

    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    m_ClientList->insert(socketDescriptor,tcpTemp);//插入到连接信息中

}

//释放线程资源
void QAsynTcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    m_ClientList->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了,释放数量
}


void QAsynTcpServer::clear()
{
    ThreadHandle::getClass().clear();//清除所有线程

    //切断所有连接，并且清除
    for (auto it = m_ClientList->begin(); it != m_ClientList->end(); ++it)
    {
        it.value()->disconnectFromHost();
    }
    m_ClientList->clear();
}
