#ifndef QMULTIDBMANAGER_H
#define QMULTIDBMANAGER_H

#include <QObject>
#include <QHash>
#include "qconnectpool.h"

/* 多数据库连接池管理器
 * 从配置文件/config/dbconfig.ini中加载多个数据库配置，
 * */
class QMultiDbManager : public QObject
{
    Q_OBJECT
public:
    explicit QMultiDbManager(QObject *parent = 0);
    ~QMultiDbManager();

    //根据数据库分组名DbName获取该数据库连接池
    static QConnectPool* getDb(QString DbName);
signals:

public slots:

private:
    static QHash<QString,QConnectPool*> m_DbList;

    static QMutex mutex;
};

#endif // QMULTIDBMANAGER_H
