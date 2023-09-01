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
namespace obelisk {
    struct socket_passive : public passive_data_base{

    };
    class socket : public io_handler, std::enable_shared_from_this<socket> {
    public:
        socket(io_context &context, SOCKET_TYPE sock);

    protected:
    public:
        void _handle(const passive_data_base &base) override;

    protected:
        io_context &ctx_;
        SOCKET_TYPE socket_;
        struct kevent event_{};
        socket_passive passive_;
    };

} // obelisk

#endif //OBELISK_SOCKET_H
