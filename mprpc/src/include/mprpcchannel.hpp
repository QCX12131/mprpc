#ifndef MPRPCCHANNEL_H
#define MPRPCCHANNEL_H

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
using namespace ::google::protobuf;

class MprpcChannel : public ::google::protobuf::RpcChannel
{
public:
    // 所有通过stub代理调用的rpc方法都经过这里，统一做rpc方法调用的数据序列化和网络发送
    void CallMethod(const ::google::protobuf::MethodDescriptor *method, ::google::protobuf::RpcController *controller, const ::google::protobuf::Message *request, ::google::protobuf::Message *response, ::google::protobuf::Closure *done);

private:
};

#endif