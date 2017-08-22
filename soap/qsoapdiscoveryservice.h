#ifndef QSOAPDISCOVERYSERVICE_H
#define QSOAPDISCOVERYSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QUuid>



class QSoapDiscoveryService : public QObject
{
    Q_OBJECT
public:
    explicit QSoapDiscoveryService(QObject *parent = 0);
    ~QSoapDiscoveryService();

    bool Start();

    //srever ->client
    void Hello();

    //srever ->client
    void ProbeMatches(QString MessageID,QHostAddress to,quint16 toPort);

    void ResolveMatches(QString MessageID,QHostAddress to,quint16 toPort);

    //srever ->client
    void Bye();

    void SetDevId(QString devid){ dev_id=devid;};
    QString GetDevId(){return dev_id;};

    void SetDevUrl(QString devurl){ dev_url=devurl;};
    QString GetDevUrl(){return dev_url;};

signals:
    void OnProbe(QHostAddress from,quint16 fromPort,QString MessageID);
    void OnResolve(QHostAddress from,quint16 fromPort,QString MessageID);
public slots:
    void readPendingDatagrams();

private:
    QUdpSocket* udpSocket;

    QString dev_id;//server 设备ID
    int MessageNumber;
    QString dev_url; //设备服务
};

#endif // QSOAPDISCOVERYSERVICE_H
