/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_server.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_HTTP_SERVER_H
#define OBELISK_HTTP_SERVER_H
#include <filesystem>
#include "core/listener.h"
#include "core/io_context.h"
#include "http/middleware/middleware.h"
#include "http/router/route_item.h"
namespace obelisk {
    class socket;
    class http_accepter;
    class http_request;
    class http_response;
    class http_server {
    public:
        explicit http_server(io_context& context, const std::string& webroot);
        void listen(const std::string& address, unsigned short port);
    protected:
        io_context& ctx_;
        std::filesystem::path webroot_;
        std::atomic_int32_t alive_count_;
        std::vector<std::string> index_files_;
        std::shared_ptr<http_accepter> listener_;
        std::vector<std::shared_ptr<socket>> sock_;
        std::vector<std::shared_ptr<route_item>> routes_;
        std::vector<std::shared_ptr<http_middleware_base>> middlewares_;
        virtual std::shared_ptr<http_response> on_request_(std::shared_ptr<http_request>& request);
    };

} // obelisk

#endif //OBELISK_HTTP_SERVER_H
