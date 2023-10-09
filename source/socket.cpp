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
#include <sahara/log/log.h>

namespace obelisk {
    socket::socket(io_context &context, SOCKET_TYPE sock) : ctx_(context), socket_(sock) {
        ctx_data_.sock_ = sock;
#ifdef _WIN32
        ctx_data_.recv_buf_.len = 1*1024*1024;
        ctx_data_.recv_buf_.buf = static_cast<CHAR *>(malloc(ctx_data_.recv_buf_.len));
#endif
        socket_setting::set_nonblocking(socket_);
    }

    void socket::_handle(context_data_core &base) {
#ifdef WIN32
        if(base.bytes_transferred_> 0) {
            // TODO: Fill Windows Data Handle
        }
        _serve();
#else
        if (base.event_.type_ == event_type::DISCONNECTED) {
            close();
            return;
        }
        char data[1024 * 256] = {0};
        std::int32_t recved;
        do {
            recved = recv(socket_, reinterpret_cast<char *>(&data), 1024 * 256, 0);
            if (recved > 0) {
                buffer_.sputn(data, recved);
                if (buffer_.size() > expect_size_) {
                    if (!_e_data_recved()) {
                        close();
                    }
                }
            }
        } while (recved > 0);
#endif
    }

    socket::~socket() {
        std::cout << "SOCKET Released" << std::endl;
#ifdef _WIN32

#elif defined(__linux__)
        epoll_event ev{};
        ev.events = EPOLLIN |EPOLLET|EPOLLRDHUP;
        ev.data.ptr = &ctx_data_;
        epoll_ctl(ctx_.handle(), EPOLL_CTL_DEL,socket_,&ev);
        ::close(socket_);
#else
        EV_SET(&event_, socket_, EVFILT_READ, EV_DELETE, 0, 0, &ctx_data_);
        kevent(ctx_.handle(), &event_, 1, nullptr, 0, nullptr);
        ::close(socket_);
#endif
    }

    void socket::_serve() {
        ctx_data_.handler_ = shared_from_this();
#ifdef _WIN32
        if (CreateIoCompletionPort((HANDLE) ctx_data_.sock_, ctx_.handle(), 0, 0) == NULL) {
            auto err = WSAGetLastError();
            if(err == ERROR_INVALID_PARAMETER){
                ctx_data_.shutting_down = true;
                return;
            }
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
        }
        DWORD flags = 0;
        auto ret = WSARecv(ctx_data_.sock_, &ctx_data_.recv_buf_, 1, NULL, &flags,reinterpret_cast<LPWSAOVERLAPPED>(&ctx_data_), NULL);
        auto err = WSAGetLastError();
        if(ret == SOCKET_ERROR && err != ERROR_IO_PENDING)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
#elif defined(__linux__)
        epoll_event ev{};
        ev.events = EPOLLIN |EPOLLET|EPOLLRDHUP;
        ev.data.ptr = &ctx_data_;
        if(epoll_ctl(ctx_.handle(), EPOLL_CTL_ADD,socket_,&ev)!=0)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
#else
        EV_SET(&event_, socket_, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, &ctx_data_);
        if (kevent(ctx_.handle(), &event_, 1, nullptr, 0, nullptr) == -1)
            THROW(obelisk::network_exception, strerror(errno), "Obelisk");
#endif
        _e_connected();
    }

    void socket::_e_connected() {

    }

    bool socket::_e_data_recved() {
        return false;
    }

    void socket::_e_disconnected() {

    }

    void socket::close() {
        std::cout << "CLOSE" << std::endl;
        _e_disconnected();
        ctx_data_.handler_ = nullptr;
    }

    std::int32_t socket::send(unsigned char *data, std::size_t length) {
        long total_sent = 0;
        while (total_sent < length) {
            auto bytes_sent = ::send(socket_, (char *) &data[total_sent], length, 0);
            if (bytes_sent == -1) {
                return -1;
            }
            total_sent += bytes_sent;
        }
        return total_sent;
    }


} // obelisk