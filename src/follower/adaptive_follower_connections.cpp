#include "adaptive_follower_connections.hpp"
#include <iostream>

#include <sys/socket.h>
#include <unistd.h>
#include "adaptive_report.hpp"
#include "adaptive_message.hpp"


AdaptiveFollowerConnections::AdaptiveFollowerConnections(int nThread) : FollowerConnections(nThread) {
}

AdaptiveFollowerConnections::~AdaptiveFollowerConnections() {
}

void AdaptiveFollowerConnections::initialize(IAdaptiveFollower* parent) {
    Connections::initialize(parent);
    this->parent = parent;
}

/*
std::optional<std::pair<int64_t,Message::node>> AdaptiveFollowerConnections::sendUpdate(Message::node ipS, std::pair<int64_t,Message::node> update) {
    cout << "sendUpdate0" << endl;
    int Socket = openConnection(ipS.ip, ipS.port);
    cout << "sendUpdate0.5" << endl;
    if(Socket < 0) {
        return nullopt;
    }

    fflush(stdout);
    char buffer[10];

    //build update message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::typeNOTIFY);
    m.setCommand(Message::Command::commUPDATE);
    m.setArgument(Message::Argument::argREPORT);

    AdaptiveReport r;
    r.setHardware(this->parent->getStorage()->getHardware());
    r.setBattery(this->parent->getStorage()->getBattery());
    r.setIot(this->parent->getStorage()->getIots());

    int64_t now = this->parent->getStorage()->getTime();
    int64_t time = update.first;
    if(ipS == update.second) {          // se l'ultima update è stata fatta a questo ip
        r.setLatency(this->parent->getStorage()->getLatency(this->parent->node->sensitivity,time));
        r.setBandwidth(this->parent->getStorage()->getBandwidth(this->parent->node->sensitivity,time));
        
    } else { //send all data
        r.setLatency(this->parent->getStorage()->getLatency(0));
        r.setBandwidth(this->parent->getStorage()->getBandwidth(0));
    }
    m.setData(r);

    std::optional<std::pair<int64_t,Message::node>> result = nullopt;
    cout << "sendUpdate1" << endl;
    //send update message
    if(this->sendMessage(Socket, m)) {
        cout << "sendUpdate2" << endl;
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::typeRESPONSE &&
                res.getCommand() == Message::Command::commUPDATE &&
                res.getArgument() == Message::Argument::argPOSITIVE) {
                
                result = std::make_pair(now, ipS); // aggiornamento del dato membro 'update' con nodo e tempo dell'ultimo update
                this->parent->getStorage()->saveState(time,this->parent->node->sensitivity);        // ??? non viene svolto
            }
        }
    }
    cout << "sendUpdate3" << endl;
    close(Socket);
    return result;
}
*/


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