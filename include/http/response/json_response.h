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
        json_response(const boost::json::object& object, EHTTP_RESP_STATUS code);
        json_response(const boost::json::array& object, EHTTP_RESP_STATUS  code);
    };

} // obelisk

#endif //OBELISK_JSON_RESPONSE_H
