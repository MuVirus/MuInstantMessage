#include <QApplication>

#include "mainwindow.h"
#include "logindialog.h"
#include "registerdialog.h"

#include "singleton.h"
#include "configManager.h"

int main(int argc, char *argv[])
{
    // 设置高清分辨率兼容
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    // 加载QSS样式
    QFile qss(":/style/stylesheet.qss");

    if( qss.open(QFile::ReadOnly))
    {
        qDebug("open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("Open failed");
    }

    // 预加载一下配置文件
    Singleton<ConfigManager>::getInstance().init();
    // 测试
    qDebug() << "GateServer地址：" << Singleton<ConfigManager>::getInstance().getGateHttpAddress();

    MainWindow w;
    w.show();
    return a.exec();
}
