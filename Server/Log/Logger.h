//
// Created by lsacherer on 8/1/18.
//

#ifndef FIRSTHTTPSERVER_LOGGER_H
#define FIRSTHTTPSERVER_LOGGER_H


#include <string>
#include <memory>
#include <list>
#include <boost/regex.hpp>

class Logger {

public:
    explicit Logger(std::string logger_name);

    void info(std::list<std::pair<std::string, std::string>> toLog);
    void warn(std::list<std::pair<std::string, std::string>> toLog);
    void error(std::list<std::pair<std::string, std::string>> toLog);

private:
    void log(std::list<std::pair<std::string, std::string>>& toLog);
    const std::unique_ptr<std::string> logger_name;

    std::unique_ptr<boost::regex> newlineRegex;

};


#endif //FIRSTHTTPSERVER_LOGGER_H
