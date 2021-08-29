#ifndef ADAPTIVE_FACTORY_HPP_
#define ADAPTIVE_FACTORY_HPP_

#include "iadaptivefactory.hpp"
#include "factory.hpp"
//#include "adaptive_storage.hpp"
#include "adaptive_follower_connections.hpp"
#include "adaptive_storage_monitoring.hpp"
#include "metrics_generator.hpp"

class AdaptiveFactory : virtual public Factory, public IAdaptiveFactory {
public:
    virtual AdaptiveFollowerConnections* newConnections(int nThread);

    virtual IAdaptiveStorageMonitoring* newStorage(std::string path);
    //virtual IAdaptiveStorage* newAdaptiveStorage(std::string path);
};

#endif