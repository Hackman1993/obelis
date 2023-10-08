/***********************************************************************************************************************
 * @author Hackman Lo
 * @file passive_data.h
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#ifndef OBELISK_PASSIVE_DATA_H
#define OBELISK_PASSIVE_DATA_H
#include <mutex>
#include <memory>
#include <thread>
#include "platform_selector.h"
namespace obelisk {
    enum event_type {
        ACCEPTED,
        CONNECTED,
        DISCONNECTED,
        RECV,
    };

    struct event{
        event_type type_;
    };
    struct context_data_core {
#ifdef _WIN32 // Windows IOCP 是完成触发， EPOLL和kevent是边缘触发，因此Windows需要先给缓冲区
        OVERLAPPED overlapped_{};
        WSABUF recv_buf_{};
        DWORD bytes_transferred_ = 0;
        std::atomic_bool shutting_down = false;
#endif
        std::shared_ptr<io_handler> handler_ = nullptr;
        SOCKET_TYPE sock_ = INVALID_SOCKET;
        event event_ = {event_type::CONNECTED};
        std::mutex lock_;

        ~context_data_core(){
#ifdef _WIN32
            if(recv_buf_.buf){
                free(recv_buf_.buf);
                recv_buf_ = {};
            }
#endif
        }
    };
} // obelisk

#endif //OBELISK_PASSIVE_DATA_H
