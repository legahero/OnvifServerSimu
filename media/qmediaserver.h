#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include <QObject>
#include <QString>
#include <QThread>

class UsageEnvironment;
class RTSPServer;

class QMediaServer : public QThread
{
    Q_OBJECT
public:
    explicit QMediaServer(QObject *parent = 0);
    //QMediaServer();
    ~QMediaServer();

    int Start(QString appname,int port=544);
protected:
    virtual void run() Q_DECL_OVERRIDE;
signals:

public slots:

private:
    RTSPServer* rtspServer;
    UsageEnvironment* env;
    bool reuseFirstSource ;
    QString appName;
    int appPort;
};

#endif // MEDIASERVER_H
