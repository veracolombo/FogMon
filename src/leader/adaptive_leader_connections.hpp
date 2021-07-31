#ifndef ADAPTIVE_LEADER_CONNECTIONS_HPP_
#define ADAPTIVE_LEADER_CONNECTIONS_HPP_

#include "leader_connections.hpp"
#include "adaptive_follower_connections.hpp"
#include "iadaptiveleader_connections.hpp"
#include "iadaptiveleader.hpp"

class AdaptiveLeaderConnections : public LeaderConnections, public AdaptiveFollowerConnections, virtual public IAdaptiveLeaderConnections {
protected:
    // void handler(int fd, Message &m);
    // void call_super_handler(int fd, Message &m) override;

    IAdaptiveLeader* parent;

public:
    AdaptiveLeaderConnections(int nThread);
    ~AdaptiveLeaderConnections();

    void initialize(IAdaptiveLeader* parent);

    bool sendChangeServer();
};

#endif