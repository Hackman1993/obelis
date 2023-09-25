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
#include "core/io_context.h"
#include "core/impl/socket_setting.h"
#include <system_error>

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
            accept_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
            if (accept_socket == INVALID_SOCKET) {
                THROW(obelisk::network_exception, std::system_category().message(LASTERROR), "Obelisk");
            }
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
        socket_ = accept_socket;
        socket_setting::set_nonblocking(socket_);
        // TODO: MACOS
//        EV_SET(&changes, socket_, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &passive_);
//        if (kevent(ctx_.handle(), &changes, 1, nullptr, 0, nullptr) == -1)
//            THROW(obelisk::network_exception, strerror(errno), "Obelisk");


        auto context = new context_data_core();
        context->handler_ = this;
        context->buffer_ = malloc((sizeof(sockaddr_in) + 16) * 2);
        context->sock_ = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);;
        if (CreateIoCompletionPort((HANDLE) socket_, ctx_.handle(), 0, 0) == INVALID_HANDLE_VALUE)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        if (!AcceptEx(socket_, context->sock_, context->buffer_,
                      0, sizeof(sockaddr_in) + 16,
                      sizeof(sockaddr_in) + 16, nullptr, (LPOVERLAPPED) context)) {
            auto t=WSAGetLastError();
            if (WSAGetLastError() != ERROR_IO_PENDING)
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        }
    }

    listener::listener(io_context &ctx) : ctx_(ctx) {
        passive_.handler_ = this;
    }

    void listener::_handle(const context_data_core &context) {
#ifndef WIN32_
        free(context.buffer_);
#else
        context.sock_ = accept(socket_, nullptr, nullptr);
#endif
        if (callback_)
            callback_(context.sock_);
    }

    void listener::set_handler(listener_callback callback) {
        callback_ = std::move(callback);
    }

} // obelisk