/***********************************************************************************************************************
 * @author Hackman Lo
 * @file ip_address.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include <cerrno>
#include "core/ip_address.h"
#include "core/impl/platform_selector.h"
#include "exception/network_exception.h"


namespace obelisk {
    bool ip_address::is_v4(const std::string &addr) {
        sockaddr_in address{};
        return inet_pton(AF_INET, addr.c_str(), &address);
    }

    bool ip_address::is_v6(const std::string &addr) {
        sockaddr_in6 address{};
        return inet_pton(AF_INET6, addr.c_str(), &address);
    }

    [[maybe_unused]] ip_address::ip_address(const std::string &addr){
        if(is_v4(addr))
        {
            v4_ = true;
            in_addr data{};
            if(!inet_pton(AF_INET, addr.c_str(), &data)) THROW(obelisk::network_exception, strerror(errno), "Obelisk");
            addr_data_ = malloc(sizeof (in_addr));
            memcpy(addr_data_, &data, sizeof(in_addr));
        }
        else if(is_v6(addr)){
            v4_ = false;
            in6_addr data{};
            if(!inet_pton(AF_INET6, addr.c_str(), &data))
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
            addr_data_ = malloc(sizeof (in6_addr));
            memcpy(addr_data_, &data, sizeof(in6_addr));
        }
        else
            THROW(obelisk::network_exception, "Invalid address!", "Obelisk");
    }

    [[maybe_unused]] bool ip_address::is_v4() const noexcept {
        return v4_;
    }

    [[maybe_unused]] bool ip_address::is_v6() const noexcept {
        return !v4_;
    }

    ip_address::ip_address(in_addr addr) {
        v4_ = true;
        addr_data_ = malloc(sizeof(addr));
        memcpy(addr_data_, &addr, sizeof(addr));
    }

    ip_address::ip_address(in6_addr addr) {
        v4_ = false;
        addr_data_ = malloc(sizeof (addr));
        memcpy(addr_data_, &addr, sizeof(addr));
    }

    ip_address::~ip_address(){
        if(addr_data_) {
            free(addr_data_);
        }
    }

    in_addr ip_address::address_v4() const {
        return *(in_addr*)addr_data_;
    }

    in6_addr ip_address::address_v6() const {
        return *(in6_addr*)addr_data_;
    }
} // obelisk