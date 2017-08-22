#ifndef QSOAPELEMENT_H
#define QSOAPELEMENT_H

#include <QString>
#include <QHash>
#include <QTextStream>
#include <QDomElement>
#include <QVector>
//class QSoapElement;

class QSoapElement
{
public:
    QSoapElement();
    QSoapElement(const QSoapElement&other);
    //QSoapElement& operator=(const QSoapElement &other);

    QString GetNameSpace(QString uri);
    void AddNameSpace(QString uri,QString ns);
    QString Attributes(QString key);
    QMap<QString,QString>& Attributes();

    QSoapElement &operator[](int i);
    const QSoapElement &operator[](int i) const;

    QSoapElement &operator[](QString key);
    void AppendChild(const QSoapElement &t);
    void AppendChild(QString tag,QString value);

    int ChildCount();

#ifndef QT_NO_TEXTSTREAM
    friend QTextStream &operator<<(QTextStream & , const QSoapElement &);
    friend QTextStream &operator>>(QTextStream & , QSoapElement &);
    friend QDomElement &operator>>(QDomElement & , QSoapElement &);
#endif

private:
    QHash<QString,int>  childsmap;//子元素
    QList<QSoapElement> childs;

    QMap<QString,QString>   attributes; //属性
    QMap<QString,QString>   nameSpace; //属性url,namespace

public:
    QString value;

    QString tagName;
};

#endif // QSOAPELEMENT_H
