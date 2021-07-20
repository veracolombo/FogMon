#include "adaptive_factory.hpp"

IAdaptiveStorage* AdaptiveFactory::newAdaptiveStorage(std::string path){
    IAdaptiveStorage* ret = new AdaptiveStorage();
    ret->open(path);
    return ret;
}