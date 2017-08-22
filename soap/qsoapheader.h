#ifndef QSOAPHEADER_H
#define QSOAPHEADER_H
#include <QString>
#include <QHash>
#include "qsoapelement.h"

class QSoapHeader : public QSoapElement
{
public:
    explicit QSoapHeader();
    QSoapHeader(QSoapHeader&other);
    /*
    QSoapHeader& operator=(const QSoapHeader &other);
    */

#ifndef QT_NO_TEXTSTREAM
    friend QTextStream &operator<<(QTextStream & , const QSoapHeader &);
    friend QTextStream &operator>>(QTextStream & , QSoapHeader &);

#endif

};

#endif // QSOAPHEADER_H
