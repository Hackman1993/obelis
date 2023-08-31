/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_server.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "http_server.h"
#include "core/socket.h"
namespace obelisk {
    http_server::http_server(io_context& context): listener_(context), ctx_(context) {
        listener_.set_handler(std::bind(&http_server::accepted_, this, std::placeholders::_1));
    }

    void http_server::accepted_(SOCKET_TYPE sock) {
        new socket(ctx_, sock);
    }

    void http_server::listen(const std::string& address, unsigned short port) {
        listener_.listen(port, address);
    }
} // obelisk