// class PROTOBUF_EXPORT RpcController
// {
// public:
//     inline RpcController() {}
//     virtual ~RpcController();

//     // Client-side methods ---------------------------------------------
//     // These calls may be made from the client side only.  Their results
//     // are undefined on the server side (may crash).

//     // Resets the RpcController to its initial state so that it may be reused in
//     // a new call.  Must not be called while an RPC is in progress.
//     virtual void Reset() = 0;

//     // After a call has finished, returns true if the call failed.  The possible
//     // reasons for failure depend on the RPC implementation.  Failed() must not
//     // be called before a call has finished.  If Failed() returns true, the
//     // contents of the response message are undefined.
//     virtual bool Failed() const = 0;

//     // If Failed() is true, returns a human-readable description of the error.
//     virtual std::string ErrorText() const = 0;

//     // Advises the RPC system that the caller desires that the RPC call be
//     // canceled.  The RPC system may cancel it immediately, may wait awhile and
//     // then cancel it, or may not even cancel the call at all.  If the call is
//     // canceled, the "done" callback will still be called and the RpcController
//     // will indicate that the call failed at that time.
//     virtual void StartCancel() = 0;

//     // Server-side methods ---------------------------------------------
//     // These calls may be made from the server side only.  Their results
//     // are undefined on the client side (may crash).

//     // Causes Failed() to return true on the client side.  "reason" will be
//     // incorporated into the message returned by ErrorText().  If you find
//     // you need to return machine-readable information about failures, you
//     // should incorporate it into your response protocol buffer and should
//     // NOT call SetFailed().
//     virtual void SetFailed(const std::string &reason) = 0;

//     // If true, indicates that the client canceled the RPC, so the server may
//     // as well give up on replying to it.  The server should still call the
//     // final "done" callback.
//     virtual bool IsCanceled() const = 0;

//     // Asks that the given callback be called when the RPC is canceled.  The
//     // callback will always be called exactly once.  If the RPC completes without
//     // being canceled, the callback will be called after completion.  If the RPC
//     // has already been canceled when NotifyOnCancel() is called, the callback
//     // will be called immediately.
//     //
//     // NotifyOnCancel() must be called no more than once per request.
//     virtual void NotifyOnCancel(Closure *callback) = 0;

// private:
//     GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RpcController);
// };

#ifndef MPRPCCONTROLLER_H
#define MPRPCCONTROLLER_H

#include <google/protobuf/service.h>

using namespace ::google::protobuf;
class MprpcController : public ::google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void StartCancel();
    void SetFailed(const std::string &reason);
    bool IsCanceled() const;
    void NotifyOnCancel(Closure *callback);

private:
    bool m_failed;         // RPC方法执行过程中的状态
    std::string m_errText; // RPC方法执行过程中的错误信息
};

#endif