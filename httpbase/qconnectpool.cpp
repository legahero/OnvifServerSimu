#include "qconnectpool.h"

QMutex QConnectPool::mutex;
QWaitCondition QConnectPool::waitConnection;

QConnectPool::QConnectPool(QHash<QString,QVariant> &param,QObject *parent) : QObject(parent)
{

    QHash<QString, QVariant>::const_iterator value;

    for (value = param.constBegin(); value != param.constEnd(); ++value)
    {
        hostName=param.value("host").toString();
        databaseName=param.value("database").toString();
        username=param.value("username").toString();
        password=param.value("password").toString();
        databaseType=param.value("dbtype").toString();

        //缺省1000个连接
        if(param.contains("maxConnectionCount"))
            maxConnectionCount=param.value("maxConnectionCount").toInt();
        else
            maxConnectionCount=1000;
    }

    testOnBorrow = true;
    testOnBorrowSql = "SELECT 1";

    maxWaitTime  = 1000;
    waitInterval = 200;

}

QConnectPool::~QConnectPool() {
    // 销毁连接池的时候删除所有的连接
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

QSqlDatabase QConnectPool::openSession() {

    QString connectionName;

    QMutexLocker locker(&mutex);

    // 已创建连接数
    int connectionCount = unusedConnectionNames.size() + usedConnectionNames.size();

    // 如果连接已经用完，等待 waitInterval 毫秒看看是否有可用连接，最长等待 maxWaitTime 毫秒
    for (int i = 0;
         i < maxWaitTime
         && unusedConnectionNames.size() == 0 && connectionCount == maxConnectionCount;
         i += waitInterval) {
        waitConnection.wait(&mutex, waitInterval);

        // 重新计算已创建连接数
        connectionCount = unusedConnectionNames.size() + usedConnectionNames.size();
    }

    if (unusedConnectionNames.size() > 0) {
        // 有已经回收的连接，复用它们
        connectionName = unusedConnectionNames.dequeue();
    } else if (connectionCount < maxConnectionCount) {
        // 没有已经回收的连接，但是没有达到最大连接数，则创建新的连接
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
    } else {
        // 已经达到最大连接数
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }

    // 创建连接
    QSqlDatabase db = createConnection(connectionName);

    // 有效的连接才放入 usedConnectionNames
    if (db.isOpen()) {
        usedConnectionNames.enqueue(connectionName);
    }

    return db;
}

void QConnectPool::closeSession(QSqlDatabase connection) {

    QString connectionName = connection.connectionName();

    // 如果是我们创建的连接，从 used 里删除，放入 unused 里
    if (usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&mutex);
        usedConnectionNames.removeOne(connectionName);
        unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}

QSqlDatabase QConnectPool::createConnection(const QString &connectionName) {
    // 连接已经创建过了，复用它，而不是重新创建
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);

        if (testOnBorrow) {
            // 返回连接前访问数据库，如果连接断开，重新建立连接
            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);

            if (query.lastError().type() != QSqlError::NoError && !db1.open()) {
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                return QSqlDatabase();
            }
        }

        return db1;
    }

    // 创建一个新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    //QString dsn = QString("Driver={SQL Server};server=127.0.0.1\\SQLEX2014,1433;database=OaManageDb4;uid=sa;pwd=13925026627");
    //QString dsn = QString("Provider=SQLOLEDB.1;Password=13925026627;Persist Security Info=True;User ID=sa;Initial Catalog=OaManageDb4;Data Source=LEGAHERO\\sqlex2014");
    //db.setDatabaseName(dsn);
    db.setUserName(username);
    db.setPassword(password);
    //db.setPort(3306);

    if (!db.open()) {
        QString msg=db.lastError().text();
        //qDebug() << "Open datatabase error:" << db.lastError().text();
        qDebug() << "Open datatabase error:" << msg;
        return QSqlDatabase();
    }

    return db;
}

