#pragma once
#include "common/const.h"

/**
 * @brief Pgsql连接池
 */
class PgConnPool {
public:
	PgConnPool(const std::string& pgInfo, std::size_t count = std::thread::hardware_concurrency())
		: _count(count), _stop(false)
	{
		for (int i = 0; i < count; ++i) {
			_pool.push(std::make_unique<pqxx::connection>(pgInfo));
		}
		// tips: 可以判断一下是否open
	}
	std::unique_ptr<pqxx::connection> getConn();
	void returnConn(std::unique_ptr<pqxx::connection> conn);
	void stop() {
		_stop = true;
		_cond.notify_all();
	}
private:
	std::queue<std::unique_ptr<pqxx::connection>> _pool;
	std::mutex _mutex;
	std::size_t _count;
	std::condition_variable _cond;
	std::atomic<bool> _stop;
};