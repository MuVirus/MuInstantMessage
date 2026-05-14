#include "const.h"

#include "singleton.h"
#include "CServer.h"
#include "configManager.h"
#include "asioIOContextPool.h"

int main()
{
    try {
        // 加载配置文件
        Singleton<ConfigManager>::getInstance();

        // 从事件循环池中获取ioContext
        auto& context = Singleton<AsioIOContextPool>::getInstance().getIOContext();
        
        // 信号(ctrl + c)
        asio::signal_set signals(context, SIGINT, SIGTERM);
        signals.async_wait([&context](beast::error_code ec, int num) {
            if (ec) {
                return;
            }
            // 关闭事件循环
            context.stop();
        });

        // 启动CServer, 接收客户端TCP连接, 接收到了就创建对应的HTTP连接
        std::shared_ptr<CServer> server = std::make_shared<CServer>(context, atoi(Singleton<ConfigManager>::getInstance()["GateServer"]["Port"].c_str()));
        server->start();

        // 初始化redis
        auto& redis = Singleton<redis::Redis>::getInstance("tcp://" + Singleton<ConfigManager>::getInstance()["Redis"]["Host"] + ":" + Singleton<ConfigManager>::getInstance()["Redis"]["Port"]);
        //redis.set("hello", "ggg");

        // 执行事件循环
        context.run();
    }
    catch (std::exception const& e) {
        std::cerr << "[GateServer] Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}