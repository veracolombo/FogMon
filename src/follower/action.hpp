#ifndef ACTION_HPP_
#define ACTION_HPP_

#include "clips.h"

class Action {
public:

    Action();
    ~Action();

    static void ChangeTimeReport(Environment *env, UDFContext *udfc, UDFValue *out);
    
    static void EnableMetric(Environment *env, UDFContext *udfc, UDFValue *out);
    static void DisableMetric(Environment *env, UDFContext *udfc, UDFValue *out);

    static void SetLeaderAdequacy(Environment *env, UDFContext *udfc, UDFValue *out);
};

#endif