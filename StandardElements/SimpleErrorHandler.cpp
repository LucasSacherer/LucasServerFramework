//
// Created by lsacherer on 8/7/18.
//

#include "SimpleErrorHandler.h"

SimpleErrorHandler::SimpleErrorHandler(): Router("SimpleErrorHandler") {}

bool SimpleErrorHandler::run(std::unordered_map<std::string, std::string> &request_map, uint64_t request_num) {

    const auto error_code = request_map["error_code"];
    auto error_message = request_map["error_message"];

    auto headers = std::make_unique<std::unordered_map<std::string,std::string>>();
    headers->insert({"http_response_code", error_code});

    auto response = Router::buildResponse(*headers, error_message);

    request_map.insert({"response_string", response});
    logger->info({{"request_num", std::to_string(request_num)}, {"message", "Served error response"}});

    return true;

}