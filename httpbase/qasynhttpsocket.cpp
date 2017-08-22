#include "qasynhttpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

#include "qhttprequest.h"
#include "qhttpresponse.h"

QAsynHttpSocket::QAsynHttpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor),
      m_parser(0),
      m_parserSettings(0),
      m_transmitLen(0),
      m_transmitPos(0)
{
    this->setSocketDescriptor(socketDescriptor);

    m_parser = (http_parser *)malloc(sizeof(http_parser));
    http_parser_init(m_parser, HTTP_REQUEST);

    m_parserSettings = new http_parser_settings();

    m_parserSettings->on_message_begin = doMessageBegin;
    m_parserSettings->on_message_complete = doMessageComplete;

    m_parserSettings->on_header_field = doHeaderField;
    m_parserSettings->on_header_value = doHeaderValue;
    //在HeadersComplete完成时激发newRequest
    m_parserSettings->on_headers_complete = doHeadersComplete;
    m_parserSettings->on_body = doBody;

    m_parserSettings->on_status = doStatus;
    m_parserSettings->on_url = doUrl;

    m_parserSettings->on_chunk_header = doChunkHeader;
    m_parserSettings->on_chunk_complete = doChunkComplete;


    m_parser->data = this;

    connect(this, SIGNAL(readyRead()), this, SLOT(doParseRequest()));
    connect(this, SIGNAL(disconnected()), this, SLOT(doDisconnected()));

    //当所有的字节写完updateWriteCount激发allBytesWritten信号
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(doUpdateWriteCount(qint64)));

    qDebug() << "new connect:"<<socketDescriptor ;
}

QAsynHttpSocket::QAsynHttpSocket( QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(0),
      m_parser(0),
      m_parserSettings(0),
      m_transmitLen(0),
      m_transmitPos(0)
{
    m_parser = (http_parser *)malloc(sizeof(http_parser));
    http_parser_init(m_parser, HTTP_REQUEST);

    m_parserSettings = new http_parser_settings();

    m_parserSettings->on_message_begin = doMessageBegin;
    m_parserSettings->on_message_complete = doMessageComplete;

    m_parserSettings->on_header_field = doHeaderField;
    m_parserSettings->on_header_value = doHeaderValue;
    //在HeadersComplete完成时激发newRequest
    m_parserSettings->on_headers_complete = doHeadersComplete;
    m_parserSettings->on_body = doBody;

    m_parserSettings->on_status = doStatus;
    m_parserSettings->on_url = doUrl;

    m_parserSettings->on_chunk_header = doChunkHeader;
    m_parserSettings->on_chunk_complete = doChunkComplete;


    m_parser->data = this;

    connect(this, SIGNAL(readyRead()), this, SLOT(doParseRequest()));
    connect(this, SIGNAL(disconnected()), this, SLOT(doDisconnected()));

    //当所有的字节写完updateWriteCount激发allBytesWritten信号
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(doUpdateWriteCount(qint64)));
}

QAsynHttpSocket::~QAsynHttpSocket()
{
    free(m_parser);
    m_parser = 0;

    delete m_parserSettings;
    m_parserSettings = 0;

    qDebug() << "QAsynHttpSocket::~QAsynHttpSocket" ;

}

void QAsynHttpSocket::doDisconnected()
{
    qDebug() << "QAsynHttpSocket::doDisconnected";

    emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
    deleteLater();    
}

/*
void QAsynHttpSocket::flush()
{
    qDebug() << "QAsynHttpSocket::flush";
    QTcpSocket::flush();
}
*/

void QAsynHttpSocket::waitForBytesWritten()
{
    QTcpSocket::waitForBytesWritten();
}

void QAsynHttpSocket::doUpdateWriteCount(qint64 count)
{
    qDebug() << "QAsynHttpSocket::doUpdateWriteCount,ThreadId:"<<QThread::currentThreadId()  ;

    Q_ASSERT(m_transmitPos + count <= m_transmitLen);

    m_transmitPos += count;

    if (m_transmitPos == m_transmitLen)
    {
        m_transmitLen = 0;
        m_transmitPos = 0;
        Q_EMIT allBytesWritten();
    }
}

void QAsynHttpSocket::doParseRequest()
{
    Q_ASSERT(m_parser);

    qDebug() << "QHttpConnection:parseRequest,ThreadId:"<<QThread::currentThreadId()  ;

    while (this->bytesAvailable()) {

        QByteArray arr = this->readAll();
        qDebug() << "readAll:"<<arr  ;
        qDebug() << "http_parser_execute begin,ThreadId:"<<QThread::currentThreadId()  ;
        http_parser_execute(m_parser, m_parserSettings, arr.constData(), arr.size());

        qDebug() << "http_parser_execute end,ThreadId:"<<QThread::currentThreadId()  ;
    }
}

void QAsynHttpSocket::write(const QByteArray &data)
{
    QTcpSocket::write(data);
    m_transmitLen += data.size();
}



/* URL Utilities */
#define HAS_URL_FIELD(info, field) (info.field_set &(1 << (field)))

#define GET_FIELD(data, info, field)                                                               \
    QString::fromLatin1(data + info.field_data[field].off, info.field_data[field].len)

#define CHECK_AND_GET_FIELD(data, info, field)                                                     \
    (HAS_URL_FIELD(info, field) ? GET_FIELD(data, info, field) : QString())

QUrl createUrl(const char *urlData, const http_parser_url &urlInfo)
{
    QUrl url;
    url.setScheme(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_SCHEMA));
    url.setHost(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_HOST));
    // Port is dealt with separately since it is available as an integer.
    url.setPath(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_PATH), QUrl::TolerantMode);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    url.setQuery(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_QUERY));
#else
    if (HAS_URL_FIELD(urlInfo, UF_QUERY)) {
        url.setEncodedQuery(QByteArray(urlData + urlInfo.field_data[UF_QUERY].off,
                                       urlInfo.field_data[UF_QUERY].len));
    }
#endif
    url.setFragment(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_FRAGMENT));
    url.setUserInfo(CHECK_AND_GET_FIELD(urlData, urlInfo, UF_USERINFO));

    if (HAS_URL_FIELD(urlInfo, UF_PORT))
        url.setPort(urlInfo.port);

    return url;
}

#undef CHECK_AND_SET_FIELD
#undef GET_FIELD
#undef HAS_URL_FIELD

/********************
 * Static Callbacks *
 *******************/

int QAsynHttpSocket::doMessageBegin(http_parser *parser)
{
    qDebug() << "MessageBegin" ;

    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);
    theConnection->m_currentHeaders.clear();
    theConnection->m_currentUrl.clear();
    theConnection->m_currentUrl.reserve(128);

    qDebug() << "QHttpConnection:MessageBegin,new QHttpRequest,ThreadId:"<<QThread::currentThreadId()  ;

    return 0;
}

int QAsynHttpSocket::doHeadersComplete(http_parser *parser)
{
    qDebug() << "QHttpConnection:HeadersComplete begin,ThreadId:"<<QThread::currentThreadId()  ;

    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    // Insert last remaining header
    theConnection->m_currentHeaders[theConnection->m_currentHeaderField.toLower()] =
        theConnection->m_currentHeaderValue;

    qDebug() << "HeadersComplete end" ;

    return 0;
}

int QAsynHttpSocket::doMessageComplete(http_parser *parser)
{
    qDebug() << "doMessageComplete" ;

    // TODO: do cleanup and prepare for next request
    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);


    QHttpRequest* request = new QHttpRequest(theConnection);    

    /** set method **/
    request->setMethod(static_cast<QHttpRequest::HttpMethod>(parser->method));

    /** set version **/
    request->setVersion(
        QString("%1.%2").arg(parser->http_major).arg(parser->http_minor));

    /** get parsed url **/
    struct http_parser_url urlInfo;
    int r = http_parser_parse_url(theConnection->m_currentUrl.constData(),
                                  theConnection->m_currentUrl.size(),
                                  parser->method == HTTP_CONNECT, &urlInfo);
    Q_ASSERT(r == 0);
    Q_UNUSED(r);

    request->setUrl(createUrl(theConnection->m_currentUrl.constData(), urlInfo));

    // Insert last remaining header,这个已经在doHeadersComplete中执行
    //theConnection->m_currentHeaders[theConnection->m_currentHeaderField.toLower()] =
    //   theConnection->m_currentHeaderValue;
    request->setHeaders(theConnection->m_currentHeaders);

    /** set client information **/
    request->m_remoteAddress = theConnection->peerAddress().toString();
    request->m_remotePort = theConnection->peerPort();

    //request->storeBody();//连接data信号到appendBody槽
    request->appendBody(theConnection->m_currentBody);

    qDebug() << "QHttpResponse:new,ThreadId:"<<QThread::currentThreadId()  ;
    QHttpResponse *response = new QHttpResponse(theConnection);
    if (parser->http_major < 1 || parser->http_minor < 1)
        response->m_keepAlive = false;

    request->setSuccessful(true);

    Q_EMIT theConnection->newRequest(request, response);

    return 0;
}

int QAsynHttpSocket::doUrl(http_parser *parser, const char *at, size_t length)
{
    qDebug() << "doUrl" ;

    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    theConnection->m_currentUrl.append(at, length);
    return 0;
}

int QAsynHttpSocket::doStatus(http_parser *parser, const char *at, size_t length)
{
    qDebug() << "QAsynHttpSocket::doStatus" ;

    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    theConnection->m_currentStatus.append(at, length);
    return 0;
}

int QAsynHttpSocket::doHeaderField(http_parser *parser, const char *at, size_t length)
{
    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    // insert the header we parsed previously
    // into the header map
    if (!theConnection->m_currentHeaderField.isEmpty() &&
        !theConnection->m_currentHeaderValue.isEmpty()) {
        // header names are always lower-cased
        theConnection->m_currentHeaders[theConnection->m_currentHeaderField.toLower()] =
            theConnection->m_currentHeaderValue;
        // clear header value. this sets up a nice
        // feedback loop where the next time
        // HeaderValue is called, it can simply append
        theConnection->m_currentHeaderField = QString();
        theConnection->m_currentHeaderValue = QString();
    }

    QString fieldSuffix = QString::fromLatin1(at, length);
    theConnection->m_currentHeaderField += fieldSuffix;
    return 0;
}

int QAsynHttpSocket::doHeaderValue(http_parser *parser, const char *at, size_t length)
{
    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    QString valueSuffix = QString::fromLatin1(at, length);
    theConnection->m_currentHeaderValue += valueSuffix;
    return 0;
}

int QAsynHttpSocket::doBody(http_parser *parser, const char *at, size_t length)
{
    qDebug() << "QAsynHttpSocket::doBody" ;

    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    theConnection->m_currentBody.append(at, length);

    qDebug() << "QAsynHttpSocket::doBody"<<theConnection->m_currentBody ;
    return 0;
}

int QAsynHttpSocket::doChunkHeader(http_parser *parser)
{
    qDebug() << "doChunkHeader" ;

    // TODO: do cleanup and prepare for next request
    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    //parser->
    return 0;
}

int QAsynHttpSocket::doChunkComplete(http_parser *parser)
{
    qDebug() << "doChunkComplete" ;

    // TODO: do cleanup and prepare for next request
    QAsynHttpSocket *theConnection = static_cast<QAsynHttpSocket *>(parser->data);

    return 0;
}
/// @endcond


