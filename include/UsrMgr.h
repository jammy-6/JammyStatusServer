#ifndef USRMGR_H
#define USRMGR_H
#include "Global.h"
#include "Singleton.h"
#include <map>
#include <mutex>
class UsrMgr : public Singleton<UsrMgr>{
public:
    bool registerUser(int uid,std::string token);
    bool removeUser(int uid);
    bool judgeUser(int uid,std::string token);
    ~UsrMgr();
private:
    friend class Singleton<UsrMgr>;
    UsrMgr();
    std::mutex mutex_;
    std::map<int,std::string> allLoginUser;
};

#endif