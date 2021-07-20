#ifndef IADAPTIVEFACTORY_HPP_
#define IADAPTIVEFACTORY_HPP_

#include "ifactory.hpp"
#include "iadaptivestorage.hpp"
#include <string>

class IAdaptiveFactory {
public:
    virtual IAdaptiveStorage* newAdaptiveStorage(std::string path) = 0;
};

#endif