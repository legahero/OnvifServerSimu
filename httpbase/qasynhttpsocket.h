// legahero  QQ:1395449850,局部代码来自Nikhil Marathe
#ifndef QASYNHTTPSOCKET_H
#define QASYNHTTPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>

#include "http_parser.h"

#include "qhttpserverfwd.h"

/*
class QHttpRequest;
class QHttpResponse;
class QAsynHttpSocket;
*/

class QAsynHttpSocket: public QTcpSocket
{
    Q_OBJECT
public:
    explicit QAsynHttpSocket(qintptr socketDescriptor, QObject *parent = 0);
    explicit QAsynHttpSocket(QObject *parent = Q_NULLPTR);
    ~QAsynHttpSocket();

    void write(const QByteArray &data);
    //void flush();
    void waitForBytesWritten();
Q_SIGNALS:
    void newRequest(QHttpRequest *, QHttpResponse *);
    void allBytesWritten();
signals:    
    //NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);
private Q_SLOTS:
    void doParseRequest();
    void doDisconnected();
    void doUpdateWriteCount(qint64);

private:
    static int doMessageBegin(http_parser *parser);
    static int doMessageComplete(http_parser *parser);


    static int doHeaderField(http_parser *parser, const char *at, size_t length);
    static int doHeaderValue(http_parser *parser, const char *at, size_t length);
    static int doHeadersComplete(http_parser *parser);
    static int doBody(http_parser *parser, const char *at, size_t length);

    static int doUrl(http_parser *parser, const char *at, size_t length);
    static int doStatus(http_parser *parser, const char *at, size_t length);

    static int doChunkHeader(http_parser *parser);
    static int doChunkComplete(http_parser *parser);
private:
    qintptr socketID;

    http_parser *m_parser;
    http_parser_settings *m_parserSettings;


    //临时变量
    QByteArray m_currentUrl;
    // The ones we are reading in from the parser
    HeaderHash m_currentHeaders;
    QString m_currentHeaderField;
    QString m_currentHeaderValue;
    QByteArray m_currentBody;
    QByteArray m_currentStatus;

    // Keep track of transmit buffer status
    qint64 m_transmitLen;
    qint64 m_transmitPos;

private:
    Q_DISABLE_COPY(QAsynHttpSocket)
};

#endif // QASYNHTTPSOCKET_H
