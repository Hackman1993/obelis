/***********************************************************************************************************************
 * @author Hackman Lo
 * @file ip_address.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_IP_ADDRESS_H
#define OBELISK_IP_ADDRESS_H
#include <string>
#include "core/impl/platform_selector.h"
namespace obelisk {

    class ip_address {
    public:
        static bool is_v4(const std::string& addr);
        static bool is_v6(const std::string& addr);

        ip_address(const std::string& addr);
        ip_address(sockaddr_in addr);
        ip_address(sockaddr_in6 addr);

        [[maybe_unused]] [[nodiscard]] bool ip_v4() const noexcept;
        [[maybe_unused]] [[nodiscard]] bool ip_v6() const noexcept;
        sockaddr_in address_v4();
        sockaddr_in6 address_v6();

    protected:
        bool v4_:1 = false;
        std::string address_;
    };

} // obelisk

#endif //OBELISK_IP_ADDRESS_H
