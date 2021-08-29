#include "clips_function.hpp"
#include "adaptive_follower.hpp"
#include <cmath>
#include <iostream>

using namespace std;

ClipsFunction::ClipsFunction() {}
ClipsFunction::~ClipsFunction() {}

void ClipsFunction::GetTimeReport(Environment *env, UDFContext *udfc, UDFValue *out) {
    
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

    int tr = (int)ceil(((maxtr-mintr)*(c/(n*w))) + mintr) + 20;

    out->integerValue = CreateInteger(env,tr);
}

void ClipsFunction::GetNumActiveMetrics(Environment *env, UDFContext *udfc, UDFValue *out){
    int count = 0;

    for(auto &m : AdaptiveFollower::metrics)
        if(m.second == true)
            count += 1;
    
    out->integerValue = CreateInteger(env,count);
}