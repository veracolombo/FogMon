#ifndef I_ADAPTIVE_LEADER_HPP_
#define I_ADAPTIVE_LEADER_HPP_

#include "iadaptiveleader_connections.hpp"
#include "ileader.hpp"
#include "iadaptivefollower.hpp"

class IAdaptiveLeader : virtual public ILeader, virtual public IAdaptiveFollower {
public:
    virtual IAdaptiveLeaderConnections* getConnections() = 0;
};

#endif