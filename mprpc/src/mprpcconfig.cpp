#include "mprpcconfig.hpp"
#include <iostream>
#include <string>
using namespace std;

// 负责解析加载配置文件
void MprpcConfig::LoginConfigFile(const char *config_file)
{
    std::cout << "DEBUG:START TO LOGIN CONFIGFILE!!!" << std::endl;
    // 加载配置文件 rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port
    FILE *f = fopen(config_file, "r");
    if (f == nullptr)
    {
        std::cout << config_file << " is not exists" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 读取每一行的数据
    // 1\注释
    // 2、正确的配置项，有=
    // 3、去掉开头多余的空格
    while (!feof(f))
    {
        char buf[512] = {0};
        fgets(buf, 512, f);
        std::string src_buf(buf);
        trim(src_buf);
        // 判断#注释
        if (src_buf[0] == '#' || src_buf.empty())
        {
            continue; // 继续读文件
        }

        // 解析配置项
        int idx = src_buf.find('=');
        if (idx == -1)
        {
            // 配置项不合法
            continue;
        }

        // 解析有效配置项
        std::string key;
        std::string value;
        key = src_buf.substr(0, idx);
        trim(key);
        int endidx = src_buf.find('\n', idx);
        if (endidx != -1)
        {
            value = src_buf.substr(idx + 1, endidx - idx - 1);
            trim(value);
        }
        else
        {
            value = src_buf.substr(idx + 1, src_buf.size() - idx - 1);
            trim(value);
        }
        m_config_map.insert({key, value});
    }
}
// 查询配置项信息
std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_config_map.find(key);
    if (it != m_config_map.end())
    {
        return it->second;
    }
    return "";
}

// 去掉字符串前后的空格
void MprpcConfig::trim(std::string &src_buf)
{

    // 去掉字符串前面多余的空格
    int idx = src_buf.find_first_not_of(' '); // 找到第一个为空格的字符
    if (idx != -1)
    {
        src_buf = src_buf.substr(idx, src_buf.size() - idx);
    }
    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if (idx != -1)
    {
        src_buf = src_buf.substr(0, idx + 1);
    }
}