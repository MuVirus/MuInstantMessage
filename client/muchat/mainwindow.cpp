#include "mainwindow.h"
#include "./ui_mainwindow.h"

/******************************************************************************
 *
 * @file       mainwindow.cpp
 * @brief      MainWindows
 *
 * @author     MuVirus
 * @date       2026/04/17
 * @history
 *****************************************************************************/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // === 初始化UI
    // 1、MainWindow
    ui->setupUi(this);
    // 2、添加登录对话框（默认显示）
    _loginDialog = new LoginDialog(this);
    _loginDialog->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setCentralWidget(_loginDialog);
    _loginDialog->show();
    // 3、添加注册对话框（默认隐藏）
    _registerDialog = new RegisterDialog(this);
    _registerDialog->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    _registerDialog->hide();
    // === 连接信号槽
    // 登录界面->注册界面
    connect(_loginDialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    // 注册界面->登录界面
    connect(_registerDialog, &RegisterDialog::swtichLogin, this, &MainWindow::SlotSwitchLogin);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(_loginDialog != nullptr) {
        delete _loginDialog;
    }
    if(_registerDialog != nullptr) {
        delete _registerDialog;
    }
}

void MainWindow::SlotSwitchReg()
{
    setCentralWidget(_registerDialog);
    _loginDialog->hide();
    _registerDialog->show();
}

// tips: 有问题
void MainWindow::SlotSwitchLogin()
{
    setCentralWidget(_loginDialog);
    _registerDialog->hide();
    _loginDialog->show();
}
