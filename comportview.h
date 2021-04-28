#ifndef COMPORTVIEW_H
#define COMPORTVIEW_H

#include "comport.h"
#include <QWidget>

namespace Ui {
class ComPortView;
}

class ComPortView : public QWidget
{
    Q_OBJECT

public:
    explicit ComPortView(QWidget *parent = nullptr);
    ~ComPortView();

    void setHeader(QString header);
    ComPort* getComPort();

signals:
    void comConnectClicked(const QString &portName,
                           const QByteArray &settings);
    void comDisconnectClicked();

public slots:
    void onPortError(const QString &msg);
    void onConnected(bool isOpened);
    void onTabChanged(int index);


private slots:
    void on_comConnButton_clicked();

private:
    Ui::ComPortView *ui;

    ComPort *comPort;
    QThread *threadComPort;
    bool isPortConnected = false;
    int currentTab;

    void updatePortList();
};

#endif // COMPORTVIEW_H
