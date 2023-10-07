//
// Created by hackman on 10/7/23.
//

#ifndef OBELISK_HTTP_REQUEST_H
#define OBELISK_HTTP_REQUEST_H
#include <string>
#include <memory>
#include <fstream>
#include <unordered_map>
namespace obelisk {
    class http_connection;

    struct request_title {
        std::string method_;
        std::string target_;
        std::string protocol_;
    };

    struct http_header{
        request_title title_;
        std::unordered_map<std::string, std::string> headers_;
    };

    class http_temp_fstream : public std::fstream{
    public:
        explicit http_temp_fstream(std::string path);
        ~http_temp_fstream() override;
        std::string path();
    protected:
        std::string path_;
    };

    class http_request{
    public:
        explicit http_request(http_connection& sock_);
        http_header header_;
        std::string boundary_;
        std::string content_type_;
        std::uint64_t content_length_ = 0;
    protected:
        http_connection& socket_;
    };

} // obelisk

#endif //OBELISK_HTTP_REQUEST_H
