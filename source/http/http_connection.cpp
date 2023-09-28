/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_connection.cpp
 * @description 
 * @created_at 2023-09-28
***********************************************************************************************************************/

#include "http/http_connection.h"

namespace obelisk {
    http_connection::http_connection(io_context &ctx, SOCKET_TYPE sock) : socket(ctx, sock) {

    }

    void http_connection::on_connected(const std::function<void()>& callback) {
        connected_ = callback;
    }

    void http_connection::on_disconnected(const std::function<void()>& callback) {
        disconnected_ = callback;
    }

    void http_connection::_e_connected() {
        if(connected_)
            connected_();
    }

    void http_connection::_e_disconnected() {
        if(disconnected_)
            disconnected_();
    }

    bool http_connection::_e_data_recved() {
        return true;
    }
} // obelisk