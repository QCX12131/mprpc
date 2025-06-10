#include "rpcprovider.hpp"
#include "mprpcapplication.hpp"
#include "rpcheader.pb.h"
#include <arpa/inet.h>
#include "zookeeperutil.hpp"
#include "logger.hpp"
#include <cstring>
using namespace muduo;
using namespace muduo ::net;

// 这里是提供给外部使用的，用于发布rpc方法的函数接口
void RpcProvider::NotifyService(::google::protobuf::Service *service)
{
    ServiceInfo sinfo;
    // 获取描述服务对象的信息
    const ::google::protobuf::ServiceDescriptor *sdp = service->GetDescriptor();
    // 获取服务的名字
    std::string service_name = sdp->name();
    // 获取服务对象的service方法的数量
    int method_count = sdp->method_count();
    LOG_INFO("NotifyService:service name:%s", service_name.c_str());

    for (int i = 0; i < method_count; i++)
    {
        // 获取服务对象指定下标的服务方法的描述（框架只给抽象的描述）
        const ::google::protobuf::MethodDescriptor *mdp = sdp->method(i);
        std::string method_name = mdp->name();
        LOG_INFO("NotifyService:method name:%s", method_name.c_str());

        // 记录服务对象的服务方法
        sinfo.m_MethodMap.insert({method_name, mdp});
    }
    // 记录服务对象
    sinfo.m_Service = service;
    m_ServiceMap.insert({service_name, sinfo});
}
// 启动rpc服务，提供提供rpc远程网络调用服务
void RpcProvider::run()
{
    std::string ip = MprpcApplication::getInstance().getConfig().Load("rpcserver_ip");
    uint16_t port = atoi(MprpcApplication::getInstance().getConfig().Load("rpcserver_port").c_str());
    muduo::net::InetAddress address(ip, port);
    // 创建Tcpserver
    muduo::net::TcpServer server(&eventloop_, address, "RpcProvider");
    // 绑定连接回调和消息读写回调，分离网络模块与业务模块
    server.setConnectionCallback(std::bind(&RpcProvider::onConnectionCallback, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::onMessageCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    // 设置线程数量
    server.setThreadNum(6);

    // 把当前rpc节点上要发布的服务全部注册到zk节点上，让rpc client可以从zk上发现服务
    // session timeout 30s zkclient网络IO线程 1/3*timeout时间发送ping消息
    ZkClient zkclient;
    zkclient.start(); // 连接zkserver
    // service_name为永久性节点，method_name为临时性节点
    for (auto &sp : m_ServiceMap)
    {
        // /service_name
        std::string service_path = "/" + sp.first;
        zkclient.create(service_path.c_str(), nullptr, 0);
        for (auto &mp : sp.second.m_MethodMap)
        {
            // 先创建父节点再创建子节点
            //  /service_name/method_name /UserServiceRpc/Login
            std::string method_path = service_path + "/" + mp.first;

            char method_path_data[128] = {0};
            sprintf(method_path_data, "%s:%d", ip.c_str(), port);

            zkclient.create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL); // 临时性节点
        }
    }
    LOG_INFO("rpcprovider start service!!!ip:%s,port:%d", ip.c_str(), port);
    // 启动网络服务
    server.start();
    eventloop_.loop();
}

// 连接回调函数
void RpcProvider::onConnectionCallback(const TcpConnectionPtr &tcp)
{
    if (!tcp->connected())
    {
        // 连接断开
        tcp->shutdown();
    }
}

/*
在框架内部
RPCProvider和RPComsumer协商好之间通信要用的protobuf数据类型
// service_name method_name request
定义Proto message类型 进行数据的序列化和反序列化
       service_name method_name args_size
       粘包问题
看user.proto
16UserserviceLoginzhangsan123456
header_size(4) + header_str + arg_str
*/
// 读写事件回调，如果远程有一个RPC服务的调用请求，这个回调函数就会响应

void RpcProvider::onMessageCallback(const TcpConnectionPtr &tcp,
                                    Buffer *buffer,
                                    Timestamp time)
{
    std::string rec_buf = buffer->retrieveAllAsString(); // 网络上接收的RPC远程调用请求--字符流
    // 从数据流中取前4个字节
    // 不能直接用substr取四个字节，例如 "10"和"10000"看作字符是不同的字节数，但是按照内存二进制存储不会超过4个字节
    // uint32_t header_size = 0;                 // 记录rpc_header_str的长度
    // rec_buf.copy((char *)&header_size, 4, 0); // 把rec_buf从0开始拷贝4个字节内容到header_size

    uint32_t header_size = 0;
    rec_buf.copy(reinterpret_cast<char *>(&header_size), 4, 0); // 把rec_buf从0开始拷贝4个字节内容到header_size_network

    // 根据Header_size读取数据头的原始字符流,反序列化数据，得到rpc请求的详细信息
    std::string rpc_header_str = rec_buf.substr(4, header_size);

    mprpc::RpcHeader rpch;
    std::string Service_name;
    std::string Method_name;
    uint32_t Args_size;
    if (rpch.ParseFromString(rpc_header_str))
    {
        // 数据头反序列化成功
        Service_name = rpch.service_name();
        Method_name = rpch.method_name();
        Args_size = rpch.args_size();
    }
    else
    {
        // 数据头反序列化失败
        LOG_ERR("RpcHeader ParseFromString error");
        return;
    }
    // rpc方法参数
    std::string Arg_str = rec_buf.substr(4 + header_size, Args_size);

    // 获取service对象和method对象
    // 首先判断是否存在
    auto it = m_ServiceMap.find(Service_name);
    if (it == m_ServiceMap.end())
    {
        LOG_ERR("Service name is not exists");
        return;
    }
    auto mit = it->second.m_MethodMap.find(Method_name);
    if (mit == it->second.m_MethodMap.end())
    {
        LOG_ERR("Service_name:%s,Method_name:%s,is not exists!", Service_name.c_str(), Method_name.c_str());
        return;
    }

    ::google::protobuf::Service *sv = it->second.m_Service;        // service对象
    const ::google::protobuf::MethodDescriptor *mdp = mit->second; // method对象

    // 调用的相应的服务方法
    // 生成rpc方法的request和response
    ::google::protobuf::Message *request = sv->GetRequestPrototype(mdp).New();
    if (!request->ParseFromString(Arg_str))
    {
        LOG_ERR("request parse error,content:%s", Arg_str.c_str());
        return;
    }
    ::google::protobuf::Message *response = sv->GetResponsePrototype(mdp).New(); // 等业务做完填充response

    // 给method方法绑定一个Closure回调函数
    ::google::protobuf::Closure *done = ::google::protobuf::NewCallback<RpcProvider, const TcpConnectionPtr &, ::google::protobuf::Message *>(this, &RpcProvider::sendRpcResponse, tcp, response);
    sv->CallMethod(mdp, nullptr, request, response, done); // 例子：UserService对象调用Login方法
}

// 回调操作，用于序列化reponse和网络发送
void RpcProvider::sendRpcResponse(const TcpConnectionPtr &tcp, ::google::protobuf::Message *response)
{
    // 将response序列化
    std::string response_str;

    if (response->SerializeToString(&response_str))
    {
        // 序列化成功之后，通过网络把rpc方法执行的结果返回给调用方
        tcp->send(response_str);
    }
    else
    {
        LOG_INFO("response serialize error");
    }
    tcp->shutdown(); // 模拟http的短链接服务，由rpc服务提供方主动断开连接
}