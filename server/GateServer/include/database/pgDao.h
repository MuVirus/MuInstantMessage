#include "common/const.h"
#include "database/pgConnPool.h"

/**
 * @brief Pgsql数据访问对象
 */
class PgDao {
public:
	PgDao();
	~PgDao() { _pool->stop(); };
	/**
	 * @brief 注册用户：调用存储过程(逻辑在数据库端)
	 * @param name 
	 * @param email 
	 * @param pass 
	 */
	int regUser(const std::string& name, const std::string& email, const std::string& pass);
	/**
	 * @brief 注册用户：事务
	 * @param name 
	 * @param email 
	 * @param pass 
	 * @return 
	 */
	int regUserTransaction(const std::string& name, const std::string& email, const std::string& pass);
private:
	std::unique_ptr<PgConnPool> _pool;
};