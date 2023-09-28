/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_accepter.h
 * @description 
 * @created_at 2023-09-28
***********************************************************************************************************************/

#ifndef OBELISK_HTTP_ACCEPTER_H
#define OBELISK_HTTP_ACCEPTER_H
#include "core/listener.h"
namespace obelisk {
    class http_connection;

    class http_accepter : public listener{
    public:
        http_accepter(io_context& context);
        void on_accepted(std::function<void (std::shared_ptr<http_connection>)> callback);
    protected:
        std::function<void (std::shared_ptr<http_connection>)> accepted_;
        std::shared_ptr<io_handler> _accepted(SOCKET_TYPE sock) override;
    };

} // obelisk

#endif //OBELISK_HTTP_ACCEPTER_H
