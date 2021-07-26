#ifndef ADAPTIVE_FACTORY_HPP_
#define ADAPTIVE_FACTORY_HPP_

#include "adaptive_storage.hpp"
#include "iadaptivefactory.hpp"

class AdaptiveFactory : public IAdaptiveFactory {
public:
    virtual IAdaptiveStorage* newAdaptiveStorage(std::string path);
};

#endif