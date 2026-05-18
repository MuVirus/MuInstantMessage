#include "database/pgDao.h"
#include "common/singleton.h"
#include "config/configManager.h"
#include "database/pgConnPool.h"

PgDao::PgDao() {
	// 读取配置
	auto& config = Singleton<ConfigManager>::getInstance();
	std::string host = config["PgSQL"]["Host"];
	std::string port = config["PgSQL"]["Port"];
	std::string user = config["PgSQL"]["User"];
	std::string pass = config["PgSQL"]["PassWd"];
	std::string schema = config["PgSQL"]["Schema"];
	std::string pgInfo = "dbname=" + schema 
		+ " user=" + user 
		+ " password=" + pass 
		+ " hostaddr=" + host 
		+ " port=" + port;
	std::cout << pgInfo << std::endl;

	// 创建pgsql连接池
	_pool = std::make_unique<PgConnPool>(pgInfo, 5);

}

int PgDao::regUser(const std::string& name, const std::string& email, const std::string& pass) {
	// 获取一个pg连接
	auto conn = _pool->getConn();
	if (!conn) {
		// tips: 这里也应该回收conn的，但是这里没有输出错误码，不好判断
		std::cerr << "[PgDao] Failed to get connection from pool." << std::endl;
		return -1;
	}
	// 添加一个事务，注册用户
	try {
		pqxx::work txn(*conn);

		pqxx::result result = txn.exec_params(
			"SELECT reg_user($1, $2, $3)",
			name, email, pass
		);

		int ret = result[0][0].as<int>();

		txn.commit();

		_pool->returnConn(std::move(conn));

		return ret;
	}
	catch (const pqxx::sql_error& error) {
		std::cerr << "[PgDAO] PgSQL err: " << error.what() <<
			", was query: " << error.query() << std::endl;
		_pool->returnConn(std::move(conn));
		return -1;
	}
	catch (const std::exception& error) {
		std::cerr << "[PgDAO] err: " << error.what() << std::endl;
		_pool->returnConn(std::move(conn));
		return -1;
	}
	return -1;
}

int PgDao::regUserTransaction(const std::string& name, const std::string& email, const std::string& pass) {
	// 获取一个pg连接
	auto conn = _pool->getConn();
	if (!conn) {
		// tips: 这里也应该回收conn的，但是这里没有输出错误码，不好判断
		std::cerr << "[PgDao] Failed to get connection from pool." << std::endl;
		return -1;
	}
	// 添加一个事务，注册用户
	try {
		pqxx::work txn(*conn);

		// 检查邮箱是否存在
		auto emailRes = txn.exec_params(
			"SELECT 1 FROM users WHERE email = $1", email
		);
		if (!emailRes.empty()) {
			txn.abort();
			_pool->returnConn(std::move(conn));
			return 0;
		}

		// 检查用户名是否存在
		auto nameRes = txn.exec_params(
			"SELECT 1 FROM users WHERE user = $1", name
		);
		if (!nameRes.empty()) {
			txn.abort();
			_pool->returnConn(std::move(conn));
			return 0;
		}

		// 插入用户
		auto insertRes = txn.exec_params(
			"INSERT INTO users(name, email, pwd) "
			"VALUES($1, $2, $3) RETURNING uid",
			name, email, pass
		);
		int newId = insertRes[0][0].as<int>();

		// 提交事务
		txn.commit();

		_pool->returnConn(std::move(conn));
		
		return newId;
	}
	catch (const pqxx::sql_error& error) {
		std::cerr << "[PgDAO] PgSQL err: " << error.what() <<
			", was query: " << error.query() << std::endl;
		_pool->returnConn(std::move(conn));
		return -1;
	}
	catch (const std::exception& error) {
		std::cerr << "[PgDAO] err: " << error.what() << std::endl;
		_pool->returnConn(std::move(conn));
		return -1;
	}
}