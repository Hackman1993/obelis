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
namespace obelisk {

    class io_context {
    public:
        io_context(std::uint32_t threads = std::thread::hardware_concurrency() + 2);
        void run();
        [[nodiscard]] CONTEXT_TYPE handle() const;
    protected:
        CONTEXT_TYPE ctx_;
    };

} // obelisk

#endif //OBELISK_IO_CONTEXT_H
