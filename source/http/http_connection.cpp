/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_connection.cpp
 * @description 
 * @created_at 2023-09-28
***********************************************************************************************************************/

#include "http/http_connection.h"
#include "http/parser/http_parser.h"
#include "http/parser/http_request.h"
#include <boost/algorithm/string.hpp>
#include <sahara/log/log.h>
#include <sahara/utils/uuid.h>
#include <sstream>
namespace obelisk {
    http_connection::http_connection(io_context &ctx, SOCKET_TYPE sock) : socket(ctx, sock) {
        request_ = std::make_shared<http_request>(*this);
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
        if(buffer_.size() <= 4){ return true; }

        bool result;
        if(!header_received_)
        {
            result =  _handle_header();
        }
        else{
            result = _handle_body();
        }
        if(!result)
            return false;
        if(bytes_reamains_ == 0 && request_received_){
            request_received_(*request_);
        }

        return result;
    }

    bool http_connection::_handle_header() {
        std::string_view received_data(boost::asio::buffer_cast<const char *>(buffer_.data()),buffer_.size());
        bool contains_header_eof = received_data.contains("\r\n\r\n");
        if(buffer_.size() > 10 * 1024 && !contains_header_eof)
        {
            LOG_DEBUG("Header Size Exceed, Shutting Down!{}", "");
            return false;
        }
        if(!contains_header_eof) return true;
        auto separator = boost::algorithm::find_first(received_data, "\r\n\r\n");
        received_data = std::string_view(received_data.data(), separator.end() - received_data.begin());
        auto parse_result = package_header_parse(received_data, request_.get());
        if(!parse_result){
            LOG_DEBUG("Invalid Package Header, Shutting Down!{}", "");
            return false;
        }
        buffer_.consume(received_data.size());
        if(request_->header_.headers_.contains("Content-Length") && request_->header_.headers_.contains("Content-Type")){
            request_->content_type_ = request_->header_.headers_["Content-Type"];
            request_->content_length_ = std::stoull(request_->header_.headers_["Content-Length"]);
            bytes_reamains_ = request_->content_length_;
            if(request_->content_type_.contains("multipart/form-data")){
                if(!boundary_parse(request_->content_type_, request_.get())){
                    LOG_DEBUG("Extract Boundary With multipart/form-data Header Failed, Shutting Down!{}", "");
                    return false;
                }
            }
        }
        if(request_->content_length_ > 0){
            header_received_ = true;
        }
        return true;
    }

    bool http_connection::_handle_body() {
        std::string_view received_data(boost::asio::buffer_cast<const char *>(buffer_.data()),buffer_.size());
        if(request_->content_length_ > 1*1024*1024)
        {
            std::string path = "./" + sahara::utils::uuid::generate();
            data_buffer_ = std::make_shared<http_temp_fstream>(path);
        }else{
            data_buffer_ = std::make_shared<std::stringstream>();
        }
        std::uint32_t bytes_write = std::min<std::uint64_t>(received_data.size(), bytes_reamains_);
        if(bytes_write > 0){
            data_buffer_->write(received_data.data(), bytes_write);
            buffer_.consume(bytes_write);
            bytes_reamains_ -= bytes_write;
        }
        if(bytes_reamains_ == 0){
            data_buffer_->flush();
            header_received_ = false;
        }
        return true;
    }

    void http_connection::on_request_received(const std::function<void(http_request &)> &callback) {
        request_received_ = callback;
    }
} // obelisk