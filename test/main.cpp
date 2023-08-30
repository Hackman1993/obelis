#include <iostream>
#include <sahara/exception/database_exception.h>
#include "obelisk.h"
#include "exception/network_exception.h"
#include "core/listener.h"
#include "core/ip_address.h"
#include "core/impl/platform_selector.h"
int main()
{
    try{

        WSAData wsd{};
        WSAStartup(MAKEWORD(2,2), &wsd);
        std::cout << sizeof(sockaddr_in) << std::endl;
        std::cout << sizeof(sockaddr_in6) << std::endl;

        struct addrinfo hints;
        struct addrinfo *result;
        int s;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET6;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        s = getaddrinfo("www.baidu.com", "", &hints, &result);
        if (s != 0) {

            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            exit(EXIT_FAILURE);
        }

        /* getaddrinfo() returns a list of address structures.
           Try each address until we successfully bind(2).
           If socket(2) (or bind(2)) fails, we (close the socket
           and) try the next address. */

        freeaddrinfo(result);           /* No longer needed */
        obelisk::listener listener;
//        listener.listen(3307, "www.baidu.com");
//        int listenfd, connfd, efd, ret;
//        listenfd = socket(AF_INET, SOCK_STREAM, 0);
//        sockaddr_in addr{};
//        addr.sin_family = AF_INET;
//        addr.sin_addr.s_addr = htonl(INADDR_ANY);
//        addr.sin_port = htons(3307);
//
//        char data[100] = {0};
//        if(bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
//            THROW(obelisk::network_exception,strerror(errno), "module");
//        }
//
//        if(listen(listenfd, OBELISK_MAX_LISTEN_CONNECTION)){
//            THROW(sahara::exception::database_exception,"message", "module", 200);
//        }
//
//
    }catch (sahara::exception::exception_base& e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}
