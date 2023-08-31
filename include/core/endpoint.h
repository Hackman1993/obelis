/***********************************************************************************************************************
 * @author Hackman Lo
 * @file endpoint.h
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#ifndef OBELISK_ENDPOINT_H
#define OBELISK_ENDPOINT_H

#include "ip_address.h"

namespace obelisk {

    class endpoint {
    public:
        endpoint(const ip_address& address, std::uint16_t port);
        endpoint(const std::string& address, std::uint16_t port);
    protected:
        std::uint16_t port_;
        ip_address address_;
    };

} // obelisk

#endif //OBELISK_ENDPOINT_H
