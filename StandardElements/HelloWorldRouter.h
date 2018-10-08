//
// Created by lsacherer on 7/30/18.
//

#ifndef FIRSTHTTPSERVER_HELLOWORLDROUTER_H
#define FIRSTHTTPSERVER_HELLOWORLDROUTER_H


#include "../Server/Element/Router.h"

class HelloWorldRouter: public Router {

public:
    HelloWorldRouter();
    bool run(std::unordered_map<std::string,std::string>& request_map, uint64_t request_num) override;

};


#endif //FIRSTHTTPSERVER_HELLOWORLDROUTER_H
