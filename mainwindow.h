#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtNetwork>
#include <QList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newClient();

private:
    void serverStartListen(QHostAddress host = QHostAddress::LocalHost);
    QTcpServer *server;
    QList <QTcpSocket *> *clients;
    QPainterPath *picture;
    QLabel *server_log;
};

#endif // MAINWINDOW_H
