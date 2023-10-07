//
// Created by hackman on 10/7/23.
//

#ifndef OBELISK_HTTP_PARSER_H
#define OBELISK_HTTP_PARSER_H
#include <string>
#include <memory>
#include <unordered_map>
#include "http_request.h"

namespace obelisk {

    class socket;

    bool boundary_parse(std::string_view  data, http_request *request);
    bool package_header_parse(std::string_view data, http_request *request);

} // obelisk

#endif //OBELISK_HTTP_PARSER_H
