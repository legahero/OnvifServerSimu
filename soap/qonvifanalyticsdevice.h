#ifndef QONVIFANALYTICSDEVICE_H
#define QONVIFANALYTICSDEVICE_H

#include "qsoapmessage.h"


class QOnvifAnalyticsdevice
{
public:
    QOnvifAnalyticsdevice();

    QSoapMessage OnGetAnalyticsEngines(QSoapMessage& req);

};

#endif // QONVIFANALYTICSDEVICE_H
