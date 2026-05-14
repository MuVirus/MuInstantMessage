import { RedisClient } from "bun";
import { redis_host, redis_port, redis_pass } from "./config";

const redisUrl = `redis://${redis_host}:${redis_port}`;
const RedisCli = new RedisClient("redis://" + redis_host + ":" + redis_port, {});
RedisCli.close();

/**
 * 监听redis连接成功
 */
RedisCli.onconnect = () => {
    console.log("Connected to Redis server");
};

/**
 * 监听Redis连接错误事件
 * @param error 
 */
RedisCli.onclose = error => {
    console.error("Redis服务器关闭: ", error);
    RedisCli.close();
};

/**
 * 设置心跳60秒
 */
setInterval(async () => {
    await RedisCli.set("heartbeat", String(Date.now()));
}, 60000);

/**
 * key 获取 value
 */
export async function GetRedis(key: string): Promise<string | null> {
    try {
        const result = await RedisCli.get(key);
        if (result === null) {
            console.log("result:", "<" + result + ">", "This key cannot be found...");
            return null;
        }
        console.log("result:", "<" + result + ">", "Get key success!");
        return result;
    } catch (error) {
        console.log("GetRedis error is ", error);
        return null;
    }
}

/**
 * 查询redis中是否存在key
 */
export async function QueryRedis(key: string): Promise<boolean | null> {
    try {
        const result = await RedisCli.exists(key);
        return result;
    } catch (error) {
        console.log("QueryRedis error is ", error);
        return null;
    }
}

/**
 * 设置kv键值对，并添加过期时间(秒)
 */
export async function SetRedisExpire(key: string, value: string, expire: number): Promise<boolean> {
    try {
        await RedisCli.set(key, value);
        await RedisCli.expire(key, expire);
        return true;
    } catch (error) {
        console.log("Set Redis Expire error is ", error);
        return false;
    }
}

/**
 * 退出方法
 */

export function close() {
    RedisCli.close();
}