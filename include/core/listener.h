/***********************************************************************************************************************
 * @author Hackman Lo
 * @file listener.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_LISTENER_H
#define OBELISK_LISTENER_H

#include <string>
#include <functional>
#include "ip_address.h"
#include "io_handler.h"
#include "impl/passive_data.h"
#include "impl/platform_selector.h"

namespace obelisk {

    class io_context;

    class socket;

    class listener : public io_handler {
    public:
        explicit listener(io_context &ctx);

        void close() override;

        void listen(unsigned short port, const std::string &addr);

        void listen(const ip_address &addr, unsigned short port);

        void _handle(context_data_core &handler) override;
        void _serve() override;
    protected:
        virtual std::shared_ptr<io_handler> _accepted(SOCKET_TYPE type);
        io_context &ctx_;
        context_data_core passive_;
#ifdef _WIN32
#elif defined(__linux__)

#else
        struct kevent event_{};
#endif
        SOCKET_TYPE socket_ = INVALID_SOCKET;
    };

} // obelisk

#endif //OBELISK_LISTENER_H
