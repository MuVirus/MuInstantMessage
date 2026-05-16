#include "logic/logicSystem.h"

#include "common/singleton.h"
#include "network/httpConnection.h"
#include "network/varifyGrpcClient.h"

LogicSystem::LogicSystem() {
	// 注册Http Get请求路由
	registerGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->_response.body())
			<< "receive /get_test req" << std::endl;
		int i = 0;
		for (auto& param : connection->_get_params) {
			i++;
			beast::ostream(connection->_response.body())
				<< "param" << i << " key is " << param.first;
			beast::ostream(connection->_response.body())
				<< ", value is " << param.second << std::endl;
		}
	});
	// 注册Post请求路由(json)
	registerPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
		// 获取请求体body，转换成字符串
		auto body_str = beast::buffers_to_string(connection->_request.body().data());

		Json::Value root;
		Json::Value src_root;
		Json::Reader reader;
		// 解析成Json类型
		// src_root : 解析后
		// root : 响应
		// 结构: {"error" : "xxx" , "email" : "xxx"}
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonStr = root.toStyledString();
			beast::ostream(connection->_response.body())
				<< jsonStr;
			return;
		}

		// get email
		auto email = src_root["email"].asString();

		//// 调用grpc服务
		GetVarifyResp resp = Singleton<VarifyGrpcClient>::getInstance().GetVarifyCode(email);

		root["error"] = resp.error();
		//root["error"] = 0;
		root["email"] = src_root["email"];
		std::string jsonStr = root.toStyledString();
		beast::ostream(connection->_response.body())
			<< jsonStr;
		});
	// 用户注册
	registerPost("/user_register", [](std::shared_ptr<HttpConnection> httpConnection) {
		// 解析请求体Json数据：获取请求体body, 转换成字符串, 判断是不是json数据

		// 判断当前验证码是否过期：从Redis中获取请求Json数据中的email值

		// 判断当前验证码(Json)与Redis中验证码是否一致

		// 先从Redis中判断是否有该用户

		// 如果Redis中没有该用户，再从数据库中判断是否有该用户

		// 创建用户
		});
}

LogicSystem::~LogicSystem() {

}

bool LogicSystem::handlerGet(std::string path, std::shared_ptr<HttpConnection> con) {
	if (_get_handlers.find(path) == _get_handlers.end()) {
		return false;
	}

	_get_handlers[path](con);
	return true;
}

void LogicSystem::registerGet(std::string url, HttpHandler handler) {
	_get_handlers[url] = handler;
}


void LogicSystem::registerPost(std::string url, HttpHandler handler) {
	_post_handlers[url] = handler;
}

bool LogicSystem::handlerPost(std::string path, std::shared_ptr<HttpConnection> con) {
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}

	_post_handlers[path](con);
	return true;
}