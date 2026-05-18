#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmanager.h"
#include "singleton.h"
#include "configManager.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    //  === 初始化UI
    ui->setupUi(this);
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);
    // 连接信号槽
    // 1、http注册完成
    connect(&HttpManager::getInstance(), &HttpManager::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
    // 2、输入框检测(QT设计中已经转到到槽设置)
    // 3、取消按钮切换到登录界面(同上)
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_verify_button_clicked()
{
    // 判断邮箱格式的正确性
    if(!checkEmailValid()) {
        return;
    }
    auto email = ui->email_edit->text();
    // 向服务器发送邮箱获取验证码指令
    QJsonObject jsonObj;
    jsonObj["email"] = email;
    HttpManager::getInstance().PostHttpReq(QUrl(Singleton<ConfigManager>::getInstance().getGateHttpAddress() + "/get_varifycode"), jsonObj, ReqId::ID_GET_VARIFY_CODE, Modules::REGISTER_MOD);
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS) {
        showTip("网络请求错误", false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson((res.toUtf8()));
    if(jsonDoc.isNull()) {
        showTip("json解析错误", false);
        return;
    }

    if(!jsonDoc.isObject()) {
        showTip("json解析错误", false);
        return;
    }

    _handlers[id](jsonDoc.object());

    return;
}


void RegisterDialog::showTip(QString payload, bool code) {
    if(code) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "err");
    }
    ui->err_tip->setText(payload);
    repolish(ui->err_tip);
}

void RegisterDialog::initHttpHandlers()
{
    // 获取验证码HTTP请求回调函数
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](QJsonObject jsonObj) {
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS) {
            showTip("错误: " + QString::number(error), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip("验证码已发送到邮箱，注意查收", true);
        qDebug() << "email is " << email;
    });
    // 用户注册HTTP请求回调函数
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj) {
        int error = jsonObj["error"].toInt();
        int uid = jsonObj["uid"].toInt();
        if(error != ErrorCodes::SUCCESS) {
            showTip(tr("错误: ") + QString::number(error), false);
            return;
        }
        showTip(tr("注册成功"), true);
        qDebug() << "uid is " << uid;
    });
}

void RegisterDialog::on_confirm_button_clicked()
{
    // ===检查输入框是否正确
    checkUserValid();
    checkEmailValid();
    checkPasswordValid();
    checkConfirmValid();
    checkVarifyValid();
    // ===将数据打包成json格式发送
    QJsonObject jsonObj;
    jsonObj["name"] = ui->user_edit->text();
    jsonObj["email"] = ui->email_edit->text();
    jsonObj["pass"] = ui->pass_edit->text();
    jsonObj["confirm"] = ui->confirm_edit->text();
    jsonObj["code"] = ui->verify_edit->text();

    HttpManager::getInstance().PostHttpReq(
        QUrl(Singleton<ConfigManager>::getInstance().getGateHttpAddress() + "/user_register"),
        jsonObj,
        ReqId::ID_REG_USER, Modules::REGISTER_MOD
    );
}


void RegisterDialog::on_cancel_button_clicked()
{
    emit swtichLogin();
}

// 检查用户名是否合法
bool RegisterDialog::checkUserValid() {
    if(ui->user_edit->text().isEmpty()) {
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

// 检查邮箱是否合法
bool RegisterDialog::checkEmailValid() {
    auto email = ui->email_edit->text();
    // 验证地址是否合法（正则表达式匹配）
    QRegularExpression emailExp(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = emailExp.match(email).hasMatch();
    if(!match) {
        addTipErr(TipErr::TIP_EMAIL_ERR, "邮箱匹配错误");
        return false;
    }
    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

// 检查密码是否合法
bool RegisterDialog::checkPasswordValid() {
    auto pass = ui->pass_edit->text();

    // 判断密码长度
    if(pass.length() < 6 || pass.length() > 15) {
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应该在6~15"));
        return false;
    }

    // 判断密码长度
    if(pass.length() < 6 || pass.length() > 15) {
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应该在6~15"));
        return false;
    }

    // 匹配密码是否符合要求
    QRegularExpression passExp("^[a-zA-Z0-9!@#$&*.]{6,15}$");
    bool match = passExp.match(pass).hasMatch();
    if(!match) {
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    delTipErr(TipErr::TIP_PWD_ERR);

    return true;

}

// 检查验证码是否合法
bool RegisterDialog::checkVarifyValid() {
    if(ui->verify_edit->text().isEmpty()) {
        addTipErr(TipErr::TIP_VARIFY_ERR, "验证码为空");
        return false;
    }
    delTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

// 检查确认密码是否合法
bool RegisterDialog::checkConfirmValid() {
    auto pass = ui->pass_edit->text();
    auto confirm = ui->confirm_edit->text();

    // 判断确认密码长度
    if(confirm.length() < 6 || confirm.length() > 15) {
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应该在6~15"));
        return false;
    }

    // 匹配确认密码是否符合要求
    QRegularExpression passExp("^[a-zA-Z0-9!@#$&*.]{6,15}$");
    bool match = passExp.match(pass).hasMatch();
    if(!match) {
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    delTipErr(TipErr::TIP_PWD_ERR);
    // 匹配确认密码与密码是否一致
    if(pass != confirm) {
        addTipErr(TipErr::TIP_CONFIRM_ERR, tr("密码与确认密码不匹配"));
        return false;
    }
    delTipErr(TipErr::TIP_CONFIRM_ERR);
    return true;
}

void RegisterDialog::addTipErr(TipErr err, QString tips){
    _tip_errs[err] = tips;
    showTip(tips, false);
}

void RegisterDialog::delTipErr(TipErr err){
    _tip_errs.remove(err);
    if(_tip_errs.empty()) {
        ui->err_tip->clear();
        return;
    }

    showTip(_tip_errs.first(), false);
}

void RegisterDialog::changeTipPage(){

}

void RegisterDialog::on_user_edit_editingFinished()
{
    checkUserValid();
}

void RegisterDialog::on_email_edit_editingFinished()
{
    checkEmailValid();
}


void RegisterDialog::on_pass_edit_editingFinished()
{
    checkPasswordValid();
}


void RegisterDialog::on_confirm_edit_editingFinished()
{
    checkConfirmValid();
}


void RegisterDialog::on_verify_edit_editingFinished()
{
    checkVarifyValid();
}

