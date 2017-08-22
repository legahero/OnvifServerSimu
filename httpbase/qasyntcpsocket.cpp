#include "qasyntcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

QAsynTcpSocket::QAsynTcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor),
       m_transmitLen(0),
      m_transmitPos(0)
{
    this->setSocketDescriptor(socketDescriptor);

    //connect(this, SIGNAL(readyRead()), this, SLOT(doParseRequest()));
    connect(this, SIGNAL(disconnected()), this, SLOT(doDisconnected()));

    //当所有的字节写完updateWriteCount激发allBytesWritten信号
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(doUpdateWriteCount(qint64)));


}

QAsynTcpSocket::~QAsynTcpSocket()
{
    qDebug() << "~QAsynTcpSocket" ;
}

void QAsynTcpSocket::doDisconnected()
{
    emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
    deleteLater();
}


void QAsynTcpSocket::doUpdateWriteCount(qint64 count)
{
    Q_ASSERT(m_transmitPos + count <= m_transmitLen);

    m_transmitPos += count;

    if (m_transmitPos == m_transmitLen)
    {
        m_transmitLen = 0;
        m_transmitPos = 0;
        Q_EMIT allBytesWritten();
    }
}

void QAsynTcpSocket::write(const QByteArray &data)
{
    this->write(data);
    m_transmitLen += data.size();
}

void QAsynTcpSocket::waitForBytesWritten()
{
    QTcpSocket::waitForBytesWritten();
}
