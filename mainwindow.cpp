#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QStringList>
#include <qsql.h>
#include <qsqldatabase.h>
#include "soap/qsoapmessage.h"
#include "media/qmediaserver.h"
#include "media/WindowsAudioInputDevice_mixer.hh"

/*
 * QDB2，IBM DB2 7.1 以上的数据库版本
    QIBASE，Borland InterBase
    QMYSQL，MySQL
    QOCI，Qracel
    QODBC，ODBC，包括 Microsoft SQL Server
    QPSQL，PostgreSQL 7.3 及以上版本
    QSQLITE，SQLite 3
    QSQLITE2，SQLite 2
    QTDS，Sybase Adaptive Server
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->teLog->append("系统支持以下数据库:");
    QStringList drivers=QSqlDatabase::drivers();
    foreach(QString driver,drivers)
    {
        ui->teLog->append(driver);
    }

    ui->teLog->append("系统支持以下麦克风端口:");
    AudioPortNames* portNames = AudioInputDevice::getPortNames();
    if (portNames == NULL) {
        ui->teLog->append("AudioInputDevice::getPortNames() failed!\n");
    }

    printf("%d available audio input ports:\n", portNames->numPorts);
    for (unsigned i = 0; i < portNames->numPorts; ++i) {
        //QString AudioPortName=QString::fromLatin1(portNames->portName[i]);
        QString AudioPortName2=QString::fromLocal8Bit(portNames->portName[i]);
        //QString AudioPortName3=QString::fromWCharArray((const wchar_t *)portNames->portName[i]);
        //QString AudioPortName4=QString::fromUtf8(portNames->portName[i]);

        ui->teLog->append(AudioPortName2);
    }

    /*
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");
    db.setHostName("112.124.60.199");
    db.setDatabaseName("orcl");
    db.setUserName("szxf");
    db.setPassword("Zdst1234");

    if (!db.open())
    {
        QMessageBox::information(this, tr("提示"), tr("Oracle数据库连接失败！"), tr("确定"));
        qDebug() <<"error_Oracle:\n" << db.lastError().text();
    }


    QSqlQuery query;
    QMap<QString,QString> AreaMap;
    query.exec("SELECT AREA_ID,AREA_NAME FROM T_FIREPROOF_AREA");
    while (query.next())
    {
        QString AreaId=query.record().field(0).value().toString();
        QString AreaName=query.record().field(1).value().toString();
        AreaMap[AreaName]=AreaId;
    }
    //QSqlDatabase::database().commit();
    db.close();         //释放数据库连接
    */

    server=NULL;
    ds=NULL;
    onvif=NULL;
    mediaServer=NULL;

    ui->pbStart->setVisible(false);//多余的http Server

    //新建QSystemTrayIcon对象
    m_pSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon(":/logo32.png");
    //将icon设到QSystemTrayIcon对象中
    m_pSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    m_pSysTrayIcon->setToolTip(QObject::trUtf8("onvif模拟器系统托盘图标"));
    //给QSystemTrayIcon添加槽函数
    connect(m_pSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

}

MainWindow::~MainWindow()
{
    Stop();
    delete  m_pSysTrayIcon;
    delete ui;
}

void MainWindow::Stop()
{
    if(ds!=NULL)
    {
        //ds->Bye();
        //ds->deleteLater();
        delete ds;
        ds=NULL;
    }
    if(onvif!=NULL)
    {
        delete onvif;
        onvif=NULL;
    }
    if(mediaServer!=NULL)
    {
        mediaServer->exit();

        mediaServer->deleteLater();
        mediaServer=NULL;
    }

    if(server!=NULL)
    {
        delete server;
        server=NULL;
    }
    Sleep(10);
}

void MainWindow::on_pbSysTrayIcon_clicked()
{
    //隐藏程序主窗口
    this->hide();

    //在系统托盘显示此对象
    m_pSysTrayIcon->show();
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
        //显示主程序窗口
        this->show();

        /*
        //显示消息球，1s后自动消失,第一个参数是标题,第二个参数是消息内容,第三个参数图标,第四个参数是超时毫秒数
        m_pSysTrayIcon->showMessage(QObject::trUtf8("Message Title"),
                                          QObject::trUtf8("欢迎使用此程序"),
                                          QSystemTrayIcon::Information,
                                          1000);
        */
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
        this->show();
        //仅仅显示在最前1次(点击主窗体时主窗体回到最前)
        this->raise();
        break;
    case QSystemTrayIcon::Context:
    default:
        break;
    }
}

void MainWindow::on_pbStart_clicked()
{
    qDebug() << "MainWindow:on_pbStart_clicked,ThreadId:"<<QThread::currentThreadId()  ;

    if(server==NULL) server = new QHttpServer(this);

    if(server->listen(QHostAddress::Any, qint16(ui->lePort->text().toInt())))
    {
        ui->teLog->append("http 服务已经启动!");
    }else
    {
        ui->teLog->append("http 服务启动失败!");
    }
}


void MainWindow::on_pbsoaptest_clicked()
{
    if(ds)
        Stop();
    /*
    QString soaptext="<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\""
            " xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\""
            " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
            " xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\""
            " xmlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\""
            " xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\""
            " xmlns:xmime=\"http://tempuri.org/xmime.xsd\""
            " xmlns:xop=\"http://www.w3.org/2004/08/xop/include\""
            " xmlns:tt=\"http://www.onvif.org/ver10/schema\""
            " xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\""
            " xmlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\""
            " xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\""
            " xmlns:devicews=\"http://www.onvif.org/ver10/device/wsdl\">"
            "<SOAP-ENV:Body><devicews:GetCapabilitiesResponse>"
                     "<devicews:Capabilities xsi:type=\"tt:Capabilities\">"
                     "<tt:Analytics xsi:type=\"tt:AnalyticsCapabilities\">"
                     "<tt:XAddr>http://192.168.1.51:8004</tt:XAddr>"
                     "<tt:RuleSupport>false</tt:RuleSupport>"
                     "<tt:AnalyticsModuleSupport>false</tt:AnalyticsModuleSupport>"
                     "</tt:Analytics><tt:Events xsi:type=\"tt:EventCapabilities\">"
                     "<tt:XAddr>http://192.168.1.51:8004</tt:XAddr>"
                     "<tt:WSSubscriptionPolicySupport>false</tt:WSSubscriptionPolicySupport>"
                     "<tt:WSPullPointSupport>false</tt:WSPullPointSupport>"
                     "<tt:WSPausableSubscriptionManagerInterfaceSupport>false</tt:WSPausableSubscriptionManagerInterfaceSupport>"
                     "</tt:Events><tt:Imaging xsi:type=\"tt:ImagingCapabilities\">"
                     "<tt:XAddr>http://192.168.1.51:8004</tt:XAddr></tt:Imaging>"
                     "<tt:Media xsi:type=\"tt:MediaCapabilities\">"
                     "<tt:XAddr>http://192.168.1.51:8004</tt:XAddr>"
                     "<tt:StreamingCapabilities xsi:type=\"tt:RealTimeStreamingCapabilities\"></tt:StreamingCapabilities>"
                     "</tt:Media><tt:PTZ xsi:type=\"tt:PTZCapabilities\"><tt:XAddr>http://192.168.1.51:8004</tt:XAddr></tt:PTZ>"
                     "<tt:Extension xsi:type=\"tt:CapabilitiesExtension\"><tt:AnalyticsDevice xsi:type=\"tt:AnalyticsDeviceCapabilities\">"
                     "<tt:XAddr>http://192.168.1.51:8004</tt:XAddr></tt:AnalyticsDevice></tt:Extension></devicews:Capabilities>"
                     "</devicews:GetCapabilitiesResponse>"
                     "</SOAP-ENV:Body></SOAP-ENV:Envelope>";

    QString soaptext0="<?xml version=\"1.0\" encoding=\"utf-8\"?><Envelope xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns=\"http://www.w3.org/2003/05/soap-envelope\"><Header><wsa:MessageID xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">uuid:3bef87b7-426d-4440-a770-820b3994e9b7</wsa:MessageID><wsa:To xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To><wsa:Action xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</wsa:Action></Header><Body><Probe xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"><Types>tds:Device</Types><Scopes /></Probe></Body></Envelope>";


    QTextStream text(&soaptext0);
    QSoapMessage SoapMessage;
    text>>SoapMessage;

    QString soaptext2;
    QTextStream text2(&soaptext2);
    text2<<SoapMessage;

    ui->teLog->append("\n");
    ui->teLog->append(soaptext2);
    */

    QStringList ipStrs;

    /*
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){

            qDebug() << address.toString()<<" isLoopback:"<<address.isLoopback()<<" isMulticast:"<<address.isMulticast();
            ipStrs << address.toString();            
        }
    }
    */
    foreach(const QNetworkInterface &interfaceItem, QNetworkInterface::allInterfaces())
    {
        if(interfaceItem.flags().testFlag(QNetworkInterface::IsUp)
                &&interfaceItem.flags().testFlag(QNetworkInterface::IsRunning)
                //&&interfaceItem.flags().testFlag(QNetworkInterface::CanBroadcast)
                //&&interfaceItem.flags().testFlag(QNetworkInterface::CanMulticast)
                &&!interfaceItem.flags().testFlag(QNetworkInterface::IsLoopBack)
                &&!interfaceItem.humanReadableName().contains("VMware")
                &&!interfaceItem.humanReadableName().contains("Tunneling")
                )
        {
            qDebug()<<"Adapter Name:"<<interfaceItem.name()<<" "<<interfaceItem.humanReadableName();
            qDebug()<<"Adapter Mac:"<<interfaceItem.hardwareAddress();

            QList<QNetworkAddressEntry> addressEntryList=interfaceItem.addressEntries();
            foreach(QNetworkAddressEntry addressEntryItem, addressEntryList)
            {
                const QHostAddress &address=addressEntryItem.ip();
                qDebug()<<"Adapter Address:"<<address.toString();
                if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                {
                    ipStrs << address.toString();
                }
            }
        }
    }

    if ( ipStrs.isEmpty()){
        qDebug() << "Server ip not selected";
        //return ;
    }
    QString localIP="127.0.0.1";
    if(ipStrs.length()==1) localIP=ipStrs[0];
    else
    {
        foreach (QString ipitem , ipStrs)
        {
            if(!ipitem.contains("169."))
                localIP=ipitem;
        }
        if(localIP.length()<1)
            localIP="127.0.0.1";
    }


    QString DevUrl="http://";
    DevUrl+=localIP+":"+ui->lePort->text();
    QString DevMedUrl="rtsp://";
    DevMedUrl+=localIP+"/onvif";


    if(onvif==NULL)
    {
        onvif=new QSoapServer();
        onvif->UserName=this->ui->leUser->text();
        onvif->Password=this->ui->lePwd->text();
        if(onvif->listen(QHostAddress::Any, qint16(ui->lePort->text().toInt())))
        {
            ui->teLog->append("soap 服务已经启动:");
            ui->teLog->append(DevUrl);

            onvif->m_dev.anaXAddr=DevUrl;
            onvif->m_dev.eveXAddr=DevUrl;
            onvif->m_dev.ext_adXAddr=DevUrl;
            onvif->m_dev.imgXAddr=DevUrl;
            onvif->m_dev.medXAddr=DevUrl;//DevMedUrl;
            onvif->m_dev.ptzXAddr=DevUrl;
            onvif->m_media.StreamUri=DevMedUrl;
            onvif->m_media.SnapshotUri=DevUrl+"/Snapshot";
        }else
        {
            ui->teLog->append("soap 服务启动失败!");
        }
    }

    if(mediaServer==NULL)
        mediaServer=new QMediaServer();

    if(mediaServer->Start("onvif")==0)
    {
        //qDebug()<<"Media Server started";
        ui->teLog->append("Media Server started 服务已经启动:");
        ui->teLog->append(DevMedUrl);
    }


    if(ds==NULL)
    {
        ds=new QSoapDiscoveryService();
    }

    ds->SetDevUrl(DevUrl);
    if(ds->Start())
    {
        ui->teLog->append("Discovery Service 服务已经启动!");
    }else
    {
        ui->teLog->append("Discovery Service 服务启动失败!");
    }
    //ds->Probe();

}

