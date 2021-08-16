#include "adaptive_leader_factory.hpp"
#include <iostream>

AdaptiveLeaderConnections* AdaptiveLeaderFactory::newConnections(int nThread){
    return new AdaptiveLeaderConnections(nThread);
}