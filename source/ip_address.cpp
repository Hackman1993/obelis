/***********************************************************************************************************************
 * @author Hackman Lo
 * @file ip_address.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

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

    [[maybe_unused]] ip_address::ip_address(const std::string &addr) : address_(addr){
        if(is_v4(addr))
            v4_ = true;
        else if(is_v6(addr))
            v4_ = false;
        else
            THROW(obelisk::network_exception, "Invalid address!", "Obelisk");
    }

    [[maybe_unused]] bool ip_address::ip_v4() const noexcept {
        return v4_;
    }

    [[maybe_unused]] bool ip_address::ip_v6() const noexcept {
        return !v4_;
    }

    ip_address::ip_address(sockaddr_in addr) {
        v4_ = true;
    }

    ip_address::ip_address(sockaddr_in6 addr) {
        v4_ = false;
    }

    sockaddr_in ip_address::address_v4() {
        return sockaddr_in();
    }

    sockaddr_in6 ip_address::address_v6() {
        return sockaddr_in6();
    }
} // obelisk