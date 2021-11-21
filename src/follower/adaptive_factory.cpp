#include "adaptive_factory.hpp"

<<<<<<< HEAD
AdaptiveFollowerConnections* AdaptiveFactory::newConnections(int nThread) {
    return new AdaptiveFollowerConnections(nThread);
}

IAdaptiveStorageMonitoring* AdaptiveFactory::newStorage(std::string path) {
    IAdaptiveStorageMonitoring* ret = new AdaptiveStorageMonitoring();
    ret->open(path);
    return ret;
=======
IAdaptiveStorage* AdaptiveFactory::newAdaptiveStorage(std::string path) {
    IAdaptiveStorage* ret = new AdaptiveStorage();
    ret->open(path);
    return ret;
}

AdaptiveFollowerConnections* AdaptiveFactory::newConnections(int nThread) {
    return new AdaptiveFollowerConnections(nThread);
>>>>>>> parent of 371572f (aggiunta estensione storage per altre metriche)
}