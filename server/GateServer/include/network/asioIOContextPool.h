#pragma once
#include "common/const.h"

using IOContext = asio::io_context;
using WorkGuard = asio::executor_work_guard<IOContext::executor_type>;

/**
 * @brief 异步事件循环池
 * 
 * @detail 采用多IOContext模式，以round-robin（循环）返回IoContext
 */
class AsioIOContextPool
{
public:
	~AsioIOContextPool();
	AsioIOContextPool(std::size_t size = std::thread::hardware_concurrency());
	// 删掉拷贝构造
	AsioIOContextPool(const AsioIOContextPool&) = delete;
	AsioIOContextPool& operator=(const AsioIOContextPool&) = delete;
	// 从io_context池中获取io_context引用
	IOContext& getIOContext();
	// 停止io_context池
	void stop();
private:
	std::vector<std::thread> _threads;
	std::vector<IOContext> _ioContexts;
	std::vector<WorkGuard> _works;
	std::size_t _nextIOContext;
	bool _stop;
};

