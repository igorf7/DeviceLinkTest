#include "tcpworkview.h"
#include "ui_tcpworkview.h"
#include <QDebug>
#include <QDir>
#include <QDate>

/**
 * @brief TcpWorkView::TcpWorkView
 * @param parent
 * @param client
 */
TcpWorkView::TcpWorkView(QWidget *parent, TcpClient *client) :
    QWidget(parent), ui(new Ui::TcpWorkView)
{
    ui->setupUi(this);

    isPollingRun = false;

    QObject::connect(this, &TcpWorkView::sendToServer,
                         client, &TcpClient::onSendToServer);

    QObject::connect(client, &TcpClient::confirmTcpConnection,
                         this, &TcpWorkView::onConfirmTcpConnection);

    QObject::connect(client, &TcpClient::showResponse,
                         this, &TcpWorkView::onShowResponse);

    qDebug() << "Hello from" << this;
}

/**
 * @brief TcpWorkView::~TcpWorkView
 */
TcpWorkView::~TcpWorkView()
{
    qDebug() << "By-by from" << this;
    delete ui;
}

/**
 * @brief TcpWorkView::setHeader
 * @param header
 */
void TcpWorkView::setHeader(QString header)
{
    ui->workHeadLabel->setText(header);
}

/**
 * @brief TcpWorkView::onTabChanged
 * @param index
 */
void TcpWorkView::onTabChanged(int index)
{
    currentTab = index;
}

/**
 * @brief TcpWorkView::timerEvent
 * @param event
 */
void TcpWorkView::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == pollingPeriod) {
        this->sendPacket();
    }
}

/**
 * @brief TcpWorkView::sendPacket
 */
void TcpWorkView::sendPacket()
{
    QByteArray array;

    array.append(ui->reqLineEdit->text().toUtf8());

    if (ui->crTcpCheckBox->isChecked()) {
        array.append('\n');
    }
    emit sendToServer(array);

    ui->txLcdNumber->display(ui->txLcdNumber->value()+1);
}

/**
 * @brief TcpWorkView::on_pollStartButton_clicked
 */
void TcpWorkView::on_pollStartButton_clicked()
{
    if (!isTcpConnected) {
        return;
    }

    if (isPollingRun) {
        ui->pollStartButton->setText("Start");
        isPollingRun = false;
        ui->pollPeridSpinbox->setEnabled(true);
        this->killTimer(pollingPeriod);
    }
    else {
        ui->txLcdNumber->display(0);
        ui->rxLcdNumber->display(0);
        ui->pollStartButton->setText("Stop");
        isPollingRun = true;
        this->sendPacket();
        pollingPeriod = this->startTimer(ui->pollPeridSpinbox->value() * 1000);
        ui->pollPeridSpinbox->setEnabled(false);
    }
}

/**
 * @brief TcpWorkView::onConfirmTcpConnection
 * @param isConnected
 */
void TcpWorkView::onConfirmTcpConnection(bool isConnected)
{
    if (isPollingRun) {
        ui->pollStartButton->setText("Start");
        isPollingRun = false;
        ui->pollPeridSpinbox->setEnabled(true);
        this->killTimer(pollingPeriod);
    }
    isTcpConnected = isConnected;
}

/**
 * @brief TcpWorkView::onShowResponse
 * @param response
 */
void TcpWorkView::onShowResponse(const QByteArray &response)
{
    QString showString(response);

    ui->respLineEdit->setText(showString.toUtf8());
    showString.clear();
    showString.append(response.toHex());
    ui->respLineEditHex->setText(showString.toUtf8());
    ui->rxLcdNumber->display(ui->rxLcdNumber->value()+1);
}
