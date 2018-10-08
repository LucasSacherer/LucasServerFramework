//
// Created by lsacherer on 7/29/18.
//

#ifndef FIRSTHTTPSERVER_ROUTER_H
#define FIRSTHTTPSERVER_ROUTER_H


#include "Element.h"

class Router: public Element {

protected:
    std::string buildResponse(std::unordered_map<std::string,std::string>& headers, std::string& content);

public:
    explicit Router(std::string name);
    ElementType getElementType() override;

};


#endif //FIRSTHTTPSERVER_ROUTER_H
