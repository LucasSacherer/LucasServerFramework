//
// Created by lsacherer on 8/2/18.
//

#ifndef FIRSTHTTPSERVER_TIMERFILTER_H
#define FIRSTHTTPSERVER_TIMERFILTER_H


#include "../Server/Element/Filter.h"

class TimerFilter: public Filter {

public:
    TimerFilter();
    bool run(std::unordered_map<std::string,std::string>& request_map, uint64_t request_num) override;

};


#endif //FIRSTHTTPSERVER_TIMERFILTER_H
