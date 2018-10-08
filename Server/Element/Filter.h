//
// Created by lsacherer on 8/2/18.
//

#ifndef FIRSTHTTPSERVER_FILTER_H
#define FIRSTHTTPSERVER_FILTER_H


#include "Element.h"

class Filter: public Element {

public:
    explicit Filter(std::string name);
    ElementType getElementType() override;

};


#endif //FIRSTHTTPSERVER_FILTER_H
