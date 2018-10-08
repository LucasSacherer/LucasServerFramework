//
// Created by lsacherer on 8/2/18.
//

#include <chrono>
#include <memory>
//#include <stdlib.h>
#include "TimerFilter.h"

TimerFilter::TimerFilter(): Filter("TimerFilter") {}

bool TimerFilter::run(std::unordered_map<std::string, std::string> &request_map, uint64_t request_num) {
    struct timespec current_timespec;
    clock_gettime(CLOCK_MONOTONIC_RAW, &current_timespec);
    auto current_time = (uint64_t)((current_timespec.tv_sec * 1000000) + (current_timespec.tv_nsec/1000));

    if (request_map.find("start_time") == request_map.end()) {
        //first time that the filter is called

        request_map.insert({"start_time", std::to_string(current_time)});
    } else {
        //second time the function is called, get the differenc in time and log it

        uint64_t prev_time = std::stoull(request_map["start_time"], nullptr);

        logger->info({{"request_num", std::to_string(request_num)},
                      {"response_time_in_us", std::to_string(current_time - prev_time)}});
    }

    return false;
}
