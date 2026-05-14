#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>
#include <QWidget>
#include <QStyle>
#include <QRegularExpression>
#include <QDebug>
#include <QByteArray>
#include <QMap>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QSettings>

class global
{
public:
    global();
};

extern std::function<void(QWidget *)> repolish;

// 请求id
enum ReqId {
    ID_GET_VARIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002, // 注册用户
};

// 请求的模块
enum Modules {
    REGISTER_MOD = 0,   // 注册
};

// 错误码
enum ErrorCodes {
    SUCCESS = 0,     // 成功
    ERR_JSON = 1,   // json解析失败
    ERR_NETWORK = 2,    // 网络错误
};

// 标签错误码
enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

#endif // GLOBAL_H
