//
// Created by lsacherer on 8/1/18.
//

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "Logger.h"

Logger::Logger(std::string logger_name) :
logger_name(std::make_unique<std::string>(logger_name)){

    newlineRegex = std::make_unique<boost::regex>(R"r((\n)|(\r))r",
            boost::regex_constants::optimize);
}

void Logger::log(std::list<std::pair<std::string, std::string>>& toLog) {

    //adds the logger name to the end of the list
    toLog.emplace_back("logger", *logger_name);

    auto ss = std::make_unique<std::stringstream>();

    //adds the date at the start of the log
    *ss << "[";
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    *ss << std::put_time(std::localtime(&now_time), "%FT%T");
    *ss << "] ";

    //adds the pairs from the list
    for (auto pair : toLog) {
        //replaces all the newline chars with their matching literal (makes it so all logs are on one line)
        auto value = boost::regex_replace(pair.second, *newlineRegex, R"r(?1(\\n):(\\r))r", boost::format_all);
        *ss << "(" << pair.first << " = '" << value << "') ";
    }

    //logs the message
    std::cout << ss->str() << std::endl;
}

void Logger::info(std::list<std::pair<std::string, std::string>> toLog) {
    toLog.emplace_front("level", "INFO");
    log(toLog);
}

void Logger::warn(std::list<std::pair<std::string, std::string>> toLog) {
    toLog.emplace_front("level", "WARN");
    log(toLog);
}

void Logger::error(std::list<std::pair<std::string, std::string>> toLog) {
    toLog.emplace_front("level", "ERROR");
    log(toLog);
}