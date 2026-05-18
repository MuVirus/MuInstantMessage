-- 注册
SELECT reg_user('zhangsan', 'zs@email.com', '123456');
-- 应返回 1

-- 用户名重复
SELECT reg_user('zhangsan', 'other@email.com', '123456');
-- 应返回 -1

-- 邮箱重复
SELECT reg_user('lisi', 'zs@email.com', '123456');
-- 应返回 -2

-- 查看数据
SELECT * FROM users;
