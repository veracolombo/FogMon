#ifndef ADAPTIVE_FACTORY_HPP_
#define ADAPTIVE_FACTORY_HPP_

#include "factory.hpp"
#include "adaptive_follower_connections.hpp"
<<<<<<< HEAD
#include "adaptive_storage_monitoring.hpp"
#include "metrics_generator.hpp"
=======
>>>>>>> parent of 371572f (aggiunta estensione storage per altre metriche)

class AdaptiveFactory : virtual public Factory {
public:
    virtual AdaptiveFollowerConnections* newConnections(int nThread);
<<<<<<< HEAD

    virtual IAdaptiveStorageMonitoring* newStorage(std::string path);
=======
    virtual IAdaptiveStorage* newAdaptiveStorage(std::string path);
>>>>>>> parent of 371572f (aggiunta estensione storage per altre metriche)
};

#endif