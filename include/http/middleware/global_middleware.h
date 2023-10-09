/***********************************************************************************************************************
 * @author Hackman Lo
 * @file global_middleware.h
 * @description 
 * @created_at 2023-10-09
***********************************************************************************************************************/

#ifndef OBELISK_GLOBAL_MIDDLEWARE_H
#define OBELISK_GLOBAL_MIDDLEWARE_H
#include <memory>
namespace obelisk {

    class http_response;
    class http_request;
    class http_middleware{
    public:
        virtual std::string name() = 0;
        virtual ~http_middleware() = default;
    };
    class before_middleware: public http_middleware {
    public:
        virtual std::shared_ptr<http_response> pre_handle(std::shared_ptr<http_request> request) = 0;
    };

    class after_middleware: public http_middleware {
    public:
        virtual std::shared_ptr<http_response> pre_handle(std::shared_ptr<http_request> request) = 0;
    };

} // obelisk

#endif //OBELISK_GLOBAL_MIDDLEWARE_H
