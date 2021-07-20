#ifndef ADAPTIVE_FACTORY_HPP_
#define ADAPTIVE_FACTORY_HPP_

#include "adaptive_storage.hpp"

class AdaptiveFactory {
public:
    virtual IAdaptiveStorage* newAdaptiveStorage(std::string path);
};

#endif