//
// Created by hackman on 10/8/23.
//

#ifndef OBELISK_HTTP_RESPONSE_H
#define OBELISK_HTTP_RESPONSE_H
#include <string>
#include <memory>
#include <unordered_map>
namespace obelisk {

    class http_response {
    public:
        enum EHTTP_RESP_STATUS {
            EST_CONTINUE = 100u,
            EST_OK = 200u,
            EST_CREATED = 201u,
            EST_ACCEPTED = 202u,
            EST_NO_CONTENT = 204u,
            EST_MOVED_PERMANENTLY = 301u,
            EST_FOUND = 302u,
            EST_NOT_MODIFIED = 304u,
            EST_USE_PROXY = 305u,
            EST_TEMPORARY_REDIRECT = 307u,
            EST_BAD_REQUEST = 400u,
            EST_UNAUTHORIZED = 401u,
            EST_FORBIDDEN = 403u,
            EST_NOT_FOUND = 404u,
            EST_METHOD_NOT_ALLOWED = 405u,
            EST_NOT_ACCEPTABLE = 406u,
            EST_PROXY_AUTHENTICATION_REQUIRED = 407u,
            EST_REQUEST_TIMEOUT = 408u,
            EST_CONFLICT = 409u,
            EST_GONE = 410u,
            EST_LENGTH_REQUIRED = 411u,
            EST_PRECONDITION_FAILED = 412u,
            EST_REQUEST_ENTITY_TOO_LARGE = 413u,
            EST_REQUEST_URI_TOO_LONG = 414u,
            EST_UNSUPPORTED_MEDIA_TYPE = 415u,
            EST_REQUESTED_RANGE_NOT_SATISFIABLE = 416u,
            EST_EXPECTATION_FAILED = 417u,
            EST_INTERNAL_SERVER_ERROR = 500u,
            EST_NOT_IMPLEMENTED = 501u,
            EST_BAD_GATEWAY = 502u,
            EST_SERVICE_UNAVAILABLE = 503u,
            EST_GATEWAY_TIMEOUT = 504u,
            EST_HTTP_VERSION_NOT_SUPPORTED = 505u,
        };

        explicit http_response(unsigned int code) ;
        explicit http_response(EHTTP_RESP_STATUS code);

        virtual ~http_response() = default;
        virtual void add_header(const std::string& name, const std::string& value);
        virtual std::string serialize_header();
        std::shared_ptr<std::istream> content();

    protected:
        static std::unordered_map<EHTTP_RESP_STATUS, std::string> resp_status_map_;
        std::unordered_map<std::string, std::string> headers_;
        EHTTP_RESP_STATUS resp_code_ = EHTTP_RESP_STATUS(200);
        std::shared_ptr<std::iostream> content_;
        std::uint64_t content_length_ = 0;
    };

} // obelisk

#endif //OBELISK_HTTP_RESPONSE_H
