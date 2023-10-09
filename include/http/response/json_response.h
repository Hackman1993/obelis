//
// Created by hackman on 10/9/23.
//

#ifndef OBELISK_JSON_RESPONSE_H
#define OBELISK_JSON_RESPONSE_H
#include "http_response.h"
#include <boost/json.hpp>
namespace obelisk {

    class json_response : public http_response{
    public:
        json_response(const boost::json::object& object, std::size_t code);
        json_response(const boost::json::array& object, std::size_t  code);
    };

} // obelisk

#endif //OBELISK_JSON_RESPONSE_H
