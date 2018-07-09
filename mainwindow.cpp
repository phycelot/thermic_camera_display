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

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp>
using namespace cv;

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
    QSize s(240,320);
    label_camera->resize(s);

    //initialisation for a fist run
    firstRun::run(); // test if it's the fist time, if not continue, else configuration popup pop

    //initialisation of timer to update each 10sec
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10000);


    //camera fps timer
    double fps=1.0;
    QTimer *timerCam = new QTimer(this);
    connect(timerCam, SIGNAL(timeout()), this, SLOT(updateCam()));
    timerCam->start(1000/fps);

    //connect
    QObject::connect(ui->pushButton_fullscreen,SIGNAL(clicked()),this,SLOT(setCameraFullScreen()));
    QObject::connect(ui->pushButton_switch_camera_mode,SIGNAL(clicked()),this,SLOT(switchDisplay()));

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

    //open cv
    qInfo() << "try video capture 0";
    displayCam();
    displayId=1;
    qInfo() << "try video capture 1";
    displayCam();
    displayId=0;

    //test
    //createAlert();

    //init config
    initConfig();
}
void MainWindow::updateThermicCameraConfig(int i)
{
    qDebug() << i;
    emit updateThermicCameraConfig();
}

void MainWindow::updateThermicCameraConfig(){
    qInfo() << __func__;
    int tmp_gfid = ui->spinBox_gfid->value();
    int tmp_gsk = ui->spinBox_gsk->value();
    int tmp_tint = ui->spinBox_tint->value();

    //wip check if we are in editing mode
    if(ui->tabWidget_temp_detail->currentIndex()==2) //2 is the index of the editing tab
    {
        if(gfid_!= tmp_gfid)
        {
            gfid_= tmp_gfid;
            setGfid(gfid_);
        }
        if(gsk_!= tmp_gsk)
        {
            gsk_= tmp_gsk;
            setGsk(gsk_);
        }
        if(tint_!= tmp_tint)
        {
            tint_= tmp_tint;
            setTint(tint_);
        }
    }
    else
    {
        if(gfid_!= gfid)
        {
            gfid_= gfid;
            setGfid(gfid_);
        }
        if(gsk_!= gsk)
        {
            gsk_= gsk;
            setGsk(gsk_);
        }
        if(tint_!= tint)
        {
            tint_= tint;
            setTint(tint_);
        }
    }
}

void MainWindow::setGfid(int i) // 0 - 4095
{
    if (0<=i && i<=4095)
    {
        //do smth //wip set param /!\ only thermal
    }
    else
    {
        qCritical() << "gfid not on the right interval";
    }
}

void MainWindow::setGsk(int i) //0 - 4095
{
    if (0<=i && i<=4095)
    {
        //do smth //wip set param /!\ only thermal
    }
    else
    {
        qCritical() << "gsk not on the right interval";
    }
}

void MainWindow::setTint(int i) // 1 - 147
{
    if (1<=i && i<=147)
    {
        //do smth //wip set param /!\ only thermal
    }
    else
    {
        qCritical() << "tint not on the right interval";
    }
}

void MainWindow::initConfig()
{
    connect(ui->horizontalSlider_tint, SIGNAL(valueChanged(int)), ui->spinBox_tint, SLOT(setValue(int)));
    connect(ui->spinBox_tint, SIGNAL(valueChanged(int)), ui->horizontalSlider_tint, SLOT(setValue(int)));
    connect(ui->spinBox_gfid,SIGNAL(valueChanged(int)),this,SLOT(setGfidVoltValue(int)));
    connect(ui->spinBox_gsk,SIGNAL(valueChanged(int)),this,SLOT(setGskVoltValue(int)));
    connect(ui->horizontalSlider_tint, SIGNAL(valueChanged(int)),this, SLOT(updateThermicCameraConfig(int)));
    connect(ui->spinBox_tint, SIGNAL(valueChanged(int)), this, SLOT(updateThermicCameraConfig(int)));
    connect(ui->spinBox_gfid,SIGNAL(valueChanged(int)),this, SLOT(updateThermicCameraConfig(int)));
    connect(ui->spinBox_gsk,SIGNAL(valueChanged(int)),this, SLOT(updateThermicCameraConfig(int)));
    connect(ui->buttonBox_config,SIGNAL(rejected()),this,SLOT(resetConfigData()));
    connect(ui->buttonBox_config,SIGNAL(accepted()),this,SLOT(setConfigData()));
    resetConfigData();
    updateThermicCameraConfig();
}

void MainWindow::resetConfigData()
{
    ui->spinBox_gfid->setValue(gfid);
    ui->spinBox_gsk->setValue(gsk);
    ui->spinBox_tint->setValue(tint);
    updateThermicCameraConfig();
}

void MainWindow::setConfigData()
{
    gfid = ui->spinBox_gfid->value();
    gsk = ui->spinBox_gsk->value();
    tint = ui->spinBox_tint->value();
    updateThermicCameraConfig();
    //wip set in a config file
}

void MainWindow::setGfidVoltValue(int i)
{
    double clc=0.8+i/4093.0*3.4;
    ui->label_gfid_v->setText(QString::number(clc));
    updateThermicCameraConfig();
}

void MainWindow::setGskVoltValue(int i)
{
    double clc=1+i/4093.0*3.6;
    ui->label_gsk_v->setText(QString::number(clc));
    updateThermicCameraConfig();
}

void MainWindow::updateCam()
{
    displayCam();
    //wip add detection
}

void MainWindow::displayCam()
{
//    qInfo() << displayId;
    switch (displayId) {
    case 0:
        stream0.open(0); //0 is the id of video device.0 if you have only one camera.


        if (!stream0.isOpened()) { //check if video device has been initialised
            qWarning() << "cannot open camera 0";
        }
        else
        {
            stream0.read(camera0Frame);
            image0 = Mat2QImage(camera0Frame);
            setImage(image0);
        }
        break;
    case 1:
        stream1.open(1);//1 is the id of video device.1 if you have only one camera.


        if (!stream1.isOpened()) { //check if video device has been initialised
            qWarning() << "cannot open camera 1";
        }
        else
        {
            stream1.read(camera1Frame);
            image1 = Mat2QImage(camera1Frame);
            setImage(image1);
        }
        break;
    default:
        qWarning() << "invalid id";
        break;
    }
}

void MainWindow::switchDisplay()
{
//    qInfo() << __func__;
    if(displayId==0)
    {
        displayId=1;
    }
    else if(displayId==1)
    {
        displayId=0;
    }
    else
    {
        qWarning() << "unknown id";
        displayId=0;
    }
}

QImage MainWindow::Mat2QImage(Mat const& inMat)
{

    switch ( inMat.type() )
    {
       // 8-bit, 4 channel
       case CV_8UC4:
       {
          QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_ARGB32 );

          return image;
       }

       // 8-bit, 3 channel
       case CV_8UC3:
       {
          QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_RGB888 );

          return image.rgbSwapped();
       }

       // 8-bit, 1 channel
       case CV_8UC1:
       {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
          QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_Grayscale8 );
#else
          static QVector<QRgb>  sColorTable;

          // only create our color table the first time
          if ( sColorTable.isEmpty() )
          {
             sColorTable.resize( 256 );

             for ( int i = 0; i < 256; ++i )
             {
                sColorTable[i] = qRgb( i, i, i );
             }
          }

          QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_Indexed8 );

          image.setColorTable( sColorTable );
#endif

          return image;
       }

       default:
          qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
          break;
    }

    return QImage();
}

void MainWindow::setImage(QImage image)
{
#if 1
    int tempWidth = label_camera->geometry().width();
    int tempHeight = label_camera->geometry().height();
#else
    int tempWidth = 320;
    int tempHeight = 240;
#endif
    QPixmap pix = QPixmap::fromImage(image.scaled(tempWidth, tempHeight));
    label_camera->setPixmap(pix);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == label_camera)
        {
        if (event->type() == QEvent::MouseButtonDblClick)
            {
            emit doubleClicked();
            return QMainWindow::eventFilter(target,event);
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
        label_camera->resize(320,240);
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
    alertTimer->start(500);
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
