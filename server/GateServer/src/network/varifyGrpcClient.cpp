#include "network/varifyGrpcClient.h"

#include "common/singleton.h"
#include "config/configManager.h"

VarifyGrpcClient::VarifyGrpcClient() {
	ConfigManager& config = Singleton<ConfigManager>::getInstance();
	std::string host = config["VarifyServer"]["Host"];
	std::string port = config["VarifyServer"]["Port"];
	_conn = std::make_unique<RPCConPool>(host, port, 5);
}

VarifyGrpcClient::~VarifyGrpcClient() {

}

GetVarifyResp VarifyGrpcClient::GetVarifyCode(std::string email) {
	ClientContext context;
	GetVarifyReq request;
	GetVarifyResp reply;
	
	// 设置请求email
	request.set_email(email);

	// 从连接池中获取VarifyService的存根
	_stub = _conn->getConn();
	if (_stub == nullptr) {
		reply.set_error(ErrorCodes::RPCConPoolClosed);
		return reply;
	}
	// 获取grpc服务
	Status status = _stub->GetVarifyCode(&context, request, &reply);

	// 归还存根
	_conn->returnConn(std::move(_stub));

	// 状态响应
	if (status.ok()) {
		return reply;
	}
	else {
		reply.set_error(ErrorCodes::RPCFailed);
		return reply;
	}
}