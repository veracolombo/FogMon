#include "clips_function.hpp"
<<<<<<< HEAD
#include "adaptive_follower.hpp"
#include "adaptive_leader.hpp"
=======
>>>>>>> parent of f5a88dd (added changeServer())
#include <cmath>
#include <iostream>

using namespace std;

ClipsFunction::ClipsFunction() {}
ClipsFunction::~ClipsFunction() {}

void ClipsFunction::GetTimeReport(Environment *env, UDFContext *udfc, UDFValue *out){
    
    UDFValue _c; UDFValue _n; UDFValue _w; UDFValue _mintr; UDFValue _maxtr;  

    UDFNthArgument(udfc, 1, INTEGER_BIT, &_c);
    UDFNthArgument(udfc, 2, INTEGER_BIT, &_n);
    UDFNthArgument(udfc, 3, INTEGER_BIT, &_w);
    UDFNthArgument(udfc, 4, INTEGER_BIT, &_mintr);
    UDFNthArgument(udfc, 5, INTEGER_BIT, &_maxtr);

    float c = (float)_c.integerValue->contents;
    float n = (float)_n.integerValue->contents;
    float w = (float)_w.integerValue->contents;
    float mintr = (float)_mintr.integerValue->contents;
    float maxtr = (float)_maxtr.integerValue->contents;

    int tr = (int)ceil(((maxtr-mintr)*(c/(n*w))) + mintr);

    out->integerValue = CreateInteger(env,tr);
<<<<<<< HEAD
}

void ClipsFunction::GetNumActiveMetrics(Environment *env, UDFContext *udfc, UDFValue *out){
    int count = 0;

    for(auto &m : AdaptiveFollower::metrics)
        if(m.second == true)
            count += 1;
    
    out->integerValue = CreateInteger(env,count);
}

void ClipsFunction::LoadFacts(Environment *env, UDFContext *udfc, UDFValue *out){

    AdaptiveLeader* node = AdaptiveLeader::myobj;

    vector<tuple<string, Metric, State>> data = node->getStorage()->getFollowerStates();

    string s;
    for(auto &m : data){
        s = "(metric_state (node " + get<0>(m) +  ") (metric " + Metric2String.at(get<1>(m)) + ") (state " + State2String.at(get<2>(m)) + "))";
        AssertString(env, s.c_str());
    }
=======
>>>>>>> parent of f5a88dd (added changeServer())
}