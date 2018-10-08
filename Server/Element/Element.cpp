//
// Created by lsacherer on 8/2/18.
//

#include "Element.h"

Element::Element(std::string name) {
    Element::name = std::make_unique<std::string>(name);
    logger = std::make_unique<Logger>(name);
}