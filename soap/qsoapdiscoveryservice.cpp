#include "qsoapdiscoveryservice.h"
#include "qsoapmessage.h"
#include <QUuid>

const QHostAddress IPV6_DISCOVERY_MCAST("ff02::c");
const QHostAddress IPV4_DISCOVERY_MCAST("239.255.255.250");
const int DISCOVERY_MCAST_PORT = 3702;

QSoapDiscoveryService::QSoapDiscoveryService(QObject *parent) : QObject(parent)
{
    udpSocket=NULL;

    QUuid uuid = QUuid::createUuid();
    dev_id=uuid.toString();
    dev_id=dev_id.mid(1,dev_id.length()-2);

    MessageNumber=0;

    udpSocket = new QUdpSocket(this);
}


bool QSoapDiscoveryService::Start()
{
    if(udpSocket==NULL)
        throw QString("client no create!");

    //udpSocket->bind(QHostAddress::Any, 3702);
    //udpSocket->joinMulticastGroup(QHostAddress::Any);//IPV4_DISCOVERY_MCAST);

    //if(udpSocket->bind(QHostAddress::AnyIPv4, 3702,QUdpSocket::ReuseAddressHint|QUdpSocket::ShareAddress))
    if(udpSocket->bind(QHostAddress::AnyIPv4, DISCOVERY_MCAST_PORT,QUdpSocket::ReuseAddressHint|QUdpSocket::ShareAddress))
    {
        udpSocket->joinMulticastGroup(IPV4_DISCOVERY_MCAST);//QHostAddress::Any);

        connect(udpSocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

        Hello();
        return true;
    }else
        return false;

}

QSoapDiscoveryService::~QSoapDiscoveryService()
{

    if(udpSocket!=NULL)
    {
        Bye();

        _sleep(10);
        //udpSocket->leaveMulticastGroup(QHostAddress::Any);//IPV4_DISCOVERY_MCAST
        udpSocket->leaveMulticastGroup(IPV4_DISCOVERY_MCAST);

        udpSocket->close();

        //delete udpSocket;
        udpSocket->deleteLater();
    }
}

/*
 * SOAP action:
http://www.onvif.org/ver10/network/wsdl/Hello
Input:
[Hello] [HelloType]
Output:
[HelloResponse] [ResolveType]

    <d:Hello>
        <a:EndpointReference>
            <a:Address>
                urn:uuid:98190dc2-0890-4ef8-ac9a-5940995e6119
            </a:Address>
        </a:EndpointReference>
        <d:Types>discows:NetworkVideoTransmitter</d:Types>
        <d:Scopes>onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/hardware/NetworkVideoTransmitter onvif://www.onvif.org/location/country/Azerbaijan onvif://www.onvif.org/location/city/Baku onvif://www.onvif.org/name/NVT</d:Scopes>
        <d:XAddrs>http://192.168.1.51:8004</d:XAddrs>
        <d:MetadataVersion>1</d:MetadataVersion>
        <d:MetadataVersion>
            75965
        </d:MetadataVersion>
    </d:Hello>
 * */
void QSoapDiscoveryService::Hello()
{
    qDebug()<<"DiscoveryService Hello....,dev_id="<<dev_id;

    QSoapMessage soapMessage;
    soapMessage.envelope.attributes.insert("xmlns:SOAP-ENV","http://www.w3.org/2003/05/soap-envelope");
    soapMessage.envelope.attributes.insert("xmlns:a","http://www.w3.org/2005/08/addressing");
    soapMessage.envelope.attributes.insert("xmlns:d","http://schemas.xmlsoap.org/ws/2005/04/discovery");
    soapMessage.envelope.attributes.insert("xmlns:dn","http://www.onvif.org/ver10/network/wsdl");

    QSoapElement actionElement;
    actionElement.tagName="a:Action";
    //actionElement.attributes.insert("SOAP-ENV:mustUnderstand","1");
    actionElement.value="http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello";
    soapMessage.envelope.header.AppendChild(actionElement);

    QSoapElement messageidElement;
    QUuid uuid = QUuid::createUuid();
    QString suuid=uuid.toString();
    suuid=suuid.mid(1,suuid.length()-2);
    messageidElement.value="urn:uuid:"+suuid;
    messageidElement.tagName="a:MessageID";
    soapMessage.envelope.header.AppendChild(messageidElement);

    QSoapElement toElement;
    //toElement.attributes.insert("SOAP-ENV:mustUnderstand","1");
    toElement.value="urn:schemas-xmlsoap-org:ws:2005:04:discovery";
    toElement.tagName="a:To";
    soapMessage.envelope.header.AppendChild(toElement);

    QSoapElement helloElement;    
    QSoapElement devElement;
    QSoapElement addressElement;

    addressElement.value="urn:uuid:"+dev_id;
    addressElement.tagName="a:Address";
    devElement.AppendChild(addressElement);
    devElement.tagName="a:EndpointReference";
    helloElement.AppendChild(devElement);

    //QSoapElement verElement;
    //verElement.value="1";
    //verElement.tagName="d:MetadataVersion";
    //helloElement.AppendChild(verElement);
    QSoapElement typesElement;
    typesElement.value="dn:NetworkVideoTransmitter";
    typesElement.tagName="d:Types";
    helloElement.AppendChild(typesElement);
    QSoapElement scopesElement;
    scopesElement.value="onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/hardware/NetworkVideoTransmitter onvif://www.onvif.org/location/country/china onvif://www.onvif.org/location/city/GuangZhou onvif://www.onvif.org/name/NVT";
    scopesElement.tagName="d:Scopes";
    helloElement.AppendChild(scopesElement);
    QSoapElement xAddrsElement;
    xAddrsElement.value=dev_url;//"http://192.168.1.51:8004";//有待修改
    xAddrsElement.tagName="d:XAddrs";
    helloElement.AppendChild(xAddrsElement);
    QSoapElement metadataVersionElement;
    metadataVersionElement.value="1";
    metadataVersionElement.tagName="d:MetadataVersion";
    helloElement.AppendChild(metadataVersionElement);


    helloElement.tagName="d:Hello";
    soapMessage.envelope.body.AppendChild(helloElement);

    QString helloText;
    QTextStream helloTS(&helloText);
    helloTS<<soapMessage;

    if(udpSocket->writeDatagram(helloText.toUtf8(), IPV4_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT)<0)
    {
        //重发
        udpSocket->writeDatagram(helloText.toUtf8(), IPV4_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT);
    }

    if(udpSocket->writeDatagram(helloText.toUtf8(), IPV6_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT)<0)
    {
        //重发
        udpSocket->writeDatagram(helloText.toUtf8(), IPV6_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT);
    }
    udpSocket->flush();
}




/*
 * <SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:SOAP-ENC="http://www.w3.org/2003/05/soap-encoding" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsdd="http://schemas.xmlsoap.org/ws/2005/04/discovery" xmlns:discows="http://www.onvif.org/ver10/network/wsdl">
  <SOAP-ENV:Header>
    <wsa:MessageID>urn:uuid:5928e397-6df7-4673-8000-12db0000153c</wsa:MessageID>
    <wsa:RelatesTo>uuid:{71ce08f4-a998-40f4-9d1f-0e810804abcd}</wsa:RelatesTo>
    <wsa:ReplyTo SOAP-ENV:mustUnderstand="true">
      <wsa:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:Address>
    </wsa:ReplyTo>
    <wsa:To SOAP-ENV:mustUnderstand="true">http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>
    <wsa:Action SOAP-ENV:mustUnderstand="true">http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action>
    <wsdd:AppSequence MessageNumber="8" InstanceId="0"/>
  </SOAP-ENV:Header>
  <SOAP-ENV:Body>
    <wsdd:ProbeMatches>
      <wsdd:ProbeMatch>
        <wsa:EndpointReference>
          <wsa:Address>urn:uuid:5928d907-1787-49f8-8000-002900004823</wsa:Address>
        </wsa:EndpointReference>
        <wsdd:Types>discows:NetworkVideoTransmitter</wsdd:Types>
        <wsdd:Scopes>onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/hardware/NetworkVideoTransmitter onvif://www.onvif.org/location/country/Azerbaijan onvif://www.onvif.org/location/city/Baku onvif://www.onvif.org/name/NVT</wsdd:Scopes>
        <wsdd:XAddrs>http://192.168.1.51:8004</wsdd:XAddrs>
        <wsdd:MetadataVersion>1</wsdd:MetadataVersion>
      </wsdd:ProbeMatch>
    </wsdd:ProbeMatches>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>
 * */
void QSoapDiscoveryService::ProbeMatches(QString MessageID,QHostAddress sender,quint16 senderPort)
{
    qDebug()<<"QSoapDiscoveryService::ProbeMatches MessageID="<<MessageID<<" sender="<<sender<<" port="<<senderPort;

    QSoapMessage soapMessage;
    soapMessage.envelope.attributes.insert("xmlns:SOAP-ENV","http://www.w3.org/2003/05/soap-envelope");
    soapMessage.envelope.attributes.insert("xmlns:a","http://schemas.xmlsoap.org/ws/2004/08/addressing");
    soapMessage.envelope.attributes.insert("xmlns:d","http://schemas.xmlsoap.org/ws/2005/04/discovery");
    soapMessage.envelope.attributes.insert("xmlns:dn","http://www.onvif.org/ver10/network/wsdl");
    soapMessage.envelope.AddNameSpace("https://www.onvif.org/ver10/device/wsdl","tds");
    soapMessage.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    soapMessage.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    soapMessage.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");

    QSoapElement messageidElement;
    QUuid uuid = QUuid::createUuid();
    QString suuid=uuid.toString();
    suuid=suuid.mid(1,suuid.length()-2);
    messageidElement.value="urn:uuid:"+suuid;
    messageidElement.tagName="a:MessageID";
    soapMessage.envelope.header.AppendChild(messageidElement);

    QSoapElement relatesToElement;
    relatesToElement.value=MessageID;//"uuid:"+MessageID;
    relatesToElement.tagName="a:RelatesTo";
    soapMessage.envelope.header.AppendChild(relatesToElement);

    QSoapElement replyToElement;
    QSoapElement AddrElement;
    AddrElement.value="http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
    AddrElement.tagName="a:Address";
    replyToElement.AppendChild(AddrElement);
    replyToElement.tagName="a:ReplyTo";
    soapMessage.envelope.header.AppendChild(replyToElement);

    QSoapElement toElement;
    //toElement.attributes.insert("SOAP-ENV:mustUnderstand","1");
    toElement.value="http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
    toElement.tagName="a:To";
    soapMessage.envelope.header.AppendChild(toElement);

    QSoapElement actionElement;
    //actionElement.attributes.insert("SOAP-ENV:mustUnderstand","1");
    actionElement.value="http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";
    actionElement.tagName="a:Action";
    soapMessage.envelope.header.AppendChild(actionElement);

    QSoapElement appSeqElement;
    appSeqElement.Attributes().insert("MessageNumber",QString::number(MessageNumber));
    appSeqElement.Attributes().insert("InstanceId",QString::number(0));
    appSeqElement.tagName="d:AppSequence";
    soapMessage.envelope.header.AppendChild(appSeqElement);

    /*
     * <wsdd:ProbeMatches>
      <wsdd:ProbeMatch>
        <wsa:EndpointReference>
          <wsa:Address>urn:uuid:5928d907-1787-49f8-8000-002900004823</wsa:Address>
        </wsa:EndpointReference>
        <wsdd:Types>discows:NetworkVideoTransmitter</wsdd:Types>
        <wsdd:Scopes>onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/hardware/NetworkVideoTransmitter onvif://www.onvif.org/location/country/Azerbaijan onvif://www.onvif.org/location/city/Baku onvif://www.onvif.org/name/NVT</wsdd:Scopes>
        <wsdd:XAddrs>http://192.168.1.51:8004</wsdd:XAddrs>
        <wsdd:MetadataVersion>1</wsdd:MetadataVersion>
      </wsdd:ProbeMatch>
    </wsdd:ProbeMatches>
     * */
    QSoapElement probeMatchesElement;
    QSoapElement probeElement;

    QSoapElement endpointReferenceElement;
    QSoapElement addressElement;
    addressElement.value="urn:uuid:"+dev_id;
    addressElement.tagName="a:Address";
    endpointReferenceElement.AppendChild(addressElement);
    endpointReferenceElement.tagName="a:EndpointReference";
    probeElement.AppendChild(endpointReferenceElement);

    QSoapElement typesElement;
    typesElement.value="dn:NetworkVideoTransmitter";
    typesElement.tagName="d:Types";
    probeElement.AppendChild(typesElement);
    QSoapElement scopesElement;
    scopesElement.value="onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/hardware/NetworkVideoTransmitter onvif://www.onvif.org/location/country/china onvif://www.onvif.org/location/city/ShenZhen onvif://www.onvif.org/name/NVT";
    scopesElement.tagName="d:Scopes";
    probeElement.AppendChild(scopesElement);
    QSoapElement xAddrsElement;
    xAddrsElement.value=dev_url;//"http://192.168.1.51:8004";//有待修改
    xAddrsElement.tagName="d:XAddrs";
    probeElement.AppendChild(xAddrsElement);
    QSoapElement metadataVersionElement;
    metadataVersionElement.value="1";
    metadataVersionElement.tagName="d:MetadataVersion";
    probeElement.AppendChild(metadataVersionElement);

    probeElement.tagName="d:ProbeMatch";
    probeMatchesElement.AppendChild(probeElement);
    probeMatchesElement.tagName="d:ProbeMatches";
    soapMessage.envelope.body.AppendChild(probeMatchesElement);

    QString probeText;
    QTextStream probeTS(&probeText);
    probeTS<<soapMessage;
    qDebug()<<"---------------------send to"<<sender.toString()<<" "<<QString::number(senderPort)<<"-----------------------";
    qDebug()<<probeText.left(850);
    qDebug()<<probeText.mid(850);
    qDebug()<<"---------------------end-----------------------";

    /*
    probeText="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\">"
      "<SOAP-ENV:Header>"
        "<wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action>"
        "<wsa:MessageID>urn:uuid:592fa55d-96c8-47ca-8000-002900004823</wsa:MessageID>"
        "<wsa:RelatesTo>"+MessageID+"</wsa:RelatesTo>"
        "<wsa:ReplyTo SOAP-ENV:mustUnderstand=\"true\">"
         " <wsa:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:Address>"
        "</wsa:ReplyTo>"
        "<wsa:To SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>"

        "<wsdd:AppSequence MessageNumber=\"2\" InstanceId=\"0\"/>"
      "</SOAP-ENV:Header>"
      "<SOAP-ENV:Body>"
       " <wsdd:ProbeMatches>"
       "   <wsdd:ProbeMatch>"
       "     <wsa:EndpointReference>"
       "       <wsa:Address>urn:uuid:592fa538-d235-48f9-8000-002900004821</wsa:Address>"
       "     </wsa:EndpointReference>"
        "     <wsdd:Types>dn:NetworkVideoTransmitter</wsdd:Types>"
       "    <wsdd:Scopes>onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/hardware/NetworkVideoTransmitter onvif://www.onvif.org/location/country/china onvif://www.onvif.org/location/city/shenzhen onvif://www.onvif.org/name/NVT</wsdd:Scopes>"
       "     <wsdd:XAddrs>http://192.168.1.51:8004</wsdd:XAddrs>"
       "     <wsdd:MetadataVersion>1</wsdd:MetadataVersion>"

       "   </wsdd:ProbeMatch>"
       " </wsdd:ProbeMatches>"
      "</SOAP-ENV:Body>"
    "</SOAP-ENV:Envelope>";
    */
    udpSocket->writeDatagram(probeText.toUtf8(), IPV4_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT);
    if(udpSocket->writeDatagram(probeText.toUtf8(), sender, senderPort)<0)//这个对方无法收到，奇怪的很
    {
        qDebug()<<"DiscoveryService ProbeMatches error!!!";
        udpSocket->writeDatagram(probeText.toUtf8(), sender, senderPort);//重发
    }
    udpSocket->flush();
    qDebug()<<"DiscoveryService::ProbeMatches OK";
}


void QSoapDiscoveryService::ResolveMatches(QString MessageID,QHostAddress sender,quint16 senderPort)
{
    qDebug()<<"DiscoveryService::ResolveMatches MessageID="<<MessageID<<" sender="<<sender<<" port="<<senderPort;
}

/*
 * SOAP action:
http://www.onvif.org/ver10/network/wsdl/Bye
Input:
[Bye] [ByeType]
Output:
[ByeResponse] [ResolveType]

<d:Bye>
     <a:EndpointReference>
     <a:Address>
uuid:98190dc2-0890-4ef8-ac9a-5940995e6119
     </a:Address>
     </a:EndpointReference>
     </d:Bye>

 * */
void QSoapDiscoveryService::Bye()
{
    qDebug()<<"DiscoveryService::Bye....";

    QSoapMessage soapMessage;
    soapMessage.envelope.attributes.insert("xmlns:SOAP-ENV","http://www.w3.org/2003/05/soap-envelope");
    soapMessage.envelope.attributes.insert("xmlns:a","http://www.w3.org/2005/08/addressing");
    soapMessage.envelope.attributes.insert("xmlns:d","http://schemas.xmlsoap.org/ws/2005/04/discovery");
    soapMessage.envelope.attributes.insert("xmlns:dn","http://www.onvif.org/ver10/network/wsdl");
    soapMessage.envelope.attributes.insert("xmlns:dn","http://www.onvif.org/ver10/network/wsdl");

    QSoapElement messageidElement;
    QUuid uuid = QUuid::createUuid();
    QString suuid=uuid.toString();
    suuid=suuid.mid(1,suuid.length()-2);
    messageidElement.value="urn:uuid:"+suuid;//去除{}//messageidElement.value="urn:uuid:"+uuid.toString();
    messageidElement.tagName="a:MessageID";
    soapMessage.envelope.header.AppendChild(messageidElement);

    QSoapElement toElement;
    //toElement.attributes.insert("SOAP-ENV:mustUnderstand","1");
    toElement.value="urn:schemas-xmlsoap-org:ws:2005:04:discovery";
    toElement.tagName="a:To";
    soapMessage.envelope.header.AppendChild(toElement);

    QSoapElement actionElement;
    //actionElement.attributes.insert("SOAP-ENV:mustUnderstand","1");
    actionElement.value="http://schemas.xmlsoap.org/ws/2005/04/discovery/Bye";
    actionElement.tagName="a:Action";
    soapMessage.envelope.header.AppendChild(actionElement);

    QSoapElement byeElement;
    QSoapElement devElement;
    QSoapElement addressElement;
    addressElement.value="urn:uuid:"+dev_id;
    addressElement.tagName="a:Address";
    devElement.AppendChild(addressElement);
    devElement.tagName="a:EndpointReference";
    byeElement.AppendChild(devElement);

    byeElement.tagName="d:Bye";
    soapMessage.envelope.body.AppendChild(byeElement);

    QString byeText;
    QTextStream byeTS(&byeText);
    byeTS<<soapMessage;


    if(udpSocket->writeDatagram(byeText.toUtf8(), IPV4_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT)<0)
    {
        udpSocket->writeDatagram(byeText.toUtf8(), IPV4_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT);
    }

    if(udpSocket->writeDatagram(byeText.toUtf8(), IPV6_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT)<0)
    {
        udpSocket->writeDatagram(byeText.toUtf8(), IPV6_DISCOVERY_MCAST, DISCOVERY_MCAST_PORT);
    }
    udpSocket->flush();
}

void QSoapDiscoveryService::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        //QNetworkDatagram datagram = udpSocket->receiveDatagram();
        //datagram.data();
        //processTheDatagram(datagram);

        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort=0;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                        &sender, &senderPort);

        QString data(datagram);
        if(!data.contains("soap:Envelope"))
        {
            qDebug()<<"--------------------read from "<<sender.toString()<<" port:"<<senderPort<<"--------"<<endl;
            int ppos=0;
            do{
                qDebug()<<data.mid(ppos,900);
                ppos+=900;
            }while(ppos<data.length());
            qDebug()<<"--------------------end"<<endl;
        }
        QTextStream indata(&data);
        QSoapMessage soapMessage;
        indata>>soapMessage;

        //WS-Addressing (2003, 2004, and 2005 schemas)
        QString a_ns=soapMessage.envelope.GetNameSpace("http://schemas.xmlsoap.org/ws/2004/08/addressing");//
        if(a_ns.length()<1)
            a_ns=soapMessage.envelope.GetNameSpace("http://www.w3.org/2005/08/addressing");
        if(a_ns.length()<1&&soapMessage.envelope.header.ChildCount()>0)
        {

            QSoapElement element = soapMessage.envelope.header[0];
            a_ns=element.GetNameSpace("http://schemas.xmlsoap.org/ws/2004/08/addressing");
            if(a_ns.length()<1)
                a_ns=element.GetNameSpace("http://www.w3.org/2005/08/addressing");
        }

        QString messageId=soapMessage.envelope.header[a_ns+":MessageID"].value;
        QString action=soapMessage.envelope.header[a_ns+":Action"].value;

        if(action.contains("/"))
            action=action.mid(action.lastIndexOf('/')+1);

        qDebug()<<"DiscoveryService readDatagram action="<<action<<" messageId="<<messageId<<endl;
        if(action=="Probe")
        {
            qDebug()<<"action="<<action<<" call ProbeMatches";
            ProbeMatches(messageId,sender,senderPort);
        }else if(action=="Resolve")
        {
            qDebug()<<"action="<<action<<" call ResolveMatches";
            ResolveMatches(messageId,sender,senderPort);
        }
    }
}
