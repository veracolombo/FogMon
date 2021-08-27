#include "adaptive_leader_connections.hpp"

#include <iostream>
#include <unistd.h>

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
    /*

    if(m.getType() == Message::Type::typeNOTIFY){
        if(m.getCommand() == Message::Command::commUPDATE){
            if(m.getArgument() == Message::Argument::argREPORT){
                handled = true;
                //get the report
                //the report should be only a part of it
                AdaptiveReport r;
                if(m.getData(r)) {
                    Message res;
                    res.setType(Message::Type::typeRESPONSE);
                    res.setCommand(Message::Command::commUPDATE);
                    res.setArgument(Message::Argument::argPOSITIVE);
                    
                    sendMessage(fd, res);

                    AdaptiveReport::hardware_result hardware;
                    vector<AdaptiveReport::test_result> latency;
                    vector<AdaptiveReport::test_result> bandwidth;
                    vector<AdaptiveReport::IoT> iot;
                    AdaptiveReport::battery_result battery;

                    bool hw = r.getHardware(hardware);
                    bool bt = r.getBattery(battery);

                    if(hw || bt) {
                        this->parent->getStorage()->addNode(m.getSender(), hardware, battery);
                    }
                    if(r.getLatency(latency)) {
                        this->parent->getStorage()->addReportLatency(m.getSender(), latency);
                    }
                    if(r.getBandwidth(bandwidth)) {
                        this->parent->getStorage()->addReportBandwidth(m.getSender(), bandwidth);
                    }
                    if(r.getIot(iot)) {
                        this->parent->getStorage()->addReportIot(m.getSender(), iot);
                    }
                }else {
                    Message res;
                    res.setType(Message::Type::typeRESPONSE);
                    res.setCommand(Message::Command::commUPDATE);
                    res.setArgument(Message::Argument::argNEGATIVE);
                    
                    sendMessage(fd, res);
                }
            }
        }
    }
    */

    if(!handled)
        LeaderConnections::handler(fd, m);
    
    if(!handled)
        AdaptiveFollowerConnections::handler(fd, m);
}


void AdaptiveLeaderConnections::call_super_handler(int fd, Message &m) { }

/*
bool AdaptiveLeaderConnections::sendMReport(Message::node ip, vector<AdaptiveReport::adaptive_report_result> report) {
    int Socket = this->openConnection(ip.ip, ip.port);
    if(Socket < 0) {
        return false;
    }
    fflush(stdout);
    char buffer[10];

    //build message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::typeMREQUEST);
    m.setCommand(Message::Command::commSET);
    m.setArgument(Message::Argument::argREPORT);

    AdaptiveReport r;
    r.setReports(report);

    m.setData(r);
    bool ret = false;

    //send message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::typeMRESPONSE &&
                res.getCommand() == Message::Command::commSET &&
                res.getArgument() == Message::Argument::argPOSITIVE) {
                ret = true;
            }
        }
    }
    close(Socket);
    return ret;
}
*/

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