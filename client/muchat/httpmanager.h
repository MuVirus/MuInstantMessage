#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "singleton.h"
#include "global.h"

class HttpManager : public QObject, public Singleton<HttpManager>
{
    Q_OBJECT
public:
    ~HttpManager();
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
private:
    friend class Singleton<HttpManager>;
    HttpManager();
private:
    QNetworkAccessManager _manager;
signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
};

#endif // HTTPMANAGER_H
