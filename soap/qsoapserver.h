//legahero  QQ:1395449850
#ifndef Q_SOAP_SERVER
#define Q_SOAP_SERVER

#define QSOAPSERVER_VERSION_MAJOR 0
#define QSOAPSERVER_VERSION_MINOR 1
#define QSOAPSERVER_VERSION_PATCH 0


#include <QObject>
#include <QtNetwork>

#include "httpbase/qasyntcpserver.h"
#include "httpbase/qhttpserverfwd.h"
#include "httpbase/qmultidbmanager.h"
#include "qonvifdevice.h"
#include "qonvifmedia.h"
#include "qonvifanalyticsdevice.h"
#include "qonvifptz.h"

class QSoapServer : public QAsynTcpServer
{
    Q_OBJECT
public:    
    QSoapServer(QObject *parent = 0,int numConnections=1000);

    virtual ~QSoapServer();

    QString searchConfigFile();

private Q_SLOTS:
    void handleRequest(QHttpRequest *request, QHttpResponse *response);

protected slots:
    void sockDisConnectSlot(int handle,const QString & ip, quint16 prot, QThread *th);//断开连接的用户信息

protected:
    void incomingConnection(qintptr socketDescriptor);//覆盖已获取多线程

public:
    QMultiDbManager m_mdbmng;
    QOnvifDevice m_dev;
    QOnvifMedia m_media;
    QOnvifAnalyticsdevice m_AnaDev;//NVA
    QOnvifPtz m_ptz;

    //鉴权
    QString AuthMode;
    QString Qop;
    QString Realm;
    QString Nonce;
    QString AuthDate;

    QString UserName;
    QString Password;
};

#endif
