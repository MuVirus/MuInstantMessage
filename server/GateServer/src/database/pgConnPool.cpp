#include "database/pgConnPool.h"

std::unique_ptr<pqxx::connection> PgConnPool::getConn() {
	std::unique_lock<std::mutex> lock(_mutex);
	// tips: 可以改成wait_for，添加超时时间
	_cond.wait(lock, [this] { return !_pool.empty() || _stop; });
	if (_stop) {
		return nullptr;
	}
	auto conn = std::move(_pool.front());
	_pool.pop();
	return conn;
}

void PgConnPool::returnConn(std::unique_ptr<pqxx::connection> conn) {
	// 判断connection对象是否为空、_stop = true
	if (!conn || _stop) {
		return;
	}
	// 判断是否连接
	try {
		if (!conn->is_open()) {
			// tips:可以加一个重连
			return;
		}
	}
	catch (...) {
		return;
	}
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_pool.push(std::move(conn));
	}
	_cond.notify_one();
}