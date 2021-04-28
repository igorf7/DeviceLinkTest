#ifndef TCPPORTVIEW_H
#define TCPPORTVIEW_H

#include "tcpclient.h"
#include <QWidget>

namespace Ui {
class TcpPortView;
}

class TcpPortView : public QWidget
{
    Q_OBJECT

public:
    explicit TcpPortView(QWidget *parent = nullptr);
    ~TcpPortView();

    void setHeader(QString header);
    TcpClient* getClient();

signals:
    void setTcpConnection(const QString &hostName, quint16 port);
    void setTcpDisconnection();
    void sendToServer(const QByteArray &msg);

public slots:
    void onConfirmTcpConnection(bool isConnected);
    void onTabChanged(int index);

private slots:
    void on_tcpConnButton_clicked();

private:
    Ui::TcpPortView *ui;

    TcpClient *tcpClient;
    QThread *threadTcpPort;
    bool isTcpConnected = false;
    int currentTab;
};

#endif // TCPPORTVIEW_H
