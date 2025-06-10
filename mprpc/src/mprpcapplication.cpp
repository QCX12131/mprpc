#include "mprpcapplication.hpp"
#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

// int getopt(int argc, char *const argv[],
//            const char *optstring);

// 静态成员类外初始化 C++11之后允许在构造函数初始化列表中初始化
MprpcConfig MprpcApplication::config_;

void showArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
    if (argc < 2)
    {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1) // 循环解析所有选项,解析命令行参数，提取 -i 及其参数
    {
        switch (c)
        {
        case 'i':
            config_file = optarg; // optarg 存储选项参数
            break;
        case '?':
            std::cout << "invaild args!!!" << std::endl; // 遇到未定义的选项或缺少参数
            showArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            showArgsHelp();
            std::cout << "need <configfile>" << std::endl;
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    std::cout << "config_file:" << config_file << std::endl;
    // 开始加载配置文件 rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port
    config_.LoginConfigFile(config_file.c_str());

    std::cout << "rpcserver_ip:" << config_.Load("rpcserver_ip") << std::endl;
    std::cout << "rpcserver_port:" << config_.Load("rpcserver_port") << std::endl;
    std::cout << "zookeeper_ip:" << config_.Load("zookeeper_ip") << std::endl;
    std::cout << "zookeeper_port:" << config_.Load("zookeeper_port") << std::endl;
}
// 懒汉模式
MprpcApplication &MprpcApplication::getInstance()
{
    static MprpcApplication app;
    return app;
}

// 返回配置
MprpcConfig &MprpcApplication::getConfig()
{
    return config_;
}