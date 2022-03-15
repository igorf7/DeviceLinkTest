#ifndef MULTPOLLVIEW_H
#define MULTPOLLVIEW_H

#include "tcpclient.h"
#include <QWidget>
#include <QFile>

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
    int getClientNum();
    void setClientNum(int num);

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
    void on_singleButton_clicked();

private:
    Ui::TcpWorkView *ui;

    QFile logFile;

    bool isPollingRun;
    bool isTcpConnected = false;
    int currentTab;
    int clientNum;

    void sendPacket();
    void createNewFile();
};

#endif // MULTPOLLVIEW_H
