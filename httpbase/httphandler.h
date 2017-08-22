#ifndef QHTTPHANDLER_H
#define QHTTPHANDLER_H

#include <QCache>
#include <QMutex>

#include "qhttprequest.h"
#include "qhttpresponse.h"

class  QHttpHandler : public QObject {
    Q_OBJECT    
public:

    /**
     * Constructor.
     * @param parent Parent object.
     */
    QHttpHandler(QObject* parent=NULL);

    /** Destructor */
    virtual ~QHttpHandler();

    /**
      Generate a response for an incoming HTTP request.
      @param request The received HTTP request
      @param response Must be used to return the response
      @warning This method must be thread safe
    */
    virtual void Handle(QHttpRequest& request, QHttpResponse& response);

};

#endif // QHTTPHANDLER_H
