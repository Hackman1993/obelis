/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_server.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "http/http_server.h"
#include "http/http_accepter.h"
#include "http/http_connection.h"
#include "http/exception/http_exception.h"
#include "http/response/json_response.h"
#include "http/response/file_response.h"
#include "debug/cost_timer.h"

namespace obelisk {
    http_server::http_server(io_context& context, const std::string& webroot): ctx_(context), webroot_(webroot), index_files_({"index.html", "index.htm"}) {
        if(!(std::filesystem::exists(webroot_) && std::filesystem::is_directory(webroot_)))
            throw std::logic_error("Root Dirctory Not Exists");
        listener_ = std::make_shared<http_accepter>(ctx_);
        listener_->on_accepted([&](const std::shared_ptr<http_connection>& accepted){
            accepted->on_connected([&]() {
                this->alive_count_ += 1;
                });
            accepted->on_disconnected([&]() { this->alive_count_ -= 1;
            });
            accepted->on_request_received(std::bind(&http_server::on_request_,this, std::placeholders::_1));
        });
    }

    void http_server::listen(const std::string& address, unsigned short port) {
        listener_->listen(port, address);
        listener_->_serve();
    }

    std::shared_ptr<http_response> http_server::on_request_(std::shared_ptr<http_request>& request) {
        std::shared_ptr<http_response> resp = nullptr;
        {
        for(const auto& ptr : middlewares_){
            auto derived_ptr = std::dynamic_pointer_cast<before_middleware>(ptr);
            if(derived_ptr)
                resp = derived_ptr->pre_handle(request);
            if(resp)
                break;
        }

        }
        if(!resp){
            for(const auto&ptr : routes_){
                std::unordered_map<std::string,std::string> route_params;
                if(!ptr->match(request->path_, route_params))
                    continue;
                if(!ptr->method_allowed(request->header_.title_.method_))
                    THROW(http_exception, "Method Not Allowed!", "Obelisk", EHTTP_RESP_STATUS::EST_METHOD_NOT_ALLOWED);
                request->route_params_ = route_params;
                resp = ptr->handle(request);
                if(resp) break;
            }
        }
        if(!resp){
            std::filesystem::path path(webroot_);
            path.append("./" + request->path_);
            if(std::filesystem::exists(path)){
                if(!std::filesystem::is_directory(path)){
                    resp = std::make_shared<file_response>(path, EHTTP_RESP_STATUS::EST_OK);
                }else{
                    for(const auto& index: index_files_){
                        std::filesystem::path index_path(path);
                        index_path.append(index);
                        if(std::filesystem::exists(index_path) && !std::filesystem::is_directory(index_path))
                            resp = std::make_shared<file_response>(index_path, EHTTP_RESP_STATUS::EST_OK);
                    }
                }
            }
        }

        if(!resp)
            resp = std::make_shared<json_response>(boost::json::object({{"code",    404}, {"message", "Not Found"}, {"data",    boost::json::value()}}), EHTTP_RESP_STATUS::EST_NOT_FOUND);

        for(const auto& ptr : middlewares_){
            auto derived_ptr = std::dynamic_pointer_cast<after_middleware>(ptr);
            if(derived_ptr) derived_ptr->after_handle(request, resp);
        }

        resp->add_header("Host", request->header_.headers_["Host"]);
        resp->add_header("Cache-Control", "no-cache, private");
        resp->add_header("Vary", "Origin");
        resp->add_header("Content-Length", std::to_string(resp->content_length()));
        return resp;
    }
} // obelisk