#ifndef QONVIFMEDIA_H
#define QONVIFMEDIA_H
#include "qsoapmessage.h"

class QOnvifMedia
{
public:
    QOnvifMedia();

    //server
    QSoapMessage OnGetProfiles(QSoapMessage& req);
    QSoapMessage OnGetStreamUri(QSoapMessage& req);
    QSoapMessage OnGetSnapshotUri(QSoapMessage& req);
    QSoapMessage OnGetVideoEncoderConfigurations(QSoapMessage& req);
    QSoapMessage OnGetVideoEncoderConfigurationOptions(QSoapMessage& req);
    QSoapMessage OnGetVideoSources(QSoapMessage& req);
    QSoapMessage OnGetAudioSources(QSoapMessage& req);

    //client
    QSoapMessage packGetStreamUri(QString Stream,QString Protocol,QString ProfileToken);
public:
    QString StreamUri;
    QString SnapshotUri;
};

#endif // QONVIFMEDIA_H
