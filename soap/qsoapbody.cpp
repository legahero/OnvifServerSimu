#include "qsoapbody.h"

QSoapBody::QSoapBody():QSoapElement()
{
    tagName="SOAP-ENV:Body";
}

QSoapBody::QSoapBody(QSoapBody&other):QSoapElement(other)
{

}
/*
QSoapBody& QSoapBody::operator =(const QSoapBody& other)
{
    QSoapBody(other);
    return *this;
}
*/

#ifndef QT_NO_TEXTSTREAM
//! 重载操作符<<的实现
QTextStream &operator<<(QTextStream &output , const QSoapBody & var)
{
    output <<(QSoapElement&)var;
    return output ;
}

//! 重载操作符>>的实现
QTextStream &operator>>(QTextStream & input, QSoapBody & var)
{
    input>>(QSoapElement&)var;
    return input ;
}
#endif
