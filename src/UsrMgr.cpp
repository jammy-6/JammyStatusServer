#include "UsrMgr.h"
#include "log.h"
UsrMgr::UsrMgr(){

}

UsrMgr::~UsrMgr(){

}

bool UsrMgr::judgeUser(int uid,std::string token){
    if(allLoginUser.count(uid)!=0&&allLoginUser[uid]==token){
        LOG_INFO("用户uid:%d,token:%s列于用户聊天服务列表，正确！",uid,token.c_str());
        return true;
    }
    return false;
}
bool UsrMgr::registerUser(int uid,std::string token){
   
   
    allLoginUser.insert(std::pair<int,std::string>(uid,token));
    LOG_INFO("已将uid:%d,token:%s注册进用户聊天服务列表",uid,token.c_str());
    return true;
}
bool UsrMgr::removeUser(int uid){
    std::lock_guard<std::mutex> guard(mutex_);
    allLoginUser.erase(uid);
    LOG_INFO("已将uid:%d移除用户聊天服务列表",uid);
    return true;
}
