#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class ComPort : public QObject
{
    Q_OBJECT
public:
    ComPort(QObject *parent = nullptr);

    ~ComPort();

    QSerialPort serialPort;

    void sendToComPort(const QByteArray &msg);

signals:
    void portError(const QString &msg);
    void quitComPort();
    void connected(bool status);
    void showResponse(const QByteArray &response);

public slots:
    void onPortStart();
    void onDisconnectComPort();
    void onConnectClicked(const QString &portName,
                          const QByteArray &settings);

private slots:
    void onReadyRead();

private:
    QString portName;

    QList<QSerialPort::BaudRate> comBoudrate;
    QList<QSerialPort::DataBits> comDatabits;
    QList<QSerialPort::StopBits> comStopbits;
    QList<QSerialPort::Parity> comParity;
};

#endif // COMPORT_H
//eof
