#include "adaptive_leader_factory.hpp"
#include <iostream>

AdaptiveLeaderConnections* AdaptiveLeaderFactory::newConnections(int nThread){
    cout << "AdaptiveLeaderFactory::newConnections()" << endl;
    return new AdaptiveLeaderConnections(nThread);
}