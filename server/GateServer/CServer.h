#pragma once

#include "const.h"

// CServer
// 任务
// 1、接收客户端Tcp连接, 通过socket创建对应的HttpConnection

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer(asio::io_context &ioc, unsigned short port);
	void start();
	void doAccept();
private:
	tcp::acceptor _acceptor;
	asio::io_context& _context;
	boost::asio::ip::tcp::socket _socket;
};

