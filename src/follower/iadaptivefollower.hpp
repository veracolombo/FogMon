#ifndef I_ADAPTIVEFOLLOWER_HPP_
#define I_ADAPTIVEFOLLOWER_HPP_

#include "iagent.hpp"
#include "iadaptivestorage.hpp"
#include "iadaptive_storage_monitoring.hpp"
#include "iadaptivefollower_connections.hpp"

class IAdaptiveFollower : virtual public IAgent {
public:
    virtual IAdaptiveFollowerConnections* getConnections() = 0;
    virtual IAdaptiveStorageMonitoring* getStorage() = 0;
    //virtual IAdaptiveStorage* getAdaptiveStorage() = 0;

    virtual bool changeServer(vector<Message::node> nodes) = 0;
};

#endif
