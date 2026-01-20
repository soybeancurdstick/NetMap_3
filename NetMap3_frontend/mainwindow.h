#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTime>
#include <QLabel>

#include "manager.hpp"
#include "motor_controller.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct RoverData{
    QString ip;
    QString heading;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_scanBtn_clicked();
    void on_refreshScanBtn_clicked();
    void on_backBtn_clicked();
    void on_targetBtn_clicked();
    void on_stopBtn_clicked();
    //void on_autoScanBtn_clicked();
    void start_motor();
    void stop_motor();
    //void update_labels();
    void process_queue();
    void target_rover();

private:
    //UI
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    QWidget *scanPage;
    QWidget *resultsPage;

    //Result page
    QVBoxLayout *resultsLayout;
    QTableWidget *tableWidget;
    QLabel *timeLabel;

    QTime currentTime;
    QTimer* queueTimer;

    std::unique_ptr<Manager> manager; //smart pointer to backend manager.cpp
    std::unique_ptr<Motor_Controller> motor; //smart pointer to backend motor controller

};
#endif // MAINWINDOW_H
