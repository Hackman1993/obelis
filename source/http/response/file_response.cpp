/***********************************************************************************************************************
 * @author Hackman Lo
 * @file file_response.cpp
 * @description 
 * @created_at 2023-10-10
***********************************************************************************************************************/

#include "http/core/mime_types.h"
#include "http/response/file_response.h"
#include "http/exception/http_exception.h"
#include <fstream>
#include <filesystem>
namespace obelisk {
    file_response::file_response(const std::string &path, EHTTP_RESP_STATUS code) : http_response(code) {
        if(std::filesystem::is_directory(path) || !std::filesystem::exists(path))
            THROW(http_exception, "File Not Found", "Obelisk", EHTTP_RESP_STATUS::EST_NOT_FOUND);
        auto fs = std::make_shared<std::fstream>();
        fs->open(path, std::ios::in | std::ios::binary);
        if(fs->fail()){
            THROW(http_exception, "Permission Denied", "Obelisk", EHTTP_RESP_STATUS::EST_FORBIDDEN);
        }
        std::filesystem::path path_(path);
        std::string extension = path_.extension();
        headers_["Content-Type"] = mime_types::lookup(extension);
        content_length_ = std::filesystem::file_size(path_);
        content_ = fs;
    }
} // obelisk