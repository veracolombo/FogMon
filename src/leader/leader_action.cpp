#include "leader_action.hpp"
#include "connections.hpp"
#include <iostream>

void LeaderAction::ChangeTimeReportLeader(Environment *env, UDFContext *udfc, UDFValue *out) {
    
    UDFValue _time; UDFValue _ip; 
    UDFNthArgument(udfc, 1, INTEGER_BIT, &_time);
    UDFNthArgument(udfc, 2, STRING_BIT, &_ip);

    int time = _time.integerValue->contents;
    string ip = _ip.lexemeValue->contents;

    cout << "changeTimeReportLeader" << endl;

    /*
    int Socket = openConnection(ip,"5555");

    if(Socket < 0) {
        return;
    }

    fflush(stdout);
    char buffer[10];

    //build message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::typeREQUEST);
    m.setCommand(Message::Command::commGET);
    m.setArgument(Message::Argument::argREPORT);

    bool ret = false;

    //send message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::typeRESPONSE &&
                res.getCommand() == Message::Command::commGET &&
                res.getArgument() == Message::Argument::argPOSITIVE) {
                //get report and save it
                AdaptiveReport r;
                if(m.getData(r)) {
                    AdaptiveReport::hardware_result hardware;
                    vector<AdaptiveReport::test_result> latency;
                    vector<AdaptiveReport::test_result> bandwidth;
                    vector<AdaptiveReport::IoT> iot;
                    AdaptiveReport::battery_result battery;

                    bool hw = r.getHardware(hardware);
                    bool bt = r.getBattery(battery);

                    if(hw || bt) {
                        cout << "hw || bt" << endl;
                        this->parent->getStorage()->addNode(ip, hardware, battery);
                    }
                    if(r.getLatency(latency)) {
                        this->parent->getStorage()->addReportLatency(ip, latency);
                    }
                    if(r.getBandwidth(bandwidth)) {
                        this->parent->getStorage()->addReportBandwidth(ip, bandwidth);
                    }
                    if(r.getIot(iot)) {
                        this->parent->getStorage()->addReportIot(ip, iot);
                    }
                    ret = true;
                }
            }
        }
    }
    close(Socket);
    return ret;

    */
}