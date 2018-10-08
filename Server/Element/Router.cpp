//
// Created by lsacherer on 7/29/18.
//

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "Router.h"

Router::Router(std::string name): Element(name) {
    logger->info({{"message", "Router created: " + name}});
}

std::string Router::buildResponse(std::unordered_map<std::string,std::string>& headers, std::string& content) {

    //mandatory headers
    headers.insert({"http_version", "1.1"});
    headers.insert({"http_response_code", "200 OK"});

    //good to have headers that are default for ease of use
    headers.insert({"Server","Lucas-Server/1.0"});
    headers.insert({"Content-Type","text/html"});
    headers.insert({std::string("Content-Length"),std::to_string(content.length())});

    //adds the response time if not already in the map
    if (headers.find("Date") == headers.end()) {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        auto ss = std::make_unique<std::stringstream>();
        *ss << std::put_time(std::localtime(&now_time), "%a, %d %b %Y %T %Z");
        headers.insert({"Date",ss->str()});
    }

    //builds the full response message
    std::string response("HTTP/");
    response.reserve(content.length() + (headers.size() * 55));
    response.append(headers["http_version"]);
    response.append(" ");
    response.append(headers["http_response_code"]);
    response.append("\n");

    for (const auto& [key, val]: headers){
        if (key != "http_version" && key != "http_response_code") {
            response.append(key);
            response.append(": ");
            response.append(val);
            response.append("\n");
        }
    }

    //adds the content to the response
    response.append("\n");
    response.append(content);

    return response;
}

ElementType Router::getElementType() {
    return ElementType::ROUTER;
}
