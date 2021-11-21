#ifndef ADAPTIVE_LEADER_HPP
#define ADAPTIVE_LEADER_HPP

#include "leader.hpp"
#include "adaptive_follower.hpp"
#include "adaptive_leader_connections.hpp"
#include "adaptive_leader_factory.hpp"
#include "adaptive_leader_controller.hpp"

class AdaptiveLeader : virtual public IAdaptiveLeader, public Leader, public AdaptiveFollower {

public:
<<<<<<< HEAD
    static AdaptiveLeader* myobj;
=======
    static bool leaderAdequacy;
>>>>>>> parent of f5a88dd (added changeServer())

    AdaptiveLeader();
    AdaptiveLeader(Message::node node, int nThreads);
    ~AdaptiveLeader();

    void initialize(AdaptiveLeaderFactory* factory = NULL);

    void start(std::vector<Message::node> mNodes);
    void stop();

    IAdaptiveLeaderConnections* getConnections();
    IAdaptiveLeaderStorageMonitoring* getStorage();

    AdaptiveLeaderController* getAdaptiveController();

protected:
<<<<<<< HEAD
    AdaptiveLeaderController *adaptive_controller;

    void timerFun() override;
=======
    // void timerFun() override;
>>>>>>> parent of f5a88dd (added changeServer())

    AdaptiveLeaderFactory tFactory;
    AdaptiveLeaderFactory *factory;

    AdaptiveLeaderConnections *connections;
    IAdaptiveLeaderStorageMonitoring *storage;
};

#endif