#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update();

private:
    Ui::MainWindow *ui;
};


QString getIPAddress();
QString getTemperature();

#endif // MAINWINDOW_H
