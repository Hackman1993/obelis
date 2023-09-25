//
// Created by Hackman.Lo on 9/22/2023.
//

#ifndef OBELISK_SOCKET_SETTING_H
#define OBELISK_SOCKET_SETTING_H
#include "platform_selector.h"
namespace obelisk {

    class socket_setting {
    public:
        static void set_nonblocking(SOCKET_TYPE sock);
    };

} // obelisk

#endif //OBELISK_SOCKET_SETTING_H
