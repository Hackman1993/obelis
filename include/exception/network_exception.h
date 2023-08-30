/***********************************************************************************************************************
 * @author Hackman Lo
 * @file network_exception.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_NETWORK_EXCEPTION_H
#define OBELISK_NETWORK_EXCEPTION_H

#include <sahara/exception/exception_base.h>

namespace obelisk {
    class network_exception : public sahara::exception::exception_base {
    public:
        network_exception(const std::string& message) : sahara::exception::exception_base(message){}
    };

} // obelisk

#endif //OBELISK_NETWORK_EXCEPTION_H
