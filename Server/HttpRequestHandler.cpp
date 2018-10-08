//
// Created by lsacherer on 7/27/18.
//

#include <iostream>
#include <memory>
#include <sys/socket.h>
#include "HttpRequestHandler.h"
#include "Log/Logger.h"

HttpRequestHandler::HttpRequestHandler(uint64_t request_num, const std::list<Element*>* appList, Element* errorHandler):
    request_num(request_num), logger(std::make_unique<Logger>("HttpRequestHandler")), appList(appList){
    HttpRequestHandler::errorHandler = errorHandler;
}

void HttpRequestHandler::run(std::string& requestString, int sockfd) {

    auto request_map = std::make_unique<std::unordered_map<std::string,std::string>>();
    request_map->insert({"request_string", requestString});

    //run through the request_map through each element until the request is satisfied
    //we need to store what filters are called so that we can call them again once the request is satisfied
    auto filterList = std::make_unique<std::list<Element*>>();
    for (Element* e : *appList) {
        if (e->getElementType() == ElementType::FILTER) {
            filterList->push_back(e);
        }

        if (e->run(*request_map, request_num)) {
            //stop running new elements once the request has been satisfied
            break;
        }
    }

    //go through the list of filters that were already called for their second pass
    filterList->reverse();
    for (Element* e : *filterList) {
        e->run(*request_map, request_num);
    }

    //runs the error handler if an error handler was set during the handling of the request
    if (request_map->find("error_message") != request_map->end()) {
        errorHandler->run(*request_map, request_num);
    }

    //send the response after all elements have ran
    auto response_string = request_map->find("response_string");
    if (response_string == request_map->end()) {
        logger->error({{"request_num", std::to_string(request_num)}, {"message", "response_string was not sent in any element!"}});
    } else if (! sendResponse(sockfd, response_string->second)) {
        logger->error({{"request_num", std::to_string(request_num)}, {"message", "Failed to send response"}});
    } else {
        logger->info({{"request_num", std::to_string(request_num)}, {"message", "Sent the response succesfully!"}});
    }
}

bool HttpRequestHandler::sendResponse(int sockfd, const std::string& response) {

    auto bytesToSend = response.length();
    auto buffer = response.c_str();

    ssize_t sent;
    while (bytesToSend > 0){
        sent = send(sockfd, buffer, bytesToSend, 0);
        if (sent < 1) {
            perror("sendResponse");
            logger->error({{"request_num", std::to_string(request_num)},
                           {"message", "Failed to send response to sockfd: " + std::to_string(sockfd) }});
            return false;
        }

        buffer += sent;
        bytesToSend -= sent;
    }

    return true;
}