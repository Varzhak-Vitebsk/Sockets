#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), server(new QTcpServer(this)), clients(new QList <QTcpSocket *> ())
    , picture(new QPainterPath())
{
    setMinimumSize(800, 600);
    server_log = new QLabel(this);
    setCentralWidget(server_log);
    connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));
    serverStartListen();
}

MainWindow::~MainWindow()
{
    server->close();
}

void MainWindow::newClient()
{
    clients->push_back(server->nextPendingConnection());
    server_log->setText(server_log->text()
                        + "\n" + "New connection established: client - "
                        + clients->back()->peerName() + ", address - "
                        + clients->back()->peerAddress().toString()
                        + ", port - " + QString::number(clients->back()->peerPort()));
}

void MainWindow::serverStartListen(QHostAddress host)
{
    server_log->setText(server_log->text()
                        + "Starting server...");
    //dynamic ports 49152–65535
    for(quint16 port = 49152; port != 65535; ++port)
    {
        if(server->listen(host, port))
        {
            server_log->setText(server_log->text()
                                + "\n" + "Server is online: host - "
                                + host.toString() + ", port - "
                                + QString::number(port));
            return;
        }
    }
    server_log->setText(server_log->text()
                        + "\n" + "No free port");
}
