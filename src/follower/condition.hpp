#ifndef CONDITION_HPP_
#define CONDITION_HPP_

#include "common.hpp"
#include "clips.h"
#include <map>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

class AdaptiveFollower;

class Condition {
public:
    Condition();
    ~Condition();

    static void MetricInStateFor(Environment *env, UDFContext *udfc, UDFValue *out);
};

#endif