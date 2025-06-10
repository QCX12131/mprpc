#ifndef LOGQUEUE_H
#define LOGQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
// 异步写日志的日志队列
template <typename T>
class LogQueue
{
public:
    // 多个worker工作线程将日志写入queue
    void push(const T &data)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_queue.push(data);
        m_cv.notify_one();
    }
    // 只有一个专门用来将日志写入磁盘IO的线程
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        while (m_queue.empty())
        {
            // 日志队列为空，线程进入等待状态
            m_cv.wait(lock);
        }
        T data = m_queue.front();
        m_queue.pop();
        return data;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};

#endif