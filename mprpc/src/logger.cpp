#include "logger.hpp"
#include <thread>
#include <time.h>
#include <iostream>
Logger::Logger()
{
    // 启动写日志线程，将日志从queue里拿出来写入磁盘文件中
    std::thread t([&]()
                  {
for(;;)
{
    // 获取当前的日期，将日志从queue里拿出来写入磁盘文件中，写入相应的文件当中a+
    time_t t1=time(nullptr);
    tm* nowtm=localtime(&t1);
    char txtname[1024]={0};
    sprintf(txtname,"%d-%d-%d-log.txt",nowtm->tm_year+1900,nowtm->tm_mon+1,nowtm->tm_mday);

    FILE* pf=fopen(txtname,"a+");
    if(pf==nullptr)
    {
        std::cout<<"Logger filename open error"<<std::endl;
        exit(EXIT_FAILURE);
    }

    std::string msg=m_lockqueue_.pop();

    //给日志加上时分秒信息
    char time_buf[1024]={0};
    sprintf(time_buf,"%d:%d:%d=>",
        nowtm->tm_hour,
        nowtm->tm_min,
        nowtm->tm_sec);
    msg.insert(0,time_buf);
    msg.append("\n");
    fputs(msg.c_str(),pf);
    fclose(pf);
} });
    t.detach();
}

Logger &Logger::getInstance()
{
    static Logger log;
    return log;
}
// 设置日志级别
// void Logger::SetLogLevel(MyLogLevel level)
// {
//     loglevel_ = level;
// }
// 写日志，把日志信息写入lockqueue缓冲区当中
void Logger::Log(MyLogLevel level, std::string str)
{
    loglevel_ = level;
    std::string c = (loglevel_ == MyLogLevel::INFO ? "[INFO]" : "[ERROR]") + str;
    m_lockqueue_.push(c);
}
