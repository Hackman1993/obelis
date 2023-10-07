/***********************************************************************************************************************
 * @author Hackman Lo
 * @file io_context.cpp
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/
#include <iostream>
#include <sahara/log/log.h>
#include "core/io_context.h"
#include "core/impl/platform_selector.h"
#include "exception/network_exception.h"
#include "core/io_handler.h"
#include "core/impl/passive_data.h"

namespace obelisk {
    io_context::io_context(std::uint32_t threads) {
#if defined(__linux__)
        ctx_ = epoll_create1(EPOLL_CLOEXEC);
#elif defined(_WIN32)
        WSAData wsd{};
        WSAStartup(MAKEWORD(2, 2), &wsd);
        ctx_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, threads);
        if (ctx_ == INVALID_CONTEXT) {
            THROW(network_exception, strerror(errno), "Obelisk");
        }
#else
        ctx_ = kqueue();
#endif
    }

    void io_context::run() {
        context_data_core *ctxd = nullptr;
#ifdef _WIN32
        DWORD lpNumberOfBytesTransferred = 0;
        void *lpCompletionKey = nullptr;
        while (true) {
            if(GetQueuedCompletionStatus(ctx_, &lpNumberOfBytesTransferred, (PULONG_PTR) &lpCompletionKey, (LPOVERLAPPED *) &ctxd, INFINITE)){
                if(ctxd->handler_)
                    ctxd->handler_->_handle(*ctxd);
            }
        }
#elif defined(__linux__)
        while(true){
            epoll_event ev{};
            int ret = epoll_wait(ctx_, &ev,1,-1);
            if(ret == -1){
                LOG_DEBUG("DEBUG:{}", strerror(LASTERROR));
                continue;
            }
            ctxd = static_cast<context_data_core *>(ev.data.ptr);
            if(ctxd && ctxd->handler_)
            {
                ctxd->lock_.lock();
                if(EPOLLRDHUP == (ev.events & EPOLLRDHUP))
                {
                    ctxd->event_.type_ = event_type::DISCONNECTED;
                }
                ctxd->handler_->_handle(*ctxd);
                ctxd->lock_.unlock();
            }
        }
#else
        int ready_cnt = 0;
        while(ready_cnt != -1){
            struct kevent evt{};
            ready_cnt = kevent(ctx_, nullptr, 0, &evt, 1, nullptr);
            if(ready_cnt ==1)
            {
                ctxd = (context_data_core *)evt.udata;
                if(ctxd->lock_.try_lock()){
                    if((evt.flags & EV_EOF) == EV_EOF){
                        ctxd->event_.type_ = event_type::DISCONNECTED;
                    }
                    if (ctxd && ctxd->handler_!= nullptr){
                        ctxd->handler_->_handle(*ctxd);
                    }
                    ctxd->lock_.unlock();
                }
            }
        }
#endif



    }

    CONTEXT_TYPE io_context::handle() const {
        return ctx_;
    }

} // obelisk