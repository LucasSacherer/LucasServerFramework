//
// Created by lsacherer on 7/30/18.
//

#include <memory>
#include <iostream>
#include "HelloWorldRouter.h"

HelloWorldRouter::HelloWorldRouter(): Router("HelloWorldRouter") {}

bool HelloWorldRouter::run(std::unordered_map<std::string,std::string>& request_map, uint64_t request_num) {

    auto headers = std::make_unique<std::unordered_map<std::string,std::string>>();
    auto content = std::string("Hello, World!");
    auto response = Router::buildResponse(*headers, content);

    request_map.insert({"response_string", response});
    logger->info({{"request_num", std::to_string(request_num)}, {"message", "Served HelloWorld response"}});

    return true;
}