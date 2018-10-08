//
// Created by lsacherer on 7/29/18.
//

#include <iostream>
#include "AppConfig.h"

AppConfig::AppConfig() {
    elements = std::make_unique<std::list<Element*>>();
}

void AppConfig::registerElement(Element& toAdd) {
    elements->push_back(&toAdd);
}

void AppConfig::registerErrorHandler(Element &element) {
    errorHandler = &element;
}

std::list<Element*> AppConfig::getElementList() {
    //return by copy since the list is owned by this class (this should only really be called once)
    return *elements;
}

Element* AppConfig::getErrorHandler() {
    return errorHandler;
}