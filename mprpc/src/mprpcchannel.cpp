#include "mprpcchannel.hpp"
#include <string>
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include "mprpcapplication.hpp"
#include "mprcpcontroller.hpp"
#include "logger.hpp"
#include "zookeeperutil.hpp"
#include <unistd.h>

// request格式=》
// header_size+service_name+method_name+args_size+args_str
//  所有通过stub代理调用的rpc方法都经过这里，统一做rpc方法调用的数据序列化和网络发送
void MprpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor *method, ::google::protobuf::RpcController *controller, const ::google::protobuf::Message *request, ::google::protobuf::Message *response, ::google::protobuf::Closure *done)
{
    const ServiceDescriptor *ds = method->service();
    std::string service_name = ds->name();
    std::string method_name = method->name();

    // 获取参数的序列化字符串长度 args_size
    int args_size = 0;
    std::string args_str;
    // request目前只包含args_str部分
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("serialize request error");
        return;
    }

    // 定义rpc的请求header
    mprpc::RpcHeader rpcheader;
    rpcheader.set_service_name(service_name);
    rpcheader.set_method_name(method_name);
    rpcheader.set_args_size(args_size);
    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcheader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        controller->SetFailed("rpcheader serialize error!");
        return;
    }

    // 将所有的数据放在一个字符串里面，通过网络发送
    std::string rpc_send_str;
    // 前4个字节使用二进制存储
    rpc_send_str.insert(0, std::string((char *)&header_size, 4));
    rpc_send_str = rpc_send_str + rpc_header_str + args_str;

    /*-------------------序列化完成-----------------------------*/

    // 打印调试信息

    // 使用tcp编程完成rpc方法的远程调用
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        char errtext[512] = {0};
        sprintf(errtext, "create socket errno:%d", errno);
        controller->SetFailed(errtext);
        return;
    }
    // =================从配置文件中获取rpcserver服务器的ip地址和端口号
    // std::string ip = MprpcApplication::getInstance().getConfig().Load("rpcserver_ip");
    // uint16_t port = atoi(MprpcApplication::getInstance().getConfig().Load("rpcserver_port").c_str());
    //==========================从配置中心获取rpcserver服务器的ip地址和端口号
    ZkClient zkclient;
    zkclient.start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string hostdata = zkclient.GetData(method_path.c_str());
    if (hostdata == "")
    {
        controller->SetFailed(method_path + "is not exits");
        return;
    }
    int idx = hostdata.find(":");
    if (idx == -1)
    {
        controller->SetFailed(method_path + "address is invaild");
        return;
    }
    std::string ip = hostdata.substr(0, idx);
    uint16_t port = atoi(hostdata.substr(idx + 1, hostdata.size() - idx - 1).c_str());
    struct sockaddr_in server_addr;
    // 绑定ip地址和端口号
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 发起连接,连接rpc服务节点(事实上是要在服务配置中心先注册服务的ip地址和端口号，之后客户端再请求连接)
    if (-1 == connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        close(clientfd);
        char errtext[512] = {0};
        sprintf(errtext, "connect error!errno:%d", errno);
        controller->SetFailed(errtext);
        return;
    }

    // 发送rpc请求给服务器
    if (-1 == send(clientfd, rpc_send_str.c_str(), rpc_send_str.size(), 0))
    {
        close(clientfd);
        char errtext[512] = {0};
        sprintf(errtext, "send error!errno:%d", errno);
        controller->SetFailed(errtext);
        return;
    }
    // 接收rpc请求的响应值
    // 从服务器接收rpc方法的响应（也就是返回值）
    //  接收完整数据
    char buffer[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, buffer, sizeof(buffer), 0)))
    {
        close(clientfd);
        char errtext[512] = {0};
        sprintf(errtext, "recv error!errno:%d", errno);
        controller->SetFailed(errtext);
        return;
    }

    // rpc调用完成之后,框架帮忙反序列化解析respose，在calluserservice.cpp完成结果的读取
    std::string response_str(buffer, 0, recv_size);
    // 反序列化response_str,并放入函数参数response
    if (!response->ParseFromString(response_str))
    {
        close(clientfd);
        char errtext[4096] = {0};
        sprintf(errtext, "response ParseFromString error!response_str:%s", buffer);
        controller->SetFailed(errtext);
        return;
    }
    close(clientfd);
}