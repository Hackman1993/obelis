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
    class http_accepter;
    class http_server {
    public:
        explicit http_server(io_context& context);
        void listen(const std::string& address, unsigned short port);
    protected:
        io_context& ctx_;
        std::shared_ptr<http_accepter> listener_;
        std::atomic_int32_t alive_count_;
        std::vector<std::shared_ptr<socket>> sock_;
    };

} // obelisk

#endif //OBELISK_HTTP_SERVER_H
