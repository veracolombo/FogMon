#ifndef I_ADAPTIVEFOLLOWER_HPP_
#define I_ADAPTIVEFOLLOWER_HPP_

#include "iagent.hpp"
#include "iadaptivestorage.hpp"

#include "iadaptivefollower_connections.hpp"

class IAdaptiveFollower : virtual public IAgent {
public:
    virtual IAdaptiveFollowerConnections* getConnections() = 0;
    virtual IAdaptiveStorage* getAdaptiveStorage() = 0;
};

#endif
