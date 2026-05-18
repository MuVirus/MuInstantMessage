#include "database/pgManager.h"
#include "database/pgDao.h"
#include "common/singleton.h"

int PgManager::RegUser(const std::string& name, const std::string& email, const std::string& pass) {
	// tips: 可以加一个参数校验

	// === 业务逻辑
	// tips: 可加密码加密

	return Singleton<PgDao>::getInstance().regUserTransaction(name, email, pass);
}