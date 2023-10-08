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
#include "core/socket.h"

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
#ifdef _WIN32
            accept_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
#else
            accept_socket = ::socket(addr_in.sin_family, SOCK_STREAM, IPPROTO_TCP);
#endif
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
            accept_socket = ::socket(addr_in.sin6_family, SOCK_STREAM, IPPROTO_TCP);
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
        passive_.handler_ = shared_from_this();
    }

    listener::listener(io_context &ctx) : ctx_(ctx) {
#ifdef _WIN32
        passive_.recv_buf_.len = (sizeof(sockaddr_in) + 16) * 2;
        passive_.recv_buf_.buf = static_cast<CHAR *>(malloc(passive_.recv_buf_.len));
#endif
        passive_.event_.type_ = event_type::ACCEPTED;
    }

    void listener::_handle(context_data_core &context) {
#ifdef _WIN32
#else
        context.sock_ = accept(socket_, nullptr, nullptr);
#endif
        if(context.sock_ == INVALID_SOCKET)
            return;
        std::shared_ptr<io_handler> sock = _accepted(context.sock_);
        sock->_serve();
#ifdef _WIN32
        _serve();
#endif
    }

    std::shared_ptr<io_handler> listener::_accepted(SOCKET_TYPE sock) {
        return std::make_shared<socket>(ctx_, sock);
    }

    void listener::_serve() {
#ifdef _WIN32
        passive_.sock_ = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);;
        if (CreateIoCompletionPort((HANDLE) socket_, ctx_.handle(), 0, 0) == INVALID_HANDLE_VALUE)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        if (!AcceptEx(socket_, passive_.sock_, passive_.recv_buf_.buf,
                      0, sizeof(sockaddr_in) + 16,
                      sizeof(sockaddr_in) + 16, nullptr, (LPOVERLAPPED)&passive_)) {
            auto t=WSAGetLastError();
            if (WSAGetLastError() != ERROR_IO_PENDING)
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        }
#elif defined(__linux__)
        epoll_event ev{};
        ev.events = EPOLLIN |EPOLLET;
        ev.data.ptr = &passive_;
        if(epoll_ctl(ctx_.handle(), EPOLL_CTL_ADD,socket_,&ev)!=0)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
#else
        EV_SET(&event_, socket_, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, &passive_);
        if (kevent(ctx_.handle(), &event_, 1, nullptr, 0, nullptr) == -1)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
#endif
    }

    void listener::close() {
        socket_setting::close(socket_);
    }

} // obelisk