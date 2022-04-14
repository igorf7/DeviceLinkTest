#include "comworkview.h"
#include "ui_comworkview.h"
#include <QDir>
#include <QDateTime>
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

    connect(pComPort, &ComPort::connected, this, &ComWorkView::onConnected);
    connect(pComPort, &ComPort::showResponse, this, &ComWorkView::onShowResponse);
    connect(this, &ComWorkView::sendToComPort, pComPort, &ComPort::onSendToComPort);

    ui->eolCharComboBox->setCurrentIndex(1);
    ui->deleteEolCheckBox->setChecked(true);
}

/**
 * @brief ComWorkView::~ComWorkView
 */
ComWorkView::~ComWorkView()
{
    if (logFile.isOpen()) {
        logFile.close();
    }

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

int ComWorkView::getClientNum()
{
    return clientNum;
}

void ComWorkView::setClientNum(int num)
{
    clientNum = num;
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

    if (ui->deleteEolCheckBox->isChecked()) {
        if (showString.right(1) == '\r') {
            showString.chop(1);
        }
        if (showString.right(1) == '\n') {
            showString.chop(1);
        }
        if (showString.right(1) == '\r') {
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

    emit sendToComPort(array);

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
 * @brief ComWorkView::on_singleButton_clicked
 */
void ComWorkView::on_singleButton_clicked()
{
    if (!isComOpened) {
        return;
    }

    this->sendPacket();
}

void ComWorkView::createNewFile()
{
    QString LogDir = QDir::currentPath() + "/Log";

    QDir folder = LogDir;

    if (!folder.exists()) {
        folder.mkdir(LogDir);
    }
    folder.setPath(LogDir = folder.path() + "/ComLog" + QString::number(clientNum));
    if (!folder.exists()) {
        folder.mkdir(LogDir);
    }

    QString filename = LogDir + "/com" + QString::number(clientNum) + "log_";
    filename.append((QDate::currentDate().toString("yyyyMMdd_")));
    filename.append(QTime::currentTime().toString("hhmmss").append(".txt"));
    logFile.setFileName(filename);

    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Could not open file for append";
        return;
    }
}
