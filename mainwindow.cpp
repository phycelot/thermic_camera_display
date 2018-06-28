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

    //initialisation for a fist run
    firstRun::run(); // test if it's the fist time, if not continue, else configuration popup pop

    //initialisation of timer to update each 10sec
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10000);


    //init connection progressbar
    QString style = "QProgressBar{background-color:red}";
    QString text="initialisation";
    ui->progressBar_server->setStyleSheet(style);
    ui->progressBar_internet->setStyleSheet(style);
    ui->progressBar_server->setFormat(text);
    ui->progressBar_internet->setFormat(text);

    //firstUpdate
    update();
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
