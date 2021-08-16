#ifndef CLIPS_FUNCTION_HPP_
#define CLIPS_FUNCTION_HPP_

#include "clips.h"

class ClipsFunction {
public:

    ClipsFunction();
    ~ClipsFunction();

    static void GetTimeReport(Environment *env, UDFContext *udfc, UDFValue *out);
    static void GetNumActiveMetrics(Environment *env, UDFContext *udfc, UDFValue *out);
};

#endif