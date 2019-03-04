#ifndef MSTSCSERVER_H
#define MSTSCSERVER_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QNetworkInterface>
#include <QList>
#include <QDateTime>

namespace Ui {
class MstscServer;
}

class MstscServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MstscServer(QWidget *parent = nullptr);
    ~MstscServer();

private slots:
    void ClientConnect();
    void ClientDisConnect();
    void ClientReadData();

private:
    Ui::MstscServer *ui;
    QTcpSocket *Socket;
    QTcpServer *Server;
    QList<QTcpSocket*> *ClientList;
    QDateTime Time;

};

#endif // MSTSCSERVER_H
