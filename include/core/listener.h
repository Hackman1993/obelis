/***********************************************************************************************************************
 * @author Hackman Lo
 * @file listener.h
 * @description 
 * @created_at 2023-08-30
***********************************************************************************************************************/

#ifndef OBELISK_LISTENER_H
#define OBELISK_LISTENER_H
#include <string>
#include <functional>
#include "ip_address.h"
#include "io_handler.h"
#include "impl/passive_data.h"
#include "impl/platform_selector.h"
namespace obelisk {

    class io_context;

    struct listen_passive : public context_data_core{

    };

    using listener_callback = std::function<void (SOCKET_TYPE)>;
    class listener : public io_handler{
    public:
        explicit listener(io_context& ctx);
        void listen(unsigned short port, const std::string& addr);
        void listen(const ip_address& addr, unsigned short port);
        void _handle(const context_data_core &handler) override;
        void set_handler(listener_callback callback);

    protected:
        io_context& ctx_;
        listen_passive passive_;
        //TODO: MACOS
        //struct kevent changes{};
        listener_callback callback_;
        SOCKET_TYPE socket_ = INVALID_SOCKET;
    };

} // obelisk

#endif //OBELISK_LISTENER_H
