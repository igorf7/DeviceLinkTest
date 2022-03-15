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

    connect(this, &TcpWorkView::sendToServer,
                         client, &TcpClient::onSendToServer);

    connect(client, &TcpClient::confirmTcpConnection,
                         this, &TcpWorkView::onConfirmTcpConnection);

    connect(client, &TcpClient::showResponse,
                         this, &TcpWorkView::onShowResponse);

    ui->eolCharComboBox->setCurrentIndex(0);
    ui->deleteEolCheckBox->setChecked(true);
}

/**
 * @brief TcpWorkView::~TcpWorkView
 */
TcpWorkView::~TcpWorkView()
{
    if (logFile.isOpen()) {
        logFile.close();
    }

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

int TcpWorkView::getClientNum()
{
    return clientNum;
}

void TcpWorkView::setClientNum(int num)
{
    clientNum = num;
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

    switch(ui->eolCharComboBox->currentIndex())
    {
    case 0:
        array.append('\n');
        break;
    case 1:
        array.append('\r');
        break;
    case 2:
        array.append('\r');
        array.append('\n');
        break;
    case 3:
        array.append('\n');
        array.append('\r');
        break;
    default:
        break;
    }

    emit sendToServer(array);

    ui->txLcdNumber->display(ui->txLcdNumber->value()+1);

    if (ui->writeFileCheckBox->isChecked()) {
        if (!logFile.exists()) {
            this->createNewFile();
            qDebug() << "New log file created";
        }
        logFile.write("\nRequest: " + array + '\n');
    }
}

/**
 * @brief ComWorkView::createNewFile
 */
void TcpWorkView::createNewFile()
{
    QString LogDir = QDir::currentPath() + "/Log";

    QDir folder = LogDir;

    if (!folder.exists()) {
        folder.mkdir(LogDir);
    }
    folder.setPath(LogDir = folder.path() + "/TcpLog" + QString::number(clientNum));
    if (!folder.exists()) {
        folder.mkdir(LogDir);
    }

    QString filename = LogDir+"/log_";
    filename.append((QDate::currentDate().toString("yyyyMMdd_")));
    filename.append(QTime::currentTime().toString("hhmmss").append(".txt"));
    logFile.setFileName(filename);

    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Could not open file for append";
        return;
    }
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

    if (ui->deleteEolCheckBox->isChecked()) {
        if (showString.right(1) == '\r') {
            showString.chop(1);
        }
        if (showString.right(1) == '\n') {
            showString.chop(1);
        }
    }

    ui->respLineEdit->setText(showString.toUtf8());
    ui->respLineEditHex->setText(showString.toUtf8().toHex());
    ui->rxLcdNumber->display(ui->rxLcdNumber->value()+1);

    if (ui->writeFileCheckBox->isChecked()) {
        QTextStream filename(&logFile);
        filename << "Response:" << Qt::endl;
        filename << "ASCII => " << ui->respLineEdit->text() << Qt::endl;
        filename << "Hex => " << ui->respLineEditHex->text() << Qt::endl;
    }
}

/**
 * @brief TcpWorkView::on_singleButton_clicked
 */
void TcpWorkView::on_singleButton_clicked()
{
    if (!isTcpConnected) {
        return;
    }

    this->sendPacket();
}
