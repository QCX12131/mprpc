#ifndef RPCPROVIDER_H
#define RPCPROVIDER_H

#include <google/protobuf/service.h>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <functional>
#include <string>
#include <google/protobuf/descriptor.h> // 提供 ServiceDescriptor 的完整定义
using namespace std;
using namespace muduo;
using namespace muduo::net;

/*
servicename ---- service描述
                   service* 记录服务对象
                   method_name----->method方法对象
*/
/*
service_name:UserServiceRpc
method_name:Login
*/

// 框架提供专门负责发布rpc服务的网络对象类
class RpcProvider
{
public:
    // 这里是提供给外部使用的，用于发布rpc方法的函数接口
    void NotifyService(::google::protobuf::Service *service);
    // 启动rpc服务，提供提供rpc远程网络调用服务
    void run();

private:
    // 组合了TcpServer对象
    std::unique_ptr<muduo::net::TcpServer> tcpserver_Ptr_;
    // 组合Eventloop
    muduo::net::EventLoop eventloop_;

    // 服务类型信息
    struct ServiceInfo
    {
        ::google::protobuf::Service *m_Service;                                                    // 保存服务对象
        std::unordered_map<std::string, const ::google::protobuf::MethodDescriptor *> m_MethodMap; // 保存服务方法
    };
    std::unordered_map<std::string, ServiceInfo> m_ServiceMap; // 存储注册成功的服务对象和服务方法的所有信息

    // 连接回调函数
    void onConnectionCallback(const TcpConnectionPtr &tcp);
    // 读写事件回调
    void onMessageCallback(const TcpConnectionPtr &tcp,
                           Buffer *buffer,
                           Timestamp time);
    // 回调操作，用于序列化reponse和网络发送
    void sendRpcResponse(const TcpConnectionPtr &tcp, ::google::protobuf::Message *);
};

#endif