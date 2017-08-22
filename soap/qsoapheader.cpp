#include "qsoapheader.h"

QSoapHeader::QSoapHeader() :QSoapElement()
{
    tagName="SOAP-ENV:Header";
}

QSoapHeader::QSoapHeader(QSoapHeader&other):QSoapElement(other)
{

}
/*
QSoapHeader& QSoapHeader::operator =(const QSoapHeader& other)
{
    QSoapHeader(other);
    return *this;
}
*/
#ifndef QT_NO_DATASTREAM
//! 重载操作符<<的实现
QTextStream &operator<<(QTextStream &output , const QSoapHeader & var)
{
    output <<(QSoapElement&)var;
    return output ;
}

//! 重载操作符>>的实现
QTextStream &operator>>(QTextStream & input, QSoapHeader & var)
{
    input>>(QSoapElement&)var;
    return input ;
}
#endif
