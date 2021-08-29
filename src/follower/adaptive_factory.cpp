#include "adaptive_factory.hpp"

AdaptiveFollowerConnections* AdaptiveFactory::newConnections(int nThread) {
    return new AdaptiveFollowerConnections(nThread);
}

IAdaptiveStorageMonitoring* AdaptiveFactory::newStorage(std::string path) {
    IAdaptiveStorageMonitoring* ret = new AdaptiveStorageMonitoring();
    ret->open(path);
    return ret;
}

/*
IAdaptiveStorage* AdaptiveFactory::newAdaptiveStorage(std::string path) {
    IAdaptiveStorage* ret = new AdaptiveStorage();
    ret->open(path);
    return ret;
}
*/