#include "qmultidbmanager.h"

QMutex QMultiDbManager::mutex;
QHash<QString,QConnectPool*>  QMultiDbManager::m_DbList;

QMultiDbManager::QMultiDbManager(QObject *parent) : QObject(parent)
{
    //从数据库配置文件/config/dbconfig.ini中加载
    QString binDir=QCoreApplication::applicationDirPath();
    QString fileName("dbconfig.ini");
    QFile file(binDir+"/config/"+fileName);
    if (file.exists())
    {
        QString configFileName=binDir+"/config/"+fileName;
        QSettings settings(configFileName,QSettings::IniFormat);

        QStringList groups=settings.childGroups();
        for(int i = 0; i < groups.size(); i++)
        {
            settings.beginGroup(groups[i]);

            /*
            hostName     = "localhost";//主机名
            databaseName = "quick";//需要访问的数据库
            username     = "root";//用户名
            password     = "****";//密码
            databaseType = "QMYSQL";//数据库类型
            */

            QHash<QString,QVariant> param ;
            param["host"]=settings.value("host","localhost");
            param["database"]=settings.value("database","").toString();
            param["username"]=settings.value("username","root").toString();
            param["password"]=settings.value("password","").toString();
            param["dbtype"]=settings.value("dbtype","QMYSQL").toString();
            //缺省1000个连接
            param["maxConnectionCount"]=settings.value("maxConnectionCount","1000").toInt();
            settings.endGroup();

            QConnectPool* db=new QConnectPool(param);

            if(!m_DbList.contains(groups[i]))
            {
                m_DbList[groups[i]]=db;
            }
        }
    }
}

QMultiDbManager::~QMultiDbManager()
{
    QHash<QString,QConnectPool*>::const_iterator i;
    for (i = m_DbList.constBegin(); i != m_DbList.constEnd(); ++i)
    {
        i.value()->deleteLater();
    }
}

QConnectPool* QMultiDbManager::getDb(QString DbName)
{
    QMutexLocker locker(&mutex);
    if(m_DbList.contains(DbName))
        return m_DbList[DbName];
    else
        return NULL;
}
