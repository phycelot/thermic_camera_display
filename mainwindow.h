#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp>
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *e);
    bool eventFilter(QObject *target, QEvent *event);
    QLabel *label_camera;
    QImage image0;
    QImage image1;
    void setImage(QImage image);
    QImage Mat2QImage(cv::Mat const& inMat);
    int displayId=0;
    Mat camera0Frame;
    Mat camera1Frame;
    VideoCapture stream0;
    VideoCapture stream1;
    void displayCam();
    double gsk=1788;//0 -4095 //1V - 3.6V
    double tint=128;// 1 - 147
    double gfid=2048;// 0 -4095 //0.8V - 3.4V
    void initConfig();

public slots:
    void update();
    void clignAlert();
    void setCameraFullScreen();
    void onDoubleClicked();
    void switchDisplay();
    void updateCam();
    void setGfidVoltValue(int i);
    void setGskVoltValue(int i);
    void resetConfigData();
    void setConfigData();
    void updateThermicCameraConfig();

private:
    Ui::MainWindow *ui;
    bool isAlert=false;
    void createAlert();
    void stopAlert();
    bool clignUpAlert = false;
    bool isMaximized=false;
    QTimer *alertTimer;

signals:
    void doubleClicked();
};


QString getIPAddress();
QString getTemperature();

#endif // MAINWINDOW_H
