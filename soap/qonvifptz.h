#ifndef QONVIFPTZ_H
#define QONVIFPTZ_H

#include "qsoapmessage.h"

class QOnvifPtz
{
public:
    QOnvifPtz();

    QSoapMessage OnGetNodes(QSoapMessage& req);
};

#endif // QONVIFPTZ_H
