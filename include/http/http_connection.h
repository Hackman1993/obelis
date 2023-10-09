/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_connection.h
 * @description 
 * @created_at 2023-09-28
***********************************************************************************************************************/

#ifndef OBELISK_HTTP_CONNECTION_H
#define OBELISK_HTTP_CONNECTION_H
#include "core/socket.h"
#include "parser/http_request.h"
namespace obelisk {
    class http_response;
    class http_connection : public socket{
    public:
        http_connection(io_context& ctx, SOCKET_TYPE sock);
        void on_connected(const std::function<void ()>& callback);
        void on_disconnected(const std::function<void ()>& callback);
        void on_request_received(const std::function<std::shared_ptr<http_response>(std::shared_ptr<http_request>&)>& callback);

        void write_response(const std::shared_ptr<http_response>& response);

    protected:
        void _e_connected() override;
        bool _e_data_recved() override;
        void _e_disconnected() override;
        virtual void _e_request_received();

        bool _handle_header();
        bool _handle_body();

        std::function<void ()> connected_;
        std::function<void ()> disconnected_;
        std::function<std::shared_ptr<http_response> (std::shared_ptr<http_request>&)> request_received_;

        std::shared_ptr<http_request> request_;
        std::atomic_bool header_received_ = false;
        std::uint64_t bytes_reamains_ = 0;
    };

} // obelisk

#endif //OBELISK_HTTP_CONNECTION_H
