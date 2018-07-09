#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    void addPasswordText(QString t);

public slots:
    void tryLogin();
    void cancelLogin();
    void addPasswordText_0();
    void addPasswordText_1();
    void addPasswordText_2();
    void addPasswordText_3();
    void addPasswordText_4();
    void addPasswordText_5();
    void addPasswordText_6();
    void addPasswordText_7();
    void addPasswordText_8();
    void addPasswordText_9();
    void addPasswordText_sharp();
    void addPasswordText_star();

private:
    Ui::LoginDialog *ui;
    QString password="";
    void updatePasswordView();

signals:
    void acceptLogin(QString&);
};

#endif // LOGINDIALOG_H
