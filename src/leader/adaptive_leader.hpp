#ifndef ADAPTIVE_LEADER_HPP
#define ADAPTIVE_LEADER_HPP

#include "leader.hpp"
#include "adaptive_follower.hpp"
#include "adaptive_leader_connections.hpp"
#include "adaptive_leader_factory.hpp"

class AdaptiveLeader : virtual public IAdaptiveLeader, public Leader, public AdaptiveFollower {

public:
    AdaptiveLeader();
    AdaptiveLeader(Message::node node, int nThreads);
    ~AdaptiveLeader();

    void initialize(AdaptiveLeaderFactory* factory = NULL);

    void start(std::vector<Message::node> mNodes);
    void stop();

    IAdaptiveLeaderConnections* getConnections();

protected:
    void timerFun() override;

    AdaptiveLeaderFactory tFactory;
    AdaptiveLeaderFactory *factory;

    AdaptiveLeaderConnections *connections;
};

#endif