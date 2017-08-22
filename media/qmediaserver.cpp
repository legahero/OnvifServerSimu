#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "H264LiveVideoServerMediaSubsession.h"
#include "qmediaserver.h"
#include <QDebug>

//QMediaServer::QMediaServer(QObject *parent) : QObject(parent)
QMediaServer::QMediaServer(QObject *parent): QThread(parent)
{
    reuseFirstSource = false;
    rtspServer=NULL;
    env=NULL;
    appName="live";
}
QMediaServer::~QMediaServer()
{
    if (rtspServer != NULL)
        rtspServer->close(*env,appName.toStdString().c_str());

    appName="";
    appPort=0;
}

//port 544
int QMediaServer::Start(QString app_name,int port)
{
    appName=app_name;
    appPort=port;
    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    env = BasicUsageEnvironment::createNew(*scheduler);

    UserAuthenticationDatabase* authDB = NULL;
    // Create the RTSP server:
    rtspServer = RTSPServer::createNew(*env, 554, authDB);
    if (rtspServer == NULL) {
        *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
        return 1;
    }
    OutPacketBuffer::maxSize = 128800;
    qDebug()<<"rtsp Server started";

    char const* descriptionString = "Session streamed by \"testOnDemandRTSPServer\"";

    // Set up each of the possible streams that can be served by the
    // RTSP server.  Each such stream is implemented using a
    // "ServerMediaSession" object, plus one or more
    // "ServerMediaSubsession" objects for each audio/video substream.

    // A H.264 video elementary stream:

    if(appName.length()<1)
        appName="live";
    const char *streamName=appName.toStdString().c_str();

    ServerMediaSession* sms = ServerMediaSession::createNew(*env,streamName , streamName,
                      descriptionString);
    sms->addSubsession(H264LiveVideoServerMediaSubsession::createNew(*env, reuseFirstSource));
    rtspServer->addServerMediaSession(sms);


    char* url = rtspServer->rtspURL(sms);
    //UsageEnvironment& env1 = rtspServer->envir();
    *env << "Play using the URL \"" << url << "\"\n";
    qDebug()<< "Play using the URL \"" << url << "\"\n";
    delete[] url;

    // Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
    // Try first with the default HTTP port (80), and then with the alternative HTTP
    // port numbers (8000 and 8080).
    if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
    //if (rtspServer->setUpTunnelingOverHTTP(8000)){
        *env << "\n(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)\n";
    } else {
        *env << "\n(RTSP-over-HTTP tunneling is not available.)\n";
        qDebug()<<"RTSP-over-HTTP tunneling is not available";
    }

    this->start();

    return 0; // only to prevent compiler warning
}

void QMediaServer::run()
{
    if(env!=NULL)
        env->taskScheduler().doEventLoop(); // does not return
}
