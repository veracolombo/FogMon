#ifndef I_ADAPTIVE_FACTORY_HPP_
#define I_ADAPTIVE_FACTORY_HPP_

#include "iadaptivestorage.hpp"

#include <string>

class IAdaptiveFactory {
public:
   virtual IAdaptiveStorage* newAdaptiveStorage(std::string path) = 0;
};

#endif