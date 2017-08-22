#ifndef QSOAPENVELOPE_H
#define QSOAPENVELOPE_H

#include <QObject>
#include <QMultiMap>
#include <QHash>
#include "qsoapheader.h"
#include "qsoapbody.h"

class QSoapEnvelope
{
public:
    explicit QSoapEnvelope();
    QSoapEnvelope(QSoapEnvelope& other);
    //QSoapEnvelope& operator=(const QSoapEnvelope &other);

    QString GetNameSpace(QString uri);
    void AddNameSpace(QString uri,QString ns);

#ifndef QT_NO_TEXTSTREAM
    friend QTextStream &operator<<(QTextStream & , const QSoapEnvelope &);
    friend QTextStream &operator>>(QTextStream & , QSoapEnvelope &);
#endif

public:
    QMap<QString,QString>   attributes; //属性
    QMap<QString,QString>   nameSpace; //属性url,namespace

    QSoapHeader header;
    QSoapBody body;
};

#endif // QSOAPENVELOPE_H
