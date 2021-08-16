#include "adaptive_leader_connections.hpp"
#include <iostream>

AdaptiveLeaderConnections::AdaptiveLeaderConnections(int nThread) : LeaderConnections(nThread),  AdaptiveFollowerConnections(nThread), FollowerConnections(nThread) { }

AdaptiveLeaderConnections::~AdaptiveLeaderConnections() { }

void AdaptiveLeaderConnections::initialize(IAdaptiveLeader* parent) {
    LeaderConnections::initialize(parent);
    AdaptiveFollowerConnections::parent = parent;
    this->parent = parent;
}


void AdaptiveLeaderConnections::handler(int fd, Message &m){
    
    string strIp = this->getSource(fd,m);

    bool handled = false;

    // { ... } gestione messaggi Adaptive Leader

    if(!handled)
        LeaderConnections::handler(fd, m);
    
    if(!handled)
        AdaptiveFollowerConnections::handler(fd, m);
}


void AdaptiveLeaderConnections::call_super_handler(int fd, Message &m) { }


bool AdaptiveLeaderConnections::sendChangeServer(){
    vector<Message::node> mnodes = this->parent->getStorage()->getMNodes();

    vector<Message::node> nodes;
    for(int i=0; i<mnodes.size(); i++){
        if(mnodes[i].ip != this->parent->getMyNode().ip)
            nodes.push_back(mnodes[i]);
    }

    Message broadcast;
    broadcast.setSender(this->parent->getMyNode());
    broadcast.setType(Message::Type::typeNOTIFY);
    broadcast.setCommand(Message::Command::commSELECT_NEW_SERVER);
    broadcast.setArgument(Message::Argument::argMNODES);
    broadcast.setData(nodes);

    cout << "Sending changeServer()" << endl;

    return this->notifyAll(broadcast);
}