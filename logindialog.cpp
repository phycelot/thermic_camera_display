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

    connect(ui->pushButton_0,SIGNAL(pressed()),this,SLOT(addPasswordText_0()));
    connect(ui->pushButton_1,SIGNAL(pressed()),this,SLOT(addPasswordText_1()));
    connect(ui->pushButton_2,SIGNAL(pressed()),this,SLOT(addPasswordText_2()));
    connect(ui->pushButton_3,SIGNAL(pressed()),this,SLOT(addPasswordText_3()));
    connect(ui->pushButton_4,SIGNAL(pressed()),this,SLOT(addPasswordText_4()));
    connect(ui->pushButton_5,SIGNAL(pressed()),this,SLOT(addPasswordText_5()));
    connect(ui->pushButton_6,SIGNAL(pressed()),this,SLOT(addPasswordText_6()));
    connect(ui->pushButton_7,SIGNAL(pressed()),this,SLOT(addPasswordText_7()));
    connect(ui->pushButton_8,SIGNAL(pressed()),this,SLOT(addPasswordText_8()));
    connect(ui->pushButton_9,SIGNAL(pressed()),this,SLOT(addPasswordText_9()));
    connect(ui->pushButton_sharp,SIGNAL(pressed()),this,SLOT(addPasswordText_sharp()));
    connect(ui->pushButton_star,SIGNAL(pressed()),this,SLOT(addPasswordText_star()));

    ui->lineEdit_password->setStyleSheet("border: 2px solid gray;");
}

void LoginDialog::addPasswordText_0(){addPasswordText("0");}
void LoginDialog::addPasswordText_1(){addPasswordText("1");}
void LoginDialog::addPasswordText_2(){addPasswordText("2");}
void LoginDialog::addPasswordText_3(){addPasswordText("3");}
void LoginDialog::addPasswordText_4(){addPasswordText("4");}
void LoginDialog::addPasswordText_5(){addPasswordText("5");}
void LoginDialog::addPasswordText_6(){addPasswordText("6");}
void LoginDialog::addPasswordText_7(){addPasswordText("7");}
void LoginDialog::addPasswordText_8(){addPasswordText("8");}
void LoginDialog::addPasswordText_9(){addPasswordText("9");}
void LoginDialog::addPasswordText_sharp(){addPasswordText("#");}
void LoginDialog::addPasswordText_star(){addPasswordText("*");}

void LoginDialog::addPasswordText(QString t)
{
    password.append(t);
    updatePasswordView();
}

void LoginDialog::updatePasswordView()
{
    ui->lineEdit_password->setText(password);
}

void LoginDialog::tryLogin()
{
    qInfo() << __func__;
    //wip test password
    if(password=="#123#")
    {
        //wip token create
        QString token = "kbd3oi3092d";
        emit acceptLogin(token);
        close();
    }
    else
    {
        ui->lineEdit_password->setStyleSheet("border: 2px solid red;");
    }
}

void LoginDialog::cancelLogin()
{
    if(password.length()>0){password="";updatePasswordView();}
    else{close();}
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
