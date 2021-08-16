#include "adaptive_follower_connections.hpp"
#include <iostream>


AdaptiveFollowerConnections::AdaptiveFollowerConnections(int nThread) : FollowerConnections(nThread) {
}

AdaptiveFollowerConnections::~AdaptiveFollowerConnections() {
}

void AdaptiveFollowerConnections::initialize(IAdaptiveFollower* parent) {
    Connections::initialize(parent);
    this->parent = parent;
}


void AdaptiveFollowerConnections::handler(int fd, Message &m){
    string strIp = this->getSource(fd,m);

    bool handled = false;

    if(m.getType() == Message::Type::typeNOTIFY){
        if(m.getCommand() == Message::Command::commSELECT_NEW_SERVER){
            if(m.getArgument() == Message::Argument::argMNODES){
                handled = true;

                vector<Message::node> res;
                m.getData(res);

                cout << "Message changeServer() received" << endl; 
                
                if(!this->parent->changeServer(res)){
                    cout << "Server not changed" << endl;
                } else{
                    cout << "Server changed" << endl;
                }
            }
        }
    }

    if(!handled)
        this->call_super_handler(fd, m);
}



void AdaptiveFollowerConnections::call_super_handler(int fd, Message &m) {
    FollowerConnections::handler(fd, m);
}