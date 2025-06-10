#include <iostream>
#include <string>
#include "mprpcapplication.hpp"
#include "rpcprovider.hpp"
#include "friends.pb.h"
#include "logger.hpp"
using namespace std;
using namespace fixbug;

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t id)
    {
        std::cout << "do GetFriendList service:id:" << id << std::endl;
        std::vector<std::string> vec;
        vec.push_back("QQ");
        vec.push_back("TECENT");
        vec.push_back("WECHAT");
        return vec;
    }
    virtual void GetFriendList(::google::protobuf::RpcController *controller,
                               const ::fixbug::GetFriednsListRequest *request,
                               ::fixbug::GetFriednsListResponse *response,
                               ::google::protobuf::Closure *done)
    {
        uint32_t id = request->id();
        std::vector<std::string> frivec = GetFriendList(id);

        response->mutable_result()->set_errcode(1);
        response->mutable_result()->set_errmsg("success");

        for (auto it : frivec)
        {
            std::string *p = response->add_friends();
            *p = it;
        }

        done->Run();
    }

private:
};

int main(int argc, char **argv)
{
    /*框架基础类设计*/
    // 先调用框架的初始化 provider -i config.conf
    MprpcApplication::Init(argc, argv);

    // 把UserService对象发送到网络节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    provider.run(); // 启动RPC服务发布节点
    // 进程进入阻塞状态，等待远程的RPC请求
    return 0;
}