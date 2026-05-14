#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"

// UI界面枚举
enum class UIStatus {
    LOGIN_UI,
    REGISTER_UI,
    RESET_UI,
    CHAT_UI,
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();

private:
    Ui::MainWindow *ui;
    LoginDialog *_loginDialog;
    RegisterDialog *_registerDialog;
};
#endif // MAINWINDOW_H
