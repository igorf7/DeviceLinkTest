#ifndef BARPOLLVIEW_H
#define BARPOLLVIEW_H

#include "comport.h"
#include <QWidget>
#include <QFile>

namespace Ui {
class ComWorkView;
}

class ComWorkView : public QWidget
{
    Q_OBJECT

public:
    explicit ComWorkView(QWidget *parent = nullptr);
    explicit ComWorkView(QWidget *parent = nullptr,
                         ComPort *port = nullptr);
    ~ComWorkView();

    void setHeader(QString header);
    int getClientNum();
    void setClientNum(int num);

signals:
    void sendToComPort(const QByteArray &request);

public slots:
    void onConnected(bool isOpened);
    void onShowResponse(const QByteArray &response);
    void onTabChanged(int index);

private slots:
    void on_pollStartButton_clicked();
    void on_singleButton_clicked();

protected:
    qint32 pollingPeriod;
    void timerEvent(QTimerEvent *event);

private:
    Ui::ComWorkView *ui;

    QFile logFile;
    ComPort *pComPort = nullptr;

    bool isPollingRun;
    bool isComOpened = false;
    int currentTab;
    int clientNum;

    void sendPacket();
    void createNewFile();
};

#endif // BARPOLLVIEW_H
