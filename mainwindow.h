#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>

#include <cv.h>
#include "opencv2/opencv.hpp"

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
    QImage image;
    void setImage(QImage image);
    QImage Mat2QImage(cv::Mat const& src);

public slots:
    void update();
    void clignAlert();
    void setCameraFullScreen();
    void onDoubleClicked();

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
