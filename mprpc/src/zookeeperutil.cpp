#include "zookeeperutil.hpp"
#include "mprpcapplication.hpp"
#include <iostream>
#include <semaphore.h>

// 全局观察器watcher
void global_watcher(zhandle_t *zh, int type,
                    int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT) // 回调消息类型是和会话相关的消息类型，zkserver给zkclient的通知
    {
        if (state == ZOO_CONNECTED_STATE) // zkserver和zkclient连接成功
        {
            sem_t *sem = (sem_t *)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient() : m_zhandle_(nullptr)
{
}
ZkClient::~ZkClient()
{
    if (m_zhandle_ != nullptr)
    {
        zookeeper_close(m_zhandle_);
    }
}

// zkclient启动连接zkserver
void ZkClient::start()
{
    std::string host = MprpcApplication::getInstance().getConfig().Load("zookeeper_ip");
    std::string port = MprpcApplication::getInstance().getConfig().Load("zookeeper_port");

    std::string connstr = host + ":" + port;

    /*
    zookeeper_mt：多线程版本
    zookeeper的API客户端程序提供三个线程
    API调用线程
    网络IO线程 pthread_create poll
    watcher回调线程
*/
    m_zhandle_ = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);

    if (m_zhandle_ == nullptr)
    {
        std::cout << "zookeeper_init error" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle_, &sem); // 给句柄设置一个信号量

    sem_wait(&sem);
    std::cout << "zookeeper_init success" << std::endl;
}
// 在zkclient上根据指定的path创建znode节点
void ZkClient::create(const char *path, const char *data, int datalen, int state) // 默认是永久性节点
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    flag = zoo_exists(m_zhandle_, path, 0, nullptr);
    if (ZNONODE == flag) // path表示的znode不存在
    {
        flag = zoo_create(m_zhandle_, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if (flag == ZOK)
        {
            std::cout << "znode create success... path:" << path << std::endl;
        }
        else
        {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
// 根据参数指定的znode节点路径，或者znode节点的值
std::string ZkClient::GetData(const char *path)
{
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle_, path, 0, buffer, &bufferlen, nullptr);
    if (flag != ZOK)
    {
        std::cout << "get znode error...path:" << path << std::endl;
        return "";
    }
    else
    {
        return buffer;
    }
}
