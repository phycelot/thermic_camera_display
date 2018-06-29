#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>


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

public slots:
    void update();
    void clignAlert();
    void setCameraFullScreen();

private:
    Ui::MainWindow *ui;
    bool isAlert=false;
    void createAlert();
    void stopAlert();
    bool clignUpAlert = false;
    bool isMaximized=false;
    QTimer *alertTimer;
};


QString getIPAddress();
QString getTemperature();

#endif // MAINWINDOW_H
