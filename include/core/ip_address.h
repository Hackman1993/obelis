/***********************************************************************************************************************
 * @author Hackman Lo
 * @file ip_address.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_IP_ADDRESS_H
#define OBELISK_IP_ADDRESS_H
#include <string>
#include <variant>
#include "core/impl/platform_selector.h"

namespace obelisk {

    class ip_address {
    public:
        static bool is_v4(const std::string& addr);
        static bool is_v6(const std::string& addr);

        ip_address(const std::string& addr);
        ip_address(in_addr addr);
        ip_address(in6_addr addr);
        ~ip_address();

        [[maybe_unused]] [[nodiscard]] bool is_v4() const noexcept;
        [[maybe_unused]] [[nodiscard]] bool is_v6() const noexcept;
        in_addr address_v4() const;
        in6_addr address_v6() const;

    protected:
        bool v4_:1 = false;
        void *addr_data_ = nullptr;
    };

} // obelisk

#endif //OBELISK_IP_ADDRESS_H
