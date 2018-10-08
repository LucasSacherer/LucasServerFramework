//
// Created by lsacherer on 8/2/18.
//

#include "Filter.h"

Filter::Filter(std::string name): Element(name) {
    logger->info({{"message","Filter created: " + name}});
}

ElementType Filter::getElementType() {
    return ElementType::FILTER;
}