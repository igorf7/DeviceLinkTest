#include "comportview.h"
#include "ui_comportview.h"
#include <QThread>
#include <QDebug>
#include <QMessageBox>

/**
 * @brief Class constructor
 * @param parent
 */
ComPortView::ComPortView(QWidget *parent) :
    QWidget(parent), ui(new Ui::ComPortView)
{
    ui->setupUi(this);

    comPort = new ComPort();

    threadComPort = new QThread;
    threadComPort->setObjectName("threadComPort");
    comPort->moveToThread(threadComPort);
    comPort->serialPort.moveToThread(threadComPort);

    QObject::connect(threadComPort, &QThread::started, comPort, &ComPort::onPortStart);
    QObject::connect(threadComPort, &QThread::finished, comPort, &ComPort::deleteLater);
    QObject::connect(comPort, &ComPort::quitComPort, threadComPort, &QThread::deleteLater);

    QObject::connect(this, &ComPortView::comConnectClicked, comPort, &ComPort::onConnectClicked);
    QObject::connect(this, &ComPortView::comDisconnectClicked, comPort, &ComPort::onDisconnectComPort);
    QObject::connect(comPort, &ComPort::portError, this, &ComPortView::onPortError);
    QObject::connect(comPort, &ComPort::connected, this, &ComPortView::onConnected);

    threadComPort->start(); // Start Com Port thread
    this->updatePortList();

    qDebug() << "Hello from" <<this;
}

/**
 * @brief Class dectructor
 */
ComPortView::~ComPortView()
{
    if (threadComPort->isRunning()) {
        threadComPort->quit();
        threadComPort->deleteLater();
    }

    qDebug() << "By-by from" << this;
    delete ui;
}

/**
 * @brief ComPortView::setHeader
 * @param header
 */
void ComPortView::setHeader(QString header)
{
    ui->comHeadLabel->setText(header);
}

/**
 * @brief ComPortView::getComPort
 * @return
 */
ComPort* ComPortView::getComPort()
{
    return comPort;
}

/**
 * @brief ComPortView::onTabChanged
 * @param index
 */
void ComPortView::onTabChanged(int index)
{
    currentTab = index;
}

/**
 * @brief ComPortView::updatePortList
 */
void ComPortView::updatePortList()
{
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    while (infoList.size() > 0) {
        QSerialPortInfo info = infoList.takeFirst();
        ui->comPortCombo->addItem(info.portName());
    }
}

/**
 * @brief ComPortView::on_comConnButton_clicked
 */
void ComPortView::on_comConnButton_clicked()
{
    if (isPortConnected) {
        emit comDisconnectClicked();
    }
    else {
        QByteArray comSettings;
        comSettings.append(ui->comSpeedCombo->currentIndex());
        comSettings.append(ui->comBitCombo->currentIndex());
        comSettings.append(ui->comStopbitCombo->currentIndex());
        comSettings.append(ui->comParityCombo->currentIndex());

        emit comConnectClicked(ui->comPortCombo->currentText(), comSettings);
    }
}

/**
 * @brief ComPortView::onPortError
 * @param msg
 */
void ComPortView::onPortError(const QString &msg)
{
    QMessageBox::warning(this, tr("Connection"), msg);
}

/**
 * @brief ComPortView::onConfirmComConnection
 * @param isOpened
 */
void ComPortView::onConnected(bool isOpened)
{
    if (isOpened) {
        ui->comConnButton->setText("Disconnect");
        ui->statusLineEdit->setText("Connected");
    }
    else {
        ui->comConnButton->setText("Connect");
        ui->statusLineEdit->setText("Disconnected");
    }
    isPortConnected = isOpened;
}

