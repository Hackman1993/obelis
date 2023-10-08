//
// Created by Hackman.Lo on 9/22/2023.
//

#include <cstdint>
#include "core/impl/socket_setting.h"

namespace obelisk {
    void socket_setting::set_nonblocking(SOCKET_TYPE sock) {
#ifdef _WIN32
        unsigned long ul = 1;
        ioctlsocket(sock, FIONBIO, &ul);
#else
        fcntl(sock, F_SETFL, O_NONBLOCK);
#endif
    }

    void socket_setting::close(SOCKET_TYPE sock) {
#ifdef _WIN32
        closesocket(sock);
#else
        ::close(sock);
#endif

    }
} // obelisk