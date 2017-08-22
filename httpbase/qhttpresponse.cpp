/*
 * Copyright 2011-2014 Nikhil Marathe <nsm.nikhil@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "qhttpresponse.h"
#include "qasynhttpsocket.h"

#include <QDateTime>
#include <QLocale>
#include <QThread>

QHash<int, QString> STATUS_CODES;

QHttpResponse::QHttpResponse(QAsynHttpSocket *connection)
    // TODO: parent child relation
    : QObject(0),
      m_connection(connection),
      m_headerWritten(false),
      m_sentConnectionHeader(false),
      m_sentContentLengthHeader(false),
      m_sentTransferEncodingHeader(false),
      m_sentDate(false),
      m_keepAlive(true),
      m_last(false),
      m_useChunkedEncoding(false),
      m_finished(false)
{
   connect(m_connection, SIGNAL(allBytesWritten()), this, SIGNAL(allBytesWritten()));

#define STATUS_CODE1(num, reason) STATUS_CODES.insert(num, reason);
   if(STATUS_CODES.size()<1)
   {
    // {{{
    STATUS_CODE1(100, "Continue")
    STATUS_CODE1(101, "Switching Protocols")
    STATUS_CODE1(102, "Processing") // RFC 2518) obsoleted by RFC 4918
    STATUS_CODE1(200, "OK")
    STATUS_CODE1(201, "Created")
    STATUS_CODE1(202, "Accepted")
    STATUS_CODE1(203, "Non-Authoritative Information")
    STATUS_CODE1(204, "No Content")
    STATUS_CODE1(205, "Reset Content")
    STATUS_CODE1(206, "Partial Content")
    STATUS_CODE1(207, "Multi-Status") // RFC 4918
    STATUS_CODE1(300, "Multiple Choices")
    STATUS_CODE1(301, "Moved Permanently")
    STATUS_CODE1(302, "Moved Temporarily")
    STATUS_CODE1(303, "See Other")
    STATUS_CODE1(304, "Not Modified")
    STATUS_CODE1(305, "Use Proxy")
    STATUS_CODE1(307, "Temporary Redirect")
    STATUS_CODE1(400, "Bad Request")
    STATUS_CODE1(401, "Unauthorized")
    STATUS_CODE1(402, "Payment Required")
    STATUS_CODE1(403, "Forbidden")
    STATUS_CODE1(404, "Not Found")
    STATUS_CODE1(405, "Method Not Allowed")
    STATUS_CODE1(406, "Not Acceptable")
    STATUS_CODE1(407, "Proxy Authentication Required")
    STATUS_CODE1(408, "Request Time-out")
    STATUS_CODE1(409, "Conflict")
    STATUS_CODE1(410, "Gone")
    STATUS_CODE1(411, "Length Required")
    STATUS_CODE1(412, "Precondition Failed")
    STATUS_CODE1(413, "Request Entity Too Large")
    STATUS_CODE1(414, "Request-URI Too Large")
    STATUS_CODE1(415, "Unsupported Media Type")
    STATUS_CODE1(416, "Requested Range Not Satisfiable")
    STATUS_CODE1(417, "Expectation Failed")
    STATUS_CODE1(418, "I\"m a teapot")        // RFC 2324
    STATUS_CODE1(422, "Unprocessable Entity") // RFC 4918
    STATUS_CODE1(423, "Locked")               // RFC 4918
    STATUS_CODE1(424, "Failed Dependency")    // RFC 4918
    STATUS_CODE1(425, "Unordered Collection") // RFC 4918
    STATUS_CODE1(426, "Upgrade Required")     // RFC 2817
    STATUS_CODE1(500, "Internal Server Error")
    STATUS_CODE1(501, "Not Implemented")
    STATUS_CODE1(502, "Bad Gateway")
    STATUS_CODE1(503, "Service Unavailable")
    STATUS_CODE1(504, "Gateway Time-out")
    STATUS_CODE1(505, "HTTP Version not supported")
    STATUS_CODE1(506, "Variant Also Negotiates") // RFC 2295
    STATUS_CODE1(507, "Insufficient Storage")    // RFC 4918
    STATUS_CODE1(509, "Bandwidth Limit Exceeded")
    STATUS_CODE1(510, "Not Extended") // RFC 2774
    // }}}
   }
}

QHttpResponse::~QHttpResponse()
{
    qDebug() << "QHttpResponse::~QHttpResponse";
}

void QHttpResponse::setHeader(const QString &field, const QString &value)
{
    if (!m_finished)
        m_headers[field] = value;
    else
        qWarning() << "QHttpResponse::setHeader() Cannot set headers after response has finished.";
}

void QHttpResponse::writeHeader(const char *field, const QString &value)
{
    QString szfield=QString(field)+": "+value+"\r\n";
    qDebug() << szfield;

    if (!m_finished) {
        m_connection->write(field);
        m_connection->write(": ");
        m_connection->write(value.toUtf8());
        m_connection->write("\r\n");
    } else
        qWarning()
            << "QHttpResponse::writeHeader() Cannot write headers after response has finished.";
}

void QHttpResponse::writeHeaders()
{
    if (m_finished)
        return;

    foreach(const QString & name, m_headers.keys()) {
        QString value = m_headers[name];
        if (name.compare("connection", Qt::CaseInsensitive) == 0) {
            m_sentConnectionHeader = true;
            if (value.compare("close", Qt::CaseInsensitive) == 0)
                m_last = true;
            else
                m_keepAlive = true;
        } else if (name.compare("transfer-encoding", Qt::CaseInsensitive) == 0) {
            m_sentTransferEncodingHeader = true;
            if (value.compare("chunked", Qt::CaseInsensitive) == 0)
                m_useChunkedEncoding = true;
        } else if (name.compare("content-length", Qt::CaseInsensitive) == 0)
            m_sentContentLengthHeader = true;
        else if (name.compare("date", Qt::CaseInsensitive) == 0)
            m_sentDate = true;

        /// @todo Expect case (??)

        writeHeader(name.toLatin1(), value.toLatin1());
    }

    if (!m_sentConnectionHeader) {
        if (m_keepAlive && (m_sentContentLengthHeader || m_useChunkedEncoding)) {
            writeHeader("Connection", "keep-alive");
        } else {
            m_last = true;
            writeHeader("Connection", "close");
        }
    }

    if (!m_sentContentLengthHeader && !m_sentTransferEncodingHeader) {
        if (m_useChunkedEncoding)
            writeHeader("Transfer-Encoding", "chunked");
        else
            m_last = true;
    }

    // Sun, 06 Nov 1994 08:49:37 GMT - RFC 822. Use QLocale::c() so english is used for month and
    // day.
    if (!m_sentDate)
        writeHeader("Date",
                    QLocale::c().toString(QDateTime::currentDateTimeUtc(),
                                          "ddd, dd MMM yyyy hh:mm:ss") + " GMT");
}

void QHttpResponse::writeHead(int status)
{
    //qDebug() << "QHttpResponse:writeHead,ThreadId:"<<QThread::currentThreadId()  ;

    if (m_finished) {
        qWarning()
            << "QHttpResponse::writeHead() Cannot write headers after response has finished.";
        return;
    }

    if (m_headerWritten) {
        qWarning() << "QHttpResponse::writeHead() Already called once for this response.";
        return;
    }
    QString tmpdata=QString("HTTP/1.1 %1 %2\r\n").arg(status).arg(STATUS_CODES[status]);
    qDebug() << "head:"<<tmpdata;

    m_connection->write(
        QString("HTTP/1.1 %1 %2\r\n").arg(status).arg(STATUS_CODES[status]).toLatin1());
    writeHeaders();
    m_connection->write("\r\n");

    m_headerWritten = true;
}

void QHttpResponse::writeHead(StatusCode statusCode)
{
    writeHead(static_cast<int>(statusCode));
}

void QHttpResponse::write(const QByteArray &data)
{
    if (m_finished) {
        qWarning() << "QHttpResponse::write() Cannot write body after response has finished.";
        return;
    }

    if (!m_headerWritten) {
        qWarning() << "QHttpResponse::write() You must call writeHead() before writing body data.";
        return;
    }

    m_connection->write(data);
}

void QHttpResponse::flush()
{
    m_connection->flush();
}

void QHttpResponse::waitForBytesWritten()
{
    m_connection->waitForBytesWritten();
}

void QHttpResponse::end(const QByteArray &data)
{
    qDebug() << "QHttpResponse::end,ThreadId:"<<QThread::currentThreadId()  ;

    if (m_finished) {
        qWarning() << "QHttpResponse::end() Cannot write end after response has finished.";
        return;
    }

    if (data.size() > 0)
        write(data);
    m_finished = true;    

    Q_EMIT done();

}

void QHttpResponse::connectionClosed()
{
    qDebug() << "QHttpResponse::connectionClosed" ;

    m_finished = true;
    Q_EMIT done();
    deleteLater();
}
