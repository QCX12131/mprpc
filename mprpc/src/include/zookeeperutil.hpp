#ifndef ZOOKEEPERUTIL_H
#define ZOOKEEPERUTIL_H

#include <zookeeper/zookeeper.h>
#include <string>
#include <semaphore.h>

// 封装zk客户端类
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    void start();
    void create(const char *path, const char *data, int datalen, int state = 0);
    std::string GetData(const char *path);

private:
    // zk的客户端句柄
    zhandle_t *m_zhandle_;
};

#endif