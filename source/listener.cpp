/***********************************************************************************************************************
 * @author Hackman Lo
 * @file listener.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "core/listener.h"
#include <regex>
#include "core/ip_address.h"
#include <iostream>
#include <utility>
#include "exception/network_exception.h"
#include "core/impl/platform_selector.h"
#include "core/impl/io_context.h"

namespace obelisk {
    void listener::listen(unsigned short port, const std::string &addr = "") {
        ip_address address(addr);
        listen(address, port);
    }

    void listener::listen(const ip_address &addr, unsigned short port) {
        SOCKET_TYPE accept_socket = {0};
        if (addr.is_v4()) {
            sockaddr_in addr_in{};
            addr_in.sin_family = AF_INET;
            addr_in.sin_port = htons(port);
            addr_in.sin_addr = addr.address_v4();
            accept_socket = socket(addr_in.sin_family, SOCK_STREAM, IPPROTO_TCP);
            if (accept_socket == INVALID_SOCKET)
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
            if (bind(accept_socket, reinterpret_cast<const sockaddr *>(&addr_in), sizeof(sockaddr_in)) == -1)
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        } else {
            sockaddr_in6 addr_in{};
            addr_in.sin6_family = AF_INET6;
            addr_in.sin6_port = htons(port);
            addr_in.sin6_addr = addr.address_v6();
            accept_socket = socket(addr_in.sin6_family, SOCK_STREAM, IPPROTO_TCP);
            if (accept_socket == INVALID_SOCKET)
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
            if (bind(accept_socket, reinterpret_cast<const sockaddr *>(&addr_in), sizeof(sockaddr_in6)) == -1)
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        }
        if (::listen(accept_socket, SOMAXCONN) == -1) {
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        }
        fcntl(socket_, F_SETFL, O_NONBLOCK);
        socket_ = accept_socket;

        EV_SET(&changes, socket_, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &passive_);
        if (kevent(ctx_.handle(), &changes, 1, nullptr, 0, nullptr) == -1)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
    }

    listener::listener(io_context& ctx) : ctx_(ctx){
        passive_.handle_ = &socket_;
        passive_.handler_ = this;
    }

    void listener::_handle(const passive_data_base& data) {
        auto accepted_ = accept(*data.handle_, nullptr, nullptr);
        if(callback_)
            callback_(accepted_);
    }

    void listener::set_handler(listener_callback callback) {
        callback_ = std::move(callback);
    }

} // obelisk