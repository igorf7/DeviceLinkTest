#include "comworkview.h"
#include "ui_comworkview.h"
#include <QDebug>

/**
 * @brief ComWorkView::ComWorkView
 * @param parent
 * @param port
 */
ComWorkView::ComWorkView(QWidget *parent, ComPort *port) :
    QWidget(parent), ui(new Ui::ComWorkView)
{
    ui->setupUi(this);

    pComPort = port;
    isPollingRun = false;

    QObject::connect(pComPort, &ComPort::connected, this, &ComWorkView::onConnected);
    QObject::connect(pComPort, &ComPort::showResponse, this, &ComWorkView::onShowResponse);

    qDebug() << "Hello from" << this;
}

/**
 * @brief ComWorkView::~ComWorkView
 */
ComWorkView::~ComWorkView()
{
    qDebug() << "By-by from" << this;
    delete ui;
}

/**
 * @brief ComWorkView::setHeader
 * @param header
 */
void ComWorkView::setHeader(QString header)
{
    ui->workHeadLabel->setText(header);
}

void ComWorkView::onTabChanged(int index)
{
    currentTab = index;
}

/**
 * @brief ComWorkView::timerEvent
 * @param event
 */
void ComWorkView::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == pollingPeriod) {
        this->sendPacket();
    }
}

/**
 * @brief ComWorkView::onConnected
 * @param isOpened
 */
void ComWorkView::onConnected(bool isOpened)
{
    if (isPollingRun) {
        ui->pollStartButton->setText("Start");
        isPollingRun = false;
        ui->pollPeridSpinbox->setEnabled(true);
        this->killTimer(pollingPeriod);
    }
    isComOpened = isOpened;
}

/**
 * @brief ComWorkView::onShowResponse
 * @param response
 */
void ComWorkView::onShowResponse(const QByteArray &response)
{
    QString showString(response);

    ui->respLineEdit->setText(showString.toUtf8());
    showString.clear();
    showString.append(response.toHex());
    ui->respLineEditHex->setText(showString.toUtf8());
    ui->rxLcdNumber->display(ui->rxLcdNumber->value()+1);
}

/**
 * @brief ComWorkView::on_pollStartButton_clicked
 */
void ComWorkView::on_pollStartButton_clicked()
{
    if (!isComOpened) {
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
 * @brief ComWorkView::sendPacket
 */
void ComWorkView::sendPacket()
{
    QByteArray array;

    array.append(ui->reqLineEdit->text().toUtf8());

    if (ui->crCheckBox->isChecked()) {
        array.append('\n');
    }

    pComPort->sendToComPort(array);
    ui->txLcdNumber->display(ui->txLcdNumber->value()+1);
}
