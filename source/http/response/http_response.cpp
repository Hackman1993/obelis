//
// Created by hackman on 10/8/23.
//

#include <sstream>
#include "http/response/http_response.h"

namespace obelisk {
    std::unordered_map<http_response::EHTTP_RESP_STATUS, std::string> http_response::resp_status_map_{
            {EST_CONTINUE, "100 Continue"},
            {EST_OK, "200 OK"},
            {EST_CREATED, "201 Created"},
            {EST_ACCEPTED, "202 Accepted"},
            {EST_NO_CONTENT, "204 No Content"},
            {EST_MOVED_PERMANENTLY, "301 Moved Permanently"},
            {EST_FOUND, "302 Found"},
            {EST_NOT_MODIFIED, "304 Not Modified"},
            {EST_USE_PROXY, "305 Use Proxy"},
            {EST_TEMPORARY_REDIRECT, "307 Temporary Redirect"},
            {EST_BAD_REQUEST, "400 Bad Request"},
            {EST_UNAUTHORIZED, "401 Unauthorized"},
            {EST_FORBIDDEN, "403 Forbidden"},
            {EST_NOT_FOUND, "404 Not Found"},
            {EST_METHOD_NOT_ALLOWED, "405 Method Not Allowed"},
            {EST_NOT_ACCEPTABLE, "406 Not Acceptable"},
            {EST_PROXY_AUTHENTICATION_REQUIRED, "407 Proxy Authentication Required"},
            {EST_REQUEST_TIMEOUT, "408 Request Timeout"},
            {EST_CONFLICT, "409 Conflict"},
            {EST_GONE, "410 Gone"},
            {EST_LENGTH_REQUIRED, "411 Length Required"},
            {EST_PRECONDITION_FAILED, "412 Precondition Failed"},
            {EST_REQUEST_ENTITY_TOO_LARGE, "413 Request Entity Too Large"},
            {EST_REQUEST_URI_TOO_LONG, "414 Request-URI Too Long"},
            {EST_UNSUPPORTED_MEDIA_TYPE, "415 Unsupported Media Type"},
            {EST_REQUESTED_RANGE_NOT_SATISFIABLE, "416 Requested Range Not Satisfiable"},
            {EST_EXPECTATION_FAILED, "417 Expectation Failed"},
            {EST_INTERNAL_SERVER_ERROR, "500 Internal Server Error"},
            {EST_NOT_IMPLEMENTED, "501 Not Implemented"},
            {EST_BAD_GATEWAY, "502 Bad Gateway"},
            {EST_SERVICE_UNAVAILABLE, "503 Service Unavailable"},
            {EST_GATEWAY_TIMEOUT, "504 Gateway Timeout"},
            {EST_HTTP_VERSION_NOT_SUPPORTED, "505 HTTP Version Not Supported"},

    };

    http_response::http_response(unsigned int code) : http_response(EHTTP_RESP_STATUS(code)){}
    http_response::http_response(http_response::EHTTP_RESP_STATUS code): resp_code_(code) {}


    void http_response::add_header(const std::string &name, const std::string &value) {
        headers_.emplace(name, value);
    }

    std::string http_response::serialize_header() {
        std::ostringstream result;
        result << "HTTP/1.1 " << resp_status_map_[resp_code_] << "\r\n";
        for(auto &header: headers_){
            result << header.first << ": " << header.second << "\r\n";
        }
        result << "\r\n";
        return result.str();
    }

    std::shared_ptr<std::istream> http_response::content() {
        return content_;
    }
} // obelisk