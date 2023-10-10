/***********************************************************************************************************************
 * @author Hackman Lo
 * @file io_handler.h
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#ifndef OBELISK_IO_HANDLER_H
#define OBELISK_IO_HANDLER_H
#include <iostream>
#include <memory>
#include "core/impl/platform_selector.h"

namespace obelisk {

    class context_data_core;

    class io_handler: public std::enable_shared_from_this<io_handler> {
    public:
        virtual void _handle(context_data_core &) = 0;
        virtual void _serve() = 0;
        virtual ~io_handler() = default;
        virtual void close() = 0;
    };

} // obelisk

#endif //OBELISK_IO_HANDLER_H
