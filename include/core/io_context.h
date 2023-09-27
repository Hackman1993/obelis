/***********************************************************************************************************************
 * @author Hackman Lo
 * @file io_context.h
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#ifndef OBELISK_IO_CONTEXT_H
#define OBELISK_IO_CONTEXT_H
#include <vector>
#include <thread>
#include <functional>
#include <unordered_map>
#include "core/impl/platform_selector.h"
#include "exception/network_exception.h"
namespace obelisk {

    class io_context {
    public:
        io_context(std::uint32_t threads = std::thread::hardware_concurrency() + 2);
        void run();
        [[nodiscard]] CONTEXT_TYPE handle() const;
    protected:
        CONTEXT_TYPE ctx_;
#ifdef _WIN32
#elif defined(__linux__)
#else
        std::mutex changes_lock_;
        std::vector<struct kevent> changes_;
    public:
        void append_event(const struct kevent event){
            changes_lock_.lock();
            changes_.push_back(event);
            if (kevent(ctx_, changes_.data(), changes_.size(), nullptr, 0, nullptr) == -1)
                THROW(obelisk::network_exception, strerror(errno), "Obelisk");
            changes_lock_.unlock();
        }

#endif
    };

} // obelisk

#endif //OBELISK_IO_CONTEXT_H
