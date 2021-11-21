
#ifndef ADAPTIVE_LEADER_FACTORY_HPP_
#define ADAPTIVE_LEADER_FACTORY_HPP_

#include "leader_factory.hpp"
#include "adaptive_factory.hpp"
#include "adaptive_leader_connections.hpp"

class AdaptiveLeaderFactory : public LeaderFactory, public AdaptiveFactory {
public:
    virtual AdaptiveLeaderConnections* newConnections(int nThread);
};

#endif