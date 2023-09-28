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
    }

    void socket::_handle(context_data_core &base) {
        if (base.event_.type_ == event_type::DISCONNECTED) {
            close();
            return;
        }
        char data[1024 * 256] = {0};
        ssize_t recved;
        do {
            recved = recv(socket_, reinterpret_cast<char *>(&data), 1024 * 256, 0);
            if (recved > 0) {
                buffer_.sputn(data, recved);
                if (buffer_.size() > expect_size_) {
                    if(!_e_data_recved()){
                        close();
                    }
                }
            }
        } while (recved > 0);
    }

    socket::~socket() {
        std::cout << "SOCKET Released" << std::endl;
        EV_SET(&event_, socket_, EVFILT_READ, EV_DELETE, 0, 0, &ctx_data_);
        kevent(ctx_.handle(), &event_, 1, nullptr, 0, nullptr);
    }

    void socket::_serve() {
        socket_setting::set_nonblocking(socket_);
        ctx_data_.handler_ = shared_from_this();
#ifdef _WIN32
#elif defined(__linux__)
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
        _e_disconnected();
        ctx_data_.handler_ = nullptr;
        ::close(socket_);
    }


} // obelisk