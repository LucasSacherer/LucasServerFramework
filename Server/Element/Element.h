//
// Created by lsacherer on 7/29/18.
//

#ifndef FIRSTHTTPSERVER_ELEMENT_H
#define FIRSTHTTPSERVER_ELEMENT_H


#include <optional>
#include <unordered_map>
#include <memory>
#include "../Log/Logger.h"

enum class ElementType: unsigned short {ROUTER, FILTER, OTHER};

class Element {

public:
    Element(std::string name);

    virtual bool run(std::unordered_map<std::string,std::string>& request_map, uint64_t request_num) = 0;

    virtual ElementType getElementType() = 0;

protected:
    std::unique_ptr<Logger> logger;
    std::unique_ptr<std::string> name;

};


#endif //FIRSTHTTPSERVER_ELEMENT_H
