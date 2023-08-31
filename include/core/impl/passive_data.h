/***********************************************************************************************************************
 * @author Hackman Lo
 * @file passive_data.h
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#ifndef OBELISK_PASSIVE_DATA_H
#define OBELISK_PASSIVE_DATA_H
#include "platform_selector.h"
namespace obelisk {

    struct passive_data_base {
        SOCKET_TYPE* handle_ = nullptr;
        io_handler* handler_ = nullptr;
    };

} // obelisk

#endif //OBELISK_PASSIVE_DATA_H
