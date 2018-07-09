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

public slots:
    void tryLogin();
    void cancelLogin();

private:
    Ui::LoginDialog *ui;
    QString password;

signals:
    void acceptLogin(QString&);
};

#endif // LOGINDIALOG_H
