#include "adaptive_follower_connections.hpp"
#include <iostream>

AdaptiveFollowerConnections::AdaptiveFollowerConnections(int nThread) : FollowerConnections(nThread) {
}

AdaptiveFollowerConnections::~AdaptiveFollowerConnections() {
}

void AdaptiveFollowerConnections::initialize(IAdaptiveFollower* parent) {
    this->parent = parent;
}

/*
void AdaptiveFollowerConnections::handler(int fd, Message &m){
    string strIp = this->getSource(fd,m);

    bool handled = false;

    if(m.getType() == Message::Type::typeNOTIFY){
        if(m.getCommand() == Message::Command::commSELECT_NEW_SERVER){
            if(m.getArgument() == Message::Argument::argNONE){
                handled = true;
                //this->parent->changeServer();
                cout << "changeServer()" << endl;
            }
        }
    }

    if(!handled)
        FollowerConnections::handler(fd, m);
}
*/

/*
void AdaptiveFollowerConnections::call_super_handler(int fd, Message &m) {
    FollowerConnections::handler(fd, m);
}
*/