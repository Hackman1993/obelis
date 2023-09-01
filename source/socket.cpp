/***********************************************************************************************************************
 * @author Hackman Lo
 * @file socket.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "core/socket.h"
#include <iostream>
#include "exception/network_exception.h"
namespace obelisk {
    socket::socket(io_context &context, SOCKET_TYPE sock) : ctx_(context), socket_(sock) {
        fcntl(sock, F_SETFL, O_NONBLOCK);
        passive_.handler_ = this;
        EV_SET(&event_, socket_, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &passive_);
        if (kevent(ctx_.handle(), &event_, 1, nullptr, 0, nullptr) == -1)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
    }

    void socket::_handle(const passive_data_base &base) {
        std::cout <<"data Received" << std::endl;
        unsigned char data[10000] = {0};
        auto recved = recv(socket_, data,10000,0);
        int a = 0;
    }
} // obelisk