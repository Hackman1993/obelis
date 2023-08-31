/***********************************************************************************************************************
 * @author Hackman Lo
 * @file io_context.h
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#ifndef OBELISK_IO_CONTEXT_H
#define OBELISK_IO_CONTEXT_H
#include <vector>
#include <functional>
#include <unordered_map>
#include "platform_selector.h"
namespace obelisk {

    class io_context {
    public:
        io_context();
        void run();
        [[nodiscard]] int handle() const;
    protected:
        int ctx_;
    };

} // obelisk

#endif //OBELISK_IO_CONTEXT_H
