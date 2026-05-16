#include "network/asioIOContextPool.h"

AsioIOContextPool::AsioIOContextPool(std::size_t size)
	: _nextIOContext(0), _stop(false), _ioContexts(size)
{
	_works.reserve(size);
	for (std::size_t i = 0; i < size; ++i) {
		_works.emplace_back(asio::make_work_guard(_ioContexts[i]));
	}

	for (std::size_t i = 0; i < size; ++i) {
		_threads.emplace_back([this, i] {
			_ioContexts[i].run();
		});
	}
}

AsioIOContextPool::~AsioIOContextPool() {
	stop();
}

void AsioIOContextPool::stop() {
	if (!_stop) {
		for (auto& work : _works) {
			work.reset();
		}

		for (auto& t : _threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	}
}

IOContext& AsioIOContextPool::getIOContext() {
	_nextIOContext = (_nextIOContext + 1) % _ioContexts.size();
	return _ioContexts[_nextIOContext];
}