#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QCoreApplication>
#include "global.h"

class ConfigManager {
public:
    ConfigManager() {
    }
    void init() {
        QString appPath = QCoreApplication::applicationDirPath();
        qDebug() << "当前程序路径为：" << appPath;
        // 获取配置内容
        QString configPath = QDir::toNativeSeparators(appPath + QDir::separator() + "config.ini");
        qDebug() << configPath;

        QFile file(configPath);
        if(!file.exists()) {
            qDebug() << "错误: config.ini 文件不存在！";
            return;
        }

        QSettings settings(configPath, QSettings::IniFormat);

        gateHost = settings.value("GateServer/Host").toString();
        gatePort = settings.value("GateServer/Port").toString();

    }
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
public:
    QString getGateHost() const { return gateHost; }
    QString getGatePort() const { return gatePort; }
    QString getGateHttpAddress() const { return "http://" + gateHost + ":" + gatePort; }
private:
    QString gateHost;
    QString gatePort;
};

#endif // CONFIGMANAGER_H
