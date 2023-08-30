/***********************************************************************************************************************
 * @author Hackman Lo
 * @file listener.cpp
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#include "core/listener.h"
#include <regex>
#include "core/ip_address.h"
#include <iostream>
#include "core/impl/platform_selector.h"
namespace obelisk {
    void listener::listen(unsigned short port, const std::string &addr = "") {
        if(ip_address::is_v4(addr)){

        }
        else if(ip_address::is_v6(addr))
            std::cout << "IP V6" << std::endl;
        else {
//            auto data = gethostbyname(addr.c_str());
//            freehostent(data);
        }


    }
} // obelisk