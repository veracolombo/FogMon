#ifndef CLIPS_FUNCTION_HPP_
#define CLIPS_FUNCTION_HPP_

#include "clips.h"

class ClipsFunction {
public:

    ClipsFunction();
    ~ClipsFunction();

    static void GetTimeReport(Environment *env, UDFContext *udfc, UDFValue *out);
<<<<<<< HEAD
    static void GetNumActiveMetrics(Environment *env, UDFContext *udfc, UDFValue *out);
    static void LoadFacts(Environment *env, UDFContext *udfc, UDFValue *out);
=======
>>>>>>> parent of f5a88dd (added changeServer())
};

#endif