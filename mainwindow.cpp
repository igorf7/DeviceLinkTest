#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

/**
 * @brief Class constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("DeviceLinkTest");

    /* Create and set layouts */
    this->initClientViews(tcpQuantity, comQuantity);

    ui->tabWidget->setCurrentIndex(0);
    ui->ComDeleteEOL->setChecked(true);
    ui->TcpDeleteEOL->setChecked(true);
}

/**
 * @brief Class dectructor
 */
MainWindow::~MainWindow()
{
    qDebug() << "By-by from" << this;
    delete ui;
}

/**
 * @brief MainWindow::initClientViews
 * @param tcp_num
 * @param com_num
 */
void MainWindow::initClientViews(int tcp_num, int com_num)
{
    int rows = 0, cols = 0;

    connTabLayout = new QGridLayout;
    workTabLayout = new QGridLayout;

    tcpQuantity = tcp_num;
    comQuantity = com_num;

    /* Create TCP views */
    for (int i = 0; i < tcpQuantity; ++i) {
        tcpConnList.append(new TcpPortView);
        tcpConnList.at(i)->setHeader("<H2>TCP Connection " + QString::number(i+1) + "</H2>");
        tcpConnList.at(i)->setObjectName(tcpConnList.at(i)->objectName()+QString::number(i+1));
        connTabLayout->addWidget(tcpConnList.at(i), rows, cols);
        tcpPollList.append(new TcpWorkView(this, tcpConnList.at(i)->getClient()));
        tcpPollList.at(i)->setHeader("<H2>TCP Exchange " + QString::number(i+1) + "</H2>");
        tcpPollList.at(i)->setObjectName(tcpPollList.at(i)->objectName()+QString::number(i+1));
        workTabLayout->addWidget(tcpPollList.at(i), rows, cols);
        tcpPollList.at(i)->setClientNum(i+1);
        connect(ui->tabWidget, SIGNAL(currentChanged(int)), tcpConnList.at(i), SLOT(onTabChanged(int)));
        connect(ui->tabWidget, SIGNAL(currentChanged(int)), tcpPollList.at(i), SLOT(onTabChanged(int)));

        if (++cols > (tcpMaximum-1)) {
            rows += 1; cols = 0;
        }
    }
    /* Create COM views */
    for (int i = 0; i < comQuantity; ++i) {
        comConnList.append(new ComPortView);
        comConnList.at(i)->setHeader("<H2>COM Connection " + QString::number(i+1) + "</H2>");
        comConnList.at(i)->setObjectName(comConnList.at(i)->objectName()+QString::number(i+1));
        connTabLayout->addWidget(comConnList.at(i), rows, cols);
        comPollList.append(new ComWorkView(this, comConnList.at(i)->getComPort()));
        comPollList.at(i)->setHeader("<H2>COM Exchange " + QString::number(i+1) + "</H2>");
        comPollList.at(i)->setObjectName(comPollList.at(i)->objectName()+QString::number(i+1));
        workTabLayout->addWidget(comPollList.at(i), rows, cols);
        comPollList.at(i)->setClientNum(i+1);
        connect(ui->tabWidget, SIGNAL(currentChanged(int)), comConnList.at(i), SLOT(onTabChanged(int)));
        connect(ui->tabWidget, SIGNAL(currentChanged(int)), comPollList.at(i), SLOT(onTabChanged(int)));

        if (++cols > (comMaximum-1)) {
            rows += 1; cols = 0;
        }
    }
    /* Set layouts */
    ui->tabWidget->widget(0)->setLayout(connTabLayout);
    ui->tabWidget->widget(1)->setLayout(workTabLayout);
}

/**
 * @brief MainWindow::deinitClientViews
 * @param tcp_num
 * @param com_num
 */
void MainWindow::deinitClientViews(int tcp_num, int com_num)
{
    for(int i = 0; i < tcp_num; ++i) {
        disconnect(ui->tabWidget, SIGNAL(currentChanged(int)),
                   tcpConnList.at(i), SLOT(onTabChanged(int)));
        disconnect(ui->tabWidget, SIGNAL(currentChanged(int)),
                   tcpPollList.at(i), SLOT(onTabChanged(int)));
        connTabLayout->removeWidget(tcpConnList.at(i));
        workTabLayout->removeWidget(tcpPollList.at(i));
        delete tcpConnList.at(i);
        delete tcpPollList.at(i);
    }
    tcpConnList.clear();
    tcpPollList.clear();

    for(int i = 0; i < com_num; ++i) {
        disconnect(ui->tabWidget, SIGNAL(currentChanged(int)),
                   comConnList.at(i), SLOT(onTabChanged(int)));
        disconnect(ui->tabWidget, SIGNAL(currentChanged(int)),
                   comPollList.at(i), SLOT(onTabChanged(int)));
        connTabLayout->removeWidget(comConnList.at(i));
        workTabLayout->removeWidget(comPollList.at(i));
        delete comConnList.at(i);
        delete comPollList.at(i);
    }
    comConnList.clear();
    comPollList.clear();

    ui->tabWidget->widget(0)->layout()->removeItem(connTabLayout);
    ui->tabWidget->widget(1)->layout()->removeItem(workTabLayout);
    delete connTabLayout;
    delete workTabLayout;
}

/**
 * @brief MainWindow::on_About_triggered
 */
void MainWindow::on_About_triggered()
{
    QDialog *aboutWindow = new QDialog(this);
    aboutWindow->setWindowTitle("About");
    aboutWindow->resize(350, 230);
    aboutWindow->setModal(true);
    aboutWindow->setWindowFlags(Qt::Drawer);
    aboutWindow->setAttribute(Qt::WA_DeleteOnClose);

    QLabel* textLabel = new QLabel;
    QVBoxLayout *aboutLayot = new QVBoxLayout(aboutWindow);
    textLabel->setText(tr("<h2>DeviceLinkTest</h2>"
                          "<h4>Version 1.0.7</h4>"
                          "<p>Communication test utility</p>"));

    aboutLayot->addWidget(textLabel, 0, Qt::AlignCenter);
    aboutWindow->show();
}

/**
 * @brief MainWindow::on_Quit_triggered
 */
void MainWindow::on_Quit_triggered()
{
    this->close();
}

/**
 * @brief MainWindow::on_Number_triggered
 */
void MainWindow::on_Number_triggered()
{
    numberWindow = new QDialog(this);
    numberWindow->setWindowTitle("Number of clients");
    numberWindow->resize(200, 120);
    numberWindow->setModal(true);
    numberWindow->setWindowFlags(Qt::Drawer);
    numberWindow->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *numberLayot = new QVBoxLayout;
    QHBoxLayout *numTcpLayot = new QHBoxLayout;
    QHBoxLayout *numComLayot = new QHBoxLayout;
    QLabel *tcpNumLabel = new QLabel("Number of TCP clients");
    QLabel *comNumLabel = new QLabel("Number of COM clients");
    QPushButton *numSetButton = new QPushButton("Set");

    tcpNumSpinbox = new QSpinBox;
    comNumSpinbox = new QSpinBox;

    tcpNumSpinbox->setMinimum(0);
    tcpNumSpinbox->setMaximum(tcpMaximum);
    tcpNumSpinbox->setValue(2);
    comNumSpinbox->setMinimum(0);
    comNumSpinbox->setMaximum(comMaximum);
    comNumSpinbox->setValue(1);

    numTcpLayot->addWidget(tcpNumLabel);
    numTcpLayot->addStretch();
    numTcpLayot->addWidget(tcpNumSpinbox);
    numComLayot->addWidget(comNumLabel);
    numComLayot->addStretch();
    numComLayot->addWidget(comNumSpinbox);

    numberLayot->addLayout(numTcpLayot);
    numberLayot->addLayout(numComLayot);
    numberLayot->addWidget(numSetButton);
    numberWindow->setLayout(numberLayot);

    connect(numSetButton, SIGNAL(clicked()), this, SLOT(onNumsetButton_clicked()));

    numberWindow->show();
}

/**
 * @brief MainWindow::onNumsetButton_clicked
 */
void MainWindow::onNumsetButton_clicked()
{
    this->deinitClientViews(tcpQuantity, comQuantity);
    this->tcpQuantity = tcpNumSpinbox->value();
    this->comQuantity = comNumSpinbox->value();
    this->initClientViews(tcpQuantity, comQuantity);

    this->numberWindow->close();
}
