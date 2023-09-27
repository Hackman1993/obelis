/***********************************************************************************************************************
 * @author Hackman Lo
 * @file io_context.cpp
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/
#include <iostream>
#include "core/io_context.h"
#include "core/impl/platform_selector.h"
#include "exception/network_exception.h"
#include "core/io_handler.h"
#include "core/impl/passive_data.h"

namespace obelisk {
    io_context::io_context(std::uint32_t threads) {
#if defined(__linux__)
        //TODO: Linux
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
        //TODO:Linux
#else
        struct kevent events[32] = {0};
        int ready_cnt;
        while((ready_cnt = kevent(ctx_, nullptr, 0, events, 32, nullptr)) != -1){
            for (std::uint32_t i = 0; i < ready_cnt; i++) {
                auto *data = (context_data_core *)events[i].udata;
                data->handler_->_handle(*data);
            }
        }
#endif



    }

    CONTEXT_TYPE io_context::handle() const {
        return ctx_;
    }

} // obelisk