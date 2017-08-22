#ifndef QASYNHTTPSOCKET_H
#define QASYNHTTPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QByteArray>

#include <QTime>


#include "http_parser.h"

class QAsynTcpSocket: public QTcpSocket
{
    Q_OBJECT
public:
    explicit QAsynTcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~QAsynTcpSocket();

    void write(const QByteArray &data);

    void waitForBytesWritten();
Q_SIGNALS:
    void allBytesWritten();
signals:
    //NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);

private Q_SLOTS:
    void doDisconnected();
    void doUpdateWriteCount(qint64);
private:
    qintptr socketID;

    // Keep track of transmit buffer status
    qint64 m_transmitLen;
    qint64 m_transmitPos;
};

#endif // QASYNHTTPSOCKET_H
