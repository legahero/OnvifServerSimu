#include "qsoapmessage.h"
#include <QDomDocument>
#include <QDebug>

//QSoapMessage::QSoapMessage(QObject *parent) : QObject(parent)
QSoapMessage::QSoapMessage()
{
    attributes.insert("version","1.0");
    attributes.insert("encoding","UTF-8");
}

QSoapMessage::QSoapMessage(QSoapMessage&other)
{
    if(other.attributes.size()>0)
        attributes = other.attributes;
    else
        attributes.clear();

    envelope = other.envelope;
}

/*
QSoapMessage& QSoapMessage::operator =(const QSoapMessage& other)
{
    if(!other.attributes.empty())
        attributes = other.attributes;
    else
        attributes.clear();

    envelope = other.envelope;
    return *this;
}
*/
const char xmltag_begin[]="<?xml ";
const char xmltag_end[]=" ?> ";

#ifndef QT_NO_DATASTREAM
//! 重载操作符<<的实现
QTextStream &operator<<(QTextStream &output , const QSoapMessage & var)
{
    output <<xmltag_begin;
    /*
    QMultiMap<QString,QString>::const_iterator i;
    for (i = var.attributes.constBegin(); i != var.attributes.constEnd(); ++i) {
        output<<" "<<i.key()<<"=\""<<i.value()<<"\"";
    }
    */
    QString tmp("version=\"1.0\" encoding=\"utf-8\"");
    output<<tmp;
    output<<xmltag_end<< var.envelope;
    return output ;
}

//! 重载操作符>>的实现
QTextStream &operator>>(QTextStream & input, QSoapMessage & item)
{
    QDomDocument doc;    
    doc.setContent(input.readAll());

    QDomNode fc_node=doc.firstChild();
    if(!fc_node.isElement())
    {
        QRegExp regexp("'\"");
        QString xmlatt=fc_node.nodeValue();
        QStringList xmlattlist=xmlatt.split(' ');
        for(int i=0;i<xmlattlist.count();i++)
        {
            QString tmp=xmlattlist[i];
            if(tmp.length()>0)
            {
                QStringList attkv=tmp.split('=');
                if(attkv.count()>1&&attkv[1].length()>0)
                {
                    QString sValue=attkv[1];
                    int firstpos=0;
                    if(sValue[0]=='\''||sValue[0]=='"')
                        firstpos=1;
                    int lastpos=sValue.length()-1;
                    if(sValue[lastpos]=='\''||sValue[lastpos]=='"')
                        lastpos=lastpos-1;
                    item.attributes.insert(attkv[0],sValue.mid(firstpos,lastpos));
                }
                else item.attributes.insert(attkv[0],"");
            }
        }
    }

    QDomElement root = doc.documentElement();
    if(root.tagName().contains("Envelope"))
    {
        QDomNamedNodeMap e_att=root.attributes();
        for(int i=0;i<e_att.count();i++)
        {
            QDomNode node=e_att.item(i);
            QString nodeName=node.nodeName();
            //qDebug()<< node.nodeName()<<"--" <<node.nodeValue();
            item.envelope.attributes.insert(nodeName,node.nodeValue());
            if(nodeName.contains("xmlns"))
            {
                QStringList ns=nodeName.split(':');
                if(ns.length()>1)
                    item.envelope.nameSpace.insert(node.nodeValue(),ns[1]);
            }
        }
        QDomElement Element = root.firstChildElement();
        while(!Element.isNull())
        {
            if(Element.tagName().contains("Header"))
            {
                QSoapHeader &header=item.envelope.header;
                QDomNamedNodeMap h_att=Element.attributes();
                for(int i=0;i<h_att.count();i++)
                {
                    QDomNode node=h_att.item(i);
                    //qDebug()<< node.nodeName()<<"--" <<node.nodeValue();
                    header.Attributes().insert(node.nodeName(),node.nodeValue());
                }
                QDomElement fc_Element=Element.firstChildElement();
                while(!fc_Element.isNull())
                {
                    //qDebug()<<"header childs tagName:"<< fc_Element.tagName()<<"--nodeName:" <<fc_Element.nodeName();
                    QSoapElement soapElement;
                    fc_Element>>soapElement;
                    header.AppendChild(soapElement);

                    fc_Element = fc_Element.nextSiblingElement();
                }
            }else if(Element.tagName().contains("Body"))
            {
                QSoapBody &body=item.envelope.body;
                QDomNamedNodeMap h_att=Element.attributes();
                for(int i=0;i<h_att.count();i++)
                {
                    QDomNode node=h_att.item(i);
                    //qDebug()<< node.nodeName()<<"--" <<node.nodeValue();
                    body.Attributes().insert(node.nodeName(),node.nodeValue());
                }
                QDomElement fc_Element=Element.firstChildElement();
                while(!fc_Element.isNull())
                {
                    QSoapElement soapElement;
                    fc_Element>>soapElement;
                    body.AppendChild(soapElement);

                    fc_Element = fc_Element.nextSiblingElement();
                }
            }
            Element = Element.nextSiblingElement();
        }
    }else
    {
        qDebug()<<"not find envelope";
    }

    return input ;
}
#endif
