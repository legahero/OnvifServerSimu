#ifndef QSOAPMESSAGE_H
#define QSOAPMESSAGE_H

#include <QObject>
#include "qsoapenvelope.h"


class QSoapMessage //: public QObject
{
    //Q_OBJECT
public:
    //explicit QSoapMessage(QObject *parent = 0);
    QSoapMessage();
    QSoapMessage(QSoapMessage&other);
    //QSoapMessage& operator=(const QSoapMessage &other);

#ifndef QT_NO_TEXTSTREAM
    friend QTextStream &operator<<(QTextStream & , const QSoapMessage &);
    friend QTextStream &operator>>(QTextStream & , QSoapMessage &);
#endif
    /*
signals:

public slots:
*/
public:
    QMap<QString,QString>   attributes; //属性
    //QList<QString,QString>   attributes; //属性
    QSoapEnvelope   envelope;
};

#endif // QSOAPMESSAGE_H
