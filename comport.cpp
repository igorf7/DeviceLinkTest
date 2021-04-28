#include "comport.h"
#include <QDebug>

/**
 * @brief ComPort constructor
 * @param parent
 */
ComPort::ComPort(QObject *parent) : QObject(parent)
{
    comBoudrate << QSerialPort::Baud1200 << QSerialPort::Baud2400
                << QSerialPort::Baud4800 << QSerialPort::Baud9600
                << QSerialPort::Baud19200 << QSerialPort::Baud38400
                << QSerialPort::Baud57600 << QSerialPort::Baud115200;

    comDatabits << QSerialPort::Data5 << QSerialPort::Data6
                << QSerialPort::Data7 << QSerialPort::Data8;

    comStopbits << QSerialPort::OneStop << QSerialPort::OneAndHalfStop
                << QSerialPort::TwoStop;

    comParity << QSerialPort::NoParity << QSerialPort::EvenParity
              << QSerialPort::OddParity << QSerialPort::SpaceParity
              << QSerialPort::MarkParity;

    connect(&serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    qDebug() << "Hello from" << this;
}

/**
 * @brief ComPort destructor
 */
ComPort::~ComPort()
{
    this->onDisconnectComPort();
    emit quitComPort();
    qDebug() << "By-by from" <<this;
}

/**
 * @brief Run at class start
 */
void ComPort:: onPortStart()
{
    //connect(&serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

/**
 * @brief Opens and configures the port
 * @param port - port name
 */
void ComPort:: onConnectClicked(const QString &portName,
                                const QByteArray &settings)
{
    this->portName = portName;
    serialPort.setPortName(portName);
    if (!serialPort.isOpen()) {
        if (!serialPort.open(QIODevice::ReadWrite)) {
            if (!QSerialPortInfo::availablePorts().isEmpty())
                emit portError(tr("Port unavailable."));
        }
        else {
            serialPort.setBaudRate(comBoudrate.at(settings.at(0)));
            serialPort.setDataBits(comDatabits.at(settings.at(1)));
            serialPort.setStopBits(comStopbits.at(settings.at(2)));
            serialPort.setParity(comParity.at(settings.at(3)));
            serialPort.setFlowControl(QSerialPort::NoFlowControl);
            emit connected(true);
        }
    }
    else {
        emit portError(tr("This port is already open."));
    }
}

/**
 * @brief Closes the port
 */
void ComPort::onDisconnectComPort()
{
    if (serialPort.isOpen()) {
        serialPort.close();
        emit connected(false);
    }
}

/**
 * @brief Reads the port when the data is ready
 */
void ComPort::onReadyRead()
{
    static QByteArray rxData;

    while( 1 ) {
        rxData.append(serialPort.readAll());
        if (!serialPort.waitForReadyRead(10)) {
            break;
        }
    }

    if (rxData.size() > 0) {
        emit showResponse(rxData);
        rxData.clear();
    }
}

/**
 * @brief Sends data via port
 * @param packet - data packet
 */
void ComPort::sendToComPort(const QByteArray &msg)
{   
    if (!serialPort.isOpen()) {
        emit portError(tr("Port not open"));
    }
    else {
        serialPort.write(msg);
    }
}

