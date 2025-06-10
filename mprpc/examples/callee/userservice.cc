#include <iostream>
#include <string>
#include "mprpcapplication.hpp"
#include "rpcprovider.hpp"
#include "user.pb.h"
using namespace std;
using namespace fixbug;

class UserService : public fixbug::UserServiceRpc // 在RPC服务提供者端
{
public:
    bool Login(std::string name, string pwd)
    {
        std::cout << "doing local Service Login" << std::endl;
        std::cout << "name:" << name << " " << "password:" << pwd << std::endl;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local Service Register" << std::endl;
        std::cout << "id" << id << "name:" << name << " " << "password:" << pwd << std::endl;
        return true;
    }

    // 重写UserServiceRpc的虚函数 下面的方法都是框架直接调用--------------这不是框架代码 而是业务代码
    virtual void Login(::google::protobuf::RpcController *controller,
                       const ::fixbug::LoginRequest *request,
                       ::fixbug::LoginResponse *response,
                       ::google::protobuf::Closure *done)
    {
        // 框架给业务上报了请求参数字节流，反序列化为LoginRequest

        // 获取参数中的数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Login(name, pwd); // 本地业务调用

        // 将返回值写入response
        fixbug::ResultCode *rc = response->mutable_result();
        rc->set_errcode(1);
        rc->set_errmsg("success");
        response->set_success(ret);

        // 由框架去序列化

        // 执行回调操作------即响应消息数据的序列化和网络发送
        done->Run();
    }

    virtual void Register(::google::protobuf::RpcController *controller,
                          const ::fixbug::RegisterRequest *request,
                          ::fixbug::RegisterResponse *response,
                          ::google::protobuf::Closure *done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        fixbug::ResultCode *rc = response->mutable_result();
        rc->set_errcode(1);
        rc->set_errmsg("success");
        response->set_success(ret);

        // 执行回调操作------即响应消息数据的序列化和网络发送
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
    provider.NotifyService(new UserService());

    provider.run(); // 启动RPC服务发布节点
    // 进程进入阻塞状态，等待远程的RPC请求
    return 0;
}