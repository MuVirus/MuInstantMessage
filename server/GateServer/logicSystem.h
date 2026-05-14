#pragma once
#include "const.h"

// LogicSystem
// 任务
// 1、处理HttpConnection的注册任务(解耦)

class HttpConnection;

using HttpHandler = std::function<void(std::shared_ptr<HttpConnection>)>;

class LogicSystem
{
public:
	LogicSystem();
	~LogicSystem();
	// 注册Http路由
	// url: 路由, handler: 对应路由的事件
	void registerGet(std::string url, HttpHandler handler);
	void registerPost(std::string url, HttpHandler handler);
	// 路由触发执行
	bool handlerGet(std::string path, std::shared_ptr<HttpConnection> con);
	bool handlerPost(std::string path, std::shared_ptr<HttpConnection> con);
private:
	std::map<std::string, HttpHandler> _post_handlers;
	std::map<std::string, HttpHandler> _get_handlers;
};

