#ifndef MPRPCCONFIG_H
#define MPRPCCONFIG_H

#include <unordered_map>

#include <string>

// 框架读取配置文件类
// rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port
class MprpcConfig
{
public:
    // 负责解析加载配置文件
    void LoginConfigFile(const char *config_file);
    // 查询配置项信息
    std::string Load(const std::string &key);

private:
    std::unordered_map<std::string, std::string> m_config_map;
    // 去掉字符串前后的空格
    void trim(std::string &str);
};

#endif