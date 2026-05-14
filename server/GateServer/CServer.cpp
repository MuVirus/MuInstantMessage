#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context &context, unsigned short port) 
	: _context(context)
	, _acceptor(context, tcp::endpoint(tcp::v4(), port))
	, _socket(_context) {

}

void CServer::start() {
	doAccept();
}

// 监听连接
void CServer::doAccept() {
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
		if (ec) {
			std::cout << "[CServer] 连接错误: " << ec.message() << std::endl;
		}
		else {
			std::cout << "[CServer] 新连接: " << self->_socket.remote_endpoint().address().to_string() << std::endl;
			// 通过新socket创建HttpConnection
			std::shared_ptr<HttpConnection> connection = std::make_shared<HttpConnection>(std::move(self->_socket));
			// 启动HttpConnection
			connection->start();
		}
		// 继续监听连接
		self->doAccept();
	});
}