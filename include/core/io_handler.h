/***********************************************************************************************************************
 * @author Hackman Lo
 * @file io_handler.h
 * @description 
 * @created_at 2023-08-31
***********************************************************************************************************************/

#ifndef OBELISK_IO_HANDLER_H
#define OBELISK_IO_HANDLER_H
#include "core/impl/platform_selector.h"

namespace obelisk {

    class context_data_core;

    class io_handler {
    public:
        virtual void _handle(const context_data_core&) = 0;
        virtual ~io_handler() = default;
    };

} // obelisk

#endif //OBELISK_IO_HANDLER_H
