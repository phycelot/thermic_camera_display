#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->showFullScreen();
    this->setFixedSize(480,320);
    this->setStyleSheet("QPushButton { width :40px; height :40px; }");
    //wip set line edit in password mode
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(tryLogin()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancelLogin()));
}

void LoginDialog::tryLogin()
{
    qInfo() << __func__;
}

void LoginDialog::cancelLogin()
{
    qInfo() << __func__;
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
