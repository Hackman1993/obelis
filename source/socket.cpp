/***********************************************************************************************************************
 * @author Hackman Lo
 * @file socket.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "core/socket.h"
#include <iostream>
#include "exception/network_exception.h"
#include "core/impl/socket_setting.h"

namespace obelisk {
    socket::socket(io_context &context, SOCKET_TYPE sock) : ctx_(context), socket_(sock) {
        socket_setting::set_nonblocking(sock);

        ctx_data_.handler_ = this;
        ctx_data_.del_ = false;
        ctx_data_.sock_ = sock;
#ifdef _WIN32
#elif defined(__linux__)
#else
        EV_SET(&event_, socket_, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &ctx_data_);
        if (kevent(ctx_.handle(), &event_, 1, nullptr, 0, nullptr) == -1)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
#endif
    }

    void socket::_handle(context_data_core &base) {
        std::cout <<"data Received" << std::endl;
        unsigned char data[10000] = {0};
        auto recved = recv(socket_, reinterpret_cast<char *>(data), 10000, 0);
        std::cout << recved << std::endl;
        int a = 0;
    }
} // obelisk