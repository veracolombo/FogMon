#include "adaptive_leader_connections.hpp"
#include <iostream>

AdaptiveLeaderConnections::AdaptiveLeaderConnections(int nThread) : LeaderConnections(nThread),  AdaptiveFollowerConnections(nThread), FollowerConnections(nThread) { }

AdaptiveLeaderConnections::~AdaptiveLeaderConnections() { }

void AdaptiveLeaderConnections::initialize(IAdaptiveLeader* parent) {
    this->parent = parent;
    LeaderConnections::parent = this->parent;
}

/*
void AdaptiveLeaderConnections::handler(int fd, Message &m){
    
    string strIp = this->getSource(fd,m);

    bool handled = false;

    // gestione messaggi Adaptive Leader

    if(!handled)
        LeaderConnections::handler(fd, m);
    
    if(!handled)
        AdaptiveFollowerConnections::handler(fd, m);
}
*/

//void AdaptiveLeaderConnections::call_super_handler(int fd, Message &m) { }


bool AdaptiveLeaderConnections::sendChangeServer(){
    Message broadcast;
    broadcast.setSender(this->parent->getMyNode());
    broadcast.setType(Message::Type::typeNOTIFY);
    broadcast.setCommand(Message::Command::commSELECT_NEW_SERVER);
    broadcast.setArgument(Message::Argument::argNONE);

    return this->notifyAll(broadcast);
}