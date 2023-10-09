//
// Created by hackman on 10/9/23.
//

#include "http/response/json_response.h"
#include <sstream>
namespace obelisk {
    json_response::json_response(const boost::json::object& object, std::size_t code) : http_response(code) {
        headers_["Content-Type"] = "application/json";
        std::string data = boost::json::serialize(object);
        headers_["Content-Length"] = std::to_string(data.size());
        content_ = std::make_shared<std::stringstream>(data);
    }

    json_response::json_response(const boost::json::array& object, std::size_t code) : http_response(code) {
        headers_["Content-Type"] = "application/json";
        std::string data = boost::json::serialize(object);
        headers_["Content-Length"] = std::to_string(data.size());
        content_ = std::make_shared<std::stringstream>(data);
    }
} // obelisk