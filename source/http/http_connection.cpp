/***********************************************************************************************************************
 * @author Hackman Lo
 * @file http_connection.cpp
 * @description 
 * @created_at 2023-09-28
***********************************************************************************************************************/

#include "http/http_connection.h"
#include "http/parser/http_parser.h"
#include "http/parser/http_request.h"
#include "exception/protocol_exception.h"
#include <boost/algorithm/string.hpp>
#include <sahara/log/log.h>
#include <sahara/utils/uuid.h>
#include <sstream>
#include "http/response/http_response.h"
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

        while(buffer_.size() > 0){
            try {
                bool result = header_received_? _handle_body() : _handle_header();
                // If a function returns false, means all processable data has been processed
                if(!result)
                    return true;
            } catch (protocol_exception & e){
                return false;
            }
        }
        return true;
    }

    bool http_connection::_handle_header() {
        std::string_view received_data(boost::asio::buffer_cast<const char *>(buffer_.data()),buffer_.size());
        bool contains_header_eof = received_data.contains("\r\n\r\n");
        if(buffer_.size() > 10 * 1024 && !contains_header_eof)
            THROW(protocol_exception, "Header Size Exceed, Shutting Down!", "Obelisk");
        if(!contains_header_eof) return false;
        auto separator = boost::algorithm::find_first(received_data, "\r\n\r\n");
        received_data = std::string_view(received_data.data(), separator.end() - received_data.begin());
        auto parse_result = package_header_parse(received_data, request_);
        if(!parse_result)
            THROW(protocol_exception, "Invalid Package Header, Shutting Down!", "Obelisk");
        buffer_.consume(received_data.size());
        if(request_->header_.headers_.contains("Content-Length") && request_->header_.headers_.contains("Content-Type")){
            request_->content_type_ = request_->header_.headers_["Content-Type"];
            request_->content_length_ = std::stoull(request_->header_.headers_["Content-Length"]);
            bytes_reamains_ = request_->content_length_;
            // If Content-Type is form-data, then extract boundary
            if(request_->content_type_.contains("multipart/form-data")){
                if(!boundary_parse(request_->content_type_, request_))
                    THROW(protocol_exception, "Extract Boundary With multipart/form-data Header Failed, Shutting Down!", "Obelisk");
            }
        }
        if(request_->content_length_ > 0){
            header_received_ = true;
            if(request_->content_length_ > 1*1024*1024)
            {
                // If content length greater than 1 MB, Create a filestream
                std::string path = "./" + sahara::utils::uuid::generate();
                request_->raw_ = std::make_shared<http_temp_fstream>(path);
            }
            else{
                // If content length <= 1MB, Create a memory buffer
                request_->raw_ = std::make_shared<std::stringstream>();
            }
        }
        return true;
    }

    bool http_connection::_handle_body() {
        std::string_view received_data(boost::asio::buffer_cast<const char *>(buffer_.data()),buffer_.size());
        // Calculate data should be written, prevent buffer contains some part of content and a new request header
        std::uint32_t bytes_write = std::min<std::uint64_t>(received_data.size(), bytes_reamains_);
        if(bytes_write > 0){
            request_->raw_->write(received_data.data(), bytes_write);
            buffer_.consume(bytes_write);
            bytes_reamains_ -= bytes_write;
        }
        // if request has been fully received, flush data and reset the header_received flag
        if(bytes_reamains_ == 0){
            request_->raw_->flush();
            header_received_ = false;

            http_body_parser(request_);
            _e_request_received();

            // TODO: Handle Request
            request_ = nullptr;
        }
        return true;
    }

    void http_connection::on_request_received(const std::function<void(http_request &)> &callback) {
        request_received_ = callback;
    }

    void http_connection::_e_request_received() {
        if(request_received_)
            request_received_(*request_);
        LOG_DEBUG("Request Received{}", "");
    }

    void http_connection::write_response(const std::shared_ptr<http_response>& response) {
        if(response)
            return;
        std::string http_header = response->serialize_header();
        if(send((unsigned char *) http_header.c_str(), http_header.size()) == -1)
            return;
        else if(response->content()){
            auto content_stream = response->content();
        }

    }
} // obelisk