#ifndef MPRPCAPPLICATION_H
#define MPRPCAPPLICATION_H
#include "mprpcconfig.hpp"

// mprpc框架负责初始化的类-单例模式
class MprpcApplication
{
public:
    static void Init(int argc, char **argv);
    // 懒汉模式
    static MprpcApplication &getInstance();

    // 返回配置
    static MprpcConfig &getConfig();

private:
    static MprpcConfig config_;

    MprpcApplication() {}; // 构造函数私有化
    MprpcApplication(const MprpcApplication &) = delete;
    MprpcApplication(MprpcApplication &&) = delete;
    MprpcApplication &operator=(const MprpcApplication &) = delete;
    MprpcApplication &operator=(MprpcApplication &&) = delete;
};

#endif