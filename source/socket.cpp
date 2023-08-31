/***********************************************************************************************************************
 * @author Hackman Lo
 * @file socket.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "core/socket.h"
#include <iostream>
namespace obelisk {
    socket::socket(io_context &context, SOCKET_TYPE sock) : ctx_(context), socket_(sock) {
        fcntl(sock, F_SETFL, O_NONBLOCK);
    }

    void socket::_handle(const passive_data_base &base) {
        std::cout <<"data Received" << std::endl;
    }
} // obelisk