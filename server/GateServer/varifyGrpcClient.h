#pragma once
#include "const.h"

#include "message.grpc.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyResp;
using message::VarifyService;

/**
 * @brief RPC连接池
 * 
 * @detail 这里好像就只是为了给VarifySerivce服务的，不是通用的RPC连接池
 */
class RPCConPool {
public:
	RPCConPool(std::string host, std::string port, std::size_t poolSize = std::thread::hardware_concurrency())
		: _poolSize(poolSize), _host(host), _port(port), _stop(false)
	{
		// 连接成RPC链接
		static std::string configTarget = host + ":" + port;
		// 按照poolSize创建多个VarifyService客户端（存根）
		for (int i = 0; i < poolSize; ++i) {
			std::shared_ptr<Channel> channel = grpc::CreateChannel(configTarget, grpc::InsecureChannelCredentials());
			_connections.push(VarifyService::NewStub(channel));
		}
	}
	~RPCConPool() {
		std::lock_guard<std::mutex> lock(_mutex);
		stop();
	}
	/**
	 * @brief 获取VarifyService存根
	 * 
	 * @detail 转移存根队列头部元素的所有权
	 * 
	 * @return 
	 */
	std::unique_ptr<VarifyService::Stub> getConn() {
		std::unique_lock<std::mutex> lock(_mutex);
		// 当前队列是否为空或者连接池是否关闭
		_cond.wait(lock, [this] { return _stop || !_connections.empty(); });
		// 先判断连接池是否关闭，如果关闭则返回nullptr
		if (_stop) {
			return nullptr;
		}
		// 转移存根队列头部元素的所有权
		auto connection = std::move(_connections.front());
		_connections.pop();
		return connection;
	}
	/**
	 * @brief 返回存根
	 * 
	 * @detail 当rpc发送完毕后，将所有权归还给grpc池
	 * 
	 * @param connection 
	 */
	void returnConn(std::unique_ptr<VarifyService::Stub> connection) {
		std::lock_guard<std::mutex> lock(_mutex);
		// 判断当前连接池是否关闭
		if (_stop) {
			return;
		}
		// 将存根的所有权归还给连接池
		_connections.push(std::move(connection));
		// 唤醒
		_cond.notify_one();
	}
	/**
	 * @brief 关闭连接池
	 * 
	 * @detail 用于外部调用
	 * 
	 */
	void stop() {
		_stop = true;
		_cond.notify_all();
	}
private:
	std::atomic<bool> _stop;
	std::size_t _poolSize;
	std::string _host;
	std::string _port;
	std::queue<std::unique_ptr<VarifyService::Stub>> _connections;
	std::mutex _mutex;
	std::condition_variable _cond;
};

/**
 * @brief 验证服务gRPC客户端
 * 
 * @detail 外部应该设置成单例模式
 */
class VarifyGrpcClient
{
public:
	VarifyGrpcClient();
	~VarifyGrpcClient();
	GetVarifyResp GetVarifyCode(std::string email);
private:
	std::unique_ptr<VarifyService::Stub> _stub;
	std::unique_ptr<RPCConPool> _conn;
};