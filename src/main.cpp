#include <iostream>
#include "Global.h"
#include "ConfigMgr.h"
#include "hiredis.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "AsioIOServicePool.h"
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "StatusServiceImpl.h"
void RunServer() {
    
    LOG_INFO("JammyStatusServer启动中...");
    Log::Instance()->init(
        std::stoi(gConfigMgr["LogSystem"]["Level"]),
        gConfigMgr["LogSystem"]["Path"].c_str(),
        gConfigMgr["LogSystem"]["Suffix"].c_str(),
        std::stoi(gConfigMgr["LogSystem"]["Async"])
    );
    
    MysqlMgr::GetInstance();
    RedisMgr::GetInstance();

    std::string server_address(gConfigMgr["StatusServer"]["Host"]+":"+ gConfigMgr["StatusServer"]["Port"]);
    StatusServiceImpl service;
    grpc::ServerBuilder builder;
    // 监听端口和添加服务
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    // 构建并启动gRPC服务器
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    LOG_INFO("StatusServer启动成功，监听地址为%s",server_address.c_str());
    // 创建Boost.Asio的io_context
    boost::asio::io_context io_context;
    // 创建signal_set用于捕获SIGINT
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    // 设置异步等待SIGINT信号
    signals.async_wait([&server](const boost::system::error_code& error, int signal_number) {
        if (!error) {
            LOG_ERROR("StatusServer async_wait异常，即将退出");
            server->Shutdown(); // 优雅地关闭服务器
        }
        });
    // 在单独的线程中运行io_context
    std::thread([&io_context]() { io_context.run(); }).detach();
    // 等待服务器关闭
    server->Wait();
    io_context.stop(); // 停止io_context
}
int main(int argc, char** argv) {
    try {
        RunServer();
    }
    catch (std::exception const& e) {
        LOG_ERROR("StatusServer RunServer异常，异常为%s",e.what());
        return EXIT_FAILURE;
    }
    return 0;
}