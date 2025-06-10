#ifndef LOGGER_H
#define LOGGER_H

#include "logqueue.hpp"
#include <string>
enum class MyLogLevel
{
    INFO, // 普通信息
    ERROR // 错误信息
};

// MPRPC框架提供的日志系统
class Logger
{
public:
    static Logger &getInstance();
    // // 设置日志级别
    // void SetLogLevel(MyLogLevel level);
    // 写日志
    void Log(MyLogLevel level, std::string str);

private:
    MyLogLevel loglevel_;               // 日志级别
    LogQueue<std::string> m_lockqueue_; // 日志缓冲队列

    Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(Logger &&) = delete;
};

// 定义宏
#define LOG_INFO(logmsgformat, ...)                            \
    do                                                         \
    {                                                          \
        Logger &log = Logger::getInstance();                   \
        char ch[1024] = {0};                                   \
        snprintf(ch, sizeof(ch), logmsgformat, ##__VA_ARGS__); \
        log.Log(MyLogLevel::INFO, ch);                         \
    } while (0)

#define LOG_ERR(logmsgformat, ...)                             \
    do                                                         \
    {                                                          \
        Logger &log = Logger::getInstance();                   \
        char ch[1024] = {0};                                   \
        snprintf(ch, sizeof(ch), logmsgformat, ##__VA_ARGS__); \
        log.Log(MyLogLevel::ERROR, ch);                        \
    } while (0)
#endif