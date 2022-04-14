#include "tcpportview.h"
#include "ui_tcpportview.h"
#include <QDebug>
#include <QThread>
#include <QHostAddress>
#include <QBuffer>

/**
 * @brief Class constructor
 * @param parent
 */
TcpPortView::TcpPortView(QWidget *parent) :
    QWidget(parent), ui(new Ui::TcpPortView)
{
    ui->setupUi(this);

    tcpClient = new TcpClient;

    threadTcpPort = new QThread;
    threadTcpPort->setObjectName("threadTcpPort");
    tcpClient->moveToThread(threadTcpPort);

    QObject::connect(threadTcpPort, &QThread::started,
                     tcpClient, &TcpClient::onClientStart);

    QObject::connect(threadTcpPort, &QThread::finished,
                     tcpClient, &TcpClient::deleteLater);

    QObject::connect(tcpClient, &TcpClient::quitTcpClient,
                     threadTcpPort, &QThread::deleteLater);

    QObject::connect(this, &TcpPortView::setTcpConnection,
                     tcpClient, &TcpClient::onSetTcpConnection);

    QObject::connect(this, &TcpPortView::setTcpDisconnection,
                     tcpClient, &TcpClient::onSetTcpDisconnection);

    QObject::connect(this, &TcpPortView::sendToServer,
                     tcpClient, &TcpClient::onSendToServer);

    QObject::connect(tcpClient, &TcpClient::confirmTcpConnection,
                     this, &TcpPortView::onConfirmTcpConnection);

    threadTcpPort->start(); // Start TCP Port thread
}

/**
 * @brief Class dectructor
 */
TcpPortView::~TcpPortView()
{
    if (threadTcpPort->isRunning()) {
        threadTcpPort->quit();
        threadTcpPort->deleteLater();
    }
    qDebug() << "By-by from" << this;
    delete ui;
}

/**
 * @brief setHeader
 * @param header
 */
void TcpPortView::setHeader(QString header)
{
    ui->tcpHeadLabel->setText(header);
}

/**
 * @brief TcpPortView::getClient
 * @return
 */
TcpClient* TcpPortView::getClient()
{
    return tcpClient;
}

/**
 * @brief TcpPortView::onTabChanged
 * @param index
 */
void TcpPortView::onTabChanged(int index)
{
    currentTab = index;
}

/**
 * @brief TcpPortView::onConfirmTcpConnection
 * @param isConnected
 */
void TcpPortView::onConfirmTcpConnection(bool isConnected)
{
    if (isConnected) {
        ui->tcpConnButton->setText("Disconnect");
        ui->statusLineEdit->setText("Connected");
    }
    else {
        ui->tcpConnButton->setText("Connect");
        ui->statusLineEdit->setText("Disconnected");
    }
    isTcpConnected = isConnected;
}

/**
 * @brief TcpPortView::on_tcpConnButton_clicked
 */
void TcpPortView::on_tcpConnButton_clicked()
{
    if (isTcpConnected) {
        emit setTcpDisconnection();
    }
    else {
        emit setTcpConnection(ui->tcpHostnameEdit->text(),
                                  ui->tcpPortEdit->text().toUShort());
    }
}
