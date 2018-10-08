//
// Created by lsacherer on 7/27/18.
//

#ifndef FIRSTHTTPSERVER_HTTPREQUESTHANDLER_H
#define FIRSTHTTPSERVER_HTTPREQUESTHANDLER_H


#include <string>
#include <list>
#include "Element/Element.h"
#include "Log/Logger.h"

class HttpRequestHandler {
public:
    //todo: probably change this to directly take the app config

    explicit HttpRequestHandler(uint64_t request_num, const std::list<Element*>* appList, Element* errorHandler);
    void run(std::string& requestString, int sockfd);

private:
    const std::unique_ptr<Logger> logger;
    const uint64_t request_num;
    const std::list<Element*>* appList;
    Element* errorHandler;
    bool sendResponse(int sockfd, const std::string& response);
};


#endif //FIRSTHTTPSERVER_HTTPREQUESTHANDLER_H
