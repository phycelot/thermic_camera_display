#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "firstrun.h"

#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <QDebug>
#include <QTimer>
#include <QtWidgets>
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //user interface
    ui->setupUi(this);

    //setup layout_camera
    label_camera = new QLabel;
    label_camera->installEventFilter(this);
    label_camera->setAutoFillBackground(true);
    ui->gridLayout_global->addWidget(label_camera,0,0);
    connect(this,SIGNAL(doubleClicked()),SLOT(onDoubleClicked()));
    label_camera->setText("emplacement camera");
    label_camera->setStyleSheet("QLabel { background-color: rgb(255, 224, 179);}");
//    label_camera->setCursor();

    //initialisation for a fist run
    firstRun::run(); // test if it's the fist time, if not continue, else configuration popup pop

    //initialisation of timer to update each 10sec
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10000);

    //connect
    QObject::connect(ui->pushButton_fullscreen,SIGNAL(clicked()),this,SLOT(setCameraFullScreen()));

    //initialisation alert timer
    alertTimer = new QTimer(this);
    connect(alertTimer , SIGNAL(timeout()), this, SLOT(clignAlert()));

    //init connection progressbar
    QString style = "QProgressBar{background-color:red}";
    QString text="initialisation";
    ui->progressBar_server->setStyleSheet(style);
    ui->progressBar_internet->setStyleSheet(style);
    ui->progressBar_server->setFormat(text);
    ui->progressBar_internet->setFormat(text);

    //firstUpdate
    update();

    //style initialisation
    this->setStyleSheet("QMainWindow { background-color: rgb(231, 231, 228);} QWidget { background-color: rgb(231, 231, 228);}");
    this->showFullScreen();

    //test
    createAlert();
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == label_camera)
        {
        if (event->type() == QEvent::MouseButtonDblClick)
            {
            emit doubleClicked();
            return QMainWindow::eventFilter(target,event);
//            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
//            if (mouseEvent->button() == Qt::LeftButton )
//                {
//                //emit doubleClicked();
//                return QMainWindow::eventFilter(target,event);
//                }
            }
        if(isMaximized && event->type() == QEvent::MouseButtonPress)
            {
                emit doubleClicked();
                return QMainWindow::eventFilter(target,event);
            }
        }

    return QMainWindow::eventFilter(target,event);
}



void MainWindow::onDoubleClicked()
{
    if ((label_camera->windowState() == Qt::WindowFullScreen) && isMaximized)
    {
        qDebug("Parent");
        label_camera->setParent(this);
        ui->gridLayout_global->addWidget(label_camera,0,0);
        isMaximized = false;

    } else
    {
        qDebug("Maximize");
        isMaximized = true;
        label_camera->setParent(NULL);
        label_camera->setWindowFlags(label_camera->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
        label_camera->setWindowState(label_camera->windowState() | Qt::WindowFullScreen);
        label_camera->show();

    }

}

void MainWindow::createAlert()
{
    //wip send alert message and log
    alertTimer->start(100);
    if(isMaximized)
        {
            emit doubleClicked();
        }
    isAlert=true;
}

void MainWindow::clignAlert()
{
//    qInfo() << __func__;
    if(!clignUpAlert)
    {
        this->setStyleSheet("QMainWindow { background-color: rgb(255, 0 , 0);} QWidget { background-color: rgb(255, 0 , 0);}");
    }
    else
    {
        this->setStyleSheet("QMainWindow { background-color: rgb(231, 231, 228);} QWidget { background-color: rgb(231, 231, 228);}");
    }
    clignUpAlert= !clignUpAlert;

}

void MainWindow::stopAlert()
{
    //wip add this info to log
    alertTimer->stop();
    this->setStyleSheet("QMainWindow { background-color: rgb(231, 231, 228);} QWidget { background-color: rgb(231, 231, 228);}");
    clignUpAlert=false;
    isAlert=false;
}

void MainWindow::mousePressEvent(QMouseEvent *e){
    qInfo() << __func__;
    int x = e->x();
    int y = e->y();
    int x1= label_camera->pos().x();
    int x2=label_camera->pos().x()+label_camera->geometry().width();
    int y1= label_camera->pos().y();
    int y2=label_camera->pos().y()+label_camera->geometry().height();
    if(x1<=x && x<=x2 && y1<=y && y<=y2)
    {
        int xin=x-label_camera->pos().x();
        int yin=y-label_camera->pos().y();
        qInfo() << "x=" << xin << ", y=" << yin;
        ui->tabWidget_temp_detail->setCurrentIndex(0);

    }
    if(isAlert)
    {
        stopAlert();
    }
//    if ((label_camera->windowState() == Qt::WindowFullScreen) && isMaximized)
//    {
//    qDebug("Parent");
//    label_camera->setParent(this);

////    label_camera->;
//    ui->gridLayout_global->addWidget(label_camera,0,0);
//    isMaximized = false;
//    }
}

void MainWindow::setCameraFullScreen()
{
    qInfo() << __func__;
    emit doubleClicked();

}

void MainWindow::update() //update every 10sec
{
    ui->lineEdit_ip->setText(getIPAddress());
    ui->lineEdit_temp->setText(getTemperature());
    QString text="Disconnected";
    int value=0;
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply *reply = nam.get(req);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if(reply->bytesAvailable())
    {
        text="Connected";
        value=1;
    }
    ui->progressBar_internet->setFormat(text);
    ui->progressBar_internet->setValue(value);
    QNetworkAccessManager nam2;
    QNetworkRequest req2(QUrl("http://www.ehaou.llsebfn/")); //change with server ip
    QNetworkReply *reply2 = nam2.get(req2);
    QEventLoop loop2;
    connect(reply2, SIGNAL(finished()), &loop2, SLOT(quit()));
    loop2.exec();
    if(reply2->bytesAvailable())
    {
        text="Connected";
        value=1;
    }
    else
    {
        text="Disconnected";
        value=0;
    }
    ui->progressBar_server->setFormat(text);
    ui->progressBar_server->setValue(value);
}

QString getIPAddress(){
    QString ipAddress="Unable to get IP Address";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if(strcmp(temp_addr->ifa_name, "en0")){
                    ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return ipAddress;
}

QString getTemperature(){
    FILE *temperatureFile;
    double T;
    temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
    if (temperatureFile == NULL)
    return "error"  ; //print some message
    fscanf (temperatureFile, "%lf", &T);
    T /= 1000;
    return QString::number(T).append("°C");
}

MainWindow::~MainWindow()
{
    delete ui;
}
