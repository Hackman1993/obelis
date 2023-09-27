/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_server.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_HTTP_SERVER_H
#define OBELISK_HTTP_SERVER_H
#include "core/listener.h"
#include "core/io_context.h"

namespace obelisk {
    class socket;
    class http_server {
    public:
        explicit http_server(io_context& context);
        void listen(const std::string& address, unsigned short port);
    protected:
        io_context& ctx_;
        listener listener_;
        void accepted_(std::shared_ptr<socket> sock);
        std::atomic_int32_t alive_count_;

        std::shared_ptr<socket> sock_;
    };

} // obelisk

#endif //OBELISK_HTTP_SERVER_H
