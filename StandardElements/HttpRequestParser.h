//
// Created by lsacherer on 8/4/18.
//

#ifndef FIRSTHTTPSERVER_HTTPREQUESTPARSER_H
#define FIRSTHTTPSERVER_HTTPREQUESTPARSER_H


#include <boost/regex.hpp>
#include <memory>
#include "../Server/Element/Router.h"

class HttpRequestParser: public Router {

public:
    HttpRequestParser();
    bool run(std::unordered_map<std::string,std::string>& request_map, uint64_t request_num) override;

private:
    //loads all the needed regex expressions at construction to save time
    std::unique_ptr<boost::regex> firstLineRegex;
    std::unique_ptr<boost::regex> headersRegex;
    void setErrorState(std::unordered_map<std::string,std::string>& request_map);
};


#endif //FIRSTHTTPSERVER_HTTPREQUESTPARSER_H
