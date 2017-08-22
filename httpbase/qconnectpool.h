#ifndef QCONNECTPOOL_H
#define QCONNECTPOOL_H

#include <QObject>
#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include<QDebug>
#include<QSettings>//配置文件

class QConnectPool : public QObject{
    Q_OBJECT
public:
    explicit QConnectPool(QHash<QString,QVariant>&,QObject *parent = 0);

    QSqlDatabase openSession();                 // 获取数据库连接
    void closeSession(QSqlDatabase connection); // 释放数据库连接回连接池

    ~QConnectPool();

private:
    //不允许复制和赋值
    QConnectPool(QObject *parent = 0);
    QConnectPool(const QConnectPool &other,QObject *parent = 0);
    QConnectPool& operator=(const QConnectPool &other);

    QSqlDatabase createConnection(const QString &connectionName); // 创建数据库连接

    QQueue<QString> usedConnectionNames;   // 已使用的数据库连接名
    QQueue<QString> unusedConnectionNames; // 未使用的数据库连接名

    // 数据库信息
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;

    bool    testOnBorrow;    // 取得连接的时候验证连接是否有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL

    int maxWaitTime;  // 获取连接最大等待时间
    int waitInterval; // 尝试获取连接时等待间隔时间
    int maxConnectionCount; // 最大连接数

    static QMutex mutex;
    static QWaitCondition waitConnection;    
};

/* 使用方法：
 *
    QSqlDatabase db = openSession();// 从数据库连接池里取得连接
    QSqlQuery query(db);
    query.exec("SELECT * FROM user where id=1");

    while (query.next()) {
        qDebug() << query.value("username").toString();
    }
    closeSession(db);// 连接使用完后需要释放回数据库连接池

    在程序推出之前必须释放数据库 release(); // 程序结束时关闭连接，以免造成连接泄漏
 * */
#endif // QCONNECTPOOL_H
