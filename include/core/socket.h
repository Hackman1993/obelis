/***********************************************************************************************************************
 * @author Hackman Lo
 * @file socket.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_SOCKET_H
#define OBELISK_SOCKET_H
#include "impl/platform_selector.h"
#include "core/impl/io_context.h"
#include "io_handler.h"
namespace obelisk {

class socket : public io_handler, std::enable_shared_from_this<socket>{
    public:
        socket(io_context &context, SOCKET_TYPE sock);
    protected:
    public:
        void _handle(const passive_data_base &base) override;

    protected:
        io_context& ctx_;
        SOCKET_TYPE socket_;
    };

} // obelisk

#endif //OBELISK_SOCKET_H
