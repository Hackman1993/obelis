/***********************************************************************************************************************
 * @author Hackman Lo
 * @file file_response.h
 * @description 
 * @created_at 2023-10-10
***********************************************************************************************************************/

#ifndef OBELISK_FILE_RESPONSE_H
#define OBELISK_FILE_RESPONSE_H
#include "http_response.h"
namespace obelisk {
    class file_response : public http_response{
    public:
        file_response(const std::string &path, EHTTP_RESP_STATUS code);
    protected:

    };

} // obelisk

#endif //OBELISK_FILE_RESPONSE_H
