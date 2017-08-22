#ifndef QSOAPFAULT_H
#define QSOAPFAULT_H

#include <QObject>

class QSoapFault : public QObject
{
    Q_OBJECT
public:
    explicit QSoapFault(QObject *parent = 0);

signals:

public slots:
};

#endif // QSOAPFAULT_H