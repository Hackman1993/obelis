//
// Created by hackman on 10/8/23.
//

#ifndef OBELISK_HTTP_RESPONSE_H
#define OBELISK_HTTP_RESPONSE_H
#include <string>
#include <memory>
#include <unordered_map>
#include "http/core/http_response_code.h"

namespace obelisk {

    class http_response {
    public:
        explicit http_response(EHTTP_RESP_STATUS code);

        virtual ~http_response() = default;
        virtual void add_header(const std::string& name, const std::string& value);
        virtual std::string serialize_header();
        std::shared_ptr<std::istream> content();
        virtual std::uint64_t content_length();
        virtual bool has_header(const std::string& header);

    protected:
        static std::unordered_map<EHTTP_RESP_STATUS, std::string> resp_status_map_;
        std::unordered_map<std::string, std::string> headers_;
        EHTTP_RESP_STATUS resp_code_ = EHTTP_RESP_STATUS(200);
        std::shared_ptr<std::iostream> content_;
        std::uint64_t content_length_ = 0;
    };

} // obelisk

#endif //OBELISK_HTTP_RESPONSE_H
