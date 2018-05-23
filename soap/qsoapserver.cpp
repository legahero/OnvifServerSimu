/*
 * Copyright 2011-2014 Nikhil Marathe <nsm.nikhil@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QTcpSocket>
#include <QVariant>
#include <QDebug>
#include <QtSql>
#include <QUuid>
#include <QGuiApplication>
#include <QScreen>

#include "httpbase/threadhandle.h"
#include "httpbase/qasynhttpsocket.h"
#include "httpbase/qhttprequest.h"
#include "httpbase/qhttpresponse.h"
#include "qsoapserver.h"
#include "qsoapmessage.h"



//QHash<int, QString> STATUS_CODES;

QSoapServer::QSoapServer(QObject *parent,int numConnections) :
    QAsynTcpServer(parent,numConnections)
{    

    AuthMode="Digest";//Basic
    Qop="auth";
    Realm="test-onvif";
    Nonce=QUuid::createUuid().toString().replace("{","");
    Nonce=Nonce.replace("{","");
    Nonce=Nonce.replace("-","");
    AuthDate=QDate::currentDate().toString();

    QSettings settings("onvif.ini",QSettings::IniFormat);
    Nonce = settings.value("Auth/Nonce",Nonce).toString();
    Realm = settings.value("Auth/Realm",Realm).toString();
    AuthMode = settings.value("Auth/Mode",AuthMode).toString();
    UserName = settings.value("Auth/UserName","aaaa").toString();
    Password = settings.value("Auth/Password","test").toString();


}

QSoapServer::~QSoapServer()
{
}

void QSoapServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "QSoapServer:incomingConnection,ThreadId:"<<QThread::currentThreadId()  ;

    //继承重写此函数后，QQAsynTcpServer默认的判断最大连接数失效，自己实现
    if (m_ClientList->size() > maxPendingConnections())
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);        
        tcp.disconnectFromHost();
        qDebug() << "tcpClient->size() > maxPendingConnections(),disconnectFromHost";
        return;
    }
    auto th = ThreadHandle::getClass().getThread();
    QAsynHttpSocket* tcpTemp = new QAsynHttpSocket(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();

    //NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，线程管理计数减1,此槽必须实现
    connect(tcpTemp,SIGNAL(sockDisConnect(const int ,const QString &,const quint16, QThread *)),
            this,SLOT(sockDisConnectSlot(const int ,const QString &,const quint16, QThread *)));

    //必须在QAsynHttpSocket的线程中执行
    connect(tcpTemp, SIGNAL(newRequest(QHttpRequest *, QHttpResponse *)), this,
            SLOT(handleRequest(QHttpRequest *, QHttpResponse *)), Qt::DirectConnection);

    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    m_ClientList->insert(socketDescriptor,tcpTemp);//插入到连接信息中

    qDebug() << "QSoapServer m_ClientList add:"<<socketDescriptor  ;
}

//释放线程资源
void QSoapServer::sockDisConnectSlot(int handle,const QString & ip, quint16 port,QThread * th)
{
    qDebug() << "QSoapServer:sockDisConnectSlot,ThreadId:"<<QThread::currentThreadId()  ;

    qDebug() << "QSoapServer m_ClientList size:"<<m_ClientList->size()  ;
    qDebug() << "QSoapServer m_ClientList:remove:"<<handle  ;
    m_ClientList->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了,释放数量
    qDebug() << "QHttpServer m_ClientList size:"<<m_ClientList->size()  ;
}


//处理新的http 请求，这里处理业务
void QSoapServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    qDebug() << "QSoapServer:handleRequest,ThreadId:"<<QThread::currentThreadId()  ;
    qDebug() <<"path:"<< req->path()<<"body:"<<req->body();
    //qDebug() <<"headers:"<< req->headers();

    QString repdata(req->body());

    int ppos=0;
    do{
        qDebug()<<repdata.mid(ppos,900);
        ppos+=900;
    }while(ppos<repdata.length());
    qDebug()<<"--------------------end";
    QTextStream indata(&repdata);
    QSoapMessage soapMessage;
    indata>>soapMessage;

    if(soapMessage.envelope.body.ChildCount()>0)
    {
        QString tagName=soapMessage.envelope.body[0].tagName;
        QStringList qnamelist=tagName.split(":");

        QString xmlns="";
        if(qnamelist.length()>1)
            xmlns=qnamelist[0];
        QString funName=qnamelist[qnamelist.length()-1];

        if(xmlns.length()>0)
            xmlns="xmlns:"+xmlns;
        else
            xmlns="xmlns";
        QString nstype="";
        nstype=soapMessage.envelope.body[0].Attributes(xmlns);
        if(nstype.length()<1)
            nstype=soapMessage.envelope.body.Attributes(xmlns);
        if(nstype.length()<1)
            nstype=soapMessage.envelope.attributes[xmlns];

        QSoapMessage respMessage;
        if(nstype=="http://www.onvif.org/ver10/device/wsdl")
        {
            //执行设备功能
            if(funName=="GetDeviceInformation")
            {
                respMessage=m_dev.OnGetDeviceInformation(soapMessage);
            }else if(funName=="GetCapabilities")
            {
                respMessage=m_dev.OnGetCapabilities(soapMessage);
            }else if(funName=="GetServices")
            {
                respMessage=m_dev.OnGetServices(soapMessage);
            }else if(funName=="GetScopes")
            {
                respMessage=m_dev.OnGetScopes(soapMessage);
            }else if(funName=="GetNetworkInterfaces")
            {
                respMessage=m_dev.OnGetNetworkInterfaces(soapMessage);
            }else if(funName=="GetDNS")
            {
                respMessage=m_dev.OnGetDNS(soapMessage);
            }else
            {
                qDebug()<<"func no exits:"<<nstype<<" funName:"<<funName;
            }
        }else if(nstype=="http://www.onvif.org/ver10/media/wsdl")
        {
            bool AuthStatus=true;
            //if(soapMessage.envelope.header.ChildCount()>0)
            //    qDebug()<<"header[0].tagName:"<<soapMessage.envelope.header[0].tagName;
            //认证信息
            if(soapMessage.envelope.header.ChildCount()>0
                    &&soapMessage.envelope.header[0].tagName.contains("Security"))
            {

                QSoapElement security=soapMessage.envelope.header[0];
                if(security.ChildCount()>0)
                {

                    QSoapElement child=security[0];
                    QString fUserName;
                    QString fPassword;
                    QString fNonce;
                    QString fDate;
                    for(int i=0;i<child.ChildCount();i++)
                    {
                        if(child[i].tagName.contains("Nonce"))
                            fNonce=child[i].value;
                        else if(child[i].tagName.contains("Created"))
                            fDate=child[i].value;
                        else if(child[i].tagName.contains("Username"))
                            fUserName=child[i].value;
                        else if(child[i].tagName.contains("Password"))
                            fPassword=child[i].value;
                    }

                    AuthStatus=true;//去掉了认证校验，有需要的话向我要  ：）


                }
            }
            if(AuthStatus==false)
            {
                QString probeText="<!DOCTYPE html>\r\n<html><head><title>Document Error: Unauthorized</title></head>\r\n<body><h2>Access Error: 401 -- Unauthorized</h2>\r\n<p>Authentication Error: Access Denied! Authorization required.</p>\r\n</body>\r\n</html>";
                QByteArray data=probeText.toUtf8();
                resp->setHeader("Content-Type", "text/html; charset=utf-8");
                resp->setHeader("Content-Length", QString::number(data.length()));
                resp->setHeader("Server", "onvif");
                resp->setHeader("Connection", "close");
                resp->setHeader("Date", AuthDate);

                QString Digest="";
                if(AuthMode=="Digest")
                {
                    Digest=QString("%1 qop=\"%2\",realm=\"%3\",nonce=\"%4\"").arg(AuthMode).arg(Qop).arg(Realm).arg(Nonce);
                }else
                {
                    Digest=QString("%1 realm=\"%2\"").arg(AuthMode).arg(Realm);
                }

                resp->setHeader("WWW-Authenticate", Digest);

                resp->writeHead(401);
                resp->end(data);
                goto DoEnd;
            }


            //执行设备功能
            if(funName=="GetProfiles")
            {
                respMessage=m_media.OnGetProfiles(soapMessage);
            }else if(funName=="GetStreamUri")
            {
                respMessage=m_media.OnGetStreamUri(soapMessage);
            }else if(funName=="GetSnapshotUri")
            {
                respMessage=m_media.OnGetSnapshotUri(soapMessage);
            }else if(funName=="GetVideoEncoderConfigurationOptions")
            {
                respMessage=m_media.OnGetVideoEncoderConfigurationOptions(soapMessage);
            }else if(funName=="GetVideoEncoderConfigurations")
            {
                respMessage=m_media.OnGetVideoEncoderConfigurations(soapMessage);
            }else if(funName=="GetVideoSources")
            {                
                respMessage=m_media.OnGetVideoSources(soapMessage);
            }else if(funName=="GetAudioSources")
            {
                respMessage=m_media.OnGetAudioSources(soapMessage);
            }else
            {
                qDebug()<<"func no exits:"<<nstype<<" funName:"<<funName;
            }
        }else if(nstype=="http://www.onvif.org/ver10/analyticsdevice/wsdl")
        {
            if(funName=="GetAnalyticsEngines")
            {
                respMessage=m_AnaDev.OnGetAnalyticsEngines(soapMessage);
            }else
            {
                qDebug()<<"func no exits:"<<nstype<<" funName:"<<funName;
            }
        }else if(nstype=="http://www.onvif.org/ver20/ptz/wsdl")
        {
            if(funName=="GetNodes")
            {
                respMessage=m_ptz.OnGetNodes(soapMessage);
            }else
            {
                qDebug()<<"func no exits:"<<nstype<<" funName:"<<funName;
            }
        }else
        {

            qDebug()<<"func no exits:"<<nstype<<" funName:"<<funName;
        }
        QString probeText;
        QTextStream probeTS(&probeText);
        probeTS<<respMessage;

        qDebug()<<"-----------resp-------------";
        int ipos=0;
        do{
            qDebug()<<probeText.mid(ipos,900);
            ipos+=900;
        }while(ipos<probeText.length());

        qDebug()<<"--------------------end";
        QByteArray data=probeText.toUtf8();

        resp->setHeader("Content-Type", "application/soap+xml; charset=utf-8");
        resp->setHeader("Content-Length", QString::number(data.length()));
        resp->setHeader("Server", "onvif");
        resp->setHeader("Connection", "close");

        resp->writeHead(200);
        resp->end(data);
    }else if(req->path().contains("/Snapshot"))
    {
        //Snapshot
        QScreen *screen = QGuiApplication::primaryScreen();
        QBuffer jpegBuf;
        screen->grabWindow(0).save(&jpegBuf,"jpg");

        resp->setHeader("Content-Type", "image/jpeg");
        resp->setHeader("Content-Length", QString::number(jpegBuf.size()));
        resp->setHeader("Connection", "close");
        resp->writeHead(200);
        resp->end(jpegBuf.buffer());
    }else
    {
        qDebug() <<"get  soap::body fail";
        resp->writeHead(403);
    }
DoEnd:
    resp->flush();

    req->deleteLater();
    resp->deleteLater();
    qDebug() <<"handleRequest end";

}

