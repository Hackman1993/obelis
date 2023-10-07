/***********************************************************************************************************************
 * @author Hackman Lo
 * @file socket.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_SOCKET_H
#define OBELISK_SOCKET_H

#include "io_handler.h"
#include "impl/passive_data.h"
#include "io_context.h"
#include "impl/platform_selector.h"
#include <memory>
#include <boost/asio/streambuf.hpp>

namespace obelisk {
    class socket : public io_handler {
    public:
        socket(io_context &context, SOCKET_TYPE sock);
        ~socket() override;
        void _serve() override;
        void close();
        void _handle(context_data_core &base) override;

    protected:
        virtual void _e_connected();
        virtual bool _e_data_recved();
        virtual void _e_disconnected();
        io_context &ctx_;
        SOCKET_TYPE socket_;
        boost::asio::streambuf buffer_;
        std::atomic<std::uint32_t> expect_size_;
        context_data_core ctx_data_{};
#ifdef _WIN32
#elif defined(__linux__)
#else
        struct kevent event_{};
#endif

        friend class io_handler;
    };

} // obelisk

#endif //OBELISK_SOCKET_H
