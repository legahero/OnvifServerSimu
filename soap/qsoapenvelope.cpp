#include "qsoapenvelope.h"
#include <QMap>
#include <QMultiMap>
#include <QTextStream>

QSoapEnvelope::QSoapEnvelope()
{

}

QSoapEnvelope::QSoapEnvelope(QSoapEnvelope& other)
{
    if(other.attributes.size()>0)
        attributes = other.attributes;
    else
        attributes.clear();

    header = other.header;
    body = other.body;
}

/*
QSoapEnvelope& QSoapEnvelope::operator =(const QSoapEnvelope& other)
{
    attributes = other.attributes;
    header = other.header;
    body = other.body;
    return *this;
}
*/
QString QSoapEnvelope::GetNameSpace(QString uri)
{
    if(nameSpace.contains(uri))
        return nameSpace[uri];
    else
        return "";
}
void QSoapEnvelope::AddNameSpace(QString uri,QString ns)
{
    nameSpace.insert(uri,ns);
    attributes.insert("xmlns:"+ns,uri);
}

const char envelopetag_begin[]="<SOAP-ENV:Envelope";
const char envelopetag_end[]="</SOAP-ENV:Envelope>";

#ifndef QT_NO_DATASTREAM
//! 重载操作符<<的实现
QTextStream &operator<<(QTextStream &output , const QSoapEnvelope & var)
{
    output <<envelopetag_begin;
    QMultiMap<QString,QString>::const_iterator i;
    for (i = var.attributes.constBegin(); i != var.attributes.constEnd(); ++i) {
        output<<" "<<i.key()<<"=\""<<i.value()<<"\"";
    }
    output<<">";

    output<< var.header<<var.body;
    output <<envelopetag_end;
    return output ;
}

//! 重载操作符>>的实现
QTextStream &operator>>(QTextStream & input, QSoapEnvelope & item)
{
    //input >> item.attributes >> item.envelope ;
    return input ;
}
#endif
