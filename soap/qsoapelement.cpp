// Copyright (c) 2017-2017 legahero.  All rights reserved.
// write by legahero QQ：502706647
#include "qsoapelement.h"
#include <QDebug>

QSoapElement::QSoapElement()
{

}
QSoapElement::QSoapElement(const QSoapElement&other)
{
    attributes = other.attributes;
    nameSpace=other.nameSpace;
    value = other.value;
    childs = other.childs;
    childsmap=other.childsmap;
    tagName=other.tagName;
}
/*
QSoapElement& QSoapElement::operator =(const QSoapElement& other)
{
    attributes = other.attributes;
    nameSpace=other.nameSpace;
    value = other.value;
    childs = other.childs;
    childsmap=other.childsmap;

    return *this;
}
*/
QString QSoapElement::GetNameSpace(QString uri)
{
    if(nameSpace.contains(uri))
        return nameSpace[uri];
    else
        return "";
}
void QSoapElement::AddNameSpace(QString uri,QString ns)
{
    nameSpace.insert(uri,ns);
    if(ns.length()>0)
        attributes.insert("xmlns:"+ns,uri);
    else
        attributes.insert("xmlns",uri);
}

QString  QSoapElement::Attributes(QString key)
{
    if(attributes.contains(key))
        return attributes[key];
    else
        return "";
}

 QMap<QString,QString>& QSoapElement::Attributes()
 {
     return attributes;
 }

QSoapElement & QSoapElement::operator[](int i)
{
    return childs[i];
}
QSoapElement NullElement;
QSoapElement & QSoapElement::operator[](QString key)
{
    int i=childsmap[key];
    if(i>=0)
    {
        QSoapElement &tmp=childs[i];
        return tmp;
    }else
    {
        return NullElement;
    }
}

const QSoapElement & QSoapElement::operator[](int i) const
{
    const QSoapElement &tmp=childs[i];
    return  tmp;
}

void QSoapElement::AppendChild(const QSoapElement &t)
{
    childs.append(t);
    childsmap[t.tagName]=childs.size()-1;
}
void QSoapElement::AppendChild(QString tag,QString value)
{
    QSoapElement child;
    child.tagName=tag;
    child.value=value;

    AppendChild(child);
}

int QSoapElement::ChildCount()
{
    return childs.size();
}

#ifndef QT_NO_TEXTSTREAM
//! 重载操作符<<的实现
QTextStream &operator<<(QTextStream &output , const QSoapElement & var)
{
    output <<"<"<<var.tagName;
    QMultiMap<QString,QString>::const_iterator i;
    for (i = var.attributes.constBegin(); i != var.attributes.constEnd(); ++i) {
        output<<" "<<i.key()<<"=\""<<i.value()<<"\"";
    }
    output <<">";
    output << var.value ;

    for(int i=0;i<var.childs.size();i++)
    {
        output<<var.childs[i];
    }

    output <<"</"<<var.tagName<<">";
    return output ;
}

//! 重载操作符>>的实现
QTextStream &operator>>(QTextStream & input, QSoapElement & item)
{
    //input >> item.attributes >> item.envelope ;
    return input ;
}

QDomElement &operator>>(QDomElement & input, QSoapElement & var)
{
    var.tagName=input.tagName();

    QDomNamedNodeMap h_att=input.attributes();
    for(int i=0;i<h_att.count();i++)
    {
        QDomNode node=h_att.item(i);
        //qDebug()<< node.nodeName()<<"--" <<node.nodeValue();
        QString nodeName=node.nodeName();
        var.attributes.insert(nodeName,node.nodeValue());
        if(nodeName.contains("xmlns"))
        {
            QStringList ns=nodeName.split(':');
            if(ns.length()>1)
                var.nameSpace.insert(node.nodeValue(),ns[1]);
        }
    }
    //qDebug()<<"  node:"<< input.nodeName()<<"type:" <<input.nodeType()<<" hasChildNodes:"<<input.hasChildNodes();
    QDomNode fc_node=input.firstChild();
    if(fc_node.isCharacterData())
    {
        var.value=fc_node.nodeValue();
    }
    //qDebug()<<"  tagName:"<< fc_node.nodeName()<<" value:" <<fc_node.nodeValue()<<" nodeType:" <<fc_node.nodeType();
    QDomElement fc_Element=input.firstChildElement();
    while(!fc_Element.isNull())
    {
        QSoapElement soapElement;
        fc_Element>>soapElement;
        var.AppendChild(soapElement);

        fc_Element = fc_Element.nextSiblingElement();
    }

    //input=input.nextSiblingElement();
    return input ;
}
#endif
