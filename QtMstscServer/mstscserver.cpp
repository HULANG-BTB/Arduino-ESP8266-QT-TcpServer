#include "mstscserver.h"
#include "ui_mstscserver.h"

#include <QDebug>

MstscServer::MstscServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MstscServer)
{
    ui->setupUi(this);
    this->Server = new QTcpServer();
    this->Socket = new QTcpSocket();
    this->ClientList = new QList<QTcpSocket*>();
    connect(this->Server, SIGNAL(newConnection()), this, SLOT(ClientConnect()));
    Time = QDateTime::currentDateTime();
    ui->logs->append(tr("(%1)正在启动服务...").arg(Time.toString("yyyy.MM.dd hh:mm:ss")));
    Time = QDateTime::currentDateTime();
    ui->logs->append(tr("(%1)本机IP地址：%2").arg(Time.toString("yyyy.MM.dd hh:mm:ss")).arg(QNetworkInterface().allAddresses().at(1).toString()));
    Time = QDateTime::currentDateTime();
    ui->logs->append(tr("(%1)正在设置监听端口...").arg(Time.toString("yyyy.MM.dd hh:mm:ss")));
    if ( Server->listen(QHostAddress::Any, 5880) ) {
        Time = QDateTime::currentDateTime();
        ui->logs->append(tr("(%1)监听端口 5880").arg(Time.toString("yyyy.MM.dd hh:mm:ss")));
    } else {
        Time = QDateTime::currentDateTime();
        ui->logs->append(tr("(%1)监听端口失败，请检查端口是否被占用！").arg(Time.toString("yyyy.MM.dd hh:mm:ss")));
    }
}

 void MstscServer::ClientConnect() {
     this->Socket = this->Server->nextPendingConnection();
     QString ip = this->Socket->peerAddress().toString().split("::ffff:")[1];
     qint64 port = this->Socket->peerPort();
     this->ClientList->append(this->Socket);
     Time = QDateTime::currentDateTime();
     ui->logs->append(tr("(%1)客户端连接：%2:%3").arg(Time.toString("yyyy.MM.dd hh:mm:ss")).arg(ip).arg(port));
     connect(this->Socket, SIGNAL(readyRead()), this, SLOT(ClientReadData()));
     connect(this->Socket, SIGNAL(disconnected()), this, SLOT(ClientDisConnect()));
}

 void MstscServer::ClientReadData() {
     this->Socket = (QTcpSocket*)sender();
     QString line = QString::fromUtf8(this->Socket->readAll()).trimmed();
     for(QList<QTcpSocket*>::iterator it = this->ClientList->begin(); it != this->ClientList->end(); it++ ) {
         QTcpSocket* Client = (*it);
         Client->write(QString(line+"\r").toUtf8());
         Time = QDateTime::currentDateTime();
         QString ip = Client->peerAddress().toString().split("::ffff:")[1];
         qint64 port = Client->peerPort();
         ui->logs->append(tr("(%1)发送数据：%2:%3:%4").arg(Time.toString("yyyy.MM.dd hh:mm:ss")).arg(ip).arg(port).arg(line));
     }
 }

 void MstscServer::ClientDisConnect() {
     this->Socket = (QTcpSocket*)sender();
     QString ip = this->Socket->peerAddress().toString().split("::ffff:")[1];
     qint64 port = this->Socket->peerPort();
     Time = QDateTime::currentDateTime();
     this->ClientList->removeAll(this->Socket);
     ui->logs->append(tr("(%1)客户端断开：%2:%3").arg(Time.toString("yyyy.MM.dd hh:mm:ss")).arg(ip).arg(port));
 }

MstscServer::~MstscServer()
{
    delete this->Socket;
    delete this->Server;
    delete this->ClientList;
    delete ui;
}
