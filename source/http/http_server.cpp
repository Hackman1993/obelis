/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_server.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "http/http_server.h"
#include "core/socket.h"
#include "http/http_accepter.h"
#include "http/http_connection.h"
namespace obelisk {
    http_server::http_server(io_context& context): ctx_(context) {
        listener_ = std::make_shared<http_accepter>(ctx_);
        listener_->on_accepted([&](const std::shared_ptr<http_connection>& accepted){
            accepted->on_connected([&]() {
                this->alive_count_ += 1;
                std::cout<< "Alive Connection" << this->alive_count_ << std::endl;
                });
            accepted->on_disconnected([&]() { this->alive_count_ -= 1;
                std::cout<< "Alive Connection" << this->alive_count_ << std::endl;});
            accepted->on_request_received(std::bind(&http_server::on_request_,this, std::placeholders::_1));
        });
    }

    void http_server::listen(const std::string& address, unsigned short port) {
        listener_->listen(port, address);
        listener_->_serve();
    }

    std::shared_ptr<http_response> http_server::on_request_(std::shared_ptr<http_request>& request) {
        return nullptr;
    }
} // obelisk