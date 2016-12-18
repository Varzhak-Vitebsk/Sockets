#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), server(new QTcpServer(this))
    , clients(new QList<QTcpSocket *>())
    , buffers(new QHash<QTcpSocket *, QByteArray *>())
    , sizes(new QHash<QTcpSocket *, qint32 *>())
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
    connect(clients->back(), SIGNAL(readyRead()), this, SLOT(readClient()));
    QByteArray *buffer = new QByteArray();
    qint32 *s = new qint32(0);
    buffers->insert(clients->back(), buffer);
    sizes->insert(clients->back(), s);
}

void MainWindow::readClient()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers->value(socket);
    qint32 *s = sizes->value(socket);
    qint32 size = *s;
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size))
        {
            if (size == 0 && buffer->size() >= 4)
            {
                size = arrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size)
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                sendToClients(data);
            }
        }
    }
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

qint32 MainWindow::arrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray MainWindow::intToArray(qint32 source)
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void MainWindow::sendToClients(QByteArray &data)
{

    for(auto i = clients->begin(); i != clients->end(); ++i)
    {
        (*i)->write(intToArray((data.size())));
        (*i)->write(data);
    }
}
