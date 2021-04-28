#ifndef BARPOLLVIEW_H
#define BARPOLLVIEW_H

#include "comport.h"
#include <QWidget>

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

signals:

public slots:
    void onConnected(bool isOpened);
    void onShowResponse(const QByteArray &response);
    void onTabChanged(int index);

private slots:
    void on_pollStartButton_clicked();

protected:
    qint32 pollingPeriod;
    void timerEvent(QTimerEvent *event);

private:
    Ui::ComWorkView *ui;

    ComPort *pComPort = nullptr;

    bool isPollingRun;
    bool isComOpened = false;
    int currentTab;


    void sendPacket();
};

#endif // BARPOLLVIEW_H
