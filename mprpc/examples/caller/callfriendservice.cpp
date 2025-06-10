#include <iostream>
#include "mprpcapplication.hpp"
#include "mprpcchannel.hpp"
#include "friends.pb.h"
#include "mprcpcontroller.hpp"
using namespace std;

int main(int argc, char **argv)
{
    // 整个程序启动之后，使用mprpc框架来调用rpc服务,先初始化框架，只初始化一次
    MprpcApplication::Init(argc, argv);
    // 演示调用远程发布的rpc方法Login
    // fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // 修复后：使用智能指针管理
    auto channel = std::make_shared<MprpcChannel>();
    fixbug::FriendServiceRpc_Stub stub(channel.get());
    // rpc方法的请求参数
    fixbug::GetFriednsListRequest request;
    request.set_id(1000);
    // rpc方法的响应参数
    fixbug::GetFriednsListResponse response;
    // 发起rpc方法的调用 同步的rpc调用过程
    MprpcController controller;
    stub.GetFriendList(&controller, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送
    /*使用控制模块---打印错误信息*/
    // rpc调用完成，读调用的结果
    if (controller.Failed())
    {
        std::cout << "errText:" << controller.ErrorText() << std::endl;
    }
    else
    {
        if (response.result().errcode() == 1)
        {
            std::cout << "rpc login response success:" << std::endl;
            int size = response.friends_size();
            for (int i = 0; i < size; i++)
            {
                std::cout << "index:" << i + 1 << " " << "name:" << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
        }
        return 0;
    }
}