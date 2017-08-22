#include "httphandler.h"

QHttpHandler::QHttpHandler(QObject* parent)
    : QObject(parent)
{}

QHttpHandler::~QHttpHandler()
{}

void QHttpHandler::Handle(QHttpRequest& request, QHttpResponse& response)
{
    qCritical("HttpRequestHandler: you need to override the service() function");
    //qDebug("HttpRequestHandler: request=%s %s %s",request.getMethod().data(),request.getPath().data(),request.getVersion().data());
    //response.setStatus(501,"not implemented");
    response.writeHead(501);
    //response.write("501 not implemented",true);
}
