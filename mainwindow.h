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
    void readClient();

private:
    void serverStartListen(QHostAddress host = QHostAddress::LocalHost);
    qint32 arrayToInt(QByteArray source);
    QByteArray intToArray(qint32 source);
    void sendToClients(QByteArray &data);

    QTcpServer *server;
    QList <QTcpSocket *> *clients;
    QHash<QTcpSocket*, QByteArray*> *buffers;
    QHash<QTcpSocket*, qint32*> *sizes;
    QPainterPath *picture;
    QLabel *server_log;
};

#endif // MAINWINDOW_H
