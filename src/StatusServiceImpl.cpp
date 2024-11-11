#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "Global.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "UsrMgr.h"
std::string generate_unique_string() {
    // 创建UUID对象
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    // 将UUID转换为字符串
    std::string unique_string = boost::uuids::to_string(uuid);
    return unique_string;
}
Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
    
    _server_index = (_server_index++) % (_servers.size());
    auto &server = _servers[_server_index];
    reply->set_host(server.host);
    reply->set_port(server.port);
    reply->set_error(ERRORCODE::Success);
    reply->set_token(generate_unique_string());
    UsrMgr::GetInstance()->registerUser(request->uid(),reply->token());
    LOG_INFO("StatusServiceImpl::GetChatServer调用成功，响应为%s:%s,token=%s",server.host.c_str(),server.port.c_str(),reply->token().c_str());
    return Status::OK;
}


Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request,
        LoginRsp* reply) 
{
    
    int uid = request->uid();
    std::string token = request->token();
    if(UsrMgr::GetInstance()->judgeUser(uid,token)){
        reply->set_error(Success);

    }else{
        reply->set_error(ERROR_UID_TOKEN_NOT_CORRECT);
    }
    reply->set_token(token);
    reply->set_uid(uid);
    LOG_INFO("StatusServiceImpl::Login调用成功，响应为error = %d,uid=%d,token=%s",reply->error(),uid,token.c_str());
    return Status::OK;
}
StatusServiceImpl::StatusServiceImpl():_server_index(0)
{
    LOG_INFO("JammyStatusServer启动成功");
    ChatServer server;
    server.port = gConfigMgr["ChatServer1"]["Port"];
    server.host = gConfigMgr["ChatServer1"]["Host"];
    _servers.push_back(server);
    LOG_INFO("获取聊天服务器%s:%s成功",server.host.c_str(),server.port.c_str());
    // server.port = gConfigMgr["ChatServer2"]["Port"];
    // server.host = gConfigMgr["ChatServer2"]["Host"];
    // LOG_INFO("获取聊天服务器%s:%s成功",server.host.c_str(),server.port.c_str());
    // _servers.push_back(server);
    
}