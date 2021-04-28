#ifndef MULTPOLLVIEW_H
#define MULTPOLLVIEW_H

#include "tcpclient.h"
#include <QWidget>

namespace Ui {
class TcpWorkView;
}

class TcpWorkView : public QWidget
{
    Q_OBJECT

public:
    explicit TcpWorkView(QWidget *parent = nullptr);
    explicit TcpWorkView(QWidget *parent = nullptr,
                         TcpClient *client = nullptr);
    ~TcpWorkView();

    void setHeader(QString header);

signals:
    void sendToServer(const QByteArray &msg);

public slots:
    void onShowResponse(const QByteArray &response);
    void onConfirmTcpConnection(bool isConnected);
    void onTabChanged(int index);

protected:
    qint32 pollingPeriod;
    void timerEvent(QTimerEvent *event);

private slots:
    void on_pollStartButton_clicked();

private:
    Ui::TcpWorkView *ui;

    bool isPollingRun;
    bool isTcpConnected = false;
    int currentTab;

    void sendPacket();
};

#endif // MULTPOLLVIEW_H
