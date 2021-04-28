#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QLabel>

/**
 * @brief Class constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("DeviceLinkTest");

    /* "Connection" tab */
    TcpPortView *tcpConn = new TcpPortView(this);
    ComPortView *comConn = new ComPortView(this);
    ComPortView *comConn1 = new ComPortView(this);

    tcpConn->setHeader("<H2>TCP Connection</H1>");
    comConn->setHeader("<H2>Com Connection</H1>");
    comConn1->setHeader("<H2>Com Connection</H1>");

    QHBoxLayout *connTabLayout = new QHBoxLayout;
    connTabLayout->addWidget(tcpConn);
    connTabLayout->addWidget(comConn);
    connTabLayout->addWidget(comConn1);
    ui->tabWidget->widget(0)->setLayout(connTabLayout);

    /* "Data exchange" tab */
    TcpWorkView *tcpPoll = new TcpWorkView(this, tcpConn->getClient());
    ComWorkView *comPoll = new ComWorkView(this, comConn->getComPort());
    ComWorkView *comPoll1 = new ComWorkView(this, comConn1->getComPort());

    tcpPoll->setHeader("<H2>TCP Exchange</H2>");
    comPoll->setHeader("<H2>Com Exchange</H2>");
    comPoll1->setHeader("<H2>Com Exchange</H2>");

    QHBoxLayout *workTabLayout = new QHBoxLayout;
    workTabLayout->addWidget(tcpPoll);
    workTabLayout->addWidget(comPoll);
    workTabLayout->addWidget(comPoll1);
    ui->tabWidget->widget(1)->setLayout(workTabLayout);

    /*  */
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            tcpConn, SLOT(onTabChanged(int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            tcpPoll, SLOT(onTabChanged(int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            comConn, SLOT(onTabChanged(int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            comConn1, SLOT(onTabChanged(int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            comPoll, SLOT(onTabChanged(int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            comPoll1, SLOT(onTabChanged(int)));

    ui->tabWidget->setCurrentIndex(0);
    qDebug() << "Hello from" << this;
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
 * @brief Menu File->Quit
 */
void MainWindow::on_quit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    QDialog* aboutWindow = new QDialog(this);
    aboutWindow->setWindowTitle("About");
    aboutWindow->resize(350, 230);
    aboutWindow->setModal(true);
    aboutWindow->setWindowFlags(Qt::Drawer);
    aboutWindow->setAttribute(Qt::WA_DeleteOnClose);
    QLabel* textLabel = new QLabel;
    QVBoxLayout* aboutLayot = new QVBoxLayout;
    textLabel->setText(tr("<h2>DeviceLinkTest</h2>"
                          "<h4>Version 1.0.1</h4>"
                          "<p>Communication test utility</p>"
                          ));

    aboutLayot->addWidget(textLabel, 0, Qt::AlignCenter);
    aboutWindow->setLayout(aboutLayot);

    aboutWindow->show();
}
