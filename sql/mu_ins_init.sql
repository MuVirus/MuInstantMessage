-- 用户表，只放登录相关的
CREATE TABLE IF NOT EXISTS users (
    uid   SERIAL PRIMARY KEY,
    name  VARCHAR(255) UNIQUE NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    pwd   VARCHAR(255) NOT NULL
);

-- 注册函数
CREATE OR REPLACE FUNCTION reg_user(
    p_name  VARCHAR,
    p_email VARCHAR,
    p_pwd   VARCHAR
) RETURNS INT AS $$
BEGIN
    IF EXISTS (SELECT 1 FROM users WHERE name = p_name) THEN
        RETURN -1;  -- 用户名已存在
    END IF;

    IF EXISTS (SELECT 1 FROM users WHERE email = p_email) THEN
        RETURN -2;  -- 邮箱已存在
    END IF;

    INSERT INTO users(name, email, pwd)
    VALUES(p_name, p_email, p_pwd);

    RETURN 1;  -- 成功
END;
$$ LANGUAGE plpgsql;
