//
// Created by lsacherer on 7/29/18.
//

#ifndef FIRSTHTTPSERVER_APPCONFIG_H
#define FIRSTHTTPSERVER_APPCONFIG_H


#include "Element.h"
#include <list>
#include <memory>

class AppConfig {

public:
    AppConfig();
    void registerElement(Element& toAdd);
    void registerErrorHandler(Element& element);
    std::list<Element*> getElementList();
    Element* getErrorHandler();

private:
    std::unique_ptr<std::list<Element*>> elements;
    Element* errorHandler;
};


#endif //FIRSTHTTPSERVER_APPCONFIG_H
