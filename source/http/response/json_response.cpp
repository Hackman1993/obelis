//
// Created by hackman on 10/9/23.
//

#include "http/response/json_response.h"
#include <sstream>
namespace obelisk {
    json_response::json_response(const boost::json::object& object, EHTTP_RESP_STATUS code) : http_response(code) {
        headers_["Content-Type"] = "application/json";
        std::string data = boost::json::serialize(object);
        content_length_ = data.size();
        content_ = std::make_shared<std::stringstream>(data);
    }

    json_response::json_response(const boost::json::array& object, EHTTP_RESP_STATUS code) : http_response(code) {
        headers_["Content-Type"] = "application/json";
        std::string data = boost::json::serialize(object);
        content_length_ = data.size();
        content_ = std::make_shared<std::stringstream>(data);
    }
} // obelisk