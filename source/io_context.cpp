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
    io_context::io_context() {
        ctx_ = kqueue();
        if (ctx_ == -1)
            THROW(network_exception, strerror(errno), "Obelisk");
    }

    void io_context::run() {
        struct kevent events[32] = {0};
        int ready_cnt;
        while((ready_cnt = kevent(ctx_, nullptr, 0, events, 32, nullptr)) != -1){
            for (std::uint32_t i = 0; i < ready_cnt; i++) {
                auto *data = (passive_data_base *)events[i].udata;
                data->handler_->_handle(*data);
            }
        }
    }

    int io_context::handle() const {
        return ctx_;
    }
} // obelisk