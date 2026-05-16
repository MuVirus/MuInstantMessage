#include "network/httpConnection.h"

#include <memory>
#include <iostream>
#include "common/singleton.h"
#include "common/utils.h"
#include "logic/logicSystem.h"

HttpConnection::HttpConnection(tcp::socket socket)
	: _socket(std::move(socket)) 
{
	std::cout << "http连接: " << _socket.remote_endpoint().address().to_string() << std::endl;
}

void HttpConnection::start() {
	auto self = shared_from_this();
	// 异步读取Http请求
	http::async_read(
		_socket, 
		_buffer, 
		_request,
		[self](beast::error_code ec, std::size_t){
			try {
				if (ec) {
					std::cout << "[HttpConnection] http read err is " << ec.message() << std::endl;
				}
				// 异步处理请求
				self->handleRequest();
				// 异步等待超时
				self->checkDeadline();
			}
			catch (std::exception& e) {
				std::cout << "exception is " << e.what() << std::endl;
			}
		}
	);
}

// 处理http请求
// Get、Post
void HttpConnection::handleRequest() {
	// 版本、长连接设置
	_response.version(_request.version());
	_response.keep_alive(false);
	// 设置来源访问
	_response.set(http::field::access_control_allow_origin, "*");
	// 根据Http请求处理
	if (_request.method() == http::verb::get) {
		parseURLParam();
		// 处理Http请求任务(LogicSystem中注册的Http路由)
		// 返回值: LogicSystem是否注册过该URL路由
		bool success = Singleton<LogicSystem>::getInstance().handlerGet(_get_url, shared_from_this());
		// 没有注册过路由
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body())
				<< "url not found\r\n";
			writeResponse();
			return;
		}
		// 注册过
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");

		writeResponse();
		return;
	}
	else if (_request.method() == http::verb::post) {
		// 处理Http请求任务(LogicSystem中注册的Http路由)
		// 返回值: LogicSystem是否注册过该URL路由
		bool success = Singleton<LogicSystem>::getInstance().handlerPost(_request.target(), shared_from_this());
		// 没有注册过路由
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body())
				<< "url not found\r\n";
			writeResponse();
			return;
		}
		// 注册过
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");

		writeResponse();
		return;
	}
	else {

	}
}

// 发送响应
void HttpConnection::writeResponse() {
	auto self = shared_from_this();
	// 设置响应体内容大小
	_response.content_length(_response.body().size());
	// 异步写
	http::async_write(
		_socket,
		_response,
		[self](beast::error_code ec, std::size_t) {
			// 异步写回调函数
			// 关闭对方socket
			// 取消超时时间
			self->_socket.shutdown(tcp::socket::shutdown_send, ec);
			self->_deadline.cancel();
		}
	);
}

// 启动异步等待超时
void HttpConnection::checkDeadline() {
	auto self = shared_from_this();
	// 异步等待超时
	_deadline.async_wait(
		[self](beast::error_code ec) {
			if (ec) {
				return;
			}
			// 超时: 关闭本地HttpConnection的socket
			self->_socket.close(ec);
		}
	);
}

void HttpConnection::parseURLParam() {
	// 获取原始URL
	auto raw_url = _request.target();
	std::cout << "raw url = " << raw_url << std::endl;
	// 查询参数位置('?'位置)
	auto query_index = raw_url.find('?');
	if (query_index == std::string::npos) {
		// 未能识别到, 表示没有参数
		_get_url = raw_url;
		return;
	}
	// 截取URL区域
	_get_url = raw_url.substr(0, query_index);
	// 截取URL参数区域
	std::string_view query_string = std::string_view(raw_url.data() + query_index + 1, raw_url.length() - query_index - 1);
	// 各个部分参数
	std::string key;
	std::string value;
	std::size_t pos = 0;
	// 1、根据 & 分割不同参数(&前)
	while((pos = query_string.find('&')) != std::string::npos) {
		auto pair = query_string.substr(0, pos);
		// 根据 = 分割key和value
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			key = Uri::uriDecode(std::string_view(pair.data(), eq_pos));
			value = Uri::uriDecode(std::string_view(pair.data() + eq_pos + 1, pos - eq_pos - 1));
			_get_params[key] = value;
		}
		// 将query_string视图往后移
		query_string.remove_prefix(pos + 1);
	}
	// 2、处理&后参数 / 无&情况
	if (!query_string.empty()) {
		std::size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			key = Uri::uriDecode(std::string_view(query_string.data(), eq_pos));
			value = Uri::uriDecode(std::string_view(query_string.data() + eq_pos + 1, query_string.length() - eq_pos - 1));
			_get_params[key] = value;
		}
	}
	else {
		return;
	}
}


