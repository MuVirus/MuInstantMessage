#include "httpmanager.h"

HttpManager::HttpManager()
{
    // 连接http完成信号槽
    connect(this, &HttpManager::sig_http_finish, this, &HttpManager::slot_http_finish);
}

HttpManager::~HttpManager()
{

}

void HttpManager::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    // 创建一个HTTP POST请求
    QByteArray data = QJsonDocument(json).toJson();
    // url构造请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    // 发送请求
    QNetworkReply *reply = _manager.post(request, data);
    // 设置信号槽等待发送完成
    QObject::connect(reply, &QNetworkReply::finished, [reply, this, req_id, mod](){
        // 处理错误
        if(reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            // 发送信号通知
            emit sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();
            return;
        }
        // 读
        QString res = reply->readAll();
        // 发送信号通知
        emit sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}

void HttpManager::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    switch(mod) {
    case Modules::REGISTER_MOD:
        emit sig_reg_mod_finish(id, res, err);
        break;
    }
}
