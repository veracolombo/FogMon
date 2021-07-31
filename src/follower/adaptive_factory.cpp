#include "adaptive_factory.hpp"

IAdaptiveStorage* AdaptiveFactory::newAdaptiveStorage(std::string path) {
    IAdaptiveStorage* ret = new AdaptiveStorage();
    ret->open(path);
    return ret;
}

AdaptiveFollowerConnections* AdaptiveFactory::newConnections(int nThread) {
    return new AdaptiveFollowerConnections(nThread);
}