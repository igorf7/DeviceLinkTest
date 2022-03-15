#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpportview.h"
#include "tcpworkview.h"
#include "comportview.h"
#include "comworkview.h"
#include <QMainWindow>
#include <QLayout>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

private slots:
    void on_About_triggered();
    void on_Quit_triggered();
    void on_Number_triggered();
    void onNumsetButton_clicked();

private:
    Ui::MainWindow *ui;

    const int tcpMaximum = 6;
    const int comMaximum = 6;

    quint8 tcpQuantity = 2, // value by default
           comQuantity = 1; // value by default

    QGridLayout *connTabLayout;
    QGridLayout *workTabLayout;

    QList<TcpPortView*> tcpConnList;
    QList<ComPortView*> comConnList;
    QList<TcpWorkView*> tcpPollList;
    QList<ComWorkView*> comPollList;
    QDialog *numberWindow;
    QSpinBox *tcpNumSpinbox;
    QSpinBox *comNumSpinbox;

    void initClientViews(int tcp_num, int com_num);
    void deinitClientViews(int tcp_num, int com_num);
};
#endif // MAINWINDOW_H
