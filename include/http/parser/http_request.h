//
// Created by hackman on 10/7/23.
//

#ifndef OBELISK_HTTP_REQUEST_H
#define OBELISK_HTTP_REQUEST_H
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <fstream>
#include <variant>
#include <filesystem>
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

    struct http_file{
        http_file(std::string tmp_path, std::string filename): temp_path_(std::move(tmp_path)), filename_(std::move(filename)){}
        std::string temp_path_;
        std::string filename_;
        ~http_file(){
            if(std::filesystem::exists(temp_path_))
                std::filesystem::remove(temp_path_);
        }
    };

    class http_request{
    public:
        explicit http_request(http_connection& sock_);

        void set_param(const std::string& name, const std::string& value);
        http_header header_;
        std::string path_;
        std::string boundary_;
        std::string content_type_;
        std::uint64_t content_length_ = 0;
        std::shared_ptr<std::iostream> raw_;
        std::unordered_map<std::string, std::vector<std::string>> params_;
        std::unordered_map<std::string, std::shared_ptr<http_file>> filebag_;
    protected:
        http_connection& socket_;
    };

} // obelisk

#endif //OBELISK_HTTP_REQUEST_H
