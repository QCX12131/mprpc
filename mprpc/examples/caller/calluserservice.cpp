#include <iostream>
#include "mprpcapplication.hpp"
#include "mprpcchannel.hpp"
#include "user.pb.h"
using namespace std;

int main(int argc, char **argv)
{
    // 整个程序启动之后，使用mprpc框架来调用rpc服务,先初始化框架，只初始化一次
    MprpcApplication::Init(argc, argv);
    // 演示调用远程发布的rpc方法Login
    // fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // 修复后：使用智能指针管理
    auto channel = std::make_shared<MprpcChannel>();
    fixbug::UserServiceRpc_Stub stub(channel.get());
    // rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    // rpc方法的响应参数
    fixbug::LoginResponse response;
    // 发起rpc方法的调用 同步的rpc调用过程
    stub.Login(nullptr, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送

    // rpc调用完成，读调用的结果
    if (response.result().errcode() == 1)
    {
        std::cout << "rpc login response:" << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
    }

    // rpc方法的请求参数
    fixbug::RegisterRequest request1;
    request1.set_id(2000);
    request1.set_name("qianchenxi");
    request1.set_pwd("123456789");
    // rpc方法的响应参数
    fixbug::RegisterResponse response1;
    // 发起rpc方法的调用 同步的rpc调用过程
    stub.Register(nullptr, &request1, &response1, nullptr); // RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送
                                                            // rpc调用完成，读调用的结果
    if (response1.result().errcode() == 1)
    {
        std::cout << "rpc register response:" << response1.success() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error:" << response1.result().errmsg() << std::endl;
    }
    return 0;
}