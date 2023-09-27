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
namespace obelisk {
    class socket : public io_handler, std::enable_shared_from_this<socket> {
    public:
        socket(io_context &context, SOCKET_TYPE sock);

    protected:
    public:
        void _handle(context_data_core &base) override;

    protected:
        io_context &ctx_;
        SOCKET_TYPE socket_;
        context_data_core ctx_data_{};
        //TODO: MACOS
#ifdef _WIN32
#elif defined(__linux__)
#else
        struct kevent event_{};
#endif
    };

} // obelisk

#endif //OBELISK_SOCKET_H
