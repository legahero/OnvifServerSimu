#ifndef Q_HTTP_RESPONSE
#define Q_HTTP_RESPONSE

#include <QObject>


//#include "qasynhttpsocket.h"
#include "qhttpserverfwd.h"


class  QHttpResponse : public QObject
{
    Q_OBJECT
public:
    /// HTTP status code.
    enum StatusCode {
        STATUS_CONTINUE = 100,
        STATUS_SWITCH_PROTOCOLS = 101,
        STATUS_OK = 200,
        STATUS_CREATED = 201,
        STATUS_ACCEPTED = 202,
        STATUS_NON_AUTHORITATIVE_INFORMATION = 203,
        STATUS_NO_CONTENT = 204,
        STATUS_RESET_CONTENT = 205,
        STATUS_PARTIAL_CONTENT = 206,
        STATUS_MULTIPLE_CHOICES = 300,
        STATUS_MOVED_PERMANENTLY = 301,
        STATUS_FOUND = 302,
        STATUS_SEE_OTHER = 303,
        STATUS_NOT_MODIFIED = 304,
        STATUS_USE_PROXY = 305,
        STATUS_TEMPORARY_REDIRECT = 307,
        STATUS_BAD_REQUEST = 400,
        STATUS_UNAUTHORIZED = 401,
        STATUS_PAYMENT_REQUIRED = 402,
        STATUS_FORBIDDEN = 403,
        STATUS_NOT_FOUND = 404,
        STATUS_METHOD_NOT_ALLOWED = 405,
        STATUS_NOT_ACCEPTABLE = 406,
        STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
        STATUS_REQUEST_TIMEOUT = 408,
        STATUS_CONFLICT = 409,
        STATUS_GONE = 410,
        STATUS_LENGTH_REQUIRED = 411,
        STATUS_PRECONDITION_FAILED = 412,
        STATUS_REQUEST_ENTITY_TOO_LARGE = 413,
        STATUS_REQUEST_URI_TOO_LONG = 414,
        STATUS_REQUEST_UNSUPPORTED_MEDIA_TYPE = 415,
        STATUS_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        STATUS_EXPECTATION_FAILED = 417,
        STATUS_INTERNAL_SERVER_ERROR = 500,
        STATUS_NOT_IMPLEMENTED = 501,
        STATUS_BAD_GATEWAY = 502,
        STATUS_SERVICE_UNAVAILABLE = 503,
        STATUS_GATEWAY_TIMEOUT = 504,
        STATUS_HTTP_VERSION_NOT_SUPPORTED = 505
    };

    virtual ~QHttpResponse();

    /// @cond nodoc
    friend class QAsynHttpSocket;
    /// @endcond

public Q_SLOTS:
    //设置相应头
    void setHeader(const QString &field, const QString &value);

    /// Writes the header section of the response
    /// using @c status as the response status code.
    /** @param statusCode Status code for the response.
        @note Any headers should be set before
        invoking this function with setHeader(). */
    void writeHead(int statusCode);

    /** @overload */
    void writeHead(StatusCode statusCode);

    /// Writes a block of @c data to the client.
    /** @note writeHead() must be called before this function. */
    void write(const QByteArray &data);

    /// Flushes the written data to the client.
    /** @note writeHead() must be called before this function. */
    void flush();

    /// Waiting for bytes to be written. See QAbstractSocket::waitForBytesWritten in the Qt documentation
    /** @note writeHead() must be called before this function. */
    void waitForBytesWritten();

    /// End/finish the response.
    /** Data will be flushed to the underlying socket
        and the connection itself will be closed if
        this is the last response.

        This will emit done() and queue this object
        for deletion. For details see \ref memorymanagement.
        @param data Optional data to be written before finishing. */
    void end(const QByteArray &data = "");

Q_SIGNALS:    
    void allBytesWritten();
    void done();

private:
    QHttpResponse(QAsynHttpSocket *connection);

    void writeHeaders();
    void writeHeader(const char *field, const QString &value);

    QAsynHttpSocket *m_connection;

    HeaderHash m_headers;

    bool m_headerWritten;
    bool m_sentConnectionHeader;
    bool m_sentContentLengthHeader;
    bool m_sentTransferEncodingHeader;
    bool m_sentDate;
    bool m_keepAlive;
    bool m_last;
    bool m_useChunkedEncoding;
    bool m_finished;

private Q_SLOTS:
    void connectionClosed();
};

#endif
