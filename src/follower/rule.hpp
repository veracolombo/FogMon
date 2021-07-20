#ifndef RULE_HPP_
#define RULE_HPP_

#include "common.hpp"
#include "clips.h"
#include "action.hpp"
#include "condition.hpp"

class Rule {
public:
    Rule();
    ~Rule();

    void initialize(string f_path, string r_path);
    void run(); 

    Environment* getEnv();

private:
    Environment *env;
};

#endif