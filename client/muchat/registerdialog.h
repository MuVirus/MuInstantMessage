#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
signals:
    void swtichLogin();
private slots:
    void on_verify_button_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_confirm_button_clicked();

    void on_cancel_button_clicked();

    void on_user_edit_editingFinished();

    void on_email_edit_editingFinished();

    void on_pass_edit_editingFinished();

    void on_confirm_edit_editingFinished();

    void on_verify_edit_editingFinished();

private:
    // 初始化HTTP处理
    void initHttpHandlers();
    // 检查UI界面的输入框以及内容是否填充与
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPasswordValid();
    bool checkVarifyValid();
    bool checkConfirmValid();
    // 任务提示
    void showTip(QString, bool);
    void addTipErr(TipErr err, QString tips);
    void delTipErr(TipErr err);
    void changeTipPage();
private:
    Ui::RegisterDialog *ui;
    // 请求处理程序
    QMap<ReqId, std::function<void(const QJsonObject &)>> _handlers;
    // 错误提示
    QMap<TipErr, QString> _tip_errs;
    // 倒计时
    QTimer *_countdown_timer;
};

#endif // REGISTERDIALOG_H
