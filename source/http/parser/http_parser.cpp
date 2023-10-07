//
// Created by hackman on 10/7/23.
//

#include "http/parser/http_parser.h"
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
using namespace boost::spirit::x3;
BOOST_FUSION_ADAPT_STRUCT(obelisk::request_title, method_, target_, protocol_);
BOOST_FUSION_ADAPT_STRUCT(obelisk::http_header, title_, headers_);
namespace obelisk {
    auto mat_parser_ = rule<class mat_parser, request_title>{"MATParser"} = +~char_(" \r\n") >> " " >> +~char_(" \r\n") >> " " >> no_case["HTTP/"] >> +~char_(" \r\n") >> "\r\n";
    auto http_headers_parser_ = rule<class mat_parser, std::pair<std::string, std::string> >{"HttpHeadersParser"} = +~char_(":\r\n") >> ": " >> (+~char_("\r\n")) >> "\r\n";
    auto http_package_header_parser_ = rule<class mat_parser, http_header>{"HttpPackageHeaderParser"} = mat_parser_ >> +(http_headers_parser_) >> "\r\n";
    auto boundary_parser_ = no_case["multipart/form-data; boundary="] >> (+~char_("\r\n"));

    bool package_header_parse(std::string_view data, http_request *request) {
        auto result = parse(data.begin(), data.end(), http_package_header_parser_, request->header_);
        return result;
    }

    bool boundary_parse(std::string_view  data, http_request *request){
        auto result = parse(data.begin(), data.end(), boundary_parser_, request->boundary_);
        return result;
    }

} // obelisk