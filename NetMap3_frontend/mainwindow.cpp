#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QStackedWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>
#include <memory>
#include <vector>
#include <string>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setFixedSize(640,460);
    qDebug() << "hi hi hi updated hi hi hi";
    //qDebug() << "Hello World.";
    std::vector<std::string> server_ips = {"192.168.2.3"};

    manager = std::make_unique<Manager>(server_ips, 8080);
    motor = std::make_unique<Motor_Controller>();

    //connect(manager.get(), &Manager::roverDiscovered, this, &MainWindow::process_queue);
    queueTimer = new QTimer(this);
    connect(queueTimer, &QTimer::timeout, this, &MainWindow::process_queue);
    //queueTimer->start(200);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget {background-color: #a6e1ff;}");
    setCentralWidget(stackedWidget);

    //------------Scan Page------------
    scanPage = new QWidget();
    QVBoxLayout *scanLayout = new QVBoxLayout();
    scanLayout->setContentsMargins(0,0,0,0);
    scanLayout->setSpacing(0);

    // Header Bar
    QWidget *headerBar = new QWidget(scanPage);
    headerBar->setFixedHeight(25);
    headerBar->setStyleSheet("background-color: #C8102E;");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(20,0,20,0);
    headerLayout->addStretch();

    scanLayout->addWidget(headerBar);

   //title
    QLabel *titleLabel = new QLabel("NetMap", scanPage);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: 700; color: #2A7FFF;"); // #2A7FFF
    scanLayout->addWidget(titleLabel);

    //subtitle
    QLabel *scanLabel = new QLabel("Press to Scan");
    scanLabel->setAlignment(Qt::AlignCenter);
    scanLabel->setStyleSheet("font-size: 18px; color: #2a466e;");
    scanLayout->addWidget(scanLabel);

   //scan button
    QPushButton *scanBtn = new QPushButton("Scan", this);
    scanBtn->setFixedSize(250,100);
    scanBtn->setStyleSheet("QPushButton {"
                          "background-color: qlineargradient("
                          "x1:0, y1:0, x2:1, y2:1,"
                          "stop:0 #1E90FF, stop:1 #0066CC);"
                          "font-size: 18pt;"
                          "font-weight: bold;"
                          "color: white;"
                          "border-style: solid;"
                          "border-width: 1px;"
                          "border-radius: 50px; /* Half of the height to make it pill-shaped */"
                          "border-color: white;"
                          "padding: 10px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #939EBA;"
                          "color: white;"
                          "}");
    scanLayout->addWidget(scanBtn, 0, Qt::AlignCenter);
    connect(scanBtn, &QPushButton::clicked, this, &MainWindow::on_scanBtn_clicked);

    //logo
    auto *imgLabel = new QLabel(scanPage);
    //QPixmap pixmap(":/ncpa_logo.jpg");
    QPixmap pixmap(":/NCPA_crest.png");
    if (!pixmap.isNull()) {
        imgLabel->setPixmap(pixmap);
        qDebug() << "hi hi hi";
    } else {
        qDebug() << "Resource error: Check your .qrc prefix and file name.";
        imgLabel->setText("Image failed to load or is null.");

    }

    imgLabel->setPixmap(pixmap.scaled(150,150, Qt::KeepAspectRatio));
    imgLabel->setAlignment(Qt::AlignRight);
    scanLayout->addWidget(imgLabel);

    scanPage->setLayout(scanLayout);
    stackedWidget->addWidget(scanPage);

    //------------Results Page------------

    resultsPage = new QWidget();
    resultsLayout = new QVBoxLayout(resultsPage);

    QGridLayout *resultsGLayout = new QGridLayout(resultsPage);
    QLabel *resultsLabel = new QLabel("Results of scan: ");
    resultsLayout->addWidget(resultsLabel);
    timeLabel = new QLabel("Time updated: --:--:--");

    tableWidget = new QTableWidget(resultsPage);
    QHeaderView *horizontalHeader = tableWidget->horizontalHeader();
    QHeaderView *verticalHeader = tableWidget->verticalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader->setSectionResizeMode(QHeaderView::Stretch);
    QStringList headers = {"Rover","IP address", "Location"};

    // Connect the table's selection signal to a custom function
    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::on_selection_changed);


    //behaviors for selection
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);


    tableWidget->setColumnCount(headers.size());
    tableWidget->setRowCount(0);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setStyleSheet("QTableView { background-color: #f0f0f0; alternate-background-color: #e0e0e0; }"
                             "QTableView::item { padding: 3px; }");
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #4a4a4a; color: white; padding: 5px; border: 1px solid #5d5d5d; }");
    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: #4a4a4a; color: white; padding: 5px; border: 1px solid #5d5d5d; }");


    //Add a row
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem("Rover 1"));
    tableWidget->setItem(row, 1, new QTableWidgetItem("111.111.111.111"));
    tableWidget->setItem(row, 2, new QTableWidgetItem("90"));
    tableWidget->resizeColumnsToContents();

    resultsLayout->addWidget(tableWidget);

    QPushButton *backBtn = new QPushButton("back", this);
    backBtn->setFixedSize(90,50);
    resultsGLayout->addWidget(backBtn, 0, 1, Qt::AlignTop | Qt::AlignRight);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::on_backBtn_clicked);

    QPushButton *refreshBtn = new QPushButton("refresh", this);
    refreshBtn->setFixedSize(90,50);
    resultsGLayout->addWidget(refreshBtn, 0, 2, Qt::AlignTop | Qt::AlignRight); //(button, row, col, )
    //connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::on_refreshBtn_clicked);

    targetBtn = new QPushButton("target", this);
    targetBtn->setFixedSize(90,50);
    resultsGLayout->addWidget(targetBtn, 0, 3, Qt::AlignTop | Qt::AlignRight);
    targetBtn->setEnabled(false);
   // connect(targetBtn, &QPushButton::clicked, this, &MainWindow::on_targetBtn_clicked);

    //QPushButton *autoScanBtn = new QPushButton("Auto Scan", this);
    //autoScanBtn->setFixedSize(90,50);
    //resultsGLayout->addWidget(autoScanBtn, 0, 4, Qt::AlignTop | Qt::AlignRight);
    // connect(autoScanBtn, &QPushButton::clicked, this, &MainWindow::on_autoScanBtn_clicked);

    QPushButton *stopBtn = new QPushButton("Stop", this);
    stopBtn->setFixedSize(90,50);
    resultsGLayout->addWidget(stopBtn, 0, 4, Qt::AlignTop | Qt::AlignRight);
    connect(stopBtn, &QPushButton::clicked, this, &MainWindow::on_stopBtn_clicked);

    resultsLayout->addLayout(resultsGLayout);
    resultsPage->setLayout(resultsLayout);
    stackedWidget->addWidget(resultsPage);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_scanBtn_clicked(){
    qDebug() << "scan button clicked";
    currentTime = QTime::currentTime(); //get curr sys time
    QString timeString = "Time updated: " + currentTime.toString("hh:mm:ss ap"); //format ap=am pm
    if (timeLabel != nullptr){
        timeLabel->setText(timeString);
    } else {
        qDebug() << "error: timeLabel is null";
    }
    resultsLayout->addWidget(timeLabel);

    //manager->add_to_queue("192.168.1.50", 123);

    start_motor();
    qDebug() << "start motor and thus auto scan";
    //motor_running = true;
    //std::cout << "AUTO MODE STARTED" << std::endl;
    //std::thread autoThread(&Manager::start_auto_mode, manager);
    //autoThread.detach();
     //try{
//    std::thread autoThread(&Manager::start_auto_mode, manager.get());
  //  autoThread.detach();
   //     std::cout << "AUTO MODE STARTED" << std::endl;
     //   manager->start_auto_mode();
    //} catch (const std::exception& e){
      //  std::cerr<<"Error starting auto mode"<<e.what()<<std::endl;
    //} 

    qDebug() << "current time: "<< timeString;
    qDebug() << "stacked pointer:" << stackedWidget;
    qDebug() << "stacked page count:" << stackedWidget->count();
    qDebug() << "switching page now...";


    //next page
    stackedWidget->setCurrentIndex(1);
} 

void MainWindow::on_refreshScanBtn_clicked(){
    currentTime = QTime::currentTime(); //get curr sys time
    QString timeString = "Time updated: " + currentTime.toString("hh:mm:ss ap"); //format ap=am pm
    timeLabel->setText(timeString);
    resultsLayout->addWidget(timeLabel);

    start_motor();

    qDebug() << "current time: "<< timeString;
}
void MainWindow::on_backBtn_clicked(){
    qDebug() << "Back button clicked";
    qDebug() << "Before back: index =" << stackedWidget->currentIndex();
    queueTimer->stop();
    stop_motor();
    stackedWidget->setCurrentIndex(0);
    qDebug() << "After back: index =" << stackedWidget->currentIndex();

}
void MainWindow::on_targetBtn_clicked(){

}
void MainWindow::start_motor(){
    try{
      //  std::thread my_thread(manager->start_thread());
       // my_thread.detach();
       manager->start_thread();
    } catch (const std::exception& e){
        std::cerr<<"Error starting motor control loop"<<e.what()<<std::endl;
    } 

}
void MainWindow::stop_motor(){
    manager->stop_auto_mode();
}
void MainWindow::process_queue(){
    qDebug() << "steaming rice";
    qDebug() << "queue empty?" << manager->is_queue_empty();
    if (!manager->is_queue_empty()){
        auto [ip, heading] = manager->get_from_queue();
        QString QIp = QString::fromStdString(ip);
        QString Qheading = QString::number(heading);
        RoverData rover{QIp, Qheading};
        if(!rover.ip.isEmpty()){
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem(QIp));
            tableWidget->setItem(row, 1, new QTableWidgetItem(Qheading));
        } else {
            qDebug() << "Empty";
        }
    }

}
void MainWindow::target_rover(){
    int row = tableWidget->currentRow();
    if(row < 0){
        qDebug() << "No Rovers Selected";
        return;
    }
    QString ip = tableWidget->item(row,0)->text();
    int heading = tableWidget->item(row, 1)->text().toInt();
    manager->turn_to_target(heading);
    qDebug() << "Selected rover: " << ip << heading;


}

//void MainWindow::on_autoScanBtn_clicked(){
  //  start_motor();
   // queueTimer->start(200); //RUN every 200 ms
//}

void MainWindow::on_stopBtn_clicked(){
    qDebug() << "Stop button clicked";
    queueTimer->stop();
    stop_motor();
}

void MainWindow::on_selection_changed() {
    qDebug() << "entered selection changed method";
    if (tableWidget->selectionModel()->hasSelection()) {
        targetBtn->setEnabled(true);
        int row = tableWidget->currentRow();
        QString rover_ip_selected = tableWidget->item(row, 0)->text();
    } else {
        // While nothing is selected, do this:
        targetBtn->setEnabled(false);
    }
}

