#ifndef QSOAPBODY_H
#define QSOAPBODY_H

#include <QString>
#include <QTextStream>
#include "qsoapelement.h"


class QSoapBody:public QSoapElement
{
public:
    QSoapBody();
    QSoapBody(QSoapBody&other);
    /*
    QSoapBody& operator=(const QSoapBody &other);
    */

#ifndef QT_NO_TEXTSTREAM
    friend QTextStream &operator<<(QTextStream & , const QSoapBody &);
    friend QTextStream &operator>>(QTextStream & , QSoapBody &);
#endif

};

#endif // QSOAPBODY_H
