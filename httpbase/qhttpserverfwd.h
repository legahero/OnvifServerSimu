// legahero  QQ:1395449850,局部代码来自Nikhil Marathe


#ifndef Q_HTTP_SERVER_FWD
#define Q_HTTP_SERVER_FWD

#include <QHash>
#include <QString>

typedef QHash<QString, QString> HeaderHash;

// QHttpServer
class QHttpServer;
class QAsynHttpSocket;
class QHttpRequest;
class QHttpResponse;

// http_parser
struct http_parser_settings;
struct http_parser;

#endif
