#pragma once

#include "const.h"

// HttpConnection
// 任务:
// 1、通过tcp连接的socket创建
// 2、开启时就读取http请求
// 3、解析URL地址中的路由和参数
// 4、让LogicSystem单例执行注册过的路由任务
// 5、写http请求，并发送
// 注意:
// 1、任务4的时候会根据不同的任务会生成不同的响应体, 为了方便+, 不过代码架构看起来耦合度高


class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
public:
	HttpConnection(tcp::socket socket);
	void start();
private:
	// 启动异步等待超时
	void checkDeadline();
	// 发送响应
	void writeResponse();
	// 处理请求
	void handleRequest();
	// 解析URL参数
	void parseURLParam();

private:
	tcp::socket _socket;
	// HTTP请求/响应
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	// 临时缓存
	beast::flat_buffer _buffer{ 8192 };
	// 超时时间
	asio::steady_timer _deadline{ _socket.get_executor(), std::chrono::seconds(60) };
	// 当前HTTP连接的URL和参数
	std::string _get_url;
	std::unordered_map < std::string, std::string> _get_params;
};

