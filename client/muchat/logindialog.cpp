#include "logindialog.h"
#include "ui_logindialog.h"
#include "global.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_register_button_clicked()
{
    emit switchRegister();
}

