#include "firstrun.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace firstRun
{
QString filePath="./config_files/config.json";

bool isFirstRun()
{
    bool success;
    QString val;
    QFile file;
    file.setFileName(filePath);
    success=file.open(QIODevice::Text | QIODevice::ReadOnly);
    val = file.readAll();
    file.close();
    if (!success){
        qInfo() << "can't access to file";
    }
    return !success;
}

void run()
{
    if(isFirstRun())
    {
        //test if config_files already exist on the right directory
        QString path("./config_files/");
        QDir dir;
        // We create the directory if needed
        if (!dir.exists(path))
        {
            qCritical() << "path don't exist";
            if (dir.mkpath(path))
                {
                    qInfo() << "config dir created with success";

                }
            else {
                qCritical() << "can't create config dir";
                return; //wip
            }
        }
        bool success;
        QString val;
        QFile file;
        file.setFileName(filePath);
        success=file.open(QIODevice::Text | QIODevice::ReadWrite);
        val = file.readAll();
        if (!success){
            qInfo() << "can't access to file";
            return; //wip
        }
        else
        {
            qInfo() << "file created";
            file.write("test");
        }
        file.close();
    }
    else
    {
        bool success;
        QString val;
        QFile file;
        file.setFileName(filePath);
        success=file.open(QIODevice::Text | QIODevice::ReadWrite);
        val = file.readAll();
        if (!success){
            qInfo() << "can't access to file";
            return; //wip
        }
        else
        {
        }
        file.close();
    }

}

}
