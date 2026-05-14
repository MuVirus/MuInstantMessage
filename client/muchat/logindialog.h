#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

/******************************************************************************
 *
 * @file       logindialog.h
 * @brief      登录对话框
 *
 * @author     MuVirus
 * @date       2026/04/17
 * @history
 *****************************************************************************/

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void switchRegister();

private slots:
    void on_register_button_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
