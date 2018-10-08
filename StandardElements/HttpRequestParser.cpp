//
// Created by lsacherer on 8/4/18.
//

#include "HttpRequestParser.h"

HttpRequestParser::HttpRequestParser(): Router("HttpRequestParser") {
    firstLineRegex = std::make_unique<boost::regex>(
            R"r(^(GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE|PATCH) ([\w\d\.\-_\/:=\+!@#$%\^&\*\(\)]+) (HTTP\/\d.\d))r",
            boost::regex_constants::optimize);

    //first group has the header, second group has the value
    headersRegex = std::make_unique<boost::regex>(R"r(([\w\-]{2,}): ([\w,\-\*\/+;=\. :\(\)]+))r",
            boost::regex_constants::optimize);
}

bool HttpRequestParser::run(std::unordered_map<std::string, std::string> &request_map, uint64_t request_num) {
    //this method will return false if everything is ok (passing it on to the next element)
    //and will return true if there is an error (to be passed to the error handler)


    std::string request = request_map["request_string"];
    //logger->info({{"request", request}}); // <- this is only needed to debug the parsing

    auto parsedList = std::make_unique<std::list<std::pair<std::string,std::string>>>();
    parsedList->emplace_back("request_num", std::to_string(request_num));

    auto requestStream = std::make_unique<std::istringstream>(request);

    //tries to parse the first line of the HTTP request
    auto firstLineMatch = std::make_unique<boost::smatch>();
    auto line = std::make_unique<std::string>();
    if (std::getline(*requestStream, *line) && boost::regex_search(*line, *firstLineMatch, *firstLineRegex)) {
        //adds the first line of the http header
        const auto method = std::make_pair("method", (*firstLineMatch)[1].str());
        const auto request_uri = std::make_pair("request_uri",(*firstLineMatch)[2].str());
        const auto http_version = std::make_pair("http_version", (*firstLineMatch)[3].str());
        parsedList->emplace_back(method);
        parsedList->emplace_back(request_uri);
        parsedList->emplace_back(http_version);
        request_map.insert(method);
        request_map.insert(request_uri);
        request_map.insert(http_version);
    } else {
        logger->warn({{"request_num", std::to_string(request_num)}, {"message", "Failed to read first line: " + *line}});
        setErrorState(request_map);
        return true; //request is done and should go to the error controller
    }

    //tries to parse header lines (if any) (if less than two, probs the '\r' char and we should move on to the body)
    auto headerMatch = std::make_unique<boost::smatch>();
    while (std::getline(*requestStream, *line) && line->length() > 2) {
        if (boost::regex_search(*line, *headerMatch, *headersRegex)) {
            const auto header = std::make_pair((*headerMatch)[1].str(), (*headerMatch)[2].str());
            parsedList->emplace_back(header);
            request_map.insert(header);
        } else {
            logger->warn({{"request_num", std::to_string(request_num)}, {"message", "Failed to read header line: " + *line}});
            setErrorState(request_map);
            return true; //same reason as above return
        }
    }

    //gets the rest of the message as a string (if there is any)
    //todo: get this variable from a config thing
    const int bufferSize = 10000;
    char bodyBuffer[bufferSize];
    requestStream->read(bodyBuffer, bufferSize - 2);
    bodyBuffer[requestStream->gcount()] = '\0';
    auto requestBody = std::make_unique<std::string>(bodyBuffer);
    if (requestBody->length() > 0) {
        const auto body = std::make_pair("request_body", *requestBody);
        parsedList->emplace_back(body);
        request_map.insert(body);
    }

    //prints out everything that got parsed
    logger->info(*parsedList);

    //tells the handler to keep routing
    return false;
}

void HttpRequestParser::setErrorState(std::unordered_map<std::string, std::string> &request_map) {
    request_map.insert({"error_code", "400 Bad Request"});
    request_map.insert({"error_message", "Could not parse HTTP request"});
}