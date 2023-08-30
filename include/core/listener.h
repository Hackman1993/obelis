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
namespace obelisk {

    class listener {
    public:
        listener() = default;
        void listen(unsigned short port, const std::string& addr);

    protected:
        std::function<bool ()> accepted_;

    };

} // obelisk

#endif //OBELISK_LISTENER_H
