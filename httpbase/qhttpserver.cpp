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

#include "threadhandle.h"
#include "qasynhttpsocket.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "qhttpserver.h"
#include "staticfilecontroller.h"

//QHash<int, QString> STATUS_CODES;

QHttpServer::QHttpServer(QObject *parent,int numConnections) :
    QAsynTcpServer(parent,numConnections)
{    


}

QHttpServer::~QHttpServer()
{
}

void QHttpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "QHttpServer:incomingConnection,ThreadId:"<<QThread::currentThreadId()  ;

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

    qDebug() << "QHttpServer m_ClientList add:"<<socketDescriptor  ;
}

//释放线程资源
void QHttpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    qDebug() << "QHttpServer:sockDisConnectSlot,ThreadId:"<<QThread::currentThreadId()  ;

    qDebug() << "QHttpServer m_ClientList size:"<<m_ClientList->size()  ;
    qDebug() << "QHttpServer m_ClientList:remove:"<<handle  ;
    m_ClientList->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了,释放数量
    qDebug() << "QHttpServer m_ClientList size:"<<m_ClientList->size()  ;
}


//处理新的http 请求，这里处理业务
void QHttpServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    qDebug() << "QHttpServer:handleRequest,ThreadId:"<<QThread::currentThreadId()  ;


    qDebug() <<"path:"<< req->path()<<"body:"<<req->body();
    qDebug() <<"headers:"<< req->headers();

    if(req->path().indexOf('.')>=0)
    {
        QString configFileName=searchConfigFile();
        // Configure static file controller
        QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat);
        fileSettings->beginGroup("docroot");
        StaticFileController* staticFileController=new StaticFileController(fileSettings);

        //StaticFileController staticFileController(fileSettings);
        staticFileController->Handler(*req, *resp);
        return ;
    }

    QJsonDocument doc=QJsonDocument::fromBinaryData(req->body());
    QJsonObject recv_obj=doc.object();//这是接收到的json对象

    QConnectPool* dbpool=QMultiDbManager::getDb("sql2014");
    if(dbpool!=NULL)
    {
        QSqlDatabase db=dbpool->openSession();
        QSqlQuery query(db);
        query.exec("SELECT top 1 * FROM tLogin ");

        QJsonObject resp_obj; //返回json对象
        while (query.next()) {
            resp_obj.insert("LoginName",query.value("LoginName").toString());
            resp_obj.insert("Pwd", query.value("LoginName").toString());
        }
        dbpool->closeSession(db);

        QByteArray data = QJsonDocument(resp_obj).toJson(QJsonDocument::Compact);

        resp->setHeader("Content-Type", "text/html");
        resp->setHeader("Content-Length", QString::number(data.length()));
        resp->writeHead(200);
        resp->end(data);
    }else
    {
        qDebug() <<"get  QMultiDbManager::getDb fail";
        //resp->setHeader("Content-Type", "text/html");
        resp->writeHead(403);
    }
    resp->flush();

    req->deleteLater();
    resp->deleteLater();
    qDebug() <<"handleRequest end";

}

/** Search the configuration file */
QString QHttpServer::searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName(appName+".ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../../etc"); // for development without shadow build
    searchList.append(binDir+"/../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            // found
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    //qFatal("Cannot find config file %s",qPrintable(fileName));
    return 0;
}
