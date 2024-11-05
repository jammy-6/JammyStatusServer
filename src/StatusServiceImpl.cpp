#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "Global.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
std::string generate_unique_string() {
    // 创建UUID对象
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    // 将UUID转换为字符串
    std::string unique_string = boost::uuids::to_string(uuid);
    return unique_string;
}
Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
    std::string prefix("llfc status server has received :  ");
    _server_index = (_server_index++) % (_servers.size());
    auto &server = _servers[_server_index];
    reply->set_host(server.host);
    reply->set_port(server.port);
    reply->set_error(ERRORCODE::Success);
    reply->set_token(generate_unique_string());
    return Status::OK;
}
StatusServiceImpl::StatusServiceImpl():_server_index(0)
{

    ChatServer server;
    server.port = gConfigMgr["ChatServer1"]["Port"];
    server.host = gConfigMgr["ChatServer1"]["Host"];
    _servers.push_back(server);
    server.port = gConfigMgr["ChatServer2"]["Port"];
    server.host = gConfigMgr["ChatServer2"]["Host"];
    _servers.push_back(server);
}