#include "mprcpcontroller.hpp"
#include <string>
MprpcController::MprpcController()
{
    m_failed = false;
    m_errText = "";
}
void MprpcController::Reset()
{
    m_failed = false;
    m_errText = "";
}
bool MprpcController::Failed() const
{
    return m_failed;
}
std::string MprpcController::ErrorText() const
{
    return m_errText;
}

void MprpcController::SetFailed(const std::string &reason)
{
    m_failed = true;
    m_errText = reason;
}
// 目前没有实现具体的功能
void MprpcController::StartCancel()
{
}
bool MprpcController::IsCanceled() const
{
    return true;
}
void MprpcController::NotifyOnCancel(Closure *callback)
{
}
