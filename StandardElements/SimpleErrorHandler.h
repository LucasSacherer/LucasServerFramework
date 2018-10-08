//
// Created by lsacherer on 8/7/18.
//

#ifndef FIRSTHTTPSERVER_SIMPLEERRORHANDLER_H
#define FIRSTHTTPSERVER_SIMPLEERRORHANDLER_H


#include "../Server/Element/Router.h"

class SimpleErrorHandler: public Router {

public:
    SimpleErrorHandler();
    bool run(std::unordered_map<std::string,std::string>& request_map, uint64_t request_num) override;

};


#endif //FIRSTHTTPSERVER_SIMPLEERRORHANDLER_H
