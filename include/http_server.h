/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_server.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_HTTP_SERVER_H
#define OBELISK_HTTP_SERVER_H
#include "core/listener.h"
#include "core/impl/io_context.h"

namespace obelisk {

    class http_server {
    public:
        explicit http_server(io_context& context);
        void listen(const std::string& address, unsigned short port);
    protected:
        io_context& ctx_;
        listener listener_;
        void accepted_(SOCKET_TYPE sock);
    };

} // obelisk

#endif //OBELISK_HTTP_SERVER_H
