#include "action.hpp"
#include "adaptive_follower.hpp"
#include "node.hpp"
#include "common.hpp"
#include <iostream>

Action::Action() {}
Action::~Action() {}

void Action::ChangeTimeReport(Environment *env, UDFContext *udfc, UDFValue *out){
    UDFValue t;
    UDFFirstArgument(udfc, INTEGER_BIT, &t);
    int time = t.integerValue->contents;

    Node::timeReport = time;
    cout << "Time report changed: " << Node::timeReport << endl;
}


void Action::EnableMetric(Environment *env, UDFContext *udfc, UDFValue *out){
    UDFValue m;
    UDFFirstArgument(udfc, STRING_BIT, &m);
    const char* metric = m.lexemeValue->contents;

    AdaptiveFollower::metrics[lMetrics.at(metric)] = true;
    cout << "Metric " << metric << " enabled" << endl;
}

void Action::DisableMetric(Environment *env, UDFContext *udfc, UDFValue *out){
    UDFValue m;
    UDFFirstArgument(udfc, STRING_BIT, &m);
    const char* metric = m.lexemeValue->contents;

    AdaptiveFollower::metrics[lMetrics.at(metric)] = false;
    cout << "Metric " << metric << " disabled" << endl;
}

void Action::SetLeaderAdequacy(Environment *env, UDFContext *udfc, UDFValue *out){
    UDFValue v;
    UDFFirstArgument(udfc, INTEGER_BIT, &v);
    int value = v.integerValue->contents;

    if(value == 0){
        AdaptiveFollower::leaderAdequacy = false;
    }else{
        AdaptiveFollower::leaderAdequacy = true;
    }
    cout << "Leader Adequacy: " << AdaptiveFollower::leaderAdequacy << endl;
}
