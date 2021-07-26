#ifndef ADAPTIVE_LEADER_HPP
#define ADAPTIVE_LEADER_HPP

#include "leader.hpp"
#include "adaptive_follower.hpp"

class AdaptiveLeader : public Leader, public AdaptiveFollower {

public:
    AdaptiveLeader();
    AdaptiveLeader(Message::node node, int nThreads);
    ~AdaptiveLeader();

    void start(std::vector<Message::node> mNodes);
    void stop();

    void initialize(LeaderFactory* factory = NULL, AdaptiveFactory* adFact = NULL);
};

#endif