/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_connection.h
 * @description 
 * @created_at 2023-09-28
***********************************************************************************************************************/

#ifndef OBELISK_HTTP_CONNECTION_H
#define OBELISK_HTTP_CONNECTION_H
#include "core/socket.h"
namespace obelisk {
    class http_connection : public socket{
    public:
        http_connection(io_context& ctx, SOCKET_TYPE sock);

    protected:
        void _e_connected() override;

        bool _e_data_recved() override;

        void _e_disconnected() override;

    public:
        void on_connected(const std::function<void ()>& callback);
        void on_disconnected(const std::function<void ()>& callback);
    public:
        std::function<void ()> connected_;
        std::function<void ()> disconnected_;
    };

} // obelisk

#endif //OBELISK_HTTP_CONNECTION_H
