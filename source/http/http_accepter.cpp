/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_accepter.cpp
 * @description 
 * @created_at 2023-09-28
***********************************************************************************************************************/

#include "http/http_accepter.h"

#include <utility>
#include "http/http_connection.h"
namespace obelisk {
    std::shared_ptr<io_handler> http_accepter::_accepted(SOCKET_TYPE sock) {
        auto ptr =std::make_shared<http_connection>(ctx_, sock);
        if(accepted_)
            accepted_(ptr);
        return ptr;
    }

    http_accepter::http_accepter(io_context &context) : listener(context) {

    }

    void http_accepter::on_accepted(std::function<void(std::shared_ptr<http_connection>)> callback) {
        accepted_ = std::move(callback);
    }
} // obelisk