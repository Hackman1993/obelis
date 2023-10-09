#include <iostream>
#include <sahara/exception/database_exception.h>
#include "obelisk.h"
#include "exception/network_exception.h"
#include "core/listener.h"
#include "core/ip_address.h"
#include "core/impl/platform_selector.h"
#include "core/io_context.h"
#include "http/http_server.h"
#include <thread>
#include <vector>
#include <sahara/log/log.h>
int main()
{
    sahara::log::initialize();
    try{
        /* getaddrinfo() returns a list of address structures.
           Try each address until we successfully bind(2).
           If socket(2) (or bind(2)) fails, we (close the socket
           and) try the next address. */
        obelisk::io_context context;
        obelisk::http_server server(context);
        server.listen("0.0.0.0", 3308);
        std::vector<std::thread> threads;
        threads.reserve(3);

        context.run();
//        for(int i=0; i < 3; i++){
//            threads.emplace_back([&](){
//                context.run();
//            });
//        }
//        while (true);

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
