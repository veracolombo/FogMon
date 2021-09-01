#include "leader_action.hpp"
#include "adaptive_leader.hpp"
#include <unistd.h>
#include <iostream>
#include "string.h"

using namespace std;

void LeaderAction::ChangeTimeReportLeader(Environment *env, UDFContext *udfc, UDFValue *out) {
    
    UDFValue _time; UDFValue _ip; 
    UDFNthArgument(udfc, 1, INTEGER_BIT, &_time);
    UDFNthArgument(udfc, 2, STRING_BIT, &_ip);

    int time = _time.integerValue->contents;
    string ip = _ip.lexemeValue->contents;

    AdaptiveLeader* node = AdaptiveLeader::myobj;

    Message::node follower;
    for (auto &f : node->getStorage()->getMLRHardware(100, 0)){
        if(f.ip == ip)
            follower = f;
    }

    if (follower.id == "" || follower.ip == "" || follower.port == "")
        return;
        
    node->getConnections()->sendChangeTimeReport(follower, time);
}