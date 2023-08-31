/***********************************************************************************************************************
 * @author Hackman Lo
 * @file endpoint.cpp
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#include "core/endpoint.h"

namespace obelisk {
    endpoint::endpoint(const ip_address &address, std::uint16_t port) : address_(address), port_(port){

    }

    endpoint::endpoint(const std::string &address, std::uint16_t port) : address_(address), port_(port) {

    }
} // obelisk