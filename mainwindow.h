#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "httpbase/qhttpserver.h"
#include "httpbase/qhttprequest.h"
#include "httpbase/qhttpresponse.h"
#include "soap/qsoapdiscoveryservice.h"
#include "soap/qsoapserver.h"
#include "media/qmediaserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbStart_clicked();
    void on_pbsoaptest_clicked();

private:
    Ui::MainWindow *ui;

    QHttpServer *server;


    QSoapDiscoveryService* ds;
    QSoapServer * onvif;
    QMediaServer * mediaServer;
};


#endif // MAINWINDOW_H
